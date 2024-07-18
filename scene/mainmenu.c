



#include "mainmenu.h"


#include <isola/isola.h>
#include <isola/misc.h>
#include <timing.h>
#include <input.h>


#include "mainmenu_render.c"
#include "mainmenu_logic.c"




struct SCENE_mainmenu {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
/* 	unsigned char windowKeepratio; */
	int windowPos[2];
	int windowRes[2];
	int windowMinRes[2];
	float clearColor[4];
}mainmenuScene = {
		.windowFullscreen = 0,
		.windowBorder = 1,
		.windowResizable = 1,
		.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
		.windowRes = {800,600},
		.windowMinRes = {480,360},
		.clearColor = {0.,0.,0.,1.}, };


void sceneUpdate(struct SCENE_mainmenu* scene){

	isolaGetWindow();
	if (isolaInfoWindow.width < scene->windowMinRes[0]) {
		isolaInfoWindow.width = scene->windowMinRes[0];
	}
	if (isolaInfoWindow.height < scene->windowMinRes[1]) {
		isolaInfoWindow.height = scene->windowMinRes[1];
	}
	SDL_SetWindowSize(isolaWindow,isolaInfoWindow.width,isolaInfoWindow.height);


	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width, isolaInfoWindow.height);
}


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

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;
	unsigned char textinput = 0;

	char unistr[32] = {0};
	unsigned int unilen = {0};


	inputSetup();
	sceneSetup(&mainmenuScene);
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
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						sceneUpdate(&mainmenuScene);

						mainmenuRenderUpdate();
						mainmenuLogicUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						run = !run;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN && !event.key.repeat){
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						run = !run;
					break;
					case SDLK_p:
						pause = !pause;
					break;
					case SDLK_RETURN:
						if (!textinput) {
							SDL_StartTextInput();
							textinput = 1;
						}else {
							SDL_StopTextInput();
							textinput = 0;
							SDL_Log("%s",unistr);
							{unsigned int i;
							for(i = 0;i<isolaARRAYSIZE_(unistr);i++){
								unistr[i] = 0;
							}}
							unilen = 0;
						}
					break;
				}
			}
			if(event.type == SDL_TEXTINPUT){
				unilen += strlen(event.text.text);
				SDL_Log("%ld",strlen(event.text.text));
				if(unilen >= isolaARRAYSIZE_(unistr)){
					SDL_StopTextInput();
					textinput = 0;
					SDL_Log("%s",unistr);
					{unsigned int i;
					for(i = 0;i<isolaARRAYSIZE_(unistr);i++){
						unistr[i] = 0;
					}}
					unilen = 0;
				}else {
					strcat(unistr,event.text.text);
					SDL_Log("%s",event.text.text);;
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




