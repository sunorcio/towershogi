#ifndef RENDER_H
#define RENDER_H




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
struct BITFONT_group{
	unsigned char groupEnable;
	struct BITFONT_object textObject[16];
};
struct BITFONT_screen{
	struct BITFONT_group group[8];
};
struct BITFONT_menu{
	unsigned int currentScreen;
	struct BITFONT_screen screen[4];
}extern bitfontMenu;




extern void renderGlobalUpdate(void);
extern void renderGlobalCreate(void);
extern void renderGlobalDestroy(void);
extern void renderGlobalDraw(void);




#endif
