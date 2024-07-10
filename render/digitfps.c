



#include "digitfps_render.h"
#include "digitfps_logic.h"


#include <stdio.h>


#include <isola/isola.h>
#include <isola/mutil.h>
#include <scene/timer.h>




#define digitfpsPrintAmount 5

struct DIGITFPS_font digitfps = { 
		.color={0.25,0.25,0.25,0.75},
		.pixelSize=8*3 };


struct DIGITFPS_counter{
	unsigned long fps;
	unsigned long clockFreq;
	unsigned long frameDelay[256];
	unsigned char frameIndex;
}static digitfpsCounter = {0};


void digitfpsCount(unsigned long frameDelay){
	digitfpsCounter.frameDelay[digitfpsCounter.frameIndex] = frameDelay;
	digitfpsCounter.frameIndex++;
}


static ISOLA_State digitfpsState = 0x00000001;
static unsigned int digitfpsSP;
static unsigned int digitfpsVAO;
static unsigned int digitfpsEBO;
static char digitfpsString[16] = {0};
static unsigned short digitfpsED[16]
		[sizeof(elementDigit[0])/sizeof(elementDigit[0][0])] = {0};


void updateDigitfps(struct TIMER_timer* timer){
	
	if (timer != 0) {

		digitfpsCounter.fps = timer->fps;
		digitfpsCounter.clockFreq = timer->clockFreq;
		{unsigned int i;
		for(i = 0;i<256;i++){
			digitfpsCounter.frameDelay[i] = digitfpsCounter.clockFreq/
					digitfpsCounter.fps;
		}}
	}


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


void createDigitfps(struct TIMER_timer* timer){

	glGenVertexArrays(1,&digitfpsVAO);
	glGenBuffers(1,&digitfpsEBO);

	glBindVertexArray(digitfpsVAO);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,digitfpsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(digitfpsED),
			digitfpsED,GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
			sizeof(digitfpsED),digitfpsED);


	digitfpsSP = isolaShaderProgram("glsl/digitfps.vert","glsl/digitfps.frag");


	updateDigitfps(timer);
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
		delaySum += digitfpsCounter.frameDelay[i];
	}}

	{unsigned int i;
	for(i = 0;i<digitfpsPrintAmount;i++){
		digitfpsString[i] = 0;
	}}
	sprintf(digitfpsString,"%f",
			1./((delaySum/256.)/(double)digitfpsCounter.clockFreq) );
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




