

#include "mainmenu.h"




#include <isola/isola.h>
#include <isola/misc.h>

#include <scene/scene.h>
#include <timing.h>
#include <input.h>

#include <render/digitfps_logic.h>


#include "mainmenu_logic.h"
#include "mainmenu_render.h"




struct SCENE_window mainmenuWindow = {
		.windowFullscreen = 0,
		.windowBorder = 1,
		.windowResizable = 1,
		.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
		.windowRes = {800,600},
		.windowMinRes = {480,360},
		.clearColor = {0.0625,0.0625,0.0625,1.}, 
		};


struct SCENE_state mainmenuState = {
		.run = 1,
		.pause = 0,
		.returnControlValue = 0,
		};


struct TIMING_timer logicTimer;
struct TIMING_counter frameCounter;




void mainmenuUpdate(void){

	if (isolaInfoWindow.height > 720 && isolaInfoWindow.width > 1280) {
		digitfps.pixelSize = 8*2;
	}else {
		digitfps.pixelSize = 8*1;
	}


	isolaGetWindow();
	if (isolaInfoWindow.width < mainmenuWindow.windowMinRes[0]) {
		isolaInfoWindow.width = mainmenuWindow.windowMinRes[0];
	}
	if (isolaInfoWindow.height < mainmenuWindow.windowMinRes[1]) {
		isolaInfoWindow.height = mainmenuWindow.windowMinRes[1];
	}
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.width,isolaInfoWindow.height);

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width, isolaInfoWindow.height);


	mainmenuRenderUpdate();
	mainmenuLogicUpdate();
}


void mainmenuCreate(void){

	inputClear();


	SDL_SetWindowSize(isolaWindow,mainmenuWindow.windowRes[0],
			mainmenuWindow.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,mainmenuWindow.windowPos[0],
			mainmenuWindow.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,mainmenuWindow.windowBorder);
	SDL_SetWindowResizable(isolaWindow,mainmenuWindow.windowResizable);
	if(mainmenuWindow.windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	glClearColor(mainmenuWindow.clearColor[0],mainmenuWindow.clearColor[1],
			mainmenuWindow.clearColor[2],mainmenuWindow.clearColor[3]);


	timerSetup(&logicTimer, 60);
	counterSetup(&frameCounter, 60);


	mainmenuLogicCreate();
	mainmenuRenderCreate();

	mainmenuUpdate();
}


void mainmenuDestroy(void){

	inputClear();


	mainmenuRenderDestroy();
	mainmenuLogicDestroy();
}




unsigned char mainmenuLoop(void){

	SDL_Event event = {0};


	mainmenuCreate();


	while(mainmenuState.run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){mainmenuState.run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						mainmenuUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						mainmenuState.run = 0;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				if (!event.key.repeat) {
					switch (event.key.keysym.sym){
						case SDLK_ESCAPE:
							mainmenuState.run = 0;
						break;
						case SDLK_p:
							mainmenuState.pause = !mainmenuState.pause;
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


		if (!mainmenuState.pause) {
			if(timerStep(&logicTimer)){

				mainmenuLogicStep();
				inputRepeat();
			}

			if(counterStep(&frameCounter)){

				mainmenuRenderDraw();
			}
		}


#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	mainmenuDestroy();
	return mainmenuState.returnControlValue;
}




