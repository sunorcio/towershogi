#ifndef BITFONT_LOGIC_H
#define BITFONT_LOGIC_H




#define BITFONT_STRINGSIZE 256
#define BITFONT_PIXELSIZEX 8 
#define BITFONT_PIXELSIZEY 16
#define BITFONT_SEPARATOR "\177"




struct BITFONT_object{
	unsigned short pixelSize;
	unsigned short charWrap;
	char string[BITFONT_STRINGSIZE];
	float x, y;
	float foreColor[4];
	float backColor[4];
}extern bitfontData[256];




#endif
