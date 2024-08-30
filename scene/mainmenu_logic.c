

#include "mainmenu_logic.h"




#include <isola/isola.h>


#include <render/digitfps_logic.h>
#include <logic/bitmenu.h>




extern struct MAINMENU_state mainmenuState;




void mainmenuLogicUpdate(void){

	updateBitmenu();
}


void mainmenuLogicCreate(void){

	createBitmenu();


/* 	bitmenuBuildMenu();
	bitmenuBuildGroup(-1,1,1,1);
	bitmenuBuildScreen(2);
	bitmenuBuildObject("menu 1, screen 1, group 1");
	bitmenuBuildObject("play");
	bitmenuBuildObject("go to group 2");
	bitmenuBuildGroup(-1,0,1,1);
		bitmenuBuildObject("menu 1, screen 1, group 2");
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("go to screen 2");
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(3);
		bitmenuBuildObject("menu 1, screen 2, group 3");
		bitmenuBuildObject("go to group 4");
		bitmenuBuildGroup(-1,0,1,1);
			bitmenuBuildObject("menu 1, screen 2, group 4");
		bitmenuBuildGroupLeave();
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("quit");

	bitmenuBuildMenu();
	bitmenuBuildGroup(0,1,1,1);
	bitmenuBuildScreen(1);
	bitmenuBuildObject("menu 2, screen 1, group 1"); */
}


void mainmenuLogicDestroy(void){

	destroyBitmenu();
}


void mainmenuLogicStep(void){

	stepBitmenu();
}




