#ifndef DIGITFPS_H
#define DIGITFPS_H




struct DIGITFPS_font{
	float color[4];
	unsigned int sizePixelWidth;
}extern digitfps_option;




extern void digitfps_update(void);
extern void digitfps_create(void);
extern void digitfps_destroy(void);
extern void digitfps_draw(void);




#endif
