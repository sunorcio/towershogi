



#include "mainmenu.h"


#include <isola/isola.h>
#include <timing.h>
#include "scene.h"


#include "mainmenu_render.c"
#include "mainmenu_logic.c"




struct SCENE_scene mainmenu = {
			.windowFullscreen = 0,
			.windowBorder = 1,
			.windowResizable = 1,
			.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
			.windowRes = {800,600},
			.clearColor = {0.,0.,0.,1.},
		};


struct TIMING_timer logicTimer = {0};
struct TIMING_counter frameCounter = {0};


void mainmenuLoop(void){

	int keyLength;
	const unsigned char* keyState = SDL_GetKeyboardState(&keyLength);

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;


	timerSetup(&logicTimer, 60);
	counterSetup(&frameCounter, 60);
	sceneSetup(&mainmenu);
	mainmenuRenderCreate();
	mainmenuLogicCreate();


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

		if(!pause && timerStep(&logicTimer)){


		}

		if(counterStep(&frameCounter)){

			mainmenuRenderDraw();

		}else{ SDL_Delay(0); }

	}

	mainmenuRenderDestroy();
	mainmenuLogicDestroy();
}




