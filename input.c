



#include <isola/isola.h>




static int keyLength;
const unsigned char* keyState;




void inputSetup(void){

	if (SDL_IsTextInputActive()) {SDL_StopTextInput();}

	keyState = SDL_GetKeyboardState(&keyLength);
}









