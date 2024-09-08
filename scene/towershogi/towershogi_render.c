

#include "towershogi_render.h"
#include "towershogi_render_logic.h"




#include "towershogi.h"




unsigned char towershogiBoardstate[TOWERSHOGI_BOARD_X][TOWERSHOGI_BOARD_Y];

/* boardstate enum? */
static float colorTable[5][3] = {
		{0.5,0.5,0.5},/* void */
		{0.0,0.0,0.75},/* player 1 piece */
		{0.5,0.0,0.5},/* selected player 1 piece */
		{0.0,0.75,0.0},/* possible moves */
		{0.75,0.0,0.0},/* possible move that eats piece */
		};




void updateTowershogiRender(void){

}

void createTowershogiRender(void){

}

void destroyTowershogiRender(void){

}

void drawTowershogiRender(void){

}




