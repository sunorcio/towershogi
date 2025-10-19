#include "digitfps.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/timing.h>


#include <scene/scene.h>




/* [0-18] */
#define digitfpsPrintAmount 6


static const float digitfps_vertex[] = {
	 0.,  0. ,      0.5,  0. ,      1.,  0. ,
	 0.,  0.5,      0.5,  0.5,      1.,  0.5,
	 0.,  1. ,      0.5,  1. ,      1.,  1. ,
	 0.,  1.5,      0.5,  1.5,      1.,  1.5,
	 0.,  2. ,      0.5,  2. ,      1.,  2. ,
};

static const unsigned short digitfps_element[12][12] = {
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




struct DIGITFPS_font digitfps_option = { 
		{0.375,0.1875,0.375,0.625},
		8 };


static struct ISOLA_counter* digitfps_counter = {0};
static char digitfps_string[18] = {0};


static enum ISOLA_state digitfps_glstate = 0x00000001;
static unsigned int digitfps_shaderProg;
static unsigned int digitfps_vertArrObj;
static unsigned int digitfps_eleBufObj;
static unsigned short digitfps_eleData[18]
		[sizeof(digitfps_element[0])/
		sizeof(digitfps_element[0][0])] = {0};




void digitfps_update(void){

	ISOLA_GLDBG_( glUseProgram(digitfps_shaderProg) )

	{int loc;
	float matProj[4*4] = {0};
	float pixelScale;

	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio, 0.25,8.,matProj);
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			digitfps_shaderProg,"matProj"); )
	if(loc == -1){ SDL_Log("digitfps: matProj not found"); }
	ISOLA_GLDBG_( glUniformMatrix4fv(loc,1,GL_FALSE,matProj); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			digitfps_shaderProg,"digitColor"); )
	if(loc == -1){SDL_Log("digitfps: digitColor not found");}
	ISOLA_GLDBG_( glUniform4fv(loc,1,digitfps_option.color); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			digitfps_shaderProg,"sizePixelWidth"); )
	if(loc == -1){SDL_Log("digitfps: sizePixelWidth not found");}
	ISOLA_GLDBG_( glUniform1i(loc,digitfps_option.sizePixelWidth); )

	if (isola_info_window.xRatio == 1.f) {
		pixelScale = isola_info_window.pixelWidth;
	}else{ 
		pixelScale = isola_info_window.pixelHeight;
	}
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			digitfps_shaderProg,"pixelScale"); )
	if(loc == -1){SDL_Log("digitfps: pixelScale not found");}
	ISOLA_GLDBG_( glUniform1f(loc,pixelScale); )
	}
}


void digitfps_create(void){

	digitfps_counter = &currentScene->timing.frameCounter;


	ISOLA_GLDBG_( glGenVertexArrays(1,&digitfps_vertArrObj) )
	ISOLA_GLDBG_( glGenBuffers(1,&digitfps_eleBufObj) )

	ISOLA_GLDBG_( glBindVertexArray(digitfps_vertArrObj) )


	ISOLA_GLDBG_( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,digitfps_eleBufObj) )
	ISOLA_GLDBG_( glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(digitfps_eleData),
			digitfps_eleData,GL_DYNAMIC_DRAW) )
	ISOLA_GLDBG_( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
			sizeof(digitfps_eleData),digitfps_eleData) )


	digitfps_shaderProg = isola_shader_buildProgram(
			"module/digitfps/glsl/digitfps.vert",
			"module/digitfps/glsl/digitfps.frag");


	ISOLA_GLDBG_( glUseProgram(digitfps_shaderProg) )

	{int loc;
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			digitfps_shaderProg,"digitAmount"); )
	if(loc == -1){SDL_Log("digitfps: digitAmount not found");}
	ISOLA_GLDBG_( glUniform1i(loc,digitfpsPrintAmount); )
	}


	digitfps_update();
}


void digitfps_destroy(void){

	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(digitfps_shaderProg);
	glDeleteBuffers(1,&digitfps_eleBufObj);
	glDeleteVertexArrays(1,&digitfps_vertArrObj);
}


void digitfps_draw(void){

	isola_set_state(digitfps_glstate);


	{Uint64 delaySum = 0;
	{unsigned int i;
	for(i = 0;i<256;i++){
		delaySum += digitfps_counter->stepDelay[i];
	}}

	{unsigned short i;
	for(i = 0;i<digitfpsPrintAmount;i++){
		digitfps_string[i] = 0;
	}}
	SDL_snprintf(digitfps_string,sizeof(digitfps_string),"%f",
			1./(((double)delaySum/256.)/(double)isola_clockFreq) );
	}

	{unsigned short i;
	unsigned char digit;
	for(i = 0;i<digitfpsPrintAmount;i++){
		switch (digitfps_string[i]) {
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
		for(j = 0;j<sizeof(digitfps_element[0])/
				sizeof(digitfps_element[0][0]);j++){
			digitfps_eleData[i][j] = digitfps_element[digit][j]+15*i;
		}}
	}}

	ISOLA_GLDBG_( glBindVertexArray(digitfps_vertArrObj) )
	ISOLA_GLDBG_( glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,
			( sizeof(digitfps_eleData)/18 )*digitfpsPrintAmount,digitfps_eleData) )
	ISOLA_GLDBG_( glUseProgram(digitfps_shaderProg) )

	ISOLA_GLDBG_( glDrawRangeElements(GL_TRIANGLES,0,
			(sizeof(digitfps_element[0])/sizeof(digitfps_element[0][0]))
			*digitfpsPrintAmount-1,
			(sizeof(digitfps_element[0])/sizeof(digitfps_element[0][0]))
			*digitfpsPrintAmount,
			GL_UNSIGNED_SHORT,(void*)0) )
}




