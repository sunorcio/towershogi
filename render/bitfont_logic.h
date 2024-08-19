#ifndef BITFONT_LOGIC_H
#define BITFONT_LOGIC_H




#define BITFONT_OBJECTCHUNK 256
#define BITFONT_STRINGSIZE 256
#define BITFONT_PIXELSIZEX 8 
#define BITFONT_PIXELSIZEY 16
#define BITFONT_SEPARATOR "\177"




struct BITFONT_object{
	char string[BITFONT_STRINGSIZE];
	float x, y;
	unsigned short pixelSize;
	unsigned short charWrap;
	float foreColor[4];
	float backColor[4];
};


struct BITFONT_objectData{
	struct BITFONT_object data[BITFONT_OBJECTCHUNK];
	struct BITFONT_objectData* next;
}extern * bitfontObjectDataHead;
extern struct BITFONT_objectData* bitfontObjectDataPointer;




#endif
