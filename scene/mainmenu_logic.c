

#include "mainmenu_logic.h"




#include <isola/isola.h>

#include <scene/scene.h>

#include <render/digitfps_logic.h>
#include <logic/bitmenu.h>




extern struct SCENE_state mainmenuState;




void quitMainmenu(void){

	mainmenuState.run = 0;
}

void playMainmenu(void){

	mainmenuState.returnControlValue = 1;
	mainmenuState.run = 0;
}




void mainmenuLogicUpdate(void){

	updateBitmenu();
}


void mainmenuLogicCreate(void){

	createBitmenu();


	bitmenuBuildMenu();
	bitmenuBuildGroup(-1,1,1,1);
	bitmenuBuildScreen(2);
	bitmenuBuildObject("play",playMainmenu);
	bitmenuBuildObject("options",0);
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(2);
		bitmenuBuildObject("these are options :)",0);
		bitmenuBuildObject("press q to go back to last menu",0);
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("quit",quitMainmenu);
}


void mainmenuLogicDestroy(void){

	destroyBitmenu();
}


void mainmenuLogicStep(void){

	stepBitmenu();
}




