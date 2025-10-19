#include "bitfont.h"
#include "bitfont_data.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>




#define BITFONT_MAXDATAPOINTERS 256




static struct BITFONT_data bitfont_data[BITFONT_MAXDATAPOINTERS] = {0};


struct BITFONT_data* bitfont_requestPtr(unsigned int size){

	if (size == 0) {
		return 0;
	}

	{unsigned int i;
	for(i = 0;i<BITFONT_MAXDATAPOINTERS;i++){
		if (bitfont_data[i].size == 0) {
			bitfont_data[i].size = size;
			bitfont_data[i].data = SDL_calloc(size,sizeof(struct BITFONT_object));
			return &bitfont_data[i];
		}
	}}

	SDL_Log("bitfont_requestPtr: data pointer limit reached (%d)",
			BITFONT_MAXDATAPOINTERS);
	return 0;
}


void bitfont_freePtr(struct BITFONT_data* ptr){
	
	SDL_free(ptr->data);
	ptr->size = 0;
}




static enum ISOLA_state bitfont_state = 0x00000001;
static unsigned int bitfont_shaderProg[1] = {0};
static unsigned int bitfont_vertArrObj[1] = {0};
static unsigned int bitfont_vertBufObj[1] = {0};
static unsigned int bitfont_texObj[1] = {0};
static char bitfont_vertData[BITFONT_STRINGSIZE*6] = {0};


void bitfont_update(void){

	ISOLA_GLDBG_( glUseProgram(bitfont_shaderProg[0]) )

	{int loc;
	float matProj[4*4] = {0};
	float pixelScale;

	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio,0.25f,8.f,matProj);
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			bitfont_shaderProg[0],"matProj"); )
	if(loc == -1){ SDL_Log("bitfont: matProj not found"); }
	ISOLA_GLDBG_( glUniformMatrix4fv(loc,1,GL_FALSE,matProj); )

	if (isola_info_window.xRatio == 1.f) {
		pixelScale = isola_info_window.pixelWidth;
	}else{ 
		pixelScale = isola_info_window.pixelHeight;
	}
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			bitfont_shaderProg[0],"pixelScale"); )
	if(loc == -1){SDL_Log("digitfps: pixelScale not found");}
	ISOLA_GLDBG_( glUniform1f(loc,pixelScale); )
	}
}


void bitfont_create(void){

	{int i;
	for(i = 0;i<sizeof(bitfont_vertData)/sizeof(bitfont_vertData[0]);i++){
		bitfont_vertData[i] = 127-32;
	}}

	ISOLA_GLDBG_( glGenVertexArrays(1,&bitfont_vertArrObj[0]) )
	ISOLA_GLDBG_( glGenBuffers(1,&bitfont_vertBufObj[0]) )

	ISOLA_GLDBG_( glBindVertexArray(bitfont_vertArrObj[0]) )


	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,bitfont_vertBufObj[0]) )
	ISOLA_GLDBG_( glBufferData(GL_ARRAY_BUFFER,sizeof(bitfont_vertData),
			bitfont_vertData,GL_DYNAMIC_DRAW) )

	ISOLA_GLDBG_( glEnableVertexAttribArray(0) )
	ISOLA_GLDBG_( glVertexAttribIPointer(0,1,GL_BYTE,
			sizeof(bitfont_vertData[0]),(void*)0) )


	ISOLA_GLDBG_( glGenTextures(1,bitfont_texObj) )
	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0) )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,bitfont_texObj[0]) )

	ISOLA_GLDBG_( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE) )
	ISOLA_GLDBG_( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE) )
	ISOLA_GLDBG_( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0) )
	ISOLA_GLDBG_( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
			GL_NEAREST) )
	ISOLA_GLDBG_( glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,
			GL_NEAREST) )

	{void* imagedata;
	size_t size;
	imagedata = SDL_LoadFile(
			"module/bitfont/data/128x128p_8x16_IBM_VGA_ASCII.data",&size);
	ISOLA_GLDBG_( glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,16*8,8*16,
			0,GL_RGBA,GL_UNSIGNED_BYTE,(char*)imagedata) )
	SDL_free(imagedata);
	}


	bitfont_shaderProg[0] = isola_shader_buildProgram(
			"module/bitfont/glsl/bitfont.vert",
			"module/bitfont/glsl/bitfont.frag");

	ISOLA_GLDBG_( glUseProgram(bitfont_shaderProg[0]) )

	{int loc;
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			bitfont_shaderProg[0],"bitfontAtlas"); )
	if(loc == -1){ SDL_Log("bitfont: bitfontAtlas not found"); }
	ISOLA_GLDBG_( glUniform1i(loc,0); )
	}


	bitfont_update();
}


