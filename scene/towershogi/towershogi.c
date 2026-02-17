#include "towershogi.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>
#include <isola/input.h>


#include "module/bitfont/bitfont.h"
#include "module/bitfont/bitfont_data.h"




#define TOWERSHOGI_PIECE_NONE 0
#define TOWERSHOGI_PIECE_KING 1
#define TOWERSHOGI_PIECE_PAWN 2
#define TOWERSHOGI_PIECE_KNIGHT 3
#define TOWERSHOGI_PIECE_BISHOP 4
#define TOWERSHOGI_PIECE_ROOK 5
#define TOWERSHOGI_PIECE_QUEEN 6
#define TOWERSHOGI_PIECE_AMOUNT 7

#define TOWERSHOGI_PLAYER_WHITE 0x00
#define TOWERSHOGI_PLAYER_BLACK 0x80
#define TOWERSHOGI_PLAYER_MASK 0x80

#define TOWERSHOGI_PIECE_(piece,player) (piece|player)

#define TOWERSHOGI_STATE_NONE 0
#define TOWERSHOGI_STATE_HOVERED 1
#define TOWERSHOGI_STATE_HOVERED_AVAILIABLE 2
#define TOWERSHOGI_STATE_HOVERED_CAPTURE 3
#define TOWERSHOGI_STATE_SELECTED 4
#define TOWERSHOGI_STATE_SELECTED_UNAVAILIABLE 5
#define TOWERSHOGI_STATE_SELECTED_AVAILIABLE 6
#define TOWERSHOGI_STATE_SELECTED_CAPTURE 7

#define TOWERSHOGI_MOVEMENT_RANGE 4
#if(TOWERSHOGI_MOVEMENT_RANGE<4)
	#error TOWERSHOGI_MOVEMENT_RANGE must at least be 4
#endif
#define TOWERSHOGI_MOVEMENT_SPAN (1+TOWERSHOGI_MOVEMENT_RANGE*2)
#define TOWERSHOGI_MOVEMENT_CENTER TOWERSHOGI_MOVEMENT_RANGE

#define TOWERSHOGI_MOVEMENT_MAX 0xf8
#define TOWERSHOGI_MOVEMENT_NOCAPTURE 0xf9
#define TOWERSHOGI_MOVEMENT_CAPTURE 0xfa
#define TOWERSHOGI_MOVEMENT_FIRST 0xfb
#define TOWERSHOGI_MOVEMENT_FIRSTNOCAPTURE 0xfc
#define TOWERSHOGI_MOVEMENT_FIRSTCAPTURE 0xfd
#define TOWERSHOGI_MOVEMENT_CASTLE 0xfe
#define TOWERSHOGI_MOVEMENT_UNLIMITED 0xff

#define TOWERSHOGI_BOARD_WIDTH 8
#define TOWERSHOGI_BOARD_HEIGHT 8
#define TOWERSHOGI_BOARD_SIZE (TOWERSHOGI_BOARD_WIDTH*TOWERSHOGI_BOARD_HEIGHT)

#define TOWERSHOGI_TILE_SPAN 16
#define TOWERSHOGI_TILE_GAP 1




struct TOWERSHOGI_PIECE{
	unsigned int movement[TOWERSHOGI_MOVEMENT_SPAN][TOWERSHOGI_MOVEMENT_SPAN];
}static towershogi_piece[TOWERSHOGI_PIECE_AMOUNT] = {0};

struct TOWERSHOGI_TILE{
	unsigned int state;
	unsigned int piece;
	unsigned int moved;
	unsigned int enpasse;
}static towershogi_board[TOWERSHOGI_BOARD_SIZE] = {0};

struct TOWERSHOGI_CURSOR{
	struct TOWERSHOGI_TILE* hovered;
	struct TOWERSHOGI_TILE* selected;
	struct TOWERSHOGI_TILE* predict;
	int xHover;
	int yHover;
	int xSelected;
	int ySelected;
	int xPredict;
	int yPredict;
	int turnPlayer;
	float offsetArea[2];
	float tileSpan;
	float mousePos[2];
}static towershogi_cursor = {0};

static struct BITFONT_data* towershogi_bitfontPtr;

static int towershogi_helpShown = {0};


static ISOLA_state towershogi_glstate = ISOLA_STATE_BLEND;
static unsigned int towershogi_shaderProg[2] = {0};
static unsigned int towershogi_vertArrObj[1] = {0};
static unsigned int towershogi_vertBufObj[1] = {0};
static unsigned int towershogi_texObj[1] = {0};
static unsigned char* towershogi_vertData = {0};


static void towershogi_chessSetup(void);




