



#include <isola/isola.h>
#include "scene.h"


#include <render/digitfps_render.h>
#include <render/bitfont_render.h>




extern struct SCENE_scene mainmenu;


extern struct TIMING_counter frameCounter;




void mainmenuRenderUpdate(void){

	isolaGetWindow();
	glViewport(0,0,isolaInfoWindow.width,
			isolaInfoWindow.height);


	updateBitfont();
	updateDigitfps();
}


void mainmenuRenderCreate(void){

	createBitfont();
	createDigitfps(&frameCounter);


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




