

#include "towershogi_logic.h"




#include "string.h"

#include "isola/isola.h"
#include "isola/misc.h"

#include "towershogi.h"
#include "towershogi_render_logic.h"




struct TOWERSHOGI_piece{
	unsigned char player;
	unsigned char movement[5][5];
};

struct TOWERSHOGI_board{
	unsigned short currentTile;
	unsigned short selectedTile;
	struct TOWERSHOGI_piece tile[TOWERSHOGI_BOARD_X][TOWERSHOGI_BOARD_Y];
}towershogiBoard;




static void towershogiLoadRenderstate(void){

	{unsigned int i;
	for(i = 0;i<sizeof(towershogiBoard.tile)/
			sizeof(towershogiBoard.tile[0][0]);i++){
		towershogiBoardstate[0][i] = towershogiBoard.tile[0][i].player;
	}}
}




void updateTowershogiLogic(void){

}

void createTowershogiLogic(void){

	memset(&towershogiBoard,0,sizeof(struct TOWERSHOGI_board));
	towershogiBoard.selectedTile = 0xffff;

	towershogiBoard.tile[TOWERSHOGI_BOARD_X/2][TOWERSHOGI_BOARD_Y/2].player = 1;
	{unsigned int i;
	for(i = 0;i<5;i++){
		{unsigned int j;
		for(j = 0;j<5;j++){
			if (i<3 && i>0 && j<3 && j>0) {
				towershogiBoard.tile[TOWERSHOGI_BOARD_X/2]
						[TOWERSHOGI_BOARD_Y/2].movement[i][j] = 1;
			}
		}}
	}}
}

void destroyTowershogiLogic(void){

}

void stepTowershogiLogic(void){

	towershogiLoadRenderstate();
}




