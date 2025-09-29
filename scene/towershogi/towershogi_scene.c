#include "towershogi_scene.h"




#include <isola/isola.h>
#include <scene/scene.h>
#include <isola/timing.h>
#include <isola/input.h>


#include "towershogi.h"
#include <module/digitfps/digitfps.h>
#include <module/digitfps/digitfps_logic.h>




static struct SCENE_scene towershogi_scene = {0};




static void towershogi_scene_update(void){

	isola_get_window();
	if (isola_info_window.width < towershogi_scene.window.windowMinRes[0]) {
		isola_info_window.width = towershogi_scene.window.windowMinRes[0];
	}
	if (isola_info_window.height < towershogi_scene.window.windowMinRes[1]) {
		isola_info_window.height = towershogi_scene.window.windowMinRes[1];
	}
	SDL_SetWindowSize(isola_window,isola_info_window.width,
			isola_info_window.height);

	isola_get_window();
	glViewport(0,0,isola_info_window.width, isola_info_window.height);


	towershogi_update();
	digitfps_update();
}


static void towershogi_scene_create(void){

	currentScene = &towershogi_scene;

	towershogi_scene.window.windowFullscreen = 0;
	towershogi_scene.window.windowBorder = 1;
	towershogi_scene.window.windowResizable = 1;
	towershogi_scene.window.windowPos[0] = SDL_WINDOWPOS_CENTERED;
	towershogi_scene.window.windowPos[1] = SDL_WINDOWPOS_CENTERED;
	towershogi_scene.window.windowRes[0] = 800;
	towershogi_scene.window.windowRes[1] = 600;
	towershogi_scene.window.windowMinRes[0] = 480;
	towershogi_scene.window.windowMinRes[1] = 360;
	towershogi_scene.window.clearColor[0] = 0.0625;
	towershogi_scene.window.clearColor[1] = 0.0625;
	towershogi_scene.window.clearColor[2] = 0.0625;
	towershogi_scene.window.clearColor[3] = 1.;

	towershogi_scene.state.run = 1;
	towershogi_scene.state.returnControlValue = 0;
	towershogi_scene.state.pause = 0;

	isola_timerSetup(&towershogi_scene.timing.logicTimer, 60);
	isola_counterSetup(&towershogi_scene.timing.frameCounter, 60);

	isola_inputClear(isola_window);


	SDL_SetWindowSize(isola_window,towershogi_scene.window.windowRes[0],
			towershogi_scene.window.windowRes[1]);
	SDL_SetWindowPosition(isola_window,towershogi_scene.window.windowPos[0],
			towershogi_scene.window.windowPos[1]);
	SDL_SetWindowBordered(isola_window,towershogi_scene.window.windowBorder);
	SDL_SetWindowResizable(isola_window,towershogi_scene.window.windowResizable);
	if(towershogi_scene.window.windowFullscreen){
		SDL_SetWindowFullscreen(isola_window,1);
	}
	glClearColor(towershogi_scene.window.clearColor[0],
			towershogi_scene.window.clearColor[1],
			towershogi_scene.window.clearColor[2],
			towershogi_scene.window.clearColor[3]);


	towershogi_boardSize[0] = 8;
	towershogi_boardSize[1] = 8;


	towershogi_create();
	digitfps_create();


	towershogi_scene_update();
}


static void towershogi_scene_destroy(void){

	isola_inputClear(isola_window);


	towershogi_destroy();
	digitfps_destroy();


	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow(isola_window);
}




unsigned char towershogi_scene_loop(void){

	SDL_Event event = {0};


	towershogi_scene_create();


	while(towershogi_scene.state.run){

		while (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_EVENT_QUIT:
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					towershogi_scene.state.returnControlValue = 0;
					towershogi_scene.state.run = 0;
				break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
					towershogi_scene_update();
				break;
				case SDL_EVENT_KEY_DOWN:
					if (!event.key.repeat) {
						switch (event.key.key){
							case SDLK_ESCAPE:
								towershogi_scene.state.returnControlValue = 1;
								towershogi_scene.state.run = 0;
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


		if (!towershogi_scene.state.pause) {
			if(isola_timerStep(&currentScene->timing.logicTimer)){

				towershogi_step();

				isola_inputRepeat();
			}

			if(isola_counterStep(&currentScene->timing.frameCounter)){

				glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


				towershogi_draw();
				digitfps_draw();


				SDL_GL_SwapWindow(isola_window);
			}
		}

#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	towershogi_scene_destroy();
	return towershogi_scene.state.returnControlValue;
}




