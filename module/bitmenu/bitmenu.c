

#include "bitmenu_logic.h"




#include <stdlib.h>
#include <string.h>


#include <isola/isola.h>
#include <isola/misc.h>

#include <input.h>

#include <module/bitfont/bitfont_logic.h>




struct BITMENU_object{
	char name[BITFONT_STRINGSIZE];
	void (*function)(void);
	struct BITMENU_group* groupLink;
	struct BITMENU_object* objectNext;
};

struct BITMENU_group{
	struct BITMENU_screen* screenParent;
	float x,y;
	float w,h;
	struct BITMENU_object* objectHead;
	struct BITMENU_group* groupPrevious;
};

struct BITMENU_screen{
	unsigned char fontScaleGlobal;
	struct BITMENU_group* groupTransition;
	struct BITMENU_screen* screenNext;
};

struct BITMENU_menu{
	struct BITMENU_group* groupCurrent;
	struct BITMENU_object* objectCurrent;
	struct BITMENU_screen* screenHead;
	struct BITMENU_group* groupHead;
	struct BITMENU_menu* menuNext;
}static * bitmenuHead = 0, * bitmenuCurrent = 0, * bitmenuCounter;


static struct BITFONT_object* bitfontCounter;


static struct BITMENU_menu* menuPointer = 0;
static struct BITMENU_screen* screenPointer = 0;
static struct BITMENU_group* groupPointer = 0;
static struct BITMENU_object* objectPointer = 0;




void bitmenuGrouptreeDelete(struct BITMENU_group* group){

	void* last;
	struct BITMENU_object* object;


	if (group == 0) {return;}

	object = group->objectHead;
	while (object != 0) {

		bitmenuGrouptreeDelete(object->groupLink);
	
		last = object;
		object = object->objectNext;
		free(last);
	}

	free(group);
}

void bitmenuGrouptreeStep(struct BITMENU_group* group){

	struct BITMENU_object* object;
	unsigned int o = 0;


	if (group == 0) {return;}
	if (group->screenParent != bitmenuCounter->groupCurrent->screenParent) {
		return;
	}

	object = group->objectHead;
	while (object != 0) {

		bitfontCounter->pixelSize = 
				bitmenuCounter->groupCurrent->screenParent->fontScaleGlobal;
		bitfontCounter->charWrap = BITFONT_STRINGSIZE;
		strcpy(bitfontCounter->string,object->name);
		bitfontCounter->x = group->x;
		bitfontCounter->y = group->y
				-isolaInfoWindow.pixelHeight*16
				*bitfontCounter->pixelSize*o;
		bitfontCounter->foreColor[0] = 0.;
		bitfontCounter->foreColor[1] = 0.;
		bitfontCounter->foreColor[2] = 0.;
		bitfontCounter->foreColor[3] = 1.;
		bitfontCounter->backColor[0] = 1.
				*(bitmenuCounter->objectCurrent==object);
		bitfontCounter->backColor[1] = 1.
				*(bitmenuCounter->objectCurrent==object);
		bitfontCounter->backColor[2] = 1.;
		bitfontCounter->backColor[3] = 0.5
				+0.5*(bitmenuCounter->groupCurrent==group);

		if (bitfontCounter != &bitfontData[isolaARRAYSIZE_(bitfontData)-1]) {
			bitfontCounter += 1;
		}else {
			SDL_Log("%s","REACHED BITFONT OBJECT LIMIT");
		}

		bitmenuGrouptreeStep(object->groupLink);

		object = object->objectNext;
		o += 1;
	}
}


void bitmenuMoveForward(void){

	if (bitmenuCurrent->objectCurrent->objectNext != 0) {
		bitmenuCurrent->objectCurrent = 
				bitmenuCurrent->objectCurrent->objectNext;
	}
}

void bitmenuMoveBack(void){

	struct BITMENU_object* object;


	object = bitmenuCurrent->groupCurrent->objectHead;

	if (object == bitmenuCurrent->objectCurrent) {return;}

	while (object->objectNext != bitmenuCurrent->objectCurrent) {
		object = object->objectNext;
	}

	bitmenuCurrent->objectCurrent = object;
}

void bitmenuActivateObject(void){

	if (bitmenuCurrent->objectCurrent->function != 0) {
		bitmenuCurrent->objectCurrent->function();
	}

	if (bitmenuCurrent->objectCurrent->groupLink != 0) {
		bitmenuCurrent->groupCurrent = bitmenuCurrent->objectCurrent->groupLink;
		bitmenuCurrent->objectCurrent =
				bitmenuCurrent->groupCurrent->objectHead;
	}
}

void bitmenuLeaveGroup(void){

	if (bitmenuCurrent->groupCurrent->groupPrevious == 0) { return; }

	bitmenuCurrent->groupCurrent = bitmenuCurrent->groupCurrent->groupPrevious;
	bitmenuCurrent->objectCurrent = bitmenuCurrent->groupCurrent->objectHead;
}


void bitmenuBuildMenu(void){

	if (menuPointer != 0) {
		menuPointer->menuNext = calloc(1,sizeof(struct BITMENU_menu));
		menuPointer = menuPointer->menuNext;
	}else {
		menuPointer = calloc(1,sizeof(struct BITMENU_menu));
		bitmenuHead = menuPointer;
		bitmenuCurrent = menuPointer;
	}
}

