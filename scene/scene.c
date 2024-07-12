



#include "scene.h"




void sceneSetup(struct SCENE_scene* scene){

	SDL_SetWindowSize(isolaWindow,scene->windowRes[0],
			scene->windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,scene->windowPos[0],
			scene->windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,scene->windowBorder);
	SDL_SetWindowResizable(isolaWindow,scene->windowResizable);
	if(scene->windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	glClearColor(scene->clearColor[0],scene->clearColor[1],
			scene->clearColor[2],scene->clearColor[3]);
}




