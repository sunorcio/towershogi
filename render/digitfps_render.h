#ifndef DIGITFPS_RENDER_H
#define DIGITFPS_RENDER_H




#include <scene/timer.h>




extern void updateDigitfps(struct TIMER_timer* timer);
extern void createDigitfps(struct TIMER_timer* timer);
extern void destroyDigitfps(void);
extern void drawDigitfps(void);




#endif
