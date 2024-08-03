


#include "bitmenu.h"




#include <stdlib.h>
#include <string.h>


#include <isola/isola.h>
#include <isola/misc.h>


#include <input.h>

#include <render/bitfont_logic.h>




#define BITMENU_SCREENCHUNK 8
#define BITMENU_GROUPCHUNK 32
#define BITMENU_OBJECTCHUNK 256


struct BITMENU_screen{
	unsigned char fontScaleGlobal;
	unsigned int groupAmount;
	struct BITMENU_group* group;
};


struct BITMENU_group{
	unsigned char visible;
	unsigned char highlighted;
	unsigned char background;
	unsigned char axis;
	unsigned char alignmentX;
	unsigned char alignmentY;
	unsigned short wrap;
	unsigned short fontScale;
	float x,y;
	float w,h;
	float fontColor[4];
	float backColor[4];
	unsigned int objectAmount;
	struct BITMENU_object* object;
};


struct BITMENU_object{
	char name[BITFONT_STRINGSIZE];
};


struct BITMENU_screenData{
	struct BITMENU_screen data[BITMENU_SCREENCHUNK];
	struct BITMENU_screenData* next;
}* bitmenuScreenData;


struct BITMENU_groupData{
	struct BITMENU_group data[BITMENU_GROUPCHUNK];
	struct BITMENU_groupData* next;
}* bitmenuGroupData;


struct BITMENU_objectData{
	struct BITMENU_object data[BITMENU_OBJECTCHUNK];
	struct BITMENU_objectData* next;
}* bitmenuObjectData;




void updateBitmenu(void){

}


void createBitmenu(void){

	bitmenuScreenData = calloc(1,sizeof(struct BITMENU_screenData));
	bitmenuGroupData = calloc(1,sizeof(struct BITMENU_groupData));
	bitmenuObjectData = calloc(1,sizeof(struct BITMENU_objectData));
	bitfontObjectData = calloc(1,sizeof(struct BITFONT_objectData));

	
	strcpy(bitfontObjectData->data[0].string,"sdfasdfasdfadsf");
	bitfontObjectData->data[0].fontColor[0] = 0.;
	bitfontObjectData->data[0].fontColor[1] = 0.;
	bitfontObjectData->data[0].fontColor[2] = 0.;
	bitfontObjectData->data[0].fontColor[3] = 1.;
	bitfontObjectData->data[0].backColor[0] = 1.;
	bitfontObjectData->data[0].backColor[1] = 1.;
	bitfontObjectData->data[0].backColor[2] = 1.;
	bitfontObjectData->data[0].backColor[3] = 1.;
	bitfontObjectData->data[0].charWrap = BITFONT_STRINGSIZE;
	bitfontObjectData->data[0].pixelSize = 2;
}


void destroyBitmenu(void){

	free(bitmenuScreenData);
	free(bitmenuGroupData);
	free(bitmenuObjectData);
}


void stepBitmenu(void){

}




/* 	if (keyState[SDL_SCANCODE_J] && !keyRepeat[SDL_SCANCODE_J]) {
		bitmenuGroup[0].currentObject++;
		if (bitmenuGroup[0].currentObject==bitmenuGroup[0].objectAmount) {
			bitmenuGroup[0].currentObject = 0;
		}
	}
	if (keyState[SDL_SCANCODE_K] && !keyRepeat[SDL_SCANCODE_K]) {
		if (bitmenuGroup[0].currentObject==0) {
			bitmenuGroup[0].currentObject = bitmenuGroup[0].objectAmount;
		}
		bitmenuGroup[0].currentObject--;
	} */
