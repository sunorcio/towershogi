

#include "digitfps_render.h"
#include "digitfps_logic.h"




#include <stdio.h>

#include <isola/isola.h>
#include <isola/mutil.h>

#include <timing.h>




static float vertexDigit[] = {
	 0.,  0. ,      0.5,  0. ,      1.,  0. ,
	 0.,  0.5,      0.5,  0.5,      1.,  0.5,
	 0.,  1. ,      0.5,  1. ,      1.,  1. ,
	 0.,  1.5,      0.5,  1.5,      1.,  1.5,
	 0.,  2. ,      0.5,  2. ,      1.,  2. ,
};
static unsigned short elementDigit[12][12] = {
	{  0,  0,  0,     0,  0,  0,     0,  0,  0,     0,  0,  0 },
	{  1,  6,  3,     1,  2,  8,     6, 13, 12,     8, 11, 13 },
	{  1, 11, 10,     9, 11, 13,     0,  0,  0,     0,  0,  0 },
	{  0,  2,  3,     3, 11, 10,     9, 11, 13,     0,  0,  0 },
	{  0,  2,  4,     2,  5,  6,     6, 11, 14,    10, 14, 12 },
	{  2,  8,  7,     6,  8, 11,     6, 14, 13,     0,  0,  0 },
	{  1,  5,  3,     4,  5,  9,     9, 14, 12,     0,  0,  0 },
	{  0,  1,  5,     0,  7,  6,     5,  8,  7,     6, 13, 12 },
	{  0,  1, 14,     9, 14, 12,     0,  0,  0,     0,  0,  0 },
	{  0,  1,  5,     0,  7,  9,     5, 14,  7,     9, 14, 13 },
	{  1,  2,  8,     6,  7,  9,     7,  8, 14,     9, 14, 13 },
	{  1,  5,  3,     0,  0,  0,     0,  0,  0,     0,  0,  0 },
};




/* [0-18] */
#define digitfpsPrintAmount 6


struct DIGITFPS_font digitfps = { 
		.color={0.375,0.1875,0.375,0.625},
		.pixelSize=8*2 };


static struct TIMING_counter* digitfpsCounter = {0};


static ISOLA_State digitfpsState = 0x00000001;
static unsigned int digitfpsSP;
static unsigned int digitfpsVAO;
static unsigned int digitfpsEBO;
static char digitfpsString[18] = {0};
static unsigned short digitfpsED[18]
		[sizeof(elementDigit[0])/sizeof(elementDigit[0][0])] = {0};


void updateDigitfps(void){

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


void createDigitfps(struct TIMING_counter* counter){

	if (counter != 0) {
		digitfpsCounter = counter;
	}


	glGenVertexArrays(1,&digitfpsVAO);
	glGenBuffers(1,&digitfpsEBO);

	glBindVertexArray(digitfpsVAO);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,digitfpsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(digitfpsED),
			digitfpsED,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
			sizeof(digitfpsED),digitfpsED);


	digitfpsSP = isolaShaderProgram("glsl/digitfps.vert","glsl/digitfps.frag");


	updateDigitfps();
}


void destroyDigitfps(void){

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(digitfpsSP);
	glDeleteBuffers(1,&digitfpsEBO);
	glDeleteVertexArrays(1,&digitfpsVAO);
}


void drawDigitfps(void){

	isolaSetState(digitfpsState);


	{unsigned long delaySum = 0;
	{unsigned int i;
	for(i = 0;i<256;i++){
		delaySum += digitfpsCounter->stepDelay[i];
	}}

	{unsigned int i;
	for(i = 0;i<digitfpsPrintAmount;i++){
		digitfpsString[i] = 0;
	}}
	sprintf(digitfpsString,"%f",
			1./((delaySum/256.)/(double)clockFreq) );
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
			( sizeof(digitfpsED)/18 )*digitfpsPrintAmount,digitfpsED);
	glUseProgram(digitfpsSP);

	glDrawRangeElements(GL_TRIANGLES,0,
			(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))
			*digitfpsPrintAmount-1,
			(sizeof(elementDigit[0])/sizeof(elementDigit[0][0]))
			*digitfpsPrintAmount,
			GL_UNSIGNED_SHORT,(void*)0);
}




