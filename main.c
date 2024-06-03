

#include <time.h>
#include <stdlib.h>


#include "isola/isola.h"


#define TIMER_IMPLEMENTATION
#include "timer.h"
#include "logic.h"
#include "render.h"




void loop(void){

	int keyLength;
	const unsigned char* keyState = SDL_GetKeyboardState(&keyLength);

	SDL_Event event = {0};
	unsigned char run = 1;
	unsigned char pause = 0;


	setupTimer();


	while(run){
		while (SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){run = 0;}
			if(event.type == SDL_WINDOWEVENT){
				switch(event.window.event){
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					case SDL_WINDOWEVENT_DISPLAY_CHANGED:
						renderGlobalUpdate();
					break;
					case SDL_WINDOWEVENT_CLOSE:
						run = !run;
					break;
				}
			}
			if(event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
					case SDLK_q:
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
			if(currentStep>=lastStep+clockFreq/isolaSPS){
				lastStep = currentStep;

			}
		}

		{ unsigned long currentFrame = SDL_GetPerformanceCounter();
		if(currentFrame>=lastFrame+clockFreq/isolaFPS){
			frameDelay[frameIndex] = currentFrame-lastFrame;
			lastFrame = currentFrame;
			frameIndex++;


			renderGlobalDraw();


		}else{SDL_Delay(0);} }

	}
}




int main(int argc, char **argv){

	srand(time(0));
	isolaInit();


	renderGlobalCreate();
	loop();
	renderGlobalDestroy();


	isolaQuit();
	return 0;
}


