

#include "towermenu_scene.h"




#include <isola/isola.h>

#include <scene/scene.h>
#include <timing.h>
#include <input.h>


#include "towermenu_scene_logic.h"
#include "towermenu_scene_render.h"




struct SCENE_window towermenuWindow = {
		.windowFullscreen = 0,
		.windowBorder = 1,
		.windowResizable = 1,
		.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
		.windowRes = {800,600},
		.windowMinRes = {480,360},
		.clearColor = {0.0625,0.0625,0.0625,1.},
		};


struct SCENE_state towermenuState = {
		.run = 1,
		.pause = 0,
		.returnControlValue = 0,
		};


struct TIMING_timer towermenuLogicTimer;
struct TIMING_counter towermenuFrameCounter;




void towermenuUpdate(void){

	isolaGetWindow();
	if (isolaInfoWindow.width < towermenuWindow.windowMinRes[0]) {
		isolaInfoWindow.width = towermenuWindow.windowMinRes[0];
	}
	if (isolaInfoWindow.height < towermenuWindow.windowMinRes[1]) {
		isolaInfoWindow.height = towermenuWindow.windowMinRes[1];
	}
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.width,isolaInfoWindow.height);

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width, isolaInfoWindow.height);


	towermenuRenderUpdate();
	towermenuLogicUpdate();
}


void towermenuCreate(void){

	towermenuState.run = 1;
	towermenuState.returnControlValue = 0;
	towermenuState.pause = 0;


	inputClear();


	SDL_SetWindowSize(isolaWindow,towermenuWindow.windowRes[0],
			towermenuWindow.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,towermenuWindow.windowPos[0],
			towermenuWindow.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,towermenuWindow.windowBorder);
	SDL_SetWindowResizable(isolaWindow,towermenuWindow.windowResizable);
	if(towermenuWindow.windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	glClearColor(towermenuWindow.clearColor[0],towermenuWindow.clearColor[1],
			towermenuWindow.clearColor[2],towermenuWindow.clearColor[3]);


	timerSetup(&towermenuLogicTimer, 60);
	counterSetup(&towermenuFrameCounter, 60);


	towermenuLogicCreate();
	towermenuRenderCreate();

	towermenuUpdate();
}


void towermenuDestroy(void){

	inputClear();


	towermenuRenderDestroy();
	towermenuLogicDestroy();
}




unsigned char towermenuLoop(void){

	SDL_Event event = {0};


	towermenuCreate();

	while(towermenuState.run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){towermenuState.run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						towermenuUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						towermenuState.returnControlValue = 0;
						towermenuState.run = 0;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				if (!event.key.repeat) {
					switch (event.key.keysym.sym){
					}
				}
				switch (event.key.keysym.sym){
					case SDLK_BACKSPACE:
						inputTextEditingPop();
					break;
				}
			}
			if(event.type == SDL_TEXTINPUT){
				inputTextEditingPush(&event.text.text);
			}
		}


		if (!towermenuState.pause) {
			if(timerStep(&towermenuLogicTimer)){

				towermenuLogicStep();
				inputRepeat();
			}

			if(counterStep(&towermenuFrameCounter)){

				towermenuRenderDraw();
			}
		}


#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	towermenuDestroy();
	return towermenuState.returnControlValue;
}




