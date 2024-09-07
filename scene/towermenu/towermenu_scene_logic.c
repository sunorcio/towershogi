

#include "towermenu_scene_logic.h"




#include <isola/isola.h>

#include <scene/scene.h>

#include <module/digitfps/digitfps_logic.h>
#include <module/bitmenu/bitmenu_logic.h>




extern struct SCENE_state towermenuState;




static void quitTowermenu(void){

	towermenuState.run = 0;
}

static void playTowermenu(void){

	towermenuState.returnControlValue = 1;
	towermenuState.run = 0;
}




void towermenuLogicUpdate(void){

	updateBitmenu();


	if (isolaInfoWindow.height > 720 && isolaInfoWindow.width > 1280) {
		digitfps.pixelSize = 8*2;
	}else {
		digitfps.pixelSize = 8*1;
	}
}


void towermenuLogicCreate(void){

	createBitmenu();


	bitmenuBuildMenu();
	bitmenuBuildGroup(-1,1,1,1);
	bitmenuBuildScreen(2);
	bitmenuBuildObject("edit rules",0);
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(2);
		bitmenuBuildObject("these are rules :)",0);
		bitmenuBuildObject("press q to go back to last menu",0);
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("play",playTowermenu);
	bitmenuBuildObject("join",0);
	bitmenuBuildObject("host",0);
	bitmenuBuildObject("options",0);
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(2);
		bitmenuBuildObject("these are options :)",0);
		bitmenuBuildObject("press q to go back to last menu",0);
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("quit",quitTowermenu);
}


void towermenuLogicDestroy(void){

	destroyBitmenu();
}


void towermenuLogicStep(void){

	stepBitmenu();
}




