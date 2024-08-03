

#include "mainmenu.h"




#include <isola/isola.h>


#include <logic/bitmenu.h>

#include <render/digitfps_logic.h>




void mainmenuLogicUpdate(void){

	updateBitmenu();
}


void mainmenuLogicCreate(void){

	createBitmenu();
	/* menu description */
}


void mainmenuLogicDestroy(void){

	destroyBitmenu();
}


void mainmenuLogicStep(void){

	stepBitmenu();
}




