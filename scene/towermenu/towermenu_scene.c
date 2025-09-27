#include "towermenu_scene.h"




#include <isola/isola.h>
#include <isola/timing.h>
#include <isola/input.h>
#include <scene/scene.h>


#include <module/digitfps/digitfps.h>
#include <module/digitfps/digitfps_logic.h>
#include <module/bitfont/bitfont.h>
#include <module/bitmenu/bitmenu.h>
#include <module/bitmenu/bitmenu_logic.h>




static void towermenu_update(void);
static void towermenu_create(void);
static void towermenu_destroy(void);




static void towermenu_function_quit(void){

	currentScene->state.returnControlValue = 0;
	currentScene->state.run = 0;
}

static void towermenu_function_play(void){

	currentScene->state.returnControlValue = 1;
	currentScene->state.run = 0;
}




struct SCENE_scene towermenu_scene = {0};




static void towermenu_update(void){

	isola_get_window();
	if (isola_info_window.width < towermenu_scene.window.windowMinRes[0]) {
		isola_info_window.width = towermenu_scene.window.windowMinRes[0];
	}
	if (isola_info_window.height < towermenu_scene.window.windowMinRes[1]) {
		isola_info_window.height = towermenu_scene.window.windowMinRes[1];
	}
	SDL_SetWindowSize(isola_window,isola_info_window.width,
			isola_info_window.height);

	isola_get_window();
	glViewport(0,0,isola_info_window.width, isola_info_window.height);

	updateBitmenu();


	updateBitfont();
	updateDigitfps();
}


static void towermenu_create(void){

	currentScene = &towermenu_scene;

	towermenu_scene.window.windowFullscreen = 0;
	towermenu_scene.window.windowBorder = 1;
	towermenu_scene.window.windowResizable = 1;
	towermenu_scene.window.windowPos[0] = SDL_WINDOWPOS_CENTERED;
	towermenu_scene.window.windowPos[1] = SDL_WINDOWPOS_CENTERED;
	towermenu_scene.window.windowRes[0] = 800;
	towermenu_scene.window.windowRes[1] = 600;
	towermenu_scene.window.windowMinRes[0] = 480;
	towermenu_scene.window.windowMinRes[1] = 360;
	towermenu_scene.window.clearColor[0] = 0.125;
	towermenu_scene.window.clearColor[1] = 0.125;
	towermenu_scene.window.clearColor[2] = 0.125;
	towermenu_scene.window.clearColor[3] = 1.;

	towermenu_scene.state.run = 1;
	towermenu_scene.state.returnControlValue = 0;
	towermenu_scene.state.pause = 0;

	isola_timerSetup(&towermenu_scene.timing.logicTimer, 60);
	isola_counterSetup(&towermenu_scene.timing.frameCounter, 60);


	isola_inputClear(isola_window);


	SDL_SetWindowSize(isola_window,towermenu_scene.window.windowRes[0],
			towermenu_scene.window.windowRes[1]);
	SDL_SetWindowPosition(isola_window,towermenu_scene.window.windowPos[0],
			towermenu_scene.window.windowPos[1]);
	SDL_SetWindowBordered(isola_window,towermenu_scene.window.windowBorder);
	SDL_SetWindowResizable(isola_window,towermenu_scene.window.windowResizable);
	if(towermenu_scene.window.windowFullscreen){
		SDL_SetWindowFullscreen(isola_window,1);
	}
	glClearColor(towermenu_scene.window.clearColor[0],
			towermenu_scene.window.clearColor[1],
			towermenu_scene.window.clearColor[2],
			towermenu_scene.window.clearColor[3]);


	createBitmenu();

	bitmenuBuildMenu();
	bitmenuBuildGroup(-1,1,1,1);
	bitmenuBuildScreen(2);
	bitmenuBuildObject("MENU - 'j'/'k' - up/down",0);
	bitmenuBuildObject("MENU - 'return' - activate option",0);
	bitmenuBuildObject("edit rules",0);
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(2);
		bitmenuBuildObject("these are rules :)",0);
		bitmenuBuildObject("press q to go back to last menu",0);
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("play",towermenu_function_play);
	bitmenuBuildObject("join",0);
	bitmenuBuildObject("host",0);
	bitmenuBuildObject("options",0);
	bitmenuBuildGroup(-1,1,1,1);
		bitmenuBuildScreen(2);
		bitmenuBuildObject("these are options :)",0);
		bitmenuBuildObject("press q to go back to last menu",0);
	bitmenuBuildGroupLeave();
	bitmenuBuildObject("quit",towermenu_function_quit);

	createBitfont();
	createDigitfps();


	towermenu_update();
}


static void towermenu_destroy(void){

	isola_inputClear(isola_window);


	destroyBitmenu();

	destroyBitfont();
	destroyDigitfps();


	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow(isola_window);
}




unsigned char towermenu_loop(void){

	SDL_Event event = {0};


	towermenu_create();

	while(towermenu_scene.state.run){

		while (SDL_PollEvent(&event)){
			switch (event.type) {
				case SDL_EVENT_QUIT:
				case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
					towermenu_scene.state.returnControlValue = 0;
					towermenu_scene.state.run = 0;
				break;
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
					towermenu_update();
				break;
				case SDL_EVENT_KEY_DOWN:
					if (!event.key.repeat) {
						switch (event.key.key){
							case SDLK_ESCAPE:
								towermenu_scene.state.returnControlValue = 0;
								towermenu_scene.state.run = 0;
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


		if (!towermenu_scene.state.pause) {
			if(isola_timerStep(&currentScene->timing.logicTimer)){

				stepBitmenu();
				isola_inputRepeat();
			}

			if(isola_counterStep(&currentScene->timing.frameCounter)){

				glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


				drawBitfont();
				drawDigitfps();


				SDL_GL_SwapWindow(isola_window);
			}
		}


#define RESOURCE_PADDING
#ifdef RESOURCE_PADDING
		SDL_Delay(1);
#endif
	}


	towermenu_destroy();
	return towermenu_scene.state.returnControlValue;
}




