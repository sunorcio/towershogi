#ifndef SCENE_H
#define SCENE_H




#include <isola/isola.h>




struct SCENE_scene {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
	int windowPos[2];
	int windowRes[2];
	float clearColor[4];
};


extern void sceneSetup(struct SCENE_scene* scene);




#endif
