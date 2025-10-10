#ifndef BITMENU_BUILD_H
#define BITMENU_BUILD_H




extern void bitmenu_buildMenu(void);
extern void bitmenu_buildGroup(float x, float y, float w, float h);
extern void bitmenu_buildScreen(unsigned char fontScaleGlobal);
extern void bitmenu_buildObject(const char* name, void (*function)(void));
extern void bitmenu_buildGroupLeave(void);
extern void bitmenu_buildExample(void);




#endif
