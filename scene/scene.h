#ifndef SCENE_H
#define SCENE_H




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




#endif
