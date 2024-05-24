#ifndef TIMER_H
#define TIMER_H




extern unsigned long clockFreq;
extern unsigned long lastFrame;
extern unsigned long lastStep;
extern unsigned long frameDelay[255];




/* #define TIMER_IMPLEMENTATION */
#ifdef TIMER_IMPLEMENTATION


#include "isola.h"




unsigned long clockFreq = 0;
unsigned long lastFrame = 0;
unsigned long lastStep = 0;
unsigned long frameDelay[255] = {0};
static unsigned char currentFrame = 0;
static unsigned short isolaFPS = 60;
static unsigned int isolaSPS = 60;




void setupTimer(void){
	clockFreq = SDL_GetPerformanceFrequency();
	lastFrame = SDL_GetPerformanceCounter();
	lastStep = SDL_GetPerformanceCounter();
}


#endif




#endif
