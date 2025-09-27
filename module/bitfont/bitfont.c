#include "bitfont.h"
#include "bitfont_logic.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>




#define BITFONT_MAXDATAPOINTERS 256




static struct BITFONT_data bitfontData[BITFONT_MAXDATAPOINTERS] = {0};


struct BITFONT_data* bitfontRequestPtr(unsigned int size){

	if (size == 0) {
		return 0;
	}

	{unsigned int i;
	for(i = 0;i<BITFONT_MAXDATAPOINTERS;i++){
		if (bitfontData[i].size == 0) {
			bitfontData[i].size = size;
			bitfontData[i].data = SDL_calloc(size,sizeof(struct BITFONT_object));
			return bitfontData;
		}
	}}

	SDL_Log("bitfontRequestPtr: data pointer limit(%d) reached",
			BITFONT_MAXDATAPOINTERS);
	return 0;
}


void bitfontFreePtr(struct BITFONT_data* ptr){
	
	SDL_free(ptr->data);
	ptr->size = 0;
}




static enum ISOLA_state bitfontState = 0x00000001;
static unsigned int bitfontSP[1] = {0};
static unsigned int bitfontVAO[1] = {0};
static unsigned int bitfontVBO[1] = {0};
static unsigned int bitfontTO[1] = {0};
static unsigned char bitfontVD[BITFONT_STRINGSIZE*6] = {0};


void updateBitfont(void){

	ISOLA_GLDBG_( glBindVertexArray(bitfontVAO[0]) )
	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]) )
	ISOLA_GLDBG_( glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(bitfontVD),bitfontVD) )


	ISOLA_GLDBG_( glUseProgram(bitfontSP[0]) )

	{int loc;
	float matProj[4*4] = {0};
	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio,0.25,8.,matProj);
	loc = glGetUniformLocation(bitfontSP[0],"matProj");
	if(loc == -1){ SDL_Log("bitfont: matProj not found"); }
	glUniformMatrix4fv(loc,1,GL_FALSE,matProj);
	}

	{int loc;
	float lowRes;
	if (isola_info_window.xRatio == 1) { lowRes = isola_info_window.width;
	}else{ lowRes = isola_info_window.height; }
	loc = glGetUniformLocation(bitfontSP[0],"lowResolution");
	if(loc == -1){ SDL_Log("lowResolution not found"); }
	glUniform1f(loc,lowRes);
	}
}


void createBitfont(void){

	{int i;
	for(i = 0;i<sizeof(bitfontVD)/sizeof(bitfontVD[0]);i++){
		bitfontVD[i] = 127-32;
	}}

	ISOLA_GLDBG_( glGenVertexArrays(1,&bitfontVAO[0]) )
	ISOLA_GLDBG_( glGenBuffers(1,&bitfontVBO[0]) )

	ISOLA_GLDBG_( glBindVertexArray(bitfontVAO[0]) )


	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]) )
	ISOLA_GLDBG_( glBufferData(GL_ARRAY_BUFFER,sizeof(bitfontVD),
			bitfontVD,GL_DYNAMIC_DRAW) )

	ISOLA_GLDBG_( glEnableVertexAttribArray(0) )
	ISOLA_GLDBG_( glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,
			sizeof(bitfontVD[0]),(void*)0) )


	ISOLA_GLDBG_( glGenTextures(1,bitfontTO) )
	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0) )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,bitfontTO[0]) )

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


	bitfontSP[0] = isola_shader_buildProgram("module/bitfont/glsl/bitfont.vert",
			"module/bitfont/glsl/bitfont.frag");

	ISOLA_GLDBG_( glUseProgram(bitfontSP[0]) )

	{int loc;
	loc = glGetUniformLocation(bitfontSP[0],"bitfontAtlas");
	if(loc == -1){ SDL_Log("bitfont: bitfontAtlas not found"); }
	glUniform1i(loc,0);
	}


	updateBitfont();
}


void destroyBitfont(void){

	glUseProgram(0);
	glBindVertexArray(0);

	glDeleteTextures(1,bitfontTO);
	glDeleteProgram(bitfontSP[0]);
	glDeleteBuffers(1,bitfontVBO);
	glDeleteVertexArrays(1,bitfontVAO);
}


void drawBitfont(void){

	ISOLA_GLDBG_( glBindVertexArray(bitfontVAO[0]) )

	isola_set_state(bitfontState);
	ISOLA_GLDBG_( glUseProgram(bitfontSP[0]) )

	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0) )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,bitfontTO[0]) )
	
	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]) )


	{unsigned int d;
	for(d = 0;d<BITFONT_MAXDATAPOINTERS;d++){

		{unsigned int o;
		for(o = 0;o<bitfontData[d].size;o++){

			{int c;
			for(c = 0;c<strlen(bitfontData[d].data[o].string);c++){

				{unsigned int v;
				for(v = 0;v<6;v++){

					bitfontVD[c*6+v] = bitfontData[d].data[o].string[c]-32;
				}}
			}}

			glBufferSubData(GL_ARRAY_BUFFER,sizeof(bitfontVD[0])*0,
					sizeof(bitfontVD)/sizeof(bitfontVD[0]),bitfontVD);

			{int c;
			for(c = 0;c<SDL_strlen(bitfontData[d].data[o].string)*
					sizeof(bitfontVD[0])*6;c++){
				bitfontVD[c] = 127-32;
			}}


			{int loc;
			loc = glGetUniformLocation(bitfontSP[0],"screenPos");
			if(loc == -1){ SDL_Log("bitfont: screenPos not found"); }
			glUniform2f(loc,bitfontData[d].data[o].x,bitfontData[d].data[o].y);
			}

			{int loc;
			loc = glGetUniformLocation(bitfontSP[0],"pixelSize");
			if(loc == -1){ SDL_Log("bitfont: pixelSize not found"); }
			glUniform1i(loc,bitfontData[d].data[o].pixelSize);
			}

			{int loc;
			loc = glGetUniformLocation(bitfontSP[0],"charWrap");
			if(loc == -1){ SDL_Log("bitfont: charWrap not found"); }
			glUniform1i(loc,bitfontData[d].data[o].charWrap);
			}

			{int loc;
			loc = glGetUniformLocation(bitfontSP[0],"fontColor");
			if(loc == -1){ SDL_Log("bitfont: fontColor not found"); }
			glUniform4fv(loc,1,bitfontData[d].data[o].foreColor);
			}

			{int loc;
			loc = glGetUniformLocation(bitfontSP[0],"backColor");
			if(loc == -1){ SDL_Log("bitfont: backColor not found"); }
			glUniform4fv(loc,1,bitfontData[d].data[o].backColor);
			}

			glDrawArrays(GL_TRIANGLES,0,BITFONT_STRINGSIZE*6);
		}}
	}}
}




