

#include "mainmenu_logic.h"




#include <isola/isola.h>


#include <render/digitfps_logic.h>
#include <logic/bitmenu.h>





void mainmenuLogicUpdate(void){

	updateBitmenu();
}


void mainmenuLogicCreate(void){

	createBitmenu();
/* 	bitmenuStartScreen(2);
	bitmenuStartGroup(-1, 1, 1, 1);
	bitmenuStartObject("asdf\177asdf");
	bitmenuStartObject("asdofad\177\177asdofad");
	bitmenuStartGroup(0, 0, 1, 1);
	bitmenuStartObject("asdf\177asdf");
	bitmenuStartObject("asdofad\177\177\177\177asdofad");
	bitmenuEndScreen(); */
}


void mainmenuLogicDestroy(void){

	destroyBitmenu();
}


void mainmenuLogicStep(void){

	stepBitmenu();
}




