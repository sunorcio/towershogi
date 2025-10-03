



#include "isola/isola.h"


#include "scene/towermenu/towermenu_scene.h"
#include "scene/towershogi/towershogi_scene.h"




int main(int argc, char **argv){

	Sint64 time;


	if(!isola_init()){
		return 1;
	}

	SDL_GetCurrentTime(&time);
	SDL_srand((Uint64)time);




	while (1) {
		if (towermenu_scene_loop()) {
			if (towershogi_scene_loop()) {
				continue;
			}else {
				break;
			}
		}else {
			break;
		}
	}




	isola_quit();
	return 0;
}




