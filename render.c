

#include "render.h"


#include <stdio.h>


#include "timer.h"
#include "isola/mesh.h"
#include "isola/mutil.h"




#define digitfpsPrintAmount 5

unsigned char digitfpsBool = 1;
float digitfpsColor[4] = {0.25,0.25,0.25,0.75};
int unsigned digitfpsPixelsize = 8*3;

static ISOLA_State digitfpsState;
static unsigned int digitfpsSP;
static unsigned int digitfpsVAO;
static unsigned int digitfpsEBO;
static char digitfpsString[16] = {0};
static unsigned short digitfpsED[16]
		[sizeof(elementDigit[0])/sizeof(elementDigit[0][0])] = {0};


static void updateDigitfps(void){

	glUseProgram(digitfpsSP);

	{int locProj;
	float matProj[4*4] = {0};
	mut_proj_glortho(-isolaInfoWindow.xratio,isolaInfoWindow.xratio,
			-isolaInfoWindow.yratio,isolaInfoWindow.yratio, 0.25,8.,matProj);
	locProj = glGetUniformLocation(digitfpsSP,"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}

	{int locDigitCol;
	locDigitCol = glGetUniformLocation(digitfpsSP,"digitColor");
	if(locDigitCol == -1){SDL_Log("digitColor not found in shader %d",0);}
	glUniform4fv(locDigitCol,1,digitfpsColor);
	}

	{int locPixSize;
	locPixSize = glGetUniformLocation(digitfpsSP,"pixelSize");
	if(locPixSize == -1){SDL_Log("pixelSize not found in shader %d",0);}
	glUniform1i(locPixSize,digitfpsPixelsize);
	}

	{int locLowRes;
	float lowRes;
	if (isolaInfoWindow.xratio == 1) { lowRes = isolaInfoWindow.width;
	}else{ lowRes = isolaInfoWindow.height; }
	locLowRes = glGetUniformLocation(digitfpsSP,"lowResolution");
	if(locLowRes == -1){SDL_Log("lowResolution not found in shader %d",0);}
	glUniform1f(locLowRes,lowRes);
	}
}

static void createDigitfps(void){

	digitfpsState = 0x00000001;


	glGenVertexArrays(1,&digitfpsVAO);
	glGenBuffers(1,&digitfpsEBO);

	glBindVertexArray(digitfpsVAO);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,digitfpsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(digitfpsED),
			digitfpsED,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
			sizeof(digitfpsED),digitfpsED);


	digitfpsSP = isolaShaderProgram("glsl/digit.vert","glsl/digit.frag");


	updateDigitfps();
}

static void destroyDigitfps(void){

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(digitfpsSP);
	glDeleteBuffers(1,&digitfpsEBO);
	glDeleteVertexArrays(1,&digitfpsVAO);
}

static void drawDigitfps(void){

	isolaSetState(digitfpsState);


	{unsigned delaySum = 0;
	{unsigned int i;
	for(i = 0;i<256;i++){
		delaySum += frameDelay[i];
	}}

	{unsigned int i;
	for(i = 0;i<digitfpsPrintAmount;i++){
		digitfpsString[i] = 0;
	}}
	sprintf(digitfpsString,"%f",1./((delaySum/256.)/(double)clockFreq) );
	}

	{unsigned int i;
	unsigned char digit;
	for(i = 0;i<digitfpsPrintAmount;i++){
		switch (digitfpsString[i]) {
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
			digitfpsED[i][j] = elementDigit[digit][j]+15*i;
		}}
	}}

	glBindVertexArray(digitfpsVAO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
		( sizeof(digitfpsED)/16 )*digitfpsPrintAmount,digitfpsED);
	glUseProgram(digitfpsSP);

	glDrawRangeElements(GL_TRIANGLES,0,
		(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))*digitfpsPrintAmount-1,
		(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))*digitfpsPrintAmount,
		GL_UNSIGNED_SHORT,(void*)0);
}




#define bitfontStringBufferSize 1024

char textString[bitfontStringBufferSize];
float x;
float y;
unsigned int pixelSize;
unsigned int CharWrap;
float fontColor[4];
float backColor[4];