void bitmenuBuildScreen(unsigned char fontScaleGlobal){

	if (menuPointer->screenHead != 0) {
		screenPointer->screenNext = calloc(1,sizeof(struct BITMENU_screen));
		screenPointer = screenPointer->screenNext;
		groupPointer->screenParent = screenPointer;
		screenPointer->groupTransition = groupPointer;
		screenPointer->fontScaleGlobal = fontScaleGlobal;
	
	}else {
		screenPointer = calloc(1,sizeof(struct BITMENU_screen));
		menuPointer->screenHead = screenPointer;
		groupPointer->screenParent = screenPointer;
		screenPointer->groupTransition = groupPointer;
		screenPointer->fontScaleGlobal = fontScaleGlobal;
	}
}

void bitmenuBuildGroup(float x, float y, float w, float h){

	if (menuPointer->groupHead != 0) {
		objectPointer->groupLink = calloc(1,sizeof(struct BITMENU_group));
		objectPointer->groupLink->groupPrevious = groupPointer;
		groupPointer = objectPointer->groupLink;
		groupPointer->screenParent = screenPointer;
		groupPointer->x = x;
		groupPointer->y = y;
		groupPointer->w = w;
		groupPointer->h = h;
	}else {
		groupPointer = calloc(1,sizeof(struct BITMENU_group));
		groupPointer->screenParent = screenPointer;
		groupPointer->x = x;
		groupPointer->y = y;
		groupPointer->w = w;
		groupPointer->h = h;
		menuPointer->groupHead = groupPointer;
		menuPointer->groupCurrent = menuPointer->groupHead;
	}
}

void bitmenuBuildObject(const char* name, void (*function)(void)){

	if (groupPointer->objectHead != 0) {
		objectPointer->objectNext = calloc(1,sizeof(struct BITMENU_object));
		objectPointer = objectPointer->objectNext;

		strcpy(objectPointer->name,name);
		objectPointer->function = function;
	}else {
		objectPointer = calloc(1,sizeof(struct BITMENU_object));
		strcpy(objectPointer->name,name);
		objectPointer->function = function;

		groupPointer->objectHead = objectPointer;
		if (menuPointer->objectCurrent == 0) {
			menuPointer->objectCurrent = menuPointer->groupHead->objectHead;
		}
	}
}

void bitmenuBuildGroupLeave(void){

	groupPointer = groupPointer->groupPrevious;
	objectPointer = groupPointer->objectHead;
	while (objectPointer->objectNext!=0) {
		objectPointer=objectPointer->objectNext;
	}
}

void bitmenuBuildExample(void){

	bitmenuBuildMenu();
	bitmenuBuildGroup(-1,1,1,1);
	bitmenuBuildScreen(2);
	bitmenuBuildObject("menu 1, screen 1, group 1",0);
	bitmenuBuildObject("go to group 2",0);
	bitmenuBuildGroup(-1,0,1,1);
		bitmenuBuildObject("menu 1, screen 1, group 2",0);
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("go to screen 2",0);
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(3);
		bitmenuBuildObject("menu 1, screen 2, group 3",0);
		bitmenuBuildObject("go to group 4",0);
		bitmenuBuildGroup(-1,0,1,1);
			bitmenuBuildObject("menu 1, screen 2, group 4",0);
		bitmenuBuildGroupLeave();
	bitmenuBuildGroupLeave();

	bitmenuBuildMenu();
	bitmenuBuildGroup(0,1,1,1);
	bitmenuBuildScreen(1);
	bitmenuBuildObject("menu 2, screen 1, group 1",0);
}




void updateBitmenu(void){

}


void createBitmenu(void){

}


void destroyBitmenu(void){

	void* last;
	struct BITMENU_menu* menu;
	struct BITMENU_screen* screen;


	menu = bitmenuHead;
	while (menu != 0) {

		screen = menu->screenHead;
		while (screen != 0) {

			last = screen;
			screen = screen->screenNext;
			free(last);
		}

		bitmenuGrouptreeDelete(menu->groupHead);

		last = menu;
		menu = menu->menuNext;
		free(menu);
	}
}


void stepBitmenu(void){

	if (keyState[SDL_SCANCODE_J] && !keyRepeat[SDL_SCANCODE_J]) {
		bitmenuMoveForward();
	}
	if (keyState[SDL_SCANCODE_K] && !keyRepeat[SDL_SCANCODE_K]) {
		bitmenuMoveBack();
	}

	if (keyState[SDL_SCANCODE_Q] && !keyRepeat[SDL_SCANCODE_Q]) {
		bitmenuLeaveGroup();
	}else if(keyState[SDL_SCANCODE_RETURN] && !keyRepeat[SDL_SCANCODE_RETURN]) {
		bitmenuActivateObject();
	}


	memset(bitfontData,0,sizeof(bitfontData));
	bitfontCounter=&bitfontData[0];
	bitmenuCounter = bitmenuHead;
	while (bitmenuCounter != 0) {

		bitmenuGrouptreeStep(
				bitmenuCounter->groupCurrent->screenParent->groupTransition);

		bitmenuCounter = bitmenuCounter->menuNext;
	}
}




