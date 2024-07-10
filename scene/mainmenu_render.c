



#include "mainmenu.h"


#include <isola/isola.h>


#include <render/digitfps_render.h>
#include <render/bitfont_render.h>




void mainmenuRenderUpdate(void){

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width,
			isolaInfoWindow.height);


	updateBitfont();
	updateDigitfps(0);
}

void mainmenuRenderCreate(void){

	SDL_SetWindowSize(isolaWindow,mainmenu.windowRes[0],
			mainmenu.windowRes[1]);
	SDL_SetWindowPosition(isolaWindow,mainmenu.windowPos[0],
			mainmenu.windowPos[1]);
	SDL_SetWindowBordered(isolaWindow,mainmenu.windowBorder);
	SDL_SetWindowResizable(isolaWindow,mainmenu.windowResizable);
	if(mainmenu.windowFullscreen){
		SDL_SetWindowFullscreen(isolaWindow,SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	glClearColor(mainmenu.clearcolor[0],mainmenu.clearcolor[1],
			mainmenu.clearcolor[2],mainmenu.clearcolor[3]);


	createBitfont();
	createDigitfps(&mainmenu.timer);


	mainmenuRenderUpdate();
}


void mainmenuRenderDestroy(void){

	destroyDigitfps();
	destroyBitfont();
}


void mainmenuRenderDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawBitfont();
	drawDigitfps();


	SDL_GL_SwapWindow(isolaWindow);
}




