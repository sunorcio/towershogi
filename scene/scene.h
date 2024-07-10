#ifndef SCENE_H
#define SCENE_H




#include <isola/isola.h>
#include "timer.h"




struct SCENE {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
	int windowPos[2];
	int windowRes[2];
	float clearcolor[4];
	struct TIMER_timer timer;
};




#endif
