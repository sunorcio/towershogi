



#include "bitmenu.h"


#include <stdlib.h>
#include <string.h>


#include <isola/isola.h>
#include <isola/misc.h>


#include <input.h>

#include <render/bitfont_logic.h>




struct BITMENU_group{
	unsigned char visible;
	unsigned char actionable;
	unsigned char alignment;
	float offset;
	unsigned int objectAmount;
	unsigned int currentObject;
	unsigned int largestString;
	unsigned int fontPixelsize;
}bitmenuGroup[] = {
		{.alignment = 0, .objectAmount = 6,.offset = 1},
		{.alignment = 1, .objectAmount = 4},
		{.alignment = 0, .objectAmount = 4},};




void updateBitmenu(void){

	{int pixelsizex;
	bitmenuGroup[0].fontPixelsize = ( ( isolaInfoWindow.height/
			(int)((bitmenuGroup[0].objectAmount+1)*1.5) )/16 ) *8;
	pixelsizex = ( ( isolaInfoWindow.width/
			(int)(bitmenuGroup[0].largestString+bitmenuGroup[0].offset+1) )/
			8 )*8;
	if ( bitmenuGroup[0].fontPixelsize > pixelsizex ) {
		bitmenuGroup[0].fontPixelsize = pixelsizex;
	}}


	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		bitfontObjectData[i].pixelSize = bitmenuGroup[0].fontPixelsize;

		bitfontObjectData[i].x = -1. + isolaInfoWindow.pixelWidth*
				bitmenuGroup[0].fontPixelsize*bitmenuGroup[0].offset;
		bitfontObjectData[i].y = 1.-
				isolaInfoWindow.pixelHeight*bitmenuGroup[0].fontPixelsize*3.*i-
				isolaInfoWindow.pixelHeight*bitmenuGroup[0].fontPixelsize*2 ;
	}}
}


void createBitmenu(void){

	int i = 0.;


	{unsigned int i;
	for(i = 0;i<isolaARRAYSIZE_(bitmenuGroup);i++){
		bitfontObjectAmount += bitmenuGroup[i].objectAmount;
	}}
	bitfontObjectData = calloc(bitfontObjectAmount,
			sizeof(struct BITFONT_textobject));


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


	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		if(strlen(bitfontObjectData[i].string) > bitmenuGroup[0].largestString){
			bitmenuGroup[0].largestString = strlen(bitfontObjectData[i].string);
		}
	}}


	updateBitmenu();
}


void destroyBitmenu(void){

	free(bitfontObjectData);
}


void stepBitmenu(void){

	if (keyState[SDL_SCANCODE_J] && !keyRepeat[SDL_SCANCODE_J]) {
		bitmenuGroup[0].currentObject++;
		if (bitmenuGroup[0].currentObject==bitmenuGroup[0].objectAmount) {
			bitmenuGroup[0].currentObject = 0;
		}
	}
	if (keyState[SDL_SCANCODE_K] && !keyRepeat[SDL_SCANCODE_K]) {
		if (bitmenuGroup[0].currentObject==0) {
			bitmenuGroup[0].currentObject = bitmenuGroup[0].objectAmount;
		}
		bitmenuGroup[0].currentObject--;
	}


	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		bitfontObjectData[i].backColor[3] = 0.5;
	}}

	bitfontObjectData[bitmenuGroup[0].currentObject].backColor[3] = 0.875;
}




