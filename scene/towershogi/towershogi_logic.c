

#include "towershogi_logic.h"




#include <stdlib.h>
#include <string.h>


#include <input.h>

#include "towershogi.h"
#include "towershogi_render_logic.h"




struct TOWERSHOGI_piece{
	unsigned char player;
	unsigned char movement[5][5];
};

struct TOWERSHOGI_board{
	unsigned short currentTile;
	unsigned short selectedTile;
	struct TOWERSHOGI_piece * tile;
}towershogiBoard;




void updateTowershogiLogic(void){

}

void createTowershogiLogic(void){

	memset(&towershogiBoard,0,sizeof(struct TOWERSHOGI_board));
	towershogiBoard.tile = calloc(sizeof(struct TOWERSHOGI_piece),
			TOWERSHOGI_BOARD_SIZE);
	towershogiBoard.selectedTile = 0xffff;
	memset(towershogiBoard.tile,0,
			sizeof(struct TOWERSHOGI_piece)*TOWERSHOGI_BOARD_SIZE);


	towershogiBoard.tile[0].player = 1;
	{unsigned int i;
	for(i = 0;i<5;i++){
		{unsigned int j;
		for(j = 0;j<5;j++){
			if (i<3 && i>0 && j<3 && j>0) {
				towershogiBoard.tile[0].movement[i][j] = 1;
			}
		}}
	}}
}

void destroyTowershogiLogic(void){

}

void stepTowershogiLogic(void){

	if (keyState[SDL_SCANCODE_J] && !keyRepeat[SDL_SCANCODE_J]) {
		{unsigned int i;
		for(i = 0;i<TOWERSHOGI_BOARD_SIZE-1;i++){
			if (towershogiBoard.tile[i].player) {
				towershogiBoard.tile[i].player = 0;
				towershogiBoard.tile[i+1].player = 1;
				break;
			}
		}}
	}
	if (keyState[SDL_SCANCODE_K] && !keyRepeat[SDL_SCANCODE_K]) {
		{unsigned int i;
		for(i = 1;i<TOWERSHOGI_BOARD_SIZE;i++){
			if (towershogiBoard.tile[i].player) {
				towershogiBoard.tile[i].player = 0;
				towershogiBoard.tile[i-1].player = 1;
			}
		}}
	}

	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		towershogiBoardstate[i] = towershogiBoard.tile[i].player;
	}}
}




