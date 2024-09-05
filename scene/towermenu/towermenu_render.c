

#include "towermenu_render.h"




#include <isola/isola.h>


#include <module/digitfps/digitfps_render.h>
#include <module/bitfont/bitfont_render.h>




extern struct TIMING_counter towermenuFrameCounter;




void towermenuRenderUpdate(void){

	updateBitfont();
	updateDigitfps();
}


void towermenuRenderCreate(void){

	createBitfont();
	createDigitfps(&towermenuFrameCounter);
}


void towermenuRenderDestroy(void){

	destroyBitfont();
	destroyDigitfps();


	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	SDL_GL_SwapWindow(isolaWindow);
}


void towermenuRenderDraw(void){

	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );


	drawBitfont();
	drawDigitfps();


	SDL_GL_SwapWindow(isolaWindow);
}




