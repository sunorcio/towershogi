

#include "towershogi_scene.h"




#include <isola/isola.h>

#include <scene/scene.h>
#include <timing.h>
#include <input.h>


#include "towershogi_scene_logic.h"
#include "towershogi_scene_render.h"




struct SCENE_scene towershogiScene = {0};




void towershogiUpdate(void){

	isolaGetWindow();
	if (isolaInfoWindow.width < towershogiScene.window.windowMinRes[0]) {
		isolaInfoWindow.width = towershogiScene.window.windowMinRes[0];
	}
	if (isolaInfoWindow.height < towershogiScene.window.windowMinRes[1]) {
		isolaInfoWindow.height = towershogiScene.window.windowMinRes[1];
	}
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.width,isolaInfoWindow.height);

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width, isolaInfoWindow.height);


	towershogiRenderUpdate();
	towershogiLogicUpdate();
}


void towershogiCreate(void){

	currentScene = &towershogiScene;

	towershogiScene.window.windowFullscreen = 0;
	towershogiScene.window.windowBorder = 1;
	towershogiScene.window.windowResizable = 1;
	towershogiScene.window.windowPos[0] = SDL_WINDOWPOS_CENTERED;
	towershogiScene.window.windowPos[1] = SDL_WINDOWPOS_CENTERED;
	towershogiScene.window.windowRes[0] = 800;
	towershogiScene.window.windowRes[1] = 600;
	towershogiScene.window.windowMinRes[0] = 480;
	towershogiScene.window.windowMinRes[1] = 360;
	towershogiScene.window.clearColor[0] = 0.0625;
	towershogiScene.window.clearColor[1] = 0.0625;
	towershogiScene.window.clearColor[2] = 0.0625;
	towershogiScene.window.clearColor[3] = 1.;

	towershogiScene.state.run = 1;
	towershogiScene.state.returnControlValue = 0;
	towershogiScene.state.pause = 0;

	timerSetup(&towershogiScene.timing.logicTimer, 60);
	counterSetup(&towershogiScene.timing.frameCounter, 60);


	inputClear();


	SDL_SetWindowSize(isolaWindow,towershogiScene.window.windowRes[0],
			towershogiScene.window.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,towershogiScene.window.windowPos[0],
			towershogiScene.window.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,towershogiScene.window.windowBorder);
	SDL_SetWindowResizable(isolaWindow,towershogiScene.window.windowResizable);
	if(towershogiScene.window.windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	glClearColor(towershogiScene.window.clearColor[0],
			towershogiScene.window.clearColor[1],
			towershogiScene.window.clearColor[2],
			towershogiScene.window.clearColor[3]);


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


	while(towershogiScene.state.run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){towershogiScene.state.run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						towershogiUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						towershogiScene.state.returnControlValue = 0;
						towershogiScene.state.run = 0;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				if (!event.key.repeat) {
					switch (event.key.keysym.sym){
						case SDLK_q:
							towershogiScene.state.returnControlValue = 1;
							towershogiScene.state.run = 0;
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


		if (!towershogiScene.state.pause) {
			if(timerStep(&currentScene->timing.logicTimer)){

				towershogiLogicStep();
				inputRepeat();
			}

			if(counterStep(&currentScene->timing.frameCounter)){

				towershogiRenderDraw();
			}
		}

#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	towershogiDestroy();
	return towershogiScene.state.returnControlValue;
}




