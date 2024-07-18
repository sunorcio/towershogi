



#include <isola/isola.h>




static int keyLength;
const unsigned char* keyState;


void inputSetup(void){

	if (SDL_IsTextInputActive()) {SDL_StopTextInput();}

	keyState = SDL_GetKeyboardState(&keyLength);
}




#define textCharSize 32
#define maxTextStringLength 32

static unsigned int textCursor = 0;
static unsigned int textStringLength = 0;
static char(* textChars)[textCharSize] = {0};
char* inputTextString = {0};
unsigned char textEditing = 0;


static void inputBuildTextString(void){

	memset(inputTextString,0,textStringLength*sizeof(char)*textCharSize);
	{unsigned int i;
	for(i = 0;i<textStringLength;i++){
		strcat(inputTextString,textChars[i]);
	}}
}


void inputTextEditingStart(unsigned int textLength){

	if (textEditing) {
		return;
	}
	if (!textLength) {
		SDL_Log("text mode : text length must not be zero");
		return;
	}
	if (maxTextStringLength && textLength > maxTextStringLength) {
		SDL_Log("text mode : text length cannot exceed %d",maxTextStringLength);
		return;
	}


	textStringLength = textLength;
	textChars = calloc(sizeof(char)*textCharSize,textStringLength);
	inputTextString = calloc(sizeof(char),textCharSize*textStringLength);
	textCursor = 0;


	SDL_StartTextInput();
	textEditing = 1;
}


void inputTextEditingPush(char textChar[32]){

	if (!textEditing) {
		return;
	}
	if (textCursor == textStringLength) {
		return;
	}

	{unsigned int i;
	for(i = 0;i<textCharSize;i++){
		textChars[textCursor][i] = textChar[i];
	}}
	textCursor++;

	inputBuildTextString();
}


void inputTextEditingPop(void){

	if (!textEditing) {
		return;
	}
	if (!textCursor) {
		return;
	}

	textCursor--;
	{unsigned int i;
	for(i = 0;i<textCharSize;i++){
		textChars[textCursor][i] = 0;
	}}

	inputBuildTextString();
}


void inputTextEditingStop(void){

	if (textEditing) {
		free(textChars);
		free(inputTextString);
	}else {
		return;
	}

	SDL_StopTextInput();
	textEditing = 0;
}




