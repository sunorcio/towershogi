

#include "render.h"


#include <stdio.h>


#include "timer.h"
#include "isola/mesh.h"
#include "isola/mutil.h"




#define digitPrintAmount 5
unsigned char b_DigitFPS = 1;
float digitColor[4] = {0.25,0.25,0.25,0.75};
int unsigned digitPixelsize = 24;
static char stringDigitFPS[16] = {0};
static unsigned int digitSP[1] = {0};
static unsigned int digitVAO[1] = {0};
static unsigned int digitEBO[1] = {0};
static unsigned short digitED[16]
	[sizeof(elementDigit[0])/sizeof(elementDigit[0][0])] = {0};

void updateDigitFPS(void){

	glUseProgram(digitSP[0]);


	{int locProj;
	float matProj[4*4] = {0};
	mut_proj_glortho(-isolaInfoWindow.xratio,isolaInfoWindow.xratio,
						-isolaInfoWindow.yratio,isolaInfoWindow.yratio,
						0.25,8.,matProj);
	locProj = glGetUniformLocation(digitSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}

	{int locDigitCol;
	locDigitCol = glGetUniformLocation(digitSP[0],"digitColor");
	if(locDigitCol == -1){SDL_Log("digitColor not found in shader %d",0);}
	glUniform4fv(locDigitCol,1,digitColor);
	}

	{int locPixSize;
	locPixSize = glGetUniformLocation(digitSP[0],"pixelSize");
	if(locPixSize == -1){SDL_Log("pixelSize not found in shader %d",0);}
	glUniform1i(locPixSize,digitPixelsize);
	}

	{int locLowRes;
	float lowRes;
	if (isolaInfoWindow.xratio == 1) { lowRes = isolaInfoWindow.width;
	}else{ lowRes = isolaInfoWindow.height; }
	locLowRes = glGetUniformLocation(digitSP[0],"lowResolution");
	if(locLowRes == -1){SDL_Log("lowResolution not found in shader %d",0);}
	glUniform1f(locLowRes,lowRes);
	}
}
static void createDigitFPS(void){

	glGenVertexArrays(1,&digitVAO[0]);
	glGenBuffers(1,&digitEBO[0]);

	glBindVertexArray(digitVAO[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,digitEBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(digitED),
					digitED,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
					sizeof(digitED),digitED);


	{unsigned int vs, fs;
	digitSP[0] = glCreateProgram();
	vs = isolaShaderCompile("glsl/digit.vert",GL_VERTEX_SHADER);
	fs = isolaShaderCompile("glsl/digit.frag",GL_FRAGMENT_SHADER);
	glAttachShader(digitSP[0],vs);
	glAttachShader(digitSP[0],fs);

	isolaDBG_(glLinkProgram(digitSP[0]);)
	glDetachShader(digitSP[0],vs);
	glDetachShader(digitSP[0],fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	}


	updateDigitFPS();
}
static void destroyDigitFPS(void){

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(digitSP[0]);
	glDeleteBuffers(1,digitEBO);
	glDeleteVertexArrays(1,digitVAO);
}
static void drawDigitFPS(void){

	unsigned delaySum = 0;

	{unsigned int i;
	for(i = 0;i<256;i++){
		delaySum += frameDelay[i];
	}}

	{unsigned int i;
	for(i = 0;i<digitPrintAmount;i++){
		stringDigitFPS[i] = 0;
	}}
	sprintf(stringDigitFPS,"%f",1./((delaySum/256.)/(double)clockFreq) );

	{unsigned int i;
	unsigned char digit;
	for(i = 0;i<digitPrintAmount;i++){
		switch (stringDigitFPS[i]) {
			case 0:   digit = 0;  break;
			case ' ': digit = 0;  break;
			case '0': digit = 1;  break;
			case '1': digit = 2;  break;
			case '2': digit = 3;  break;
			case '3': digit = 4;  break;
			case '4': digit = 5;  break;
			case '5': digit = 6;  break;
			case '6': digit = 7;  break;
			case '7': digit = 8;  break;
			case '8': digit = 9;  break;
			case '9': digit = 10; break;
			case '.': digit = 11; break;
			default: digit = 11; break;
		}

		{unsigned char j;
		for(j = 0;j<sizeof(elementDigit[0])/sizeof(elementDigit[0][0]);j++){
			digitED[i][j] = elementDigit[digit][j]+15*i;
		}}
	}}

	glBindVertexArray(digitVAO[0]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
		( sizeof(digitED)/16 )*digitPrintAmount,digitED);
	glUseProgram(digitSP[0]);

	glDrawRangeElements(GL_TRIANGLES,0,
		(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))*digitPrintAmount-1,
		(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))*digitPrintAmount,
		GL_UNSIGNED_SHORT,(void*)0);
}




float bitfontPos[2] = {-1.,1.};
unsigned int bitfontPixelSize = 8;
unsigned int bitfontCharWrap = 1000;
float bitfontColor[4] = {0.,0.,0.,1.};
float bitfontBackColor[4] = {1.,1.,1.,0.75};
static unsigned int bitfontSP[1] = {0};
static unsigned int bitfontTO[1] = {0};
static unsigned int bitfontVAO[1] = {0};
static int bitfontCharIndex[1024] = {0};
/* struct RENDEROBJECT_bitfont{
	bitfontCharWrapU
}
unsigned char bitfontGroup[256] = {0}; */

void updateBitmapFont(void){

	bitfontCharWrap = isolaInfoWindow.width/bitfontPixelSize;
	if (!bitfontCharWrap) {
		bitfontCharWrap = 1024;
	}
	if (bitfontCharWrap <= isolaInfoWindow.width/bitfontPixelSize) {
		bitfontPos[0] = 
			(2.-2.*bitfontPixelSize*bitfontCharWrap/isolaInfoWindow.width)/2.-1;
	}


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

	{int locBitPos;
	locBitPos = glGetUniformLocation(bitfontSP[0],"screenPos");
	if(locBitPos == -1){SDL_Log("screenPos not found in shader %d",0);}
	glUniform2fv(locBitPos,1,bitfontPos);
	}

	{int locPixSize;
	locPixSize = glGetUniformLocation(bitfontSP[0],"pixelSize");
	if(locPixSize == -1){SDL_Log("pixelSize not found in shader %d",0);}
	glUniform1i(locPixSize,bitfontPixelSize);
	}

	{int locCharWrap;
	locCharWrap = glGetUniformLocation(bitfontSP[0],"charWrap");
	if(locCharWrap == -1){SDL_Log("charWrap not found in shader %d",0);}
	glUniform1i(locCharWrap,bitfontCharWrap);
	}

	{int locCharId;
	{unsigned int i;
	char* asdf = "Me meo depie ysen tado, jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj";
	for(i = 0;i<strlen(asdf);i++){
		bitfontCharIndex[i] = asdf[i]-32;
	}
	for(i = strlen(asdf);i<1024;i++){
		bitfontCharIndex[i] = 1;
	}}
	locCharId = glGetUniformLocation(bitfontSP[0],"charIndex");
	if(locCharId == -1){SDL_Log("charIndex not found in shader %d",0);}
	glUniform1iv(locCharId,1024,bitfontCharIndex);
	}

	{int locBitCol;
	locBitCol = glGetUniformLocation(bitfontSP[0],"fontColor");
	if(locBitCol == -1){SDL_Log("fontColor not found in shader %d",0);}
	glUniform4fv(locBitCol,1,bitfontColor);
	}

	{int locBitBackCol;
	locBitBackCol = glGetUniformLocation(bitfontSP[0],"backColor");
	if(locBitBackCol == -1){SDL_Log("backColor not found in shader %d",0);}
	glUniform4fv(locBitBackCol,1,bitfontBackColor);
	}
}
static void createBitmapFont(void){

	glGenVertexArrays(1,&bitfontVAO[0]);

	glBindVertexArray(bitfontVAO[0]);


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


	{unsigned int vs, fs;
	bitfontSP[0] = glCreateProgram();
	vs = isolaShaderCompile("glsl/bitfont.vert",GL_VERTEX_SHADER);
	fs = isolaShaderCompile("glsl/bitfont.frag",GL_FRAGMENT_SHADER);
	glAttachShader(bitfontSP[0],vs);
	glAttachShader(bitfontSP[0],fs);

	glLinkProgram(bitfontSP[0]);
	glDetachShader(bitfontSP[0],vs);
	glDetachShader(bitfontSP[0],fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	}


	glUseProgram(bitfontSP[0]);

	{int locBatl;
	locBatl = glGetUniformLocation(bitfontSP[0],"bitfontAtlas");
	if(locBatl == -1){SDL_Log("bitfontAtlas not found in shader %d",0);}
	glUniform1i(locBatl,0);
	}


	updateBitmapFont();
}
static void destroyBitmapFont(void){

	glUseProgram(0);
	glBindVertexArray(0);

	glDeleteTextures(1,bitfontTO);
	glDeleteProgram(bitfontSP[0]);
	glDeleteVertexArrays(1,bitfontVAO);
}
static void drawBitmapFont(void){

	glBindVertexArray(bitfontVAO[0]);
	glUseProgram(bitfontSP[0]);
	glActiveTexture(GL_TEXTURE0+0);
	glBindTexture(GL_TEXTURE_2D,bitfontTO[0]);

	glDrawArrays(GL_TRIANGLES,0,6*1024);
}




void renderGlobalUpdate(void){

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width,
				isolaInfoWindow.height);


	updateDigitFPS();
	updateBitmapFont();
}
void renderGlobalCreate(void){

/* 	isolaInfoWindow.desktopDisplayMode.w = 360;
	isolaInfoWindow.desktopDisplayMode.h = 480; */
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.desktopDisplayMode.w,
						isolaInfoWindow.desktopDisplayMode.h);
	SDL_SetWindowResizable(isolaWindow,1);
	isolaGetWindow();


	createDigitFPS();
	createBitmapFont();


	renderGlobalUpdate();
}
void renderGlobalDestroy(void){

	destroyDigitFPS();
	destroyBitmapFont();
}
void renderGlobalDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawBitmapFont();
	if (b_DigitFPS) {drawDigitFPS();}


	SDL_GL_SwapWindow(isolaWindow);
}


