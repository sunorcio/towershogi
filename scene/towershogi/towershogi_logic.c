

#include "towershogi_logic.h"




#include <stdlib.h>
#include <string.h>


#include <isola/input.h>

#include "towershogi.h"




#define PIECE_KING 0
#define PIECE_PAWN 1
#define PIECE_KNIGHT 2
#define PIECE_BISHOP 3
#define PIECE_ROOK 4
#define PIECE_QUEEN 5
#define PIECE_NONE 6

#define PLAYER_WHITE 0
#define PLAYER_BLACK 1
#define towershogiPIECE(piece,player) (piece+8*player)

#define STATE_NONE 0
#define STATE_SELECTED 1
#define STATE_MOVE 2
#define STATE_EAT 3

#define SELECTED_NONE 0xffff




void updateTowershogiLogic(void){

}


void createTowershogiLogic(void){

	memset(&towershogiBoard,0,sizeof(struct TOWERSHOGI_board));
	towershogiBoard.tile = calloc(sizeof(struct TOWERSHOGI_piece),
			TOWERSHOGI_BOARD_SIZE);
	towershogiBoard.selectedTile = SELECTED_NONE;
	memset(towershogiBoard.tile,0,
			sizeof(struct TOWERSHOGI_piece)*TOWERSHOGI_BOARD_SIZE);

	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		towershogiBoard.tile[i].piece = PIECE_NONE;
		towershogiBoard.tile[i].state = STATE_NONE;
	}}


	towershogiBoard.tile[0].piece = towershogiPIECE(PIECE_KING,PLAYER_WHITE);
	{unsigned int i;
	for(i = 0;i<5;i++){
		{unsigned int j;
		for(j = 0;j<5;j++){
			if ((i!=2 || j!=2) && i!=4 && j!=4 && i!=0 && j!=0) {
				towershogiBoard.tile[0].movement[i][j] = 1;
			}
		}}
	}}
}


void destroyTowershogiLogic(void){

	free(towershogiBoard.tile);
}


void stepTowershogiLogic(void){

	if (keyState[SDL_SCANCODE_J] && !keyRepeat[SDL_SCANCODE_J]) {
		if (towershogiBoard.currentTile >= towershogiBoardSize[0]) {
			towershogiBoard.currentTile -= towershogiBoardSize[0];
		}
	}
	if (keyState[SDL_SCANCODE_K] && !keyRepeat[SDL_SCANCODE_K]) {
		if (towershogiBoard.currentTile < TOWERSHOGI_BOARD_SIZE 
				-towershogiBoardSize[0]) {
			towershogiBoard.currentTile += towershogiBoardSize[0];
		}
	}
	if (keyState[SDL_SCANCODE_L] && !keyRepeat[SDL_SCANCODE_L]) {
		if ((towershogiBoard.currentTile+1)%towershogiBoardSize[0]) {
			towershogiBoard.currentTile += 1;
		}
	}
	if (keyState[SDL_SCANCODE_H] && !keyRepeat[SDL_SCANCODE_H]) {
		if (towershogiBoard.currentTile%towershogiBoardSize[0]) {
			towershogiBoard.currentTile -= 1;
		}
	}
	if (keyState[SDL_SCANCODE_SPACE] && !keyRepeat[SDL_SCANCODE_SPACE]) {
		if(towershogiBoard.selectedTile == SELECTED_NONE){
			towershogiBoard.tile[towershogiBoard.currentTile].state =
					STATE_SELECTED;
			towershogiBoard.selectedTile = towershogiBoard.currentTile;
		}
	}
	if (keyState[SDL_SCANCODE_Q] && !keyRepeat[SDL_SCANCODE_Q]) {
		if(towershogiBoard.selectedTile != SELECTED_NONE){
			towershogiBoard.currentTile = towershogiBoard.selectedTile;
			towershogiBoard.tile[towershogiBoard.selectedTile].state =
					STATE_NONE;
			towershogiBoard.selectedTile = SELECTED_NONE;
		}
	}
}




