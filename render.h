#ifndef RENDER_H
#define RENDER_H




#include "isola/isola.h"




extern unsigned char digitfpsBool;
extern unsigned int digitfpsPixelsize;
extern float digitfpsColor[4];




extern void renderGlobalUpdate(void);
extern void renderGlobalCreate(void);
extern void renderGlobalDestroy(void);
extern void renderGlobalDraw(void);




#endif
