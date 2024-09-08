#ifndef SCENE_H
#define SCENE_H




#include <timing.h>




struct SCENE_window {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
	int windowPos[2];
	int windowRes[2];
	int windowMinRes[2];
	float clearColor[4];
};


struct SCENE_state {
	unsigned char run;
	unsigned char pause;
	unsigned char returnControlValue;
};


struct SCENE_timing {
	struct TIMING_timer logicTimer;
	struct TIMING_counter frameCounter;
};


struct SCENE_scene {
	struct SCENE_state state;
	struct SCENE_window window;
	struct SCENE_timing timing;
}extern * currentScene;




#endif
