#include "towershogi_scene.h"




#include <isola/isola.h>
#include <scene/scene.h>
#include <isola/timing.h>
#include <isola/input.h>


#include "towershogi.h"
#include <module/digitfps/digitfps.h>
#include <module/digitfps/digitfps_logic.h>




struct SCENE_scene towershogiScene = {0};




static void towershogiUpdate(void){

	isola_get_window();
	if (isola_info_window.width < towershogiScene.window.windowMinRes[0]) {
		isola_info_window.width = towershogiScene.window.windowMinRes[0];
	}
	if (isola_info_window.height < towershogiScene.window.windowMinRes[1]) {
		isola_info_window.height = towershogiScene.window.windowMinRes[1];
	}
	SDL_SetWindowSize(isola_window,isola_info_window.width,
			isola_info_window.height);

	isola_get_window();
	glViewport(0,0,isola_info_window.width, isola_info_window.height);


	updateTowershogi();
	updateDigitfps();
}


static void towershogiCreate(void){

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

	isola_timerSetup(&towershogiScene.timing.logicTimer, 60);
	isola_counterSetup(&towershogiScene.timing.frameCounter, 60);

	isola_inputClear(isola_window);


	SDL_SetWindowSize(isola_window,towershogiScene.window.windowRes[0],
			towershogiScene.window.windowRes[1]);
	SDL_SetWindowPosition(isola_window,towershogiScene.window.windowPos[0],
			towershogiScene.window.windowPos[1]);
	SDL_SetWindowBordered(isola_window,towershogiScene.window.windowBorder);
	SDL_SetWindowResizable(isola_window,towershogiScene.window.windowResizable);
	if(towershogiScene.window.windowFullscreen){
		SDL_SetWindowFullscreen(isola_window,1);
	}
	glClearColor(towershogiScene.window.clearColor[0],
			towershogiScene.window.clearColor[1],
			towershogiScene.window.clearColor[2],
			towershogiScene.window.clearColor[3]);


	towershogiBoardSize[0] = 8;
	towershogiBoardSize[1] = 8;


	createTowershogi();
	createDigitfps();


	towershogiUpdate();
}


static void towershogiDestroy(void){

	isola_inputClear(isola_window);


	destroyTowershogi();
	destroyDigitfps();


	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow(isola_window);
}




unsigned char towershogi_loop(void){

	SDL_Event event = {0};


	towershogiCreate();


	while(towershogiScene.state.run){

		while (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_EVENT_QUIT:
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					towershogiScene.state.returnControlValue = 0;
					towershogiScene.state.run = 0;
				break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
					towershogiUpdate();
				break;
				case SDL_EVENT_KEY_DOWN:
					if (!event.key.repeat) {
						switch (event.key.key){
							case SDLK_ESCAPE:
								towershogiScene.state.returnControlValue = 1;
								towershogiScene.state.run = 0;
							break;
						}
					}
					switch (event.key.key){
						case SDLK_BACKSPACE:
							isola_textEditPop();
						break;
					}
				break;
				case SDL_EVENT_TEXT_INPUT:
					isola_textEditPush(/* ???well see */(char(*)[32])&event.text.text);
				break;
			}
		}


		if (!towershogiScene.state.pause) {
			if(isola_timerStep(&currentScene->timing.logicTimer)){

				stepTowershogi();

				isola_inputRepeat();
			}

			if(isola_counterStep(&currentScene->timing.frameCounter)){

				glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


				drawTowershogi();
				drawDigitfps();


				SDL_GL_SwapWindow(isola_window);
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