static void towershogi_chessSetup(void){

	SDL_memset(towershogi_piece,0,
			sizeof(*towershogi_piece)*TOWERSHOGI_PIECE_AMOUNT);

	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER-3] = TOWERSHOGI_MOVEMENT_CASTLE;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER-4] = TOWERSHOGI_MOVEMENT_CASTLE;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER+3] = TOWERSHOGI_MOVEMENT_CASTLE;
	towershogi_piece[TOWERSHOGI_PIECE_KING]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER+4] = TOWERSHOGI_MOVEMENT_CASTLE;

	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_NOCAPTURE;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_CAPTURE;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_CAPTURE;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-2]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_FIRSTNOCAPTURE;

	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-2]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-2]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER-2] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER+2] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER-2] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER+2] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+2]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_KNIGHT]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+2]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = 1;

	towershogi_piece[TOWERSHOGI_PIECE_BISHOP]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_BISHOP]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_BISHOP]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_BISHOP]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_BISHOP]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_UNLIMITED;

	towershogi_piece[TOWERSHOGI_PIECE_ROOK]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_ROOK]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_ROOK]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_ROOK]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_ROOK]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_UNLIMITED;

	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_UNLIMITED;
	towershogi_piece[TOWERSHOGI_PIECE_QUEEN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER+1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_UNLIMITED;


	SDL_memset(towershogi_board,0,
			sizeof(*towershogi_board)*TOWERSHOGI_BOARD_SIZE);

	towershogi_board[0+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_ROOK,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[1+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KNIGHT,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[2+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_BISHOP,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[3+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_QUEEN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[4+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KING,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[5+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_BISHOP,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[6+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KNIGHT,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[7+0*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_ROOK,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[0+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[1+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[2+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[3+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[4+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[5+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[6+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);
	towershogi_board[7+1*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE);

	towershogi_board[0+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[1+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[2+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[3+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[4+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[5+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[6+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[7+6*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[0+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_ROOK,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[1+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KNIGHT,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[2+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_BISHOP,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[3+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_QUEEN,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[4+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KING,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[5+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_BISHOP,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[6+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KNIGHT,TOWERSHOGI_PLAYER_BLACK);
	towershogi_board[7+7*TOWERSHOGI_BOARD_WIDTH].piece =
			TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_ROOK,TOWERSHOGI_PLAYER_BLACK);
}




void towershogi_update(void){

	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[0]); )

	{int loc = {0};
	float projection[4*4] = {0};
	int boardWidth = {0};
	int boardHeight = {0};
	int resolutionFit = {0};
	int boardFit = {0};
	int intScreenScaling = {0};
	float tileScale = {0};
	float screenOffset[2] = {0};

	isola_mut_glproj_ortho(-isola_info_window.xLowRatio,
			isola_info_window.xLowRatio,-isola_info_window.yLowRatio,
			isola_info_window.yLowRatio,0.25f,8.f,projection);
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"projection"); )
	if(loc == -1){ SDL_Log("towershogi: projection not found"); }
	ISOLA_GLDBG_( glUniformMatrix4fv(loc,1,GL_FALSE,projection); )

	boardWidth = ( (TOWERSHOGI_TILE_SPAN+TOWERSHOGI_TILE_GAP)
			*TOWERSHOGI_BOARD_WIDTH )-TOWERSHOGI_TILE_GAP;
	boardHeight = ( (TOWERSHOGI_TILE_SPAN+TOWERSHOGI_TILE_GAP)
			*TOWERSHOGI_BOARD_HEIGHT )-TOWERSHOGI_TILE_GAP;
	if ((float)isola_info_window.width/(float)isola_info_window.height >
			(float)TOWERSHOGI_BOARD_WIDTH/(float)TOWERSHOGI_BOARD_HEIGHT) {
		resolutionFit = isola_info_window.height;
		boardFit = boardHeight;
	}else{
		resolutionFit = isola_info_window.width;
		boardFit = boardWidth;
	}
	intScreenScaling = resolutionFit/boardFit;

	towershogi_cursor.offsetArea[0] = ((float)isola_info_window.width-
			(float)boardWidth*(float)intScreenScaling)/2.f;
	towershogi_cursor.offsetArea[1] = ((float)isola_info_window.height-
			(float)boardHeight*(float)intScreenScaling)/2.f;
	towershogi_cursor.tileSpan = 
			(float)(TOWERSHOGI_TILE_SPAN+TOWERSHOGI_TILE_GAP)*
			(float)intScreenScaling;

	tileScale = (float)TOWERSHOGI_TILE_SPAN*
			(float)intScreenScaling*isola_info_window.pixelScale;
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"tileScale"); )
	if(loc == -1){ SDL_Log("towershogi: tileScale not found"); }
	ISOLA_GLDBG_( glUniform1f(loc,tileScale); )

	screenOffset[0] = (1.f-(float)(boardWidth*intScreenScaling)/
			(float)isola_info_window.width)-1.f;
	screenOffset[1] = (1.f-(float)(boardHeight*intScreenScaling)/
			(float)isola_info_window.height)-1.f;
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"screenOffset"); )
	if(loc == -1){ SDL_Log("towershogi: screenOffset not found"); }
	ISOLA_GLDBG_( glUniform2f(loc,screenOffset[0],screenOffset[1]); )


	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[1]); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[1],"projection"); )
	if(loc == -1){ SDL_Log("towershogi_cursor: projection not found"); }
	ISOLA_GLDBG_( glUniformMatrix4fv(loc,1,GL_FALSE,projection); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[1],"tileScale"); )
	if(loc == -1){ SDL_Log("towershogi_cursor: tileScale not found"); }
	ISOLA_GLDBG_( glUniform1f(loc,tileScale); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[1],"screenOffset"); )
	if(loc == -1){ SDL_Log("towershogi_cursor: screenOffset not found"); }
	ISOLA_GLDBG_( glUniform2f(loc,screenOffset[0],screenOffset[1]); )
	}


	bitfont_update();

	towershogi_bitfontPtr->data[0].x = -1.f+
			2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[0].y = 1.f-
			2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[1].x = -1.f+
			2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[1].y = -1.f+
			16.f*2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[2].x = -1.f+
			2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[2].y = 1.f-
			(16.f+1.f)*2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[3].x = -1.f+
			2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[3].y = 1.f-
			(32.f+1.f)*2.f/(float)isola_info_window.height;

	towershogi_bitfontPtr->data[4].x = -1.f+
			(0.f+1.f)*2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[4].y = 0.f-
			(-32.f+1.f)*2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[5].x = -1.f+
			(0.f+1.f)*2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[5].y = 0.f-
			(-16.f+1.f)*2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[6].x = -1.f+
			(0.f+1.f)*2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[6].y = 0.f-
			(0.f+1.f)*2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[7].x = -1.f+
			(0.f+1.f)*2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[7].y = 0.f-
			(16.f+1.f)*2.f/(float)isola_info_window.height;
	towershogi_bitfontPtr->data[8].x = -1.f+
			(0.f+1.f)*2.f/(float)isola_info_window.width;
	towershogi_bitfontPtr->data[8].y = 0.f-
			(-48.f+1.f)*2.f/(float)isola_info_window.height;
}


void towershogi_create(void){

	bitfont_create();
	towershogi_bitfontPtr = bitfont_requestPtr(4+5);

	towershogi_bitfontPtr->data[0].sizeMult = 1;
	towershogi_bitfontPtr->data[0].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[0].string,
			"'H'= show help",256);
	towershogi_bitfontPtr->data[0].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[0].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[0].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[0].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[0].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[0].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[0].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[0].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[1].sizeMult = 1;
	towershogi_bitfontPtr->data[1].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[1].string,
			"current turn: white",256);
	towershogi_bitfontPtr->data[1].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[1].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[1].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[1].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[1].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[1].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[1].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[1].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[2].sizeMult = 1;
	towershogi_bitfontPtr->data[2].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[2].string, "",256);
	towershogi_bitfontPtr->data[2].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[2].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[2].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[2].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[2].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[2].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[2].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[2].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[3].sizeMult = 1;
	towershogi_bitfontPtr->data[3].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[3].string, "",256);
	towershogi_bitfontPtr->data[3].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[3].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[3].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[3].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[3].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[3].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[3].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[3].backColor[3] = 0.5f;


	towershogi_bitfontPtr->data[4].sizeMult = 1;
	towershogi_bitfontPtr->data[4].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[4].string, "KNIGHT",256);
	towershogi_bitfontPtr->data[4].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[4].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[4].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[4].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[4].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[4].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[4].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[4].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[5].sizeMult = 1;
	towershogi_bitfontPtr->data[5].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[5].string, "BISHOP",256);
	towershogi_bitfontPtr->data[5].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[5].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[5].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[5].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[5].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[5].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[5].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[5].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[6].sizeMult = 1;
	towershogi_bitfontPtr->data[6].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[6].string, "ROOK  ",256);
	towershogi_bitfontPtr->data[6].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[6].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[6].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[6].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[6].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[6].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[6].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[6].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[7].sizeMult = 1;
	towershogi_bitfontPtr->data[7].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[7].string, "QUEEN ",256);
	towershogi_bitfontPtr->data[7].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[7].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[7].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[7].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[7].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[7].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[7].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[7].backColor[3] = 0.5f;

	towershogi_bitfontPtr->data[8].sizeMult = 1;
	towershogi_bitfontPtr->data[8].charWrap = BITFONT_STRINGSIZE;
	SDL_strlcpy(towershogi_bitfontPtr->data[8].string, "PROMOTION:",256);
	towershogi_bitfontPtr->data[8].foreColor[0] = 0.f;
	towershogi_bitfontPtr->data[8].foreColor[1] = 0.f;
	towershogi_bitfontPtr->data[8].foreColor[2] = 0.f;
	towershogi_bitfontPtr->data[8].foreColor[3] = 1.f;
	towershogi_bitfontPtr->data[8].backColor[0] = 1.f;
	towershogi_bitfontPtr->data[8].backColor[1] = 1.f;
	towershogi_bitfontPtr->data[8].backColor[2] = 1.f;
	towershogi_bitfontPtr->data[8].backColor[3] = 0.5f;


	towershogi_chessSetup();


	towershogi_cursor.turnPlayer = TOWERSHOGI_PLAYER_WHITE;
	towershogi_cursor.xHover = 0;
	towershogi_cursor.yHover = 0;
	towershogi_cursor.selected = 0;
	towershogi_cursor.hovered = &towershogi_board[towershogi_cursor.xHover+
			towershogi_cursor.yHover*TOWERSHOGI_BOARD_WIDTH];
	towershogi_cursor.hovered->state = TOWERSHOGI_STATE_HOVERED;


	towershogi_vertData = (unsigned char*)SDL_calloc(TOWERSHOGI_BOARD_SIZE*6*2,
			sizeof(towershogi_vertData[0]));

	ISOLA_GLDBG_( glGenVertexArrays(1,&towershogi_vertArrObj[0]); )
	ISOLA_GLDBG_( glGenBuffers(1,&towershogi_vertBufObj[0]); )

	ISOLA_GLDBG_( glBindVertexArray(towershogi_vertArrObj[0]); )

	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,towershogi_vertBufObj[0]); )
	ISOLA_GLDBG_( glBufferData(GL_ARRAY_BUFFER,
				TOWERSHOGI_BOARD_SIZE*6*2*sizeof(towershogi_vertData[0]),
				towershogi_vertData,GL_DYNAMIC_DRAW); )

	ISOLA_GLDBG_( glEnableVertexAttribArray(0); )
	ISOLA_GLDBG_( glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,
				sizeof(towershogi_vertData[0])*2,
				(void*)(sizeof(towershogi_vertData[0])*0) ); )
	ISOLA_GLDBG_( glEnableVertexAttribArray(1); )
	ISOLA_GLDBG_( glVertexAttribIPointer(1,1,GL_UNSIGNED_BYTE,
				sizeof(towershogi_vertData[0])*2,
				(void*)(sizeof(towershogi_vertData[0])*1) ); )


	ISOLA_GLDBG_( glGenTextures(1,towershogi_texObj); )
	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0); )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,towershogi_texObj[0]); )

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	{void* imagedata;
	size_t size;
	imagedata = SDL_LoadFile("scene/towershogi/data/16x16p_4x4_chess.data",
			&size);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,16*4,16*4,
			0,GL_RGBA,GL_UNSIGNED_BYTE,(char*)imagedata);
	SDL_free(imagedata);
	}


	towershogi_shaderProg[0] = isola_shader_buildProgram(
			"scene/towershogi/glsl/towershogi.vert",
			"scene/towershogi/glsl/towershogi.frag");

	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[0]); )

	{int loc = {0};
	float gapOffset = {0};

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"boardWidth"); )
	if(loc == -1){ SDL_Log("towershogi: boardWidth not found"); }
	ISOLA_GLDBG_( glUniform1i(loc,TOWERSHOGI_BOARD_WIDTH); )

	gapOffset = (float)TOWERSHOGI_TILE_GAP/(float)TOWERSHOGI_TILE_SPAN;
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"gapOffset"); )
	if(loc == -1){ SDL_Log("towershogi: gapOffset not found"); }
	ISOLA_GLDBG_( glUniform1f(loc,gapOffset); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
				towershogi_shaderProg[0],"pieceAtlas"); )
	if(loc == -1){ SDL_Log("towershogi: pieceAtlas not found"); }
	ISOLA_GLDBG_( glUniform1i(loc,0); )


	towershogi_shaderProg[1] = isola_shader_buildProgram(
			"scene/towershogi/glsl/towershogi_cursor.vert",
			"scene/towershogi/glsl/towershogi_cursor.frag");

	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[1]); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[1],"gapOffset"); )
	if(loc == -1){ SDL_Log("towershogi_cursor: gapOffset not found"); }
	ISOLA_GLDBG_( glUniform1f(loc,gapOffset); )
	}
}


