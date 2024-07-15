




#include "mainmenu.h"


#include <isola/isola.h>


#include <render/digitfps_logic.h>
#include <logic/bitmenu.h>




void mainmenuLogicUpdate(void){

	updateBitmenu();
}


void mainmenuLogicCreate(void){

	createBitmenu();
}


void mainmenuLogicDestroy(void){

	destroyBitmenu();
}


void mainmenuLogicStep(void){

	stepBitmenu();
}




