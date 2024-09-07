

#include "towershogi_logic.h"




#include "towershogi.h"
#include "towershogi_render_logic.h"




struct TOWERSHOGI_piece{
	unsigned char player;
	unsigned char movement[5*5];
};

struct TOWERSHOGI_board{
	unsigned short currentTile;
	unsigned short selectedTile;
	struct TOWERSHOGI_piece tile[TOWERSHOGI_BOARD_X][TOWERSHOGI_BOARD_Y];
}towershogiBoard;




void updateTowershogiLogic(void){
}

void createTowershogiLogic(void){
}

void destroyTowershogiLogic(void){
}

void stepTowershogiLogic(void){
}




