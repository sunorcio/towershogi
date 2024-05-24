#ifndef RENDER_H
#define RENDER_H




#include "isola.h"




extern unsigned char b_DigitFPS;
extern float digitColor[4];
extern void updateDigitFPS(void);




extern void renderGlobalCreate(void);
extern void renderGlobalDestroy(void);
extern void renderGlobalDraw(void);




#endif
