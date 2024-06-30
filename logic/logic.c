

#include "logic.h"


#include "../render/render_logic.h"




/* static unsigned char levelsize[2] = {8,16}; */




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


struct BITFONT_menu bitfontMenu = {

			.screen[0].group[0].textObject[0] = {
				.string="NEW GAME",
				.x=0.,.y=0.,
				.pixelSize=8*2, .charWrap=strlen("NEW GAME"),
				.fontColor={0.,0.,0.,1.},
				.backColor={1.,1.,1.,1.},
			},
			.screen[0].group[0].textObject[1] = {
				.string="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
				.x=-1.,.y=1.,
				.pixelSize=8*2, .charWrap=50,
				.fontColor={1.,0.,0.,1.},
				.backColor={1.,1.,1.,1.},
			},
		};


void buildmenu(void){

	bitfontDataSize = 2;
	bitfontData = calloc(bitfontDataSize,sizeof(struct BITFONT_object));

	strcpy(bitfontData[0].string,"NEW GAME");
	bitfontData[0].x = 0.;
	bitfontData[0].y = 0.;
	bitfontData[0].pixelSize = 8*2;
	bitfontData[0].charWrap = strlen("NEW GAME");
	bitfontData[0].fontColor[0] = 0.;
	bitfontData[0].fontColor[1] = 0.;
	bitfontData[0].fontColor[2] = 0.;
	bitfontData[0].fontColor[3] = 1.;
	bitfontData[0].backColor[0] = 1.;
	bitfontData[0].backColor[1] = 1.;
	bitfontData[0].backColor[2] = 1.;
	bitfontData[0].backColor[3] = 1.;

	strcpy(bitfontData[1].string,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	bitfontData[1].x = -1.;
	bitfontData[1].y = 1.;
	bitfontData[1].pixelSize = 8*2;
	bitfontData[1].charWrap = 50;
	bitfontData[1].fontColor[0] = 1.;
	bitfontData[1].fontColor[1] = 0.;
	bitfontData[1].fontColor[2] = 0.;
	bitfontData[1].fontColor[3] = 1.;
	bitfontData[1].backColor[0] = 1.;
	bitfontData[1].backColor[1] = 1.;
	bitfontData[1].backColor[2] = 1.;
	bitfontData[1].backColor[3] = 1.;
}

