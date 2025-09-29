#include "bitmenu.h"
#include "bitmenu_logic.h"




#include <isola/isola.h>
#include <isola/misc.h>
#include <isola/input.h>


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
}static * bitmenu_head = 0, * bitmenu_current = 0, * bitmenu_counter;


static struct BITFONT_data* bitmenu_bitfontPtr;
static struct BITFONT_object* bitmenu_bitfontCounter;


static struct BITMENU_menu* bitmenu_menuPointer = 0;
static struct BITMENU_screen* bitmenu_screenPointer = 0;
static struct BITMENU_group* bitmenu_groupPointer = 0;
static struct BITMENU_object* bitmenu_objectPointer = 0;




static void bitmenu_grouptreeDelete(struct BITMENU_group* group){

	void* last;
	struct BITMENU_object* object;


	if (group == 0) {return;}

	object = group->objectHead;
	while (object != 0) {

		bitmenu_grouptreeDelete(object->groupLink);
	
		last = object;
		object = object->objectNext;
		SDL_free(last);
	}

	SDL_free(group);
}

static void bitmenu_grouptreeStep(struct BITMENU_group* group){

	struct BITMENU_object* object;
	unsigned int o = 0;


	if (group == 0) {return;}
	if (group->screenParent != bitmenu_counter->groupCurrent->screenParent) {
		return;
	}

	object = group->objectHead;
	while (object != 0) {

		bitmenu_bitfontCounter->pixelSize = 
				bitmenu_counter->groupCurrent->screenParent->fontScaleGlobal;
		bitmenu_bitfontCounter->charWrap = BITFONT_STRINGSIZE;
		SDL_strlcpy(bitmenu_bitfontCounter->string,object->name,256);
		bitmenu_bitfontCounter->x = group->x + isola_info_window.pixelWidth;
		bitmenu_bitfontCounter->y = group->y
				-isola_info_window.pixelHeight*16
				*bitmenu_bitfontCounter->pixelSize*o
				-isola_info_window.pixelHeight;
		bitmenu_bitfontCounter->foreColor[0] = 0.;
		bitmenu_bitfontCounter->foreColor[1] = 0.;
		bitmenu_bitfontCounter->foreColor[2] = 0.;
		bitmenu_bitfontCounter->foreColor[3] = 1.;
		bitmenu_bitfontCounter->backColor[0] = 1.
				*(bitmenu_counter->objectCurrent==object);
		bitmenu_bitfontCounter->backColor[1] = 1.
				*(bitmenu_counter->objectCurrent==object);
		bitmenu_bitfontCounter->backColor[2] = 1.;
		bitmenu_bitfontCounter->backColor[3] = 0.5
				+0.5*(bitmenu_counter->groupCurrent==group);

		if (bitmenu_bitfontCounter !=
				&bitmenu_bitfontPtr->data[bitmenu_bitfontPtr->size-1]) {
			bitmenu_bitfontCounter += 1;
		}else {
			SDL_Log("bitmenu_grouptreeStep: reached bitfont object limit");
		}

		bitmenu_grouptreeStep(object->groupLink);

		object = object->objectNext;
		o += 1;
	}
}


static void bitmenu_moveForward(void){

	if (bitmenu_current->objectCurrent->objectNext != 0) {
		bitmenu_current->objectCurrent = 
				bitmenu_current->objectCurrent->objectNext;
	}
}

static void bitmenu_moveBack(void){

	struct BITMENU_object* object;


	object = bitmenu_current->groupCurrent->objectHead;

	if (object == bitmenu_current->objectCurrent) {return;}

	while (object->objectNext != bitmenu_current->objectCurrent) {
		object = object->objectNext;
	}

	bitmenu_current->objectCurrent = object;
}

static void bitmenu_activateObject(void){

	if (bitmenu_current->objectCurrent->function != 0) {
		bitmenu_current->objectCurrent->function();
	}

	if (bitmenu_current->objectCurrent->groupLink != 0) {
		bitmenu_current->groupCurrent =
				bitmenu_current->objectCurrent->groupLink;
		bitmenu_current->objectCurrent =
				bitmenu_current->groupCurrent->objectHead;
	}
}