void bitfont_destroy(void){

	ISOLA_GLDBG_( glUseProgram(0) )
	ISOLA_GLDBG_( glBindVertexArray(0) )

	ISOLA_GLDBG_( glDeleteTextures(1,bitfont_texObj) )
	ISOLA_GLDBG_( glDeleteProgram(bitfont_shaderProg[0]) )
	ISOLA_GLDBG_( glDeleteBuffers(1,bitfont_vertBufObj) )
	ISOLA_GLDBG_( glDeleteVertexArrays(1,bitfont_vertArrObj) )
}


void bitfont_draw(void){

	ISOLA_GLDBG_( glBindVertexArray(bitfont_vertArrObj[0]) )

	isola_set_state(bitfont_state);
	ISOLA_GLDBG_( glUseProgram(bitfont_shaderProg[0]) )

	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0) )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,bitfont_texObj[0]) )
	
	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,bitfont_vertBufObj[0]) )


	{unsigned int d;
	for(d = 0;d<BITFONT_MAXDATAPOINTERS;d++){

		{unsigned int o;
		for(o = 0;o<bitfont_data[d].size;o++){

			{unsigned int c;
			for(c = 0;c<SDL_strlen(bitfont_data[d].data[o].string);c++){

				{unsigned int v;
				for(v = 0;v<6;v++){

					bitfont_vertData[c*6+v] = (unsigned char)bitfont_data[d].data[o].string[c]-32;
				}}
			}}

			ISOLA_GLDBG_( glBufferSubData(GL_ARRAY_BUFFER,
						sizeof(bitfont_vertData[0])*0,sizeof(bitfont_vertData)/
						sizeof(bitfont_vertData[0]),bitfont_vertData) )

			{unsigned int c;
			for(c = 0;c<SDL_strlen(bitfont_data[d].data[o].string)*
					sizeof(bitfont_vertData[0])*6;c++){
				bitfont_vertData[c] = 127-32;
			}}


			{int loc;
			ISOLA_GLDBG_( loc = glGetUniformLocation(
					bitfont_shaderProg[0],"screenPos"); )
			if(loc == -1){ SDL_Log("bitfont: screenPos not found"); }
			ISOLA_GLDBG_( glUniform2f(loc,
					bitfont_data[d].data[o].x,bitfont_data[d].data[o].y); )

			ISOLA_GLDBG_( loc = glGetUniformLocation(
					bitfont_shaderProg[0],"sizeMult"); )
			if(loc == -1){ SDL_Log("bitfont: sizeMult not found"); }
			ISOLA_GLDBG_( glUniform1i(loc,bitfont_data[d].data[o].sizeMult); )

			ISOLA_GLDBG_( loc = glGetUniformLocation(
					bitfont_shaderProg[0],"charWrap"); )
			if(loc == -1){ SDL_Log("bitfont: charWrap not found"); }
			ISOLA_GLDBG_( glUniform1i(loc,bitfont_data[d].data[o].charWrap); )

			ISOLA_GLDBG_( loc = glGetUniformLocation(
					bitfont_shaderProg[0],"fontColor"); )
			if(loc == -1){ SDL_Log("bitfont: fontColor not found"); }
			ISOLA_GLDBG_( glUniform4fv(loc,1,bitfont_data[d].data[o].foreColor); )

			ISOLA_GLDBG_( loc = glGetUniformLocation(
					bitfont_shaderProg[0],"backColor"); )
			if(loc == -1){ SDL_Log("bitfont: backColor not found"); }
			ISOLA_GLDBG_( glUniform4fv(loc,1,bitfont_data[d].data[o].backColor); )
			}

			ISOLA_GLDBG_( glDrawArrays(GL_TRIANGLES,0,BITFONT_STRINGSIZE*6) )
		}}
	}}
}




