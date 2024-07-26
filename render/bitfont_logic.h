#ifndef BITFONT_LOGIC_H
#define BITFONT_LOGIC_H




#define bitfontStringSize 256


struct BITFONT_textobject{
	char string[bitfontStringSize];
	float x, y;
	unsigned short pixelSize;
	unsigned short charWrap;
	float fontColor[4];
	float backColor[4];
}extern * bitfontObjectData;
extern unsigned int bitfontObjectAmount;




#endif
