

#include "bitmenu.h"




#include <stdlib.h>
#include <string.h>


#include <isola/isola.h>
#include <isola/misc.h>


#include <input.h>

#include <render/bitfont_logic.h>




struct BITMENU_object{
	char objectName[BITFONT_STRINGSIZE];
	struct BITMENU_object* next;
};

struct BITMENU_group{
	float x,y;
	float w,h;
	struct BITMENU_object* objectHead;
	struct BITMENU_group* next;
};

struct BITMENU_screen{
	unsigned char fontGlobalScaling;
	struct BITMENU_group* groupHead;
	struct BITMENU_screen* next;
}static * bitmenuScreenHead;




void updateBitmenu(void){

}


void createBitmenu(void){

	struct BITMENU_screen* currentScreen;
	struct BITMENU_group* currentGroup;
	struct BITMENU_object* currentObject;


	bitmenuScreenHead = calloc(1,sizeof(struct BITMENU_screen));
	currentScreen = bitmenuScreenHead;
	currentScreen->fontGlobalScaling = 2;

	currentScreen->groupHead = calloc(1,sizeof(struct BITMENU_group));
	currentGroup = currentScreen->groupHead;
	currentGroup->x =-1;
	currentGroup->y = 1;
	currentGroup->h =1;
	currentGroup->w =1;

	currentGroup->objectHead = calloc(1,sizeof(struct BITMENU_object));
	currentObject = currentGroup->objectHead;
	strcpy(currentObject->objectName,"asdf\177asdf");

	currentObject->next = calloc(1,sizeof(struct BITMENU_object));
	currentObject = currentObject->next;
	strcpy(currentObject->objectName,"asdofad\177\177asdofad");

	currentGroup->next = calloc(1,sizeof(struct BITMENU_group));
	currentGroup = currentGroup->next;
	currentGroup->x = 0;
	currentGroup->y = 0;
	currentGroup->h =1;
	currentGroup->w =1;

	currentGroup->objectHead = calloc(1,sizeof(struct BITMENU_object));
	currentObject = currentGroup->objectHead;
	strcpy(currentObject->objectName,"asdf\177asdf");

	currentObject->next = calloc(1,sizeof(struct BITMENU_object));
	currentObject = currentObject->next;
	strcpy(currentObject->objectName,"asdofad\177\177\177\177asdofad");


	bitfontObjectDataHead = calloc(1,sizeof(struct BITFONT_objectData));

/* 	strcpy(bitfontObjectDataHead->data[0].string,"asdf\177asdf");
	bitfontObjectDataHead->data[0].x = -1.;
	bitfontObjectDataHead->data[0].y = 1.;
	bitfontObjectDataHead->data[0].foreColor[0] = 0.;
	bitfontObjectDataHead->data[0].foreColor[1] = 0.;
	bitfontObjectDataHead->data[0].foreColor[2] = 0.;
	bitfontObjectDataHead->data[0].foreColor[3] = 1.;
	bitfontObjectDataHead->data[0].backColor[0] = 1.;
	bitfontObjectDataHead->data[0].backColor[1] = 1.;
	bitfontObjectDataHead->data[0].backColor[2] = 1.;
	bitfontObjectDataHead->data[0].backColor[3] = 1.;
	bitfontObjectDataHead->data[0].charWrap = BITFONT_STRINGSIZE;
	bitfontObjectDataHead->data[0].pixelSize = 2; */
}


void destroyBitmenu(void){

	void * lastDataPointer;
	struct BITMENU_screen* currentScreen;
	struct BITMENU_group* currentGroup;
	struct BITMENU_object* currentObject;
	struct BITFONT_objectData* currentDataPointer;

	currentDataPointer = bitfontObjectDataHead;
	while (currentDataPointer!=0) {
		lastDataPointer = currentDataPointer;
		currentDataPointer = currentDataPointer->next;
		free(lastDataPointer);
	}

	currentScreen = bitmenuScreenHead;
	while (currentScreen != 0) {
		currentGroup = currentScreen->groupHead;
		while (currentGroup != 0) {
			currentObject = currentGroup->objectHead;
			while (currentObject != 0) {
			
			}
		}
	}
}


void stepBitmenu(void){

}




