


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
	char option[BITFONT_STRINGSIZE];
};


struct BITMENU_screenData{
	struct BITMENU_screen data[BITMENU_SCREENCHUNK];
	struct BITMENU_screenData* next;
}* bitmenuScreenDataHead;
struct BITMENU_screenData* bitmenuScreenDataPointer;

struct BITMENU_groupData{
	struct BITMENU_group data[BITMENU_GROUPCHUNK];
	struct BITMENU_groupData* next;
}* bitmenuGroupDataHead;
struct BITMENU_groupData* bitmenuGroupDataPointer;

struct BITMENU_objectData{
	struct BITMENU_object data[BITMENU_OBJECTCHUNK];
	struct BITMENU_objectData* next;
}* bitmenuObjectDataHead;
struct BITMENU_objectData* bitmenuObjectDataPointer;




void updateBitmenu(void){

}


void createBitmenu(void){

	bitmenuScreenDataHead = calloc(1,sizeof(struct BITMENU_screenData));
	bitmenuGroupDataHead = calloc(1,sizeof(struct BITMENU_groupData));
	bitmenuObjectDataHead = calloc(1,sizeof(struct BITMENU_objectData));
	bitfontObjectDataHead = calloc(1,sizeof(struct BITFONT_objectData));

	
	strcpy(bitfontObjectDataHead->data[0].string,"asdf\77asdf");
	bitfontObjectDataHead->data[0].fontColor[0] = 0.;
	bitfontObjectDataHead->data[0].fontColor[1] = 0.;
	bitfontObjectDataHead->data[0].fontColor[2] = 0.;
	bitfontObjectDataHead->data[0].fontColor[3] = 1.;
	bitfontObjectDataHead->data[0].backColor[0] = 1.;
	bitfontObjectDataHead->data[0].backColor[1] = 1.;
	bitfontObjectDataHead->data[0].backColor[2] = 1.;
	bitfontObjectDataHead->data[0].backColor[3] = 1.;
	bitfontObjectDataHead->data[0].charWrap = BITFONT_STRINGSIZE;
	bitfontObjectDataHead->data[0].pixelSize = 2;
}


void destroyBitmenu(void){

	void* pointerBuffer = 0;

	bitmenuScreenDataPointer = bitmenuScreenDataHead;
	while (bitmenuScreenDataPointer != 0) {
		pointerBuffer = bitmenuScreenDataPointer;
		bitmenuScreenDataPointer = bitmenuScreenDataPointer->next;
		free(pointerBuffer);
	}

	bitmenuGroupDataPointer = bitmenuGroupDataHead;
	while (bitmenuGroupDataPointer != 0) {
		pointerBuffer = bitmenuGroupDataPointer;
		bitmenuGroupDataPointer = bitmenuGroupDataPointer->next;
		free(pointerBuffer);
	}

	bitmenuObjectDataPointer = bitmenuObjectDataHead;
	while (bitmenuObjectDataPointer != 0) {
		pointerBuffer = bitmenuObjectDataPointer;
		bitmenuObjectDataPointer = bitmenuObjectDataPointer->next;
		free(pointerBuffer);
	}

	bitfontObjectDataPointer = bitfontObjectDataHead;
	while (bitfontObjectDataPointer != 0) {
		pointerBuffer = bitfontObjectDataPointer;
		bitfontObjectDataPointer = bitfontObjectDataPointer->next;
		free(pointerBuffer);
	}
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
