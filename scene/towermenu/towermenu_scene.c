

#include "towermenu_scene.h"




#include <isola/isola.h>

#include <scene/scene.h>
#include <timing.h>
#include <input.h>


#include "towermenu_scene_logic.h"
#include "towermenu_scene_render.h"




struct SCENE_scene towermenuScene = {0};




void towermenuUpdate(void){

	isolaGetWindow();
	if (isolaInfoWindow.width < towermenuScene.window.windowMinRes[0]) {
		isolaInfoWindow.width = towermenuScene.window.windowMinRes[0];
	}
	if (isolaInfoWindow.height < towermenuScene.window.windowMinRes[1]) {
		isolaInfoWindow.height = towermenuScene.window.windowMinRes[1];
	}
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.width,isolaInfoWindow.height);

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width, isolaInfoWindow.height);


	towermenuRenderUpdate();
	towermenuLogicUpdate();
}


void towermenuCreate(void){

	currentScene = &towermenuScene;

	towermenuScene.window.windowFullscreen = 0;
	towermenuScene.window.windowBorder = 1;
	towermenuScene.window.windowResizable = 1;
	towermenuScene.window.windowPos[0] = SDL_WINDOWPOS_CENTERED;
	towermenuScene.window.windowPos[1] = SDL_WINDOWPOS_CENTERED;
	towermenuScene.window.windowRes[0] = 800;
	towermenuScene.window.windowRes[1] = 600;
	towermenuScene.window.windowMinRes[0] = 480;
	towermenuScene.window.windowMinRes[1] = 360;
	towermenuScene.window.clearColor[0] = 0.0625;
	towermenuScene.window.clearColor[1] = 0.0625;
	towermenuScene.window.clearColor[2] = 0.0625;
	towermenuScene.window.clearColor[3] = 1.;

	towermenuScene.state.run = 1;
	towermenuScene.state.returnControlValue = 0;
	towermenuScene.state.pause = 0;

	timerSetup(&towermenuScene.timing.logicTimer, 60);
	counterSetup(&towermenuScene.timing.frameCounter, 60);


	inputClear();


	SDL_SetWindowSize(isolaWindow,towermenuScene.window.windowRes[0],
			towermenuScene.window.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,towermenuScene.window.windowPos[0],
			towermenuScene.window.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,towermenuScene.window.windowBorder);
	SDL_SetWindowResizable(isolaWindow,towermenuScene.window.windowResizable);
	if(towermenuScene.window.windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	glClearColor(towermenuScene.window.clearColor[0],
			towermenuScene.window.clearColor[1],
			towermenuScene.window.clearColor[2],
			towermenuScene.window.clearColor[3]);


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

	while(towermenuScene.state.run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){towermenuScene.state.run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						towermenuUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						towermenuScene.state.returnControlValue = 0;
						towermenuScene.state.run = 0;
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


		if (!towermenuScene.state.pause) {
			if(timerStep(&currentScene->timing.logicTimer)){

				towermenuLogicStep();
				inputRepeat();
			}

			if(counterStep(&currentScene->timing.frameCounter)){

				towermenuRenderDraw();
			}
		}


#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	towermenuDestroy();
	return towermenuScene.state.returnControlValue;
}




