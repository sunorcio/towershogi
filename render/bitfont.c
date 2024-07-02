

#include "render_logic.h"


#include <stdio.h>




struct BITFONT_object* bitfontData = {0};
unsigned int bitfontDataSize = 0;


static ISOLA_State bitfontState = 0x00000001;
static unsigned int bitfontSP[1] = {0};
static unsigned int bitfontVAO[1] = {0};
static unsigned int bitfontVBO[1] = {0};
static unsigned int bitfontTO[1] = {0};
static unsigned char bitfontVD[bitfontStringSize*6] = {0};


static void updateBitfont(void){

	glBindVertexArray(bitfontVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0,
			sizeof(bitfontVD),bitfontVD);


	glUseProgram(bitfontSP[0]);

	{int locProj;
	float matProj[4*4] = {0};
	mut_proj_glortho(-isolaInfoWindow.xratio,isolaInfoWindow.xratio,
			-isolaInfoWindow.yratio,isolaInfoWindow.yratio,
			0.25,8.,matProj);
	locProj = glGetUniformLocation(bitfontSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}

	{int locLowRes;
	float lowRes;
	if (isolaInfoWindow.xratio == 1) { lowRes = isolaInfoWindow.width;
	}else{ lowRes = isolaInfoWindow.height; }
	locLowRes = glGetUniformLocation(bitfontSP[0],"lowResolution");
	if(locLowRes == -1){SDL_Log("lowResolution not found in shader %d",0);}
	glUniform1f(locLowRes,lowRes);
	}
}


static void createBitfont(void){

	{int i;
	for(i = 0;i<sizeof(bitfontVD)/sizeof(bitfontVD[0]);i++){
		bitfontVD[i] = 127;
	}}

	glGenVertexArrays(1,&bitfontVAO[0]);
	glGenBuffers(1,&bitfontVBO[0]);

	glBindVertexArray(bitfontVAO[0]);


	glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(bitfontVD),
			bitfontVD,GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,sizeof(bitfontVD[0]),(void*)0);


	glGenTextures(1,bitfontTO);
	glActiveTexture(GL_TEXTURE0+0);
	glBindTexture(GL_TEXTURE_2D,bitfontTO[0]);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	{void* imagedata;
	long filesize;
	FILE* imagefile = fopen("glsl/128p_8x16_IBM_VGA_ASCII.data","rb");
	fseek(imagefile,0,SEEK_END);
	filesize = ftell(imagefile);
	fseek(imagefile,0,SEEK_SET);
	imagedata = malloc(filesize+1);
	fread(imagedata,filesize,1,imagefile);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,16*8,8*16,
			0,GL_RGBA,GL_UNSIGNED_BYTE,(char*)imagedata);
	fclose(imagefile);
	free(imagedata);
	}


	bitfontSP[0] = isolaShaderProgram("glsl/bitfont.vert","glsl/bitfont.frag");

	glUseProgram(bitfontSP[0]);

	{int locBatl;
	locBatl = glGetUniformLocation(bitfontSP[0],"bitfontAtlas");
	if(locBatl == -1){SDL_Log("bitfontAtlas not found in shader %d",0);}
	glUniform1i(locBatl,0);
	}


	updateBitfont();
}


static void destroyBitfont(void){

	glUseProgram(0);
	glBindVertexArray(0);

	glDeleteTextures(1,bitfontTO);
	glDeleteProgram(bitfontSP[0]);
	glDeleteBuffers(1,bitfontVBO);
	glDeleteVertexArrays(1,bitfontVAO);
}


static void drawBitfont(void){

	glBindVertexArray(bitfontVAO[0]);

	isolaSetState(bitfontState);
	glUseProgram(bitfontSP[0]);

	glActiveTexture(GL_TEXTURE0+0);
	glBindTexture(GL_TEXTURE_2D,bitfontTO[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER,bitfontVBO[0]);

	{unsigned int o;
	for(o = 0;o<bitfontDataSize;o++){

		{int c;
		for(c = 0;c<strlen(bitfontData[o].string);c++){

			{unsigned int v;
			for(v = 0;v<6;v++){

				bitfontVD[c*6+v] = bitfontData[o].string[c]-32;
			}}
		}}

		glBufferSubData(GL_ARRAY_BUFFER,sizeof(bitfontVD[0])*0,
				sizeof(bitfontVD)/sizeof(bitfontVD[0]),bitfontVD);

		{int c;
		for(c = 0;c<strlen(bitfontData[o].string)*sizeof(bitfontVD[0])*6;c++){
			bitfontVD[c] = 127;
		}}

		{int locBitPos;
		locBitPos = glGetUniformLocation(bitfontSP[0],"screenPos");
		if(locBitPos == -1){SDL_Log("screenPos not found in shader %d",0);}
		glUniform2f(locBitPos, bitfontData[o].x, bitfontData[o].y);
		}

		{int locPixSize;
		locPixSize = glGetUniformLocation(bitfontSP[0],"pixelSize");
		if(locPixSize == -1){SDL_Log("pixelSize not found in shader %d",0);}
		glUniform1i(locPixSize, bitfontData[o].pixelSize);
		}

		{int locCharWrap;
		locCharWrap = glGetUniformLocation(bitfontSP[0],"charWrap");
		if(locCharWrap == -1){SDL_Log("charWrap not found in shader %d",0);}
		glUniform1i(locCharWrap,bitfontData[o].charWrap);
		}

		{int locBitCol;
		locBitCol = glGetUniformLocation(bitfontSP[0],"fontColor");
		if(locBitCol == -1){SDL_Log("fontColor not found in shader %d",0);}
		glUniform4fv(locBitCol,1, bitfontData[o].fontColor);
		}

		{int locBackCol;
		locBackCol = glGetUniformLocation(bitfontSP[0],"backColor");
		if(locBackCol == -1){SDL_Log("backColor not found in shader %d",0);}
		glUniform4fv(locBackCol,1, bitfontData[o].backColor);
		}

		glDrawArrays(GL_TRIANGLES,0,bitfontStringSize*6);
	}}
}




