#ifndef TIMING_H
#define TIMING_H




#include <isola/isola.h>




extern unsigned long clockFreq;




struct TIMING_timer{
	unsigned long sps;
	unsigned long currentStep;
	unsigned long lastStep;
};


extern void timerSetup(struct TIMING_timer* timer,
		unsigned long stepsPerSecond);


extern unsigned char timerStep(struct TIMING_timer* timer);




struct TIMING_counter{
	unsigned long sps;
	unsigned long currentStep;
	unsigned long lastStep;
	unsigned long stepDelay[256];
	unsigned char stepIndex;
};


extern void counterSetup(struct TIMING_counter* counter, 
		unsigned long stepsPerSecond);


extern unsigned char counterStep(struct TIMING_counter* timer);




#endif
