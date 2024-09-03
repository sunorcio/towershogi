



#include "timing.h"




unsigned long clockFreq = 0;




void timerSetup(struct TIMING_timer* timer,unsigned long stepsPerSecond){

	clockFreq = SDL_GetPerformanceFrequency();


	timer->sps = stepsPerSecond;
	timer->lastStep = SDL_GetPerformanceCounter();
}


void counterSetup(struct TIMING_counter* counter,unsigned long stepsPerSecond){

	clockFreq = SDL_GetPerformanceFrequency();


	counter->sps = stepsPerSecond;

	{unsigned int i;
	for(i = 0;i<256;i++){
		counter->stepDelay[i] = clockFreq/counter->sps;
	}}

	counter->lastStep = SDL_GetPerformanceCounter();
}




unsigned char timerStep(struct TIMING_timer* timer){

	timer->currentStep = SDL_GetPerformanceCounter();
	if(timer->currentStep >= timer->lastStep + clockFreq/timer->sps){

		timer->lastStep = timer->currentStep;
		return 1;
	}

	return 0;
}


unsigned char counterStep(struct TIMING_counter* counter){

	counter->currentStep = SDL_GetPerformanceCounter();
	if(counter->currentStep >= counter->lastStep + clockFreq/counter->sps){
		counter->stepDelay[counter->stepIndex] = counter->currentStep-
				counter->lastStep;
		counter->stepIndex++;

		counter->lastStep = counter->currentStep;
		return 1;
	}

	return 0;
}




