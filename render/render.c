

#include "render_main.h"
#include "render_logic.h"


#include "../isola/isola.h"


#include "digitfps.c"
#include "bitfont.c"




struct SCENE {
	unsigned char windowFullscreen;
	unsigned char windowBorder;
	unsigned char windowResizable;
	int windowPos[2];
	int windowRes[2];
}static const global = {
			.windowFullscreen = 0,
			.windowBorder = 1,
			.windowResizable = 1,
			.windowPos = {SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED},
			.windowRes = {800,600},
		};


void renderGlobalUpdate(void){

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width,
			isolaInfoWindow.height);


	updateBitfont();
	updateDigitfps();
}

void renderGlobalCreate(void){

	SDL_SetWindowSize(isolaWindow,global.windowRes[0],global.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,global.windowPos[0],global.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,global.windowBorder);
	SDL_SetWindowResizable(isolaWindow,global.windowResizable);
	if(global.windowFullscreen){
/* 		global.windowRes[0] = isolaInfoWindow.desktopDisplayMode.w;
		global.windowRes[1] = isolaInfoWindow.desktopDisplayMode.h;
		SDL_SetWindowSize(isolaWindow,global.windowRes[0],global.windowRes[1]);
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN); */
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}


	createBitfont();
	createDigitfps();


	renderGlobalUpdate();
}

void renderGlobalDestroy(void){

	destroyDigitfps();
	destroyBitfont();
}

void renderGlobalDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawBitfont();
	drawDigitfps();


	SDL_GL_SwapWindow(isolaWindow);
}


