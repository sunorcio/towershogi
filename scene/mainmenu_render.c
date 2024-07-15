



#include <isola/isola.h>


#include <render/digitfps_render.h>
#include <render/bitfont_render.h>




extern struct TIMING_counter frameCounter;




void mainmenuRenderUpdate(void){

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




