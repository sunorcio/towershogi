



#include "bitmenu.h"


#include <isola/isola.h>
#include <input.h>


#include <stdlib.h>
#include <string.h>


#include <render/bitfont_logic.h>




struct BITMENU_group{
	unsigned int currentObject;
	unsigned int objectAmount;
	unsigned int fontPixelsize;
}group = { .currentObject = 0, };




void updateBitmenu(void){

	group.fontPixelsize = ( ( isolaInfoWindow.height/
			(int)((group.objectAmount+1)*1.5) )/16 ) *8;


	{unsigned int i;
	for(i = 0;i<group.objectAmount;i++){
		bitfontObjectData[i].pixelSize = group.fontPixelsize;

		bitfontObjectData[i].x = -1. +
				isolaInfoWindow.pixelWidth*group.fontPixelsize;
		bitfontObjectData[i].y = 1.-
				isolaInfoWindow.pixelHeight*group.fontPixelsize*3.*i-
				isolaInfoWindow.pixelHeight*group.fontPixelsize*2 ;
	}}
}


void createBitmenu(void){

	int i = 0.;


	group.objectAmount = 6;
	bitfontObjectAmount = 6;
	bitfontObjectData = calloc(bitfontObjectAmount,
			sizeof(struct BITFONT_textobject));


	updateBitmenu();


	i = 0;
	strcpy(bitfontObjectData[i].string,"Profile\x7f\x7f\x7f\x7f< none >");
	bitfontObjectData[i].charWrap = bitfontStringSize;
	bitfontObjectData[i].fontColor[0] = 0.;
	bitfontObjectData[i].fontColor[1] = 0.;
	bitfontObjectData[i].fontColor[2] = 0.;
	bitfontObjectData[i].fontColor[3] = 1.;
	bitfontObjectData[i].backColor[0] = 1.;
	bitfontObjectData[i].backColor[1] = 1.;
	bitfontObjectData[i].backColor[2] = 1.;
	bitfontObjectData[i].backColor[3] = 0.5;
	
	i = 1;
	strcpy(bitfontObjectData[i].string,"Play");
	bitfontObjectData[i].charWrap = bitfontStringSize;
	bitfontObjectData[i].fontColor[0] = 0.25;
	bitfontObjectData[i].fontColor[1] = 0.25;
	bitfontObjectData[i].fontColor[2] = 0.25;
	bitfontObjectData[i].fontColor[3] = 1.;
	bitfontObjectData[i].backColor[0] = 1.;
	bitfontObjectData[i].backColor[1] = 1.;
	bitfontObjectData[i].backColor[2] = 1.;
	bitfontObjectData[i].backColor[3] = 0.5;

	i = 2;
	strcpy(bitfontObjectData[i].string,"New Game");
	bitfontObjectData[i].charWrap = bitfontStringSize;
	bitfontObjectData[i].fontColor[0] = 0.;
	bitfontObjectData[i].fontColor[1] = 0.;
	bitfontObjectData[i].fontColor[2] = 0.;
	bitfontObjectData[i].fontColor[3] = 1.;
	bitfontObjectData[i].backColor[0] = 1.;
	bitfontObjectData[i].backColor[1] = 1.;
	bitfontObjectData[i].backColor[2] = 1.;
	bitfontObjectData[i].backColor[3] = 0.5;

	i = 3;
	strcpy(bitfontObjectData[i].string,"Options");
	bitfontObjectData[i].charWrap = bitfontStringSize;
	bitfontObjectData[i].fontColor[0] = 0.;
	bitfontObjectData[i].fontColor[1] = 0.;
	bitfontObjectData[i].fontColor[2] = 0.;
	bitfontObjectData[i].fontColor[3] = 1.;
	bitfontObjectData[i].backColor[0] = 1.;
	bitfontObjectData[i].backColor[1] = 1.;
	bitfontObjectData[i].backColor[2] = 1.;
	bitfontObjectData[i].backColor[3] = 0.5;

	i = 4;
	strcpy(bitfontObjectData[i].string,"Credits");
	bitfontObjectData[i].charWrap = bitfontStringSize;
	bitfontObjectData[i].fontColor[0] = 0.;
	bitfontObjectData[i].fontColor[1] = 0.;
	bitfontObjectData[i].fontColor[2] = 0.;
	bitfontObjectData[i].fontColor[3] = 1.;
	bitfontObjectData[i].backColor[0] = 1.;
	bitfontObjectData[i].backColor[1] = 1.;
	bitfontObjectData[i].backColor[2] = 1.;
	bitfontObjectData[i].backColor[3] = 0.5;

	i = 5;
	strcpy(bitfontObjectData[i].string,"Quit");
	bitfontObjectData[i].charWrap = bitfontStringSize;
	bitfontObjectData[i].fontColor[0] = 0.;
	bitfontObjectData[i].fontColor[1] = 0.;
	bitfontObjectData[i].fontColor[2] = 0.;
	bitfontObjectData[i].fontColor[3] = 1.;
	bitfontObjectData[i].backColor[0] = 1.;
	bitfontObjectData[i].backColor[1] = 1.;
	bitfontObjectData[i].backColor[2] = 1.;
	bitfontObjectData[i].backColor[3] = 0.5;
}


void destroyBitmenu(void){

	free(bitfontObjectData);
}


static unsigned char kpressed;
static unsigned char jpressed;


void stepBitmenu(void){



	{unsigned int i;
	for(i = 0;i<group.objectAmount;i++){
		bitfontObjectData[i].backColor[3] = 0.5;
	}}

	bitfontObjectData[group.currentObject].backColor[3] = 0.875;
}




