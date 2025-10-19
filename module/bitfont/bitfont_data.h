#ifndef BITFONT_DATA_H
#define BITFONT_DATA_H




#define BITFONT_STRINGSIZE 256
#define BITFONT_PIXELSIZEX 8
#define BITFONT_PIXELSIZEY 16
#define BITFONT_SEPARATOR "\177"




struct BITFONT_object{
	unsigned short sizeMult;
	unsigned short charWrap;
	char string[BITFONT_STRINGSIZE];
	float x, y;
	float foreColor[4];
	float backColor[4];
};


struct BITFONT_data{
	unsigned int size;
	struct BITFONT_object* data;
};


extern struct BITFONT_data* bitfont_requestPtr(unsigned int size);
extern void bitfont_freePtr(struct BITFONT_data* ptr);




#endif
