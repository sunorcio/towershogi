

#include "towershogi_render.h"




#include <isola/isola.h>

#include <module/digitfps/digitfps_render.h>




extern struct TIMING_counter towershogiFrameCounter;




void towershogiRenderUpdate(void){

	updateDigitfps();
}


void towershogiRenderCreate(void){

	createDigitfps(&towershogiFrameCounter);
}


void towershogiRenderDestroy(void){

	destroyDigitfps();


	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow(isolaWindow);
}


void towershogiRenderDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawDigitfps();


	SDL_GL_SwapWindow(isolaWindow);
}




