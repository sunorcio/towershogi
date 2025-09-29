#ifndef TOWERSHOGI_H
#define TOWERSHOGI_H




#define TOWERSHOGI_BOARD_SIZE (towershogi_boardSize[0]*towershogi_boardSize[1])




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
}extern towershogi_board;




extern unsigned short towershogi_boardSize[2];




extern void towershogi_update(void);
extern void towershogi_create(void);
extern void towershogi_destroy(void);
extern void towershogi_step(void);
extern void towershogi_draw(void);




#endif