static void bitmenu_leaveGroup(void){

	if (bitmenu_current->groupCurrent->groupPrevious == 0) { return; }

	bitmenu_current->groupCurrent =
			bitmenu_current->groupCurrent->groupPrevious;
	bitmenu_current->objectCurrent =
			bitmenu_current->groupCurrent->objectHead;
}


void bitmenu_buildMenu(void){

	if (bitmenu_menuPointer != 0) {
		bitmenu_menuPointer->menuNext = SDL_calloc(1,sizeof(struct BITMENU_menu));
		bitmenu_menuPointer = bitmenu_menuPointer->menuNext;
	}else {
		bitmenu_menuPointer = SDL_calloc(1,sizeof(struct BITMENU_menu));
		bitmenu_head = bitmenu_menuPointer;
		bitmenu_current = bitmenu_menuPointer;
	}
}

void bitmenu_buildScreen(unsigned char fontScaleGlobal){

	if (bitmenu_menuPointer->screenHead != 0) {
		bitmenu_screenPointer->screenNext = 
				SDL_calloc(1,sizeof(struct BITMENU_screen));
		bitmenu_screenPointer = bitmenu_screenPointer->screenNext;
		bitmenu_groupPointer->screenParent = bitmenu_screenPointer;
		bitmenu_screenPointer->groupTransition = bitmenu_groupPointer;
		bitmenu_screenPointer->fontScaleGlobal = fontScaleGlobal;
	
	}else {
		bitmenu_screenPointer = SDL_calloc(1,sizeof(struct BITMENU_screen));
		bitmenu_menuPointer->screenHead = bitmenu_screenPointer;
		bitmenu_groupPointer->screenParent = bitmenu_screenPointer;
		bitmenu_screenPointer->groupTransition = bitmenu_groupPointer;
		bitmenu_screenPointer->fontScaleGlobal = fontScaleGlobal;
	}
}

void bitmenu_buildGroup(float x, float y, float w, float h){

	if (bitmenu_menuPointer->groupHead != 0) {
		bitmenu_objectPointer->groupLink =
				SDL_calloc(1,sizeof(struct BITMENU_group));
		bitmenu_objectPointer->groupLink->groupPrevious = bitmenu_groupPointer;
		bitmenu_groupPointer = bitmenu_objectPointer->groupLink;
		bitmenu_groupPointer->screenParent = bitmenu_screenPointer;
		bitmenu_groupPointer->x = x;
		bitmenu_groupPointer->y = y;
		bitmenu_groupPointer->w = w;
		bitmenu_groupPointer->h = h;
	}else {
		bitmenu_groupPointer = SDL_calloc(1,sizeof(struct BITMENU_group));
		bitmenu_groupPointer->screenParent = bitmenu_screenPointer;
		bitmenu_groupPointer->x = x;
		bitmenu_groupPointer->y = y;
		bitmenu_groupPointer->w = w;
		bitmenu_groupPointer->h = h;
		bitmenu_menuPointer->groupHead = bitmenu_groupPointer;
		bitmenu_menuPointer->groupCurrent = bitmenu_menuPointer->groupHead;
	}
}

void bitmenu_buildObject(const char* name, void (*function)(void)){

	if (bitmenu_groupPointer->objectHead != 0) {
		bitmenu_objectPointer->objectNext =
				SDL_calloc(1,sizeof(struct BITMENU_object));
		bitmenu_objectPointer = bitmenu_objectPointer->objectNext;

		SDL_strlcpy(bitmenu_objectPointer->name,name,256);
		bitmenu_objectPointer->function = function;
	}else {
		bitmenu_objectPointer = SDL_calloc(1,sizeof(struct BITMENU_object));
		SDL_strlcpy(bitmenu_objectPointer->name,name,256);
		bitmenu_objectPointer->function = function;

		bitmenu_groupPointer->objectHead = bitmenu_objectPointer;
		if (bitmenu_menuPointer->objectCurrent == 0) {
			bitmenu_menuPointer->objectCurrent =
					bitmenu_menuPointer->groupHead->objectHead;
		}
	}
}

