#ifndef BITMENU_H
#define BITMENU_H




extern void bitmenuBuildMenu(void);
extern void bitmenuBuildGroup(float x, float y, float w, float h);
extern void bitmenuBuildScreen(unsigned char fontScaleGlobal);
extern void bitmenuBuildObject(const char* name);
extern void bitmenuBuildGroupLeave(void);




extern void updateBitmenu(void);
extern void createBitmenu(void);
extern void destroyBitmenu(void);
extern void stepBitmenu(void);




#endif
