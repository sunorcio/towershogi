



#include "mainmenu.h"


#include <isola/isola.h>
#include <timing.h>


#include "mainmenu_render.c"
#include "mainmenu_logic.c"




struct SCENE_mainmenu {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
	int windowPos[2];
	int windowRes[2];
	float clearColor[4];
}mainmenu = {
		.windowFullscreen = 0,
		.windowBorder = 1,
		.windowResizable = 1,
		.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
		.windowRes = {800,600},
		.clearColor = {0.,0.,0.,1.}, };


void sceneSetup(struct SCENE_mainmenu* scene){

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




struct TIMING_timer logicTimer;
struct TIMING_counter frameCounter;


void mainmenuLoop(void){

	int keyLength;
	const unsigned char* keyState = SDL_GetKeyboardState(&keyLength);

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;


	sceneSetup(&mainmenu);
	timerSetup(&logicTimer, 60);
	counterSetup(&frameCounter, 60);
	mainmenuRenderCreate();
	mainmenuLogicCreate();


	while(run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						mainmenuRenderUpdate();
						mainmenuLogicUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						run = !run;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						run = !run;
					break;
					case SDLK_p:
						pause = !pause;
					break;
					case SDLK_SPACE:
					break;
				}
			}
		}


		if(!pause && timerStep(&logicTimer)){

			mainmenuLogicStep();
		}

		if(counterStep(&frameCounter)){

			mainmenuRenderDraw();
		}

#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	mainmenuRenderDestroy();
	mainmenuLogicDestroy();
}