void bitmenu_buildGroupLeave(void){

	bitmenu_groupPointer = bitmenu_groupPointer->groupPrevious;
	bitmenu_objectPointer = bitmenu_groupPointer->objectHead;
	while (bitmenu_objectPointer->objectNext!=0) {
		bitmenu_objectPointer=bitmenu_objectPointer->objectNext;
	}
}

void bitmenu_buildExample(void){

	bitmenu_buildMenu();
	bitmenu_buildGroup(-1,1,1,1);
	bitmenu_buildScreen(2);
	bitmenu_buildObject("menu 1, screen 1, group 1",0);
	bitmenu_buildObject("go to group 2",0);
	bitmenu_buildGroup(-1,0,1,1);
		bitmenu_buildObject("menu 1, screen 1, group 2",0);
	bitmenu_buildGroupLeave();
	bitmenu_buildObject("go to screen 2",0);
	bitmenu_buildGroup(-1,1,1,1);
		bitmenu_buildScreen(3);
		bitmenu_buildObject("menu 1, screen 2, group 3",0);
		bitmenu_buildObject("go to group 4",0);
		bitmenu_buildGroup(-1,0,1,1);
			bitmenu_buildObject("menu 1, screen 2, group 4",0);
		bitmenu_buildGroupLeave();
	bitmenu_buildGroupLeave();

	bitmenu_buildMenu();
	bitmenu_buildGroup(0,1,1,1);
	bitmenu_buildScreen(1);
	bitmenu_buildObject("menu 2, screen 1, group 1",0);
}




void bitmenu_update(void){

}


void bitmenu_create(void){

	bitmenu_bitfontPtr = bitfont_requestPtr(0xff);

	bitmenu_current = 0;
	bitmenu_head = 0;

	bitmenu_menuPointer = 0;
	bitmenu_screenPointer = 0;
	bitmenu_groupPointer = 0;
	bitmenu_objectPointer = 0;
}


void bitmenu_destroy(void){

	void* last;
	struct BITMENU_menu* menu;
	struct BITMENU_screen* screen;


	menu = bitmenu_head;
	while (menu != 0) {

		screen = menu->screenHead;
		while (screen != 0) {

			last = screen;
			screen = screen->screenNext;
			SDL_free(last);
		}

		bitmenu_grouptreeDelete(menu->groupHead);

		last = menu;
		menu = menu->menuNext;
		SDL_free(last);
	}


	bitfont_freePtr(bitmenu_bitfontPtr);
}


void bitmenu_step(void){

	if (isola_keyState[SDL_SCANCODE_J] && !isola_keyRepeat[SDL_SCANCODE_J]) {
		bitmenu_moveForward();
	}
	if (isola_keyState[SDL_SCANCODE_K] && !isola_keyRepeat[SDL_SCANCODE_K]) {
		bitmenu_moveBack();
	}

	if (isola_keyState[SDL_SCANCODE_Q] && !isola_keyRepeat[SDL_SCANCODE_Q]) {
		bitmenu_leaveGroup();
	}else if(isola_keyState[SDL_SCANCODE_RETURN] &&
			!isola_keyRepeat[SDL_SCANCODE_RETURN]) {
		bitmenu_activateObject();
	}


	SDL_memset(bitmenu_bitfontPtr->data,0,
			sizeof(struct BITFONT_object)*bitmenu_bitfontPtr->size);
	bitmenu_bitfontCounter = bitmenu_bitfontPtr->data;
	bitmenu_counter = bitmenu_head;
	while (bitmenu_counter != 0) {

		bitmenu_grouptreeStep(
				bitmenu_counter->groupCurrent->screenParent->groupTransition);

		bitmenu_counter = bitmenu_counter->menuNext;
	}
}




