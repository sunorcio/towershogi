#ifndef RENDER_LOGIC_H
#define RENDER_LOGIC_H




#include <isola/isola.h>
#include <isola/mutil.h>




struct DIGITFPS{
	float color[4];
	int unsigned pixelSize;
}extern digitfps;




#define bitfontStringSize 256
struct BITFONT_object{
	char string[bitfontStringSize];
	float x;
	float y;
	unsigned short pixelSize;
	unsigned short charWrap;
	float fontColor[4];
	float backColor[4];
}extern * bitfontData;
extern unsigned int bitfontDataSize;




#endif