void towershogi_destroy(void){

	SDL_free(towershogi_vertData);


	ISOLA_GLDBG_( glUseProgram(0); )
	ISOLA_GLDBG_( glBindVertexArray(0); )

	ISOLA_GLDBG_( glDeleteProgram(towershogi_shaderProg[0]); )
	ISOLA_GLDBG_( glDeleteBuffers(1,towershogi_vertBufObj); )
	ISOLA_GLDBG_( glDeleteVertexArrays(1,towershogi_vertArrObj); )


	bitfont_freePtr(towershogi_bitfontPtr);
	bitfont_destroy();
}


void towershogi_step(void){

	float mousePos[2];
	float boardCoord[2];


	if (isola_keyState[SDL_SCANCODE_H] && !isola_keyRepeat[SDL_SCANCODE_H]) {
		towershogi_helpShown = !towershogi_helpShown;
		if (towershogi_helpShown) {
			SDL_strlcpy(towershogi_bitfontPtr->data[0].string,
					"'wasd/mouse'= movement, '+shift'= fast",256);
			SDL_strlcpy(towershogi_bitfontPtr->data[2].string,
					"'space/click'= select, 'q'= deselect, 'esc'= quit",256);
			SDL_strlcpy(towershogi_bitfontPtr->data[3].string,
					"'H'= hide help",256);
		}else{
			SDL_strlcpy(towershogi_bitfontPtr->data[0].string,
					"'H'= show help",256);
			SDL_strlcpy(towershogi_bitfontPtr->data[2].string,"",256);
			SDL_strlcpy(towershogi_bitfontPtr->data[3].string,"",256);
		}
	}
	if ((isola_keyState[SDL_SCANCODE_A] && !isola_keyRepeat[SDL_SCANCODE_A]) ||
			(isola_keyState[SDL_SCANCODE_A] &&
			isola_keyState[SDL_SCANCODE_LSHIFT])
			) {
		if (towershogi_cursor.xHover > 0) {
			towershogi_cursor.xHover -= 1;
			towershogi_cursor.hovered -= 1;
		}
	}
	if ((isola_keyState[SDL_SCANCODE_S] && !isola_keyRepeat[SDL_SCANCODE_S]) ||
			(isola_keyState[SDL_SCANCODE_S] &&
			isola_keyState[SDL_SCANCODE_LSHIFT])
			) {
		if (towershogi_cursor.yHover > 0) {
			towershogi_cursor.yHover -= 1;
			towershogi_cursor.hovered -= TOWERSHOGI_BOARD_WIDTH;
		}
	}
	if ((isola_keyState[SDL_SCANCODE_W] && !isola_keyRepeat[SDL_SCANCODE_W]) ||
			(isola_keyState[SDL_SCANCODE_W] &&
			isola_keyState[SDL_SCANCODE_LSHIFT])
			) {
		if (towershogi_cursor.yHover < TOWERSHOGI_BOARD_HEIGHT-1) {
			towershogi_cursor.yHover += 1;
			towershogi_cursor.hovered += TOWERSHOGI_BOARD_WIDTH;
		}
	}
	if ((isola_keyState[SDL_SCANCODE_D] && !isola_keyRepeat[SDL_SCANCODE_D]) ||
			(isola_keyState[SDL_SCANCODE_D] &&
			isola_keyState[SDL_SCANCODE_LSHIFT])
			) {
		if (towershogi_cursor.xHover < TOWERSHOGI_BOARD_WIDTH-1) {
			towershogi_cursor.xHover += 1;
			towershogi_cursor.hovered += 1;
		}
	}


	mousePos[0] = isola_mousePos[0];
	mousePos[1] = isola_mousePos[1];
	if ( (int)towershogi_cursor.mousePos[0] != (int)mousePos[0] ||
			(int)towershogi_cursor.mousePos[1] != (int)mousePos[1] ) {
		towershogi_cursor.mousePos[0] = mousePos[0];
		towershogi_cursor.mousePos[1] = mousePos[1];

		mousePos[1] = (float)isola_info_window.height - mousePos[1];

		boardCoord[0] = (mousePos[0]-towershogi_cursor.offsetArea[0])/
				towershogi_cursor.tileSpan;
		boardCoord[1] = (mousePos[1]-towershogi_cursor.offsetArea[1])/
				towershogi_cursor.tileSpan;

		if(boardCoord[0] > 0.f && boardCoord[0] < TOWERSHOGI_BOARD_WIDTH &&
				boardCoord[1] > 0.f && boardCoord[1] < TOWERSHOGI_BOARD_HEIGHT){
			towershogi_cursor.xHover = (int)boardCoord[0];
			towershogi_cursor.yHover = (int)boardCoord[1];
			towershogi_cursor.hovered = &towershogi_board[(int)boardCoord[0]+
					(int)boardCoord[1]*TOWERSHOGI_BOARD_WIDTH];
		}
	}


/* update board states based on current hovered/selected piece */
	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		towershogi_board[i].state = 0;
	}}
	towershogi_cursor.hovered->state = TOWERSHOGI_STATE_HOVERED;
	if (towershogi_cursor.selected) {
		towershogi_cursor.selected->state = TOWERSHOGI_STATE_SELECTED;
		towershogi_cursor.predict = towershogi_cursor.selected;
		towershogi_cursor.xPredict = towershogi_cursor.xSelected;
		towershogi_cursor.yPredict = towershogi_cursor.ySelected;
	}else {
		towershogi_cursor.predict = towershogi_cursor.hovered;
		towershogi_cursor.xPredict = towershogi_cursor.xHover;
		towershogi_cursor.yPredict = towershogi_cursor.yHover;
	}

	{int j;
	for(j = 0;j<TOWERSHOGI_MOVEMENT_SPAN;j++){
		{int i;
		for(i = 0;i<TOWERSHOGI_MOVEMENT_SPAN;i++){
			if ( towershogi_cursor.xPredict+
					i-TOWERSHOGI_MOVEMENT_RANGE >= 0 &&
					towershogi_cursor.xPredict+
					i- TOWERSHOGI_MOVEMENT_RANGE <= TOWERSHOGI_BOARD_WIDTH-1 &&
					towershogi_cursor.yPredict+
					(TOWERSHOGI_MOVEMENT_RANGE-j) >= 0 &&
					towershogi_cursor.yPredict+
					(TOWERSHOGI_MOVEMENT_RANGE-j) <= TOWERSHOGI_BOARD_HEIGHT-1 ) {
				switch( towershogi_piece[towershogi_cursor.predict->piece&0x7f].
						movement[ j+((towershogi_cursor.predict->piece&
						TOWERSHOGI_PLAYER_MASK)!=0)*
						(TOWERSHOGI_MOVEMENT_SPAN-1-2*j) ][i] ){
					case 1:
							if( (towershogi_cursor.predict+
									(i-TOWERSHOGI_MOVEMENT_RANGE)+
									(TOWERSHOGI_MOVEMENT_RANGE-
									 j)*TOWERSHOGI_BOARD_WIDTH)->piece == 0 ) {
								(towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										 j)*TOWERSHOGI_BOARD_WIDTH)->state =
										TOWERSHOGI_STATE_HOVERED_AVAILIABLE+
										(TOWERSHOGI_STATE_SELECTED_AVAILIABLE-
										TOWERSHOGI_STATE_HOVERED_AVAILIABLE)*
										(towershogi_cursor.selected!=0);
							}else{
								if( ( ((towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										 j)*TOWERSHOGI_BOARD_WIDTH)->piece^
										towershogi_cursor.predict->piece)&
											TOWERSHOGI_PLAYER_MASK ) != 0 ) {
									(towershogi_cursor.predict+
											(i-TOWERSHOGI_MOVEMENT_RANGE)+
											(TOWERSHOGI_MOVEMENT_RANGE-
											 j)*TOWERSHOGI_BOARD_WIDTH)->state =
											TOWERSHOGI_STATE_HOVERED_CAPTURE+
											(TOWERSHOGI_STATE_SELECTED_CAPTURE-
											TOWERSHOGI_STATE_HOVERED_CAPTURE)*
											(towershogi_cursor.selected!=0);
								}else {
									(towershogi_cursor.predict+
											(i-TOWERSHOGI_MOVEMENT_RANGE)+
											(TOWERSHOGI_MOVEMENT_RANGE-
											 j)*TOWERSHOGI_BOARD_WIDTH)->state =
											TOWERSHOGI_STATE_NONE+
											(TOWERSHOGI_STATE_SELECTED_UNAVAILIABLE-
											TOWERSHOGI_STATE_NONE)*
											(towershogi_cursor.selected!=0);
								}
							}
					break;
					case TOWERSHOGI_MOVEMENT_NOCAPTURE:
							if( (towershogi_cursor.predict+
									(i-TOWERSHOGI_MOVEMENT_RANGE)+
									(TOWERSHOGI_MOVEMENT_RANGE-
									 j)*TOWERSHOGI_BOARD_WIDTH)->piece == 0 ) {
								(towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										 j)*TOWERSHOGI_BOARD_WIDTH)->state =
										TOWERSHOGI_STATE_HOVERED_AVAILIABLE+
										(TOWERSHOGI_STATE_SELECTED_AVAILIABLE-
										TOWERSHOGI_STATE_HOVERED_AVAILIABLE)*
										(towershogi_cursor.selected!=0);
							}else {
									(towershogi_cursor.predict+
											(i-TOWERSHOGI_MOVEMENT_RANGE)+
											(TOWERSHOGI_MOVEMENT_RANGE-
											 j)*TOWERSHOGI_BOARD_WIDTH)->state =
											TOWERSHOGI_STATE_NONE+
											(TOWERSHOGI_STATE_SELECTED_UNAVAILIABLE-
											TOWERSHOGI_STATE_NONE)*
											(towershogi_cursor.selected!=0);
							}
					break;
					case TOWERSHOGI_MOVEMENT_CAPTURE:
							if( (towershogi_cursor.predict+
									(i-TOWERSHOGI_MOVEMENT_RANGE)+
									(TOWERSHOGI_MOVEMENT_RANGE-
									 j)*TOWERSHOGI_BOARD_WIDTH)->piece != 0 ) {
								if( ( ((towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										 j)*TOWERSHOGI_BOARD_WIDTH)->piece^
										towershogi_cursor.predict->piece)&
											TOWERSHOGI_PLAYER_MASK ) != 0 ||
										( ((towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										 j)*TOWERSHOGI_BOARD_WIDTH)->enpasse^
										towershogi_cursor.predict->piece)&
											TOWERSHOGI_PLAYER_MASK ) != 0
										) {
									(towershogi_cursor.predict+
											(i-TOWERSHOGI_MOVEMENT_RANGE)+
											(TOWERSHOGI_MOVEMENT_RANGE-
											 j)*TOWERSHOGI_BOARD_WIDTH)->state =
											TOWERSHOGI_STATE_HOVERED_CAPTURE+
											(TOWERSHOGI_STATE_SELECTED_CAPTURE-
											TOWERSHOGI_STATE_HOVERED_CAPTURE)*
											(towershogi_cursor.selected!=0);
								}
							}
					break;
					case TOWERSHOGI_MOVEMENT_FIRST:
					break;
					case TOWERSHOGI_MOVEMENT_FIRSTNOCAPTURE:
						if( (towershogi_cursor.predict+
								(i-TOWERSHOGI_MOVEMENT_RANGE)+
								((TOWERSHOGI_MOVEMENT_RANGE-
								j)/2/* mal */)*TOWERSHOGI_BOARD_WIDTH)->piece == 0 ){
							if(towershogi_cursor.predict->moved == 0){
								if( (towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										 j)*TOWERSHOGI_BOARD_WIDTH)->piece == 0 ) {
									(towershogi_cursor.predict+
											(i-TOWERSHOGI_MOVEMENT_RANGE)+
											(TOWERSHOGI_MOVEMENT_RANGE-
											 j)*TOWERSHOGI_BOARD_WIDTH)->state =
											TOWERSHOGI_STATE_HOVERED_AVAILIABLE+
											(TOWERSHOGI_STATE_SELECTED_AVAILIABLE-
											TOWERSHOGI_STATE_HOVERED_AVAILIABLE)*
											(towershogi_cursor.selected!=0);
								}else {
										(towershogi_cursor.predict+
												(i-TOWERSHOGI_MOVEMENT_RANGE)+
												(TOWERSHOGI_MOVEMENT_RANGE-
												 j)*TOWERSHOGI_BOARD_WIDTH)->state =
												TOWERSHOGI_STATE_NONE+
												(TOWERSHOGI_STATE_SELECTED_UNAVAILIABLE-
												TOWERSHOGI_STATE_NONE)*
												(towershogi_cursor.selected!=0);
								}
							}
						}
					break;
					case TOWERSHOGI_MOVEMENT_FIRSTCAPTURE:
					break;
					case TOWERSHOGI_MOVEMENT_CASTLE:
						if(towershogi_cursor.predict->moved == 0){
								if( (towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										j)*TOWERSHOGI_BOARD_WIDTH)->moved == 0 &&
										((towershogi_cursor.predict+
										(i-TOWERSHOGI_MOVEMENT_RANGE)+
										(TOWERSHOGI_MOVEMENT_RANGE-
										j)*TOWERSHOGI_BOARD_WIDTH)->piece&0x7f) ==
										TOWERSHOGI_PIECE_ROOK) {
									{int k, l, m = {0};
									if ((int)i-TOWERSHOGI_MOVEMENT_RANGE > 0) {
									 l=-1;}else{l=+1;}
									for(k = (int)i-TOWERSHOGI_MOVEMENT_RANGE;k!=0;k+=l){
										if ( (towershogi_cursor.predict+k)->piece != 
												TOWERSHOGI_PIECE_NONE ) {
											m = 1;
										}
									}
									if (m==0) {
										(towershogi_cursor.predict+
												(i-TOWERSHOGI_MOVEMENT_RANGE)+
												(TOWERSHOGI_MOVEMENT_RANGE-
												 j)*TOWERSHOGI_BOARD_WIDTH)->state =
												TOWERSHOGI_STATE_HOVERED_AVAILIABLE+
												(TOWERSHOGI_STATE_SELECTED_AVAILIABLE-
												TOWERSHOGI_STATE_HOVERED_AVAILIABLE)*
												(towershogi_cursor.selected!=0);
									}}
								}
						}
					break;
					case TOWERSHOGI_MOVEMENT_UNLIMITED:
						{int k;
						for(k = 1;;k++){
								if (towershogi_cursor.xPredict+
										k*(i-TOWERSHOGI_MOVEMENT_RANGE) < 0 ||
										towershogi_cursor.xPredict+
										k*(i-TOWERSHOGI_MOVEMENT_RANGE) >
										TOWERSHOGI_BOARD_WIDTH-1 ||
										towershogi_cursor.yPredict+
										k*(TOWERSHOGI_MOVEMENT_RANGE-j) < 0 ||
										towershogi_cursor.yPredict+
										k*(TOWERSHOGI_MOVEMENT_RANGE-j) >
										TOWERSHOGI_BOARD_HEIGHT-1
										) {
									break;
								}
								if ( (towershogi_cursor.predict+
										k*(i-TOWERSHOGI_MOVEMENT_RANGE)+
										k*(TOWERSHOGI_MOVEMENT_RANGE-j)
										*TOWERSHOGI_BOARD_WIDTH)->piece != 0 ) {
									if ( ((towershogi_cursor.predict+
											k*(i-TOWERSHOGI_MOVEMENT_RANGE)+
											k*(TOWERSHOGI_MOVEMENT_RANGE-j)
											*TOWERSHOGI_BOARD_WIDTH)->piece&
													TOWERSHOGI_PLAYER_MASK) !=
											(towershogi_cursor.predict->piece&
											 TOWERSHOGI_PLAYER_MASK) ) {
										(towershogi_cursor.predict+
												k*(i-TOWERSHOGI_MOVEMENT_RANGE)+
												k*(TOWERSHOGI_MOVEMENT_RANGE-j)
												*TOWERSHOGI_BOARD_WIDTH)->state =
												TOWERSHOGI_STATE_HOVERED_CAPTURE+
												(TOWERSHOGI_STATE_SELECTED_CAPTURE-
												TOWERSHOGI_STATE_HOVERED_CAPTURE)*
												(towershogi_cursor.selected!=0);
									}else {
										(towershogi_cursor.predict+
												k*(i-TOWERSHOGI_MOVEMENT_RANGE)+
												k*(TOWERSHOGI_MOVEMENT_RANGE-j)*
												TOWERSHOGI_BOARD_WIDTH)->state =
												TOWERSHOGI_STATE_NONE+
												(TOWERSHOGI_STATE_SELECTED_UNAVAILIABLE-
												TOWERSHOGI_STATE_NONE)*
												(towershogi_cursor.selected!=0);
									}
									break;
								}
								(towershogi_cursor.predict+
										k*(i-TOWERSHOGI_MOVEMENT_RANGE)+
										k*(TOWERSHOGI_MOVEMENT_RANGE-j)
										*TOWERSHOGI_BOARD_WIDTH)->state =
										TOWERSHOGI_STATE_HOVERED_AVAILIABLE+
										(TOWERSHOGI_STATE_SELECTED_AVAILIABLE-
										TOWERSHOGI_STATE_HOVERED_AVAILIABLE)*
										(towershogi_cursor.selected!=0);
						}}
					break;
				}
			}
		}}
	}}


	if(towershogi_cursor.selected == 0){
		if ( (isola_keyState[SDL_SCANCODE_SPACE] &&
				!isola_keyRepeat[SDL_SCANCODE_SPACE]) ||
				(isola_mouseButtonState[SDL_BUTTON_LEFT] &&
				!isola_mouseButtonRepeat[SDL_BUTTON_LEFT]) ||
				(isola_keyState[SDL_SCANCODE_RETURN] &&
				!isola_keyRepeat[SDL_SCANCODE_RETURN])
				) {
			if ( towershogi_cursor.hovered->piece != 0 &&
					(towershogi_cursor.hovered->piece&TOWERSHOGI_PLAYER_MASK) ==
					towershogi_cursor.turnPlayer ) {
				towershogi_cursor.selected = towershogi_cursor.hovered;
				towershogi_cursor.xSelected = towershogi_cursor.xHover;
				towershogi_cursor.ySelected = towershogi_cursor.yHover;
			}
		}
	}else {
		if ( (isola_keyState[SDL_SCANCODE_Q] &&
				!isola_keyRepeat[SDL_SCANCODE_Q]) ||
				(isola_mouseButtonState[SDL_BUTTON_RIGHT] &&
				!isola_mouseButtonRepeat[SDL_BUTTON_RIGHT]) ||
				(isola_keyState[SDL_SCANCODE_BACKSPACE] &&
				!isola_keyRepeat[SDL_SCANCODE_BACKSPACE])
				) {
			towershogi_cursor.hovered = towershogi_cursor.selected;
			towershogi_cursor.xHover = towershogi_cursor.xSelected;
			towershogi_cursor.yHover = towershogi_cursor.ySelected;
			towershogi_cursor.selected = 0;
		}


		if ( (isola_keyState[SDL_SCANCODE_SPACE] &&
				!isola_keyRepeat[SDL_SCANCODE_SPACE]) ||
				(isola_mouseButtonState[SDL_BUTTON_LEFT] &&
				!isola_mouseButtonRepeat[SDL_BUTTON_LEFT]) ||
				(isola_keyState[SDL_SCANCODE_RETURN] &&
				!isola_keyRepeat[SDL_SCANCODE_RETURN])
				) {
			if ((towershogi_cursor.hovered->state ==
					TOWERSHOGI_STATE_SELECTED_AVAILIABLE) ||
					(towershogi_cursor.hovered->state == 
					TOWERSHOGI_STATE_SELECTED_CAPTURE)) {
				struct TOWERSHOGI_TILE targetTile;

				SDL_memcpy(&targetTile,towershogi_cursor.hovered,
						sizeof(struct TOWERSHOGI_TILE));
				SDL_memcpy(towershogi_cursor.hovered,towershogi_cursor.selected,
						sizeof(struct TOWERSHOGI_TILE));
				towershogi_cursor.hovered->moved = 1;
				SDL_memset(towershogi_cursor.selected,0,
						sizeof(struct TOWERSHOGI_TILE));

				if( (towershogi_cursor.hovered->piece) == 
						TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_WHITE) &&
						towershogi_cursor.yHover == TOWERSHOGI_BOARD_HEIGHT-1 ){
				}else if( (towershogi_cursor.hovered->piece) == 
						TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,TOWERSHOGI_PLAYER_BLACK) &&
						towershogi_cursor.yHover == 0 ){
				}
				
				towershogi_cursor.selected = 0;
				towershogi_cursor.turnPlayer ^= TOWERSHOGI_PLAYER_MASK;
				SDL_strlcpy(towershogi_bitfontPtr->data[1].string,
						"current turn: black",256);
				if (towershogi_cursor.turnPlayer == TOWERSHOGI_PLAYER_WHITE) {
					SDL_strlcpy(towershogi_bitfontPtr->data[1].string,
							"current turn: white",256);
				}
			}
		}
	}
}


