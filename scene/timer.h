#ifndef TIMER_H
#define TIMER_H




#include <isola/isola.h>




struct TIMER_timer{
	unsigned long fps;
	unsigned long sps;
	unsigned long clockFreq;
	unsigned long lastFrame;
	unsigned long lastStep;
};




#endif
