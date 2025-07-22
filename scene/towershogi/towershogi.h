#ifndef TOWERSHOGI_H
#define TOWERSHOGI_H




#define TOWERSHOGI_BOARD_SIZE (towershogiBoardSize[0]*towershogiBoardSize[1])




struct TOWERSHOGI_piece{
	unsigned char piece;
	unsigned char state;
	unsigned char movement[5][5];
};

struct TOWERSHOGI_board{
	unsigned short currentPlayer;
	unsigned short currentTile;
	unsigned short selectedTile;
	struct TOWERSHOGI_piece * tile;
}extern towershogiBoard;




extern unsigned short towershogiBoardSize[2];




extern void updateTowershogiRender(void);
extern void createTowershogiRender(void);
extern void destroyTowershogiRender(void);
extern void drawTowershogiRender(void);




#endif
