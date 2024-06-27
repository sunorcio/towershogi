#ifndef RENDER_H
#define RENDER_H




#include "isola/isola.h"




struct DIGITFPS{
	float color[4];
	int unsigned pixelsize;
}extern digitfps;




#define bitfontStringBufferSize 1024
struct BITFONT{
	char textString[bitfontStringBufferSize];
	float x;
	float y;
	unsigned int pixelSize;
	unsigned int charWrap;
	float fontColor[4];
	float backColor[4];
}extern bitfont;




extern void renderGlobalUpdate(void);
extern void renderGlobalCreate(void);
extern void renderGlobalDestroy(void);
extern void renderGlobalDraw(void);




#endif