void towershogi_draw(void){

	ISOLA_GLDBG_( glBindVertexArray(towershogi_vertArrObj[0]); )

	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0); )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,towershogi_texObj[0]); )

	isola_set_state(towershogi_glstate);
	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[0]); )

	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,towershogi_vertBufObj[0]); )
	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		{unsigned int j;
		for(j = 0;j<6;j++){
			switch (towershogi_board[i].piece) {
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_NONE,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 7;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KING,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 0;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 1;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KNIGHT,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 2;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_BISHOP,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 3;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_ROOK,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 4;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_QUEEN,
						TOWERSHOGI_PLAYER_WHITE):
					towershogi_vertData[i*6*2+j*2] = 5;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_NONE,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 15;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KING,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 8;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_PAWN,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 9;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_KNIGHT,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 10;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_BISHOP,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 11;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_ROOK,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 12;
				break;
				case TOWERSHOGI_PIECE_(TOWERSHOGI_PIECE_QUEEN,
						TOWERSHOGI_PLAYER_BLACK):
					towershogi_vertData[i*6*2+j*2] = 13;
				break;
			}
			towershogi_vertData[i*6*2+j*2+1] =
					(unsigned char)towershogi_board[i].state;
		}}
	}}
	ISOLA_GLDBG_( glBufferSubData(GL_ARRAY_BUFFER,
				sizeof(towershogi_vertData[0])*0,
				TOWERSHOGI_BOARD_SIZE*6*2*sizeof(towershogi_vertData[0]),
				towershogi_vertData); )

	ISOLA_GLDBG_( glDrawArrays(GL_TRIANGLES,0,TOWERSHOGI_BOARD_SIZE*6); )



	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[1]); )

	{int loc = {0};

	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[1],"boardPos"); )
	if(loc == -1){ SDL_Log("towershogi: boardPos not found"); }
	ISOLA_GLDBG_( glUniform2i(loc,towershogi_cursor.xHover,
				towershogi_cursor.yHover); )
	}

	ISOLA_GLDBG_( glDrawArrays(GL_TRIANGLES,0,6); )


	bitfont_draw();
}




