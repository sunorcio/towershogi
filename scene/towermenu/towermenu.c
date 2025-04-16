#include "towermenu.h"




#include <isola/isola.h>
#include <isola/timing.h>
#include <isola/input.h>
#include <scene/scene.h>


#include "towermenu_scene_logic.h"
#include "towermenu_scene_render.h"




static void towermenuUpdate(void);
static void towermenuCreate(void);
static void towermenuDestroy(void);




struct SCENE_scene towermenuScene = {0};




static void towermenuUpdate(void){

	isola_get_window();
	if (isola_info_window.width < towermenuScene.window.windowMinRes[0]) {
		isola_info_window.width = towermenuScene.window.windowMinRes[0];
	}
	if (isola_info_window.height < towermenuScene.window.windowMinRes[1]) {
		isola_info_window.height = towermenuScene.window.windowMinRes[1];
	}
	SDL_SetWindowSize(isola_window,isola_info_window.width,
			isola_info_window.height);

	isola_get_window();
	glViewport(0,0,isola_info_window.width, isola_info_window.height);


	towermenuLogicUpdate();
	towermenuRenderUpdate();
}


static void towermenuCreate(void){

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

	isola_timerSetup(&towermenuScene.timing.logicTimer, 60);
	isola_counterSetup(&towermenuScene.timing.frameCounter, 60);


	isola_inputClear();


	SDL_SetWindowSize(isola_window,towermenuScene.window.windowRes[0],
			towermenuScene.window.windowRes[1]);
	SDL_SetWindowPosition(isola_window,towermenuScene.window.windowPos[0],
			towermenuScene.window.windowPos[1]);
	SDL_SetWindowBordered(isola_window,towermenuScene.window.windowBorder);
	SDL_SetWindowResizable(isola_window,towermenuScene.window.windowResizable);
	if(towermenuScene.window.windowFullscreen){
		SDL_SetWindowFullscreen(isola_window,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	glClearColor(towermenuScene.window.clearColor[0],
			towermenuScene.window.clearColor[1],
			towermenuScene.window.clearColor[2],
			towermenuScene.window.clearColor[3]);


	towermenuLogicCreate();
	towermenuRenderCreate();

	towermenuUpdate();
}


static void towermenuDestroy(void){

	isola_inputClear();


	towermenuLogicDestroy();
	towermenuRenderDestroy();
}




unsigned char towermenu_loop(void){

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
						case SDLK_ESCAPE:
							towermenuScene.state.returnControlValue = 0;
							towermenuScene.state.run = 0;
						break;
					}
				}
				switch (event.key.keysym.sym){
					case SDLK_BACKSPACE:
						isola_textEditPop();
					break;
				}
			}
			if(event.type == SDL_TEXTINPUT){
				isola_textEditPush(&event.text.text);
			}
		}


		if (!towermenuScene.state.pause) {
			if(isola_timerStep(&currentScene->timing.logicTimer)){

				towermenuLogicStep();
				isola_inputRepeat();
			}

			if(isola_counterStep(&currentScene->timing.frameCounter)){

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




