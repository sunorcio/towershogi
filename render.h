#ifndef RENDER_H
#define RENDER_H




#include "isola/isola.h"




struct SCENE {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
	int windowPos[2];
	int windowRes[2];
};




struct DIGITFPS{
	float color[4];
	int unsigned pixelSize;
}extern digitfps;


#define bitfontStringBufferSize 1024
struct BITFONT{
	char textString[bitfontStringBufferSize];
	float x;
	float y;
	unsigned short pixelSize;
	unsigned short charWrap;
	float fontColor[4];
	float backColor[4];
}extern bitfontexample[2];




extern void renderGlobalUpdate(void);
extern void renderGlobalCreate(void);
extern void renderGlobalDestroy(void);
extern void renderGlobalDraw(void);




#endif
