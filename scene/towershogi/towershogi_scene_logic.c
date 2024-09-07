

#include "towershogi_scene_logic.h"




#include <isola/isola.h>

#include <module/digitfps/digitfps_logic.h>

#include "towershogi_logic.h"




void towershogiLogicUpdate(void){

	updateTowershogiLogic();


	if (isolaInfoWindow.height > 720 && isolaInfoWindow.width > 1280) {
		digitfps.pixelSize = 8*2;
	}else {
		digitfps.pixelSize = 8*1;
	}
}


void towershogiLogicCreate(void){

	createTowershogiLogic();
}


void towershogiLogicDestroy(void){

	destroyTowershogiLogic();
}


void towershogiLogicStep(void){

	stepTowershogiLogic();
}




