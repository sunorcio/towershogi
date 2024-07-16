



#include <isola/isola.h>




static int keyLength;
const unsigned char* keyState;




void inputSetup(void){

	keyState = SDL_GetKeyboardState(&keyLength);
}




