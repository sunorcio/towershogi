

#include "towershogi_logic.h"




#include <isola/isola.h>

#include <module/digitfps/digitfps_logic.h>




void towershogiLogicUpdate(void){

	if (isolaInfoWindow.height > 720 && isolaInfoWindow.width > 1280) {
		digitfps.pixelSize = 8*2;
	}else {
		digitfps.pixelSize = 8*1;
	}
}


void towershogiLogicCreate(void){

}


void towershogiLogicDestroy(void){

}


void towershogiLogicStep(void){

}




