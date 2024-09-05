

#include "towershogi.h"




#include <isola/isola.h>

#include <scene/scene.h>
#include <timing.h>
#include <input.h>


#include "towershogi_logic.h"
#include "towershogi_render.h"




struct SCENE_window towershogiWindow = {
		.windowFullscreen = 0,
		.windowBorder = 1,
		.windowResizable = 1,
		.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
		.windowRes = {800,600},
		.windowMinRes = {480,360},
		.clearColor = {0.0625,0.0625,0.0625,1.}, 
		};


struct SCENE_state towershogiState = {
		.run = 1,
		.pause = 0,
		.returnControlValue = 0,
		};


struct TIMING_timer towershogiLogicTimer;
struct TIMING_counter towershogiFrameCounter;




void towershogiUpdate(void){

	isolaGetWindow();
	if (isolaInfoWindow.width < towershogiWindow.windowMinRes[0]) {
		isolaInfoWindow.width = towershogiWindow.windowMinRes[0];
	}
	if (isolaInfoWindow.height < towershogiWindow.windowMinRes[1]) {
		isolaInfoWindow.height = towershogiWindow.windowMinRes[1];
	}
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.width,isolaInfoWindow.height);

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width, isolaInfoWindow.height);


	towershogiRenderUpdate();
	towershogiLogicUpdate();
}


void towershogiCreate(void){

	inputClear();


	SDL_SetWindowSize(isolaWindow,towershogiWindow.windowRes[0],
			towershogiWindow.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,towershogiWindow.windowPos[0],
			towershogiWindow.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,towershogiWindow.windowBorder);
	SDL_SetWindowResizable(isolaWindow,towershogiWindow.windowResizable);
	if(towershogiWindow.windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	glClearColor(towershogiWindow.clearColor[0],towershogiWindow.clearColor[1],
			towershogiWindow.clearColor[2],towershogiWindow.clearColor[3]);


	timerSetup(&towershogiLogicTimer, 60);
	counterSetup(&towershogiFrameCounter, 60);


	towershogiLogicCreate();
	towershogiRenderCreate();

	towershogiUpdate();
}


void towershogiDestroy(void){

	inputClear();


	towershogiRenderDestroy();
	towershogiLogicDestroy();
}




unsigned char towershogiLoop(void){

	SDL_Event event = {0};


	towershogiCreate();


	while(towershogiState.run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){towershogiState.run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						towershogiUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						towershogiState.run = 0;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				if (!event.key.repeat) {
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
							towershogiState.run = 0;
						break;
						case SDLK_RETURN:
						break;
						case SDLK_SPACE:
						break;
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


		if (!towershogiState.pause) {
			if(timerStep(&towershogiLogicTimer)){

				towershogiLogicStep();
				inputRepeat();
			}

			if(counterStep(&towershogiFrameCounter)){

				towershogiRenderDraw();
			}
		}

#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	towershogiDestroy();
	return towershogiState.returnControlValue;
}




