



#include <time.h>


#include "isola/isola.h"

#include "scene/towermenu/towermenu_scene.h"
#include "scene/towershogi/towershogi_scene.h"




int main(int argc, char **argv){

	srand(time(0));
	isola_init();




	start:
	if (towermenuLoop()) {
		if (towershogiLoop()) {
			goto start;
		}
	}




	isola_quit();
	return 0;
}




