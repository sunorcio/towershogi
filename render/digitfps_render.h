#ifndef DIGITFPS_RENDER_H
#define DIGITFPS_RENDER_H




#include <timing.h>




extern void updateDigitfps(void);
extern void createDigitfps(struct TIMING_counter* counter);
extern void destroyDigitfps(void);
extern void drawDigitfps(void);




#endif
