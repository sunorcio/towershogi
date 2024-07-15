



#include "bitmenu.h"


#include <isola/isola.h>


#include <stdlib.h>
#include <string.h>


#include <render/bitfont_logic.h>




void updateBitmenu(void){

	int pixelSize = 8*2;

	{unsigned int i;
	for(i = 0;i<6;i++){
		bitfontObjectData[i].pixelSize = pixelSize;
	}}
}

void createBitmenu(void){

	int i = 0.;
	int n = 0.;


	bitfontObjectAmount = 23;
	bitfontObjectData = calloc(bitfontObjectAmount,
			sizeof(struct BITFONT_textobject));


	updateBitmenu();


	n = 6;

	i = 0;
	strcpy(bitfontObjectData[0].string,"Profile\x7f\x7f\x7f\x7f< none >");
	bitfontObjectData[0].x = -0.75;
	bitfontObjectData[0].y = 1. - (2./n)*i;
	bitfontObjectData[0].charWrap = bitfontStringSize;
	bitfontObjectData[0].fontColor[0] = 0.;
	bitfontObjectData[0].fontColor[1] = 0.;
	bitfontObjectData[0].fontColor[2] = 0.;
	bitfontObjectData[0].fontColor[3] = 1.;
	bitfontObjectData[0].backColor[0] = 1.;
	bitfontObjectData[0].backColor[1] = 1.;
	bitfontObjectData[0].backColor[2] = 1.;
	bitfontObjectData[0].backColor[3] = 0.5;
	
	i = 1;
	strcpy(bitfontObjectData[1].string,"Play");
	bitfontObjectData[1].x = -0.75;
	bitfontObjectData[1].y = 1. - (2./n)*i;
	bitfontObjectData[1].charWrap = bitfontStringSize;
	bitfontObjectData[1].fontColor[0] = 0.25;
	bitfontObjectData[1].fontColor[1] = 0.25;
	bitfontObjectData[1].fontColor[2] = 0.25;
	bitfontObjectData[1].fontColor[3] = 1.;
	bitfontObjectData[1].backColor[0] = 1.;
	bitfontObjectData[1].backColor[1] = 1.;
	bitfontObjectData[1].backColor[2] = 1.;
	bitfontObjectData[1].backColor[3] = 0.5;

	i = 2;
	strcpy(bitfontObjectData[2].string,"New Game");
	bitfontObjectData[2].x = -0.75;
	bitfontObjectData[2].y = 1. - (2./n)*i;
	bitfontObjectData[2].charWrap = bitfontStringSize;
	bitfontObjectData[2].fontColor[0] = 0.;
	bitfontObjectData[2].fontColor[1] = 0.;
	bitfontObjectData[2].fontColor[2] = 0.;
	bitfontObjectData[2].fontColor[3] = 1.;
	bitfontObjectData[2].backColor[0] = 1.;
	bitfontObjectData[2].backColor[1] = 1.;
	bitfontObjectData[2].backColor[2] = 1.;
	bitfontObjectData[2].backColor[3] = 0.5;

	i = 3;
	strcpy(bitfontObjectData[3].string,"Options");
	bitfontObjectData[3].x = -0.75;
	bitfontObjectData[3].y = 1. - (2./n)*i;
	bitfontObjectData[3].charWrap = bitfontStringSize;
	bitfontObjectData[3].fontColor[0] = 0.;
	bitfontObjectData[3].fontColor[1] = 0.;
	bitfontObjectData[3].fontColor[2] = 0.;
	bitfontObjectData[3].fontColor[3] = 1.;
	bitfontObjectData[3].backColor[0] = 1.;
	bitfontObjectData[3].backColor[1] = 1.;
	bitfontObjectData[3].backColor[2] = 1.;
	bitfontObjectData[3].backColor[3] = 0.5;

	i = 4;
	strcpy(bitfontObjectData[4].string,"Credits");
	bitfontObjectData[4].x = -0.75;
	bitfontObjectData[4].y = 1. - (2./n)*i;
	bitfontObjectData[4].charWrap = bitfontStringSize;
	bitfontObjectData[4].fontColor[0] = 0.;
	bitfontObjectData[4].fontColor[1] = 0.;
	bitfontObjectData[4].fontColor[2] = 0.;
	bitfontObjectData[4].fontColor[3] = 1.;
	bitfontObjectData[4].backColor[0] = 1.;
	bitfontObjectData[4].backColor[1] = 1.;
	bitfontObjectData[4].backColor[2] = 1.;
	bitfontObjectData[4].backColor[3] = 0.5;

	i = 5;
	strcpy(bitfontObjectData[5].string,"Quit");
	bitfontObjectData[5].x = -0.75;
	bitfontObjectData[5].y = 1. - (2./n)*i;
	bitfontObjectData[5].charWrap = bitfontStringSize;
	bitfontObjectData[5].fontColor[0] = 0.;
	bitfontObjectData[5].fontColor[1] = 0.;
	bitfontObjectData[5].fontColor[2] = 0.;
	bitfontObjectData[5].fontColor[3] = 1.;
	bitfontObjectData[5].backColor[0] = 1.;
	bitfontObjectData[5].backColor[1] = 1.;
	bitfontObjectData[5].backColor[2] = 1.;
	bitfontObjectData[5].backColor[3] = 0.5;
}


void destroyBitmenu(void){

	free(bitfontObjectData);
}


void stepBitmenu(void){

}



