#ifndef INPUT_H
#define INPUT_H




#include <isola/isola.h>




extern const unsigned char* keyState;
extern unsigned char keyRepeat[SDL_NUM_SCANCODES];


extern void inputClear(void);
extern void inputRepeat(void);




extern char* inputTextString;
extern unsigned char textEditing;


extern void inputTextEditingStart(unsigned int textLength,char(* textLoad)[32]);
extern void inputTextEditingPush(char(* textChar)[32]);
extern void inputTextEditingPop(void);
extern void inputTextEditingStop(void);




#endif
