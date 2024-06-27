#ifndef TIMER_H
#define TIMER_H




struct TIMER{
	unsigned long isolaFPS;
	unsigned long isolaSPS;
	unsigned long clockFreq;
	unsigned long lastFrame;
	unsigned long lastStep;
	unsigned long frameDelay[256];
}extern timer;




/* #define TIMER_IMPLEMENTATION // lsptemp */
#ifdef TIMER_IMPLEMENTATION


#include "isola/isola.h"




struct TIMER timer = {0};
static unsigned char frameIndex = 0;




void setupTimer(void){

	timer.isolaFPS = 60;
	timer.isolaSPS = 60;
	timer.clockFreq = SDL_GetPerformanceFrequency();
	timer.lastFrame = SDL_GetPerformanceCounter();
	timer.lastStep = SDL_GetPerformanceCounter();
	{unsigned int i;
	for(i = 0;i<256;i++){
		timer.frameDelay[i] = timer.clockFreq/timer.isolaFPS;
	}}
}


#endif




#endif
