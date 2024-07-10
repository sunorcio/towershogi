



#include "mainmenu.h"


#include <isola/isola.h>


#include <render/digitfps_logic.h>
#include <logic/bitmenu.h>


#include "mainmenu_render.c"




struct SCENE mainmenu = {
			.windowFullscreen = 0,
			.windowBorder = 1,
			.windowResizable = 1,
			.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
			.windowRes = {800,600},
			.clearcolor = {0.,0.,0.,1.},
			.timer.fps = 60,
			.timer.sps = 60,
		};




void mainmenuLoop(void){

	int keyLength;
	const unsigned char* keyState = SDL_GetKeyboardState(&keyLength);

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;

	mainmenu.timer.clockFreq = SDL_GetPerformanceFrequency();
	mainmenu.timer.lastFrame = SDL_GetPerformanceCounter();
	mainmenu.timer.lastStep = SDL_GetPerformanceCounter();

	bitmenuCreate();
	mainmenuRenderCreate();


	while(run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						mainmenuRenderUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						run = !run;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						run = !run;
					break;
					case SDLK_p:
						pause = !pause;
					break;
					case SDLK_SPACE:
					break;
				}
			}
		}

		if(!pause){
			unsigned long currentStep = SDL_GetPerformanceCounter();
			if(currentStep>=mainmenu.timer.lastStep+
					mainmenu.timer.clockFreq/mainmenu.timer.sps){
				mainmenu.timer.lastStep = currentStep;


			}
		}

		{unsigned long currentFrame = SDL_GetPerformanceCounter();
		if(currentFrame>=mainmenu.timer.lastFrame+
				mainmenu.timer.clockFreq/mainmenu.timer.fps){
			digitfpsCount(currentFrame-mainmenu.timer.lastFrame);
			mainmenu.timer.lastFrame = currentFrame;

			mainmenuRenderDraw();


		}else{
			SDL_Delay(0);
		}}

	}

	mainmenuRenderDestroy();
	bitmenuDestroy();

}




