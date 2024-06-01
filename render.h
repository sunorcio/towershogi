#ifndef RENDER_H
#define RENDER_H




#include "isola/isola.h"




extern unsigned char b_DigitFPS;
extern unsigned int digitPixelsize;
extern float digitColor[4];
extern void updateDigitFPS(void);




extern float bitfontPos[2];
extern unsigned int bitfontPixelSize;
extern unsigned int bitfontCharWrap;
extern float bitfontColor[4];
extern float bitfontBackColor[4];
extern void updateBitmapFont(void);




extern void renderGlobalUpdate(void);
extern void renderGlobalCreate(void);
extern void renderGlobalDestroy(void);
extern void renderGlobalDraw(void);




#endif
