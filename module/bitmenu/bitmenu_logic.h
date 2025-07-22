#ifndef BITMENU_LOGIC_H
#define BITMENU_LOGIC_H




extern void bitmenuBuildMenu(void);
extern void bitmenuBuildGroup(float x, float y, float w, float h);
extern void bitmenuBuildScreen(unsigned char fontScaleGlobal);
extern void bitmenuBuildObject(const char* name, void (*function)(void));
extern void bitmenuBuildGroupLeave(void);
extern void bitmenuBuildExample(void);




#endif
