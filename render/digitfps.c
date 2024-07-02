

#include "render_logic.h"


#include <stdio.h>


#include "mesh.h"
#include "../timer.h"




#define digitfpsPrintAmount 5

struct DIGITFPS digitfps = { .color={0.25,0.25,0.25,0.75},.pixelSize=8*3 };


static ISOLA_State digitfpsState = 0x00000001;
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
	glUniform4fv(locDigitCol,1,digitfps.color);
	}

	{int locPixSize;
	locPixSize = glGetUniformLocation(digitfpsSP,"pixelSize");
	if(locPixSize == -1){SDL_Log("pixelSize not found in shader %d",0);}
	glUniform1i(locPixSize,digitfps.pixelSize);
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


	{unsigned long delaySum = 0;
	{unsigned int i;
	for(i = 0;i<256;i++){
		delaySum += timer.frameDelay[i];
	}}

	{unsigned int i;
	for(i = 0;i<digitfpsPrintAmount;i++){
		digitfpsString[i] = 0;
	}}
	sprintf(digitfpsString,"%f",1./((delaySum/256.)/(double)timer.clockFreq) );
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
			(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))
			*digitfpsPrintAmount-1,
			(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))
			*digitfpsPrintAmount,
			GL_UNSIGNED_SHORT,(void*)0);
}
