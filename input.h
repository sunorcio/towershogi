#ifndef INPUT_H
#define INPUT_H




extern const unsigned char* keyState;


extern void inputSetup(void);





extern char* inputTextString;
extern unsigned char textEditing;


extern void inputTextEditingStart(unsigned int textLength);
extern void inputTextEditingPush(char textChar[32]);
extern void inputTextEditingPop(void);
extern void inputTextEditingStop(void);




#endif
