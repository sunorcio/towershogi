#ifndef TOWERSHOGI_H
#define TOWERSHOGI_H




#define TOWERSHOGI_BOARD_SIZE (towershogi_boardSize[0]*towershogi_boardSize[1])




extern void towershogi_update(void);
extern void towershogi_create(void);
extern void towershogi_destroy(void);
extern void towershogi_step(void);
extern void towershogi_draw(void);




#endif
