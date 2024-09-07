

#include "towershogi_scene_render.h"




#include <isola/isola.h>

#include <module/digitfps/digitfps_render.h>

#include "towershogi_render.h"




extern struct TIMING_counter towershogiFrameCounter;




void towershogiRenderUpdate(void){

	updateDigitfps();

	updateTowershogiRender();
}


void towershogiRenderCreate(void){

	createDigitfps(&towershogiFrameCounter);

	createTowershogiRender();
}


void towershogiRenderDestroy(void){

	destroyDigitfps();

	destroyTowershogiRender();


	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow(isolaWindow);
}


void towershogiRenderDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawDigitfps();

	drawTowershogiRender();


	SDL_GL_SwapWindow(isolaWindow);
}