static ISOLA_State bitfontState = {0x00000001};
static unsigned int bitfontSP[1] = {0};
static unsigned int bitfontVAO[1] = {0};
static unsigned int bitfontVBO[1] = {0};
static unsigned int bitfontTO[1] = {0};
static unsigned char bitfontVD[bitfontStringBufferSize*6] = {0};


void updateBitfont(void){

	if (CharWrap
			<= isolaInfoWindow.width/pixelSize) {
		x = ( 2.-2.*pixelSize
				*CharWrap/isolaInfoWindow.width )/2.-1;
	}


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

	strcpy(textString,"CONTINUE");
	x = -1.;
	y = 0.;
	pixelSize = 8*2;
	CharWrap = 8;
	fontColor[0] = 0.;
	fontColor[1] = 0.;
	fontColor[2] = 0.;
	fontColor[3] = 1.;
	backColor[0] = 1.;
	backColor[1] = 1.;
	backColor[2] = 1.;
	backColor[3] = 0.;

	{int i;
	for(i = 0;i<strlen(textString);i++){
		{unsigned int j;
		for(j = 0;j<6;j++){
			bitfontVD[i*6+j] = textString[i]-32;
		}}
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
	
	isolaSetState(bitfontState);

	glBindVertexArray(bitfontVAO[0]);
	glUseProgram(bitfontSP[0]);
	glActiveTexture(GL_TEXTURE0+0);
	glBindTexture(GL_TEXTURE_2D,bitfontTO[0]);


	{int locBitPos;
	locBitPos = glGetUniformLocation(bitfontSP[0],"screenPos");
	if(locBitPos == -1){SDL_Log("screenPos not found in shader %d",0);}
	glUniform2f(locBitPos,x,y);
	}

	{int locPixSize;
	locPixSize = glGetUniformLocation(bitfontSP[0],"pixelSize");
	if(locPixSize == -1){SDL_Log("pixelSize not found in shader %d",0);}
	glUniform1i(locPixSize,pixelSize);
	}

	{int locCharWrap;
	locCharWrap = glGetUniformLocation(bitfontSP[0],"charWrap");
	if(locCharWrap == -1){SDL_Log("charWrap not found in shader %d",0);}
	glUniform1i(locCharWrap,CharWrap);
	}

	{int locBitCol;
	locBitCol = glGetUniformLocation(bitfontSP[0],"fontColor");
	if(locBitCol == -1){SDL_Log("fontColor not found in shader %d",0);}
	glUniform4fv(locBitCol,1,fontColor);
	}

	{int locBitBackCol;
	locBitBackCol = glGetUniformLocation(bitfontSP[0],"backColor");
	if(locBitBackCol == -1){SDL_Log("backColor not found in shader %d",0);}
	glUniform4fv(locBitBackCol,1,backColor);
	}

	glDrawArrays(GL_TRIANGLES,0,
			( bitfontStringBufferSize+CharWrap
			-bitfontStringBufferSize%CharWrap ) *6);
}




static unsigned char globalWindowFullscreen = 0;
static unsigned char globalWindowBorder = 0;
static unsigned char globalWindowResizable = 1;
static unsigned short globalWindowPos[2] = {0,0};
static unsigned short globalWindowRes[2] = {480,360};


void renderGlobalUpdate(void){

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width,
			isolaInfoWindow.height);


	updateBitfont();
	updateDigitfps();
}

void renderGlobalCreate(void){

	SDL_SetWindowSize(isolaWindow,globalWindowRes[0],globalWindowRes[1]);
	SDL_SetWindowPosition(isolaWindow,globalWindowPos[0],globalWindowPos[1]);
	SDL_SetWindowBordered(isolaWindow,globalWindowBorder);
	SDL_SetWindowResizable(isolaWindow,globalWindowResizable);
	if(globalWindowFullscreen){
/* 		globalWindowRes[0] = isolaInfoWindow.desktopDisplayMode.w;
		globalWindowRes[1] = isolaInfoWindow.desktopDisplayMode.h; */
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	isolaGetWindow();


	createBitfont();
	createDigitfps();


	renderGlobalUpdate();
}

void renderGlobalDestroy(void){

	destroyDigitfps();
	destroyBitfont();
}

void renderGlobalDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawBitfont();
	drawDigitfps();


	SDL_GL_SwapWindow(isolaWindow);
}


