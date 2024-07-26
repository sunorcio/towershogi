



#include "bitmenu.h"


#include <stdlib.h>
#include <string.h>


#include <isola/isola.h>
#include <isola/misc.h>


#include <input.h>

#include <render/bitfont_logic.h>




#define BITMENU_FORE 0.5
#define BITMENU_HIGH 1.5


struct BITMENU_group{
	unsigned int objectAmount;
	unsigned int fontPixelsize;
	unsigned int currentObject;
}static bitmenuGroup[] = {
			{ .objectAmount=6, .fontPixelsize=16},
		};


void updateBitmenu(void){

	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		bitfontObjectData[i].y = 1.-
				isolaInfoWindow.pixelHeight*bitmenuGroup[0].fontPixelsize*2*i;
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


	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		strcpy(bitfontObjectData[i].string,"asdfasdfasdfasdfasdfasdfasdf");
		bitfontObjectData[i].pixelSize = bitmenuGroup[0].fontPixelsize;
		bitfontObjectData[i].charWrap = bitfontStringSize;
		bitfontObjectData[i].x = -1.;
		bitfontObjectData[i].fontColor[0] = 0.;
		bitfontObjectData[i].fontColor[1] = 0.;
		bitfontObjectData[i].fontColor[2] = 0.;
		bitfontObjectData[i].fontColor[3] = 1.;
		bitfontObjectData[i].backColor[0] = 1.;
		bitfontObjectData[i].backColor[1] = 1.;
		bitfontObjectData[i].backColor[2] = 1.;
		bitfontObjectData[i].backColor[3] = 1.;
	}}
}


void destroyBitmenu(void){

	free(bitfontObjectData);
}


void stepBitmenu(void){

	/* 1 dimensional group node traversal */
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
}




	/* retrieve largest string size */
/* 	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		if(strlen(bitfontObjectData[i].string) > bitmenuGroup[0].largestString){
			bitmenuGroup[0].largestString = strlen(bitfontObjectData[i].string);
		}
	}} */


	/* update highlight */
/* 	{unsigned int i;
	for(i = 0;i<bitmenuGroup[0].objectAmount;i++){
		bitfontObjectData[i].backColor[3] = BITMENU_FORE;
		bitfontObjectData[i].fontColor[3] = BITMENU_FORE;
	}}
	bitfontObjectData[bitmenuGroup[0].currentObject].fontColor[3] *= 
			BITMENU_HIGH;
	bitfontObjectData[bitmenuGroup[0].currentObject].backColor[3] *= 
			BITMENU_HIGH; */


	/* resize font to fit dimensions */
/* 	{int pixelsizex;
	bitmenuGroup[0].fontPixelsize = ( ( isolaInfoWindow.height/
			(int)((bitmenuGroup[0].objectAmount+1)*1.5) )/16 ) *8;
	pixelsizex = ( ( isolaInfoWindow.width/
			(int)(bitmenuGroup[0].largestString+bitmenuGroup[0].fontOffset+1) )/
			8 )*8;
	if ( bitmenuGroup[0].fontPixelsize > pixelsizex ) {
		bitmenuGroup[0].fontPixelsize = pixelsizex;
	}} */
