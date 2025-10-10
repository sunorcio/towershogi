#include "towershogi.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>
#include <isola/input.h>




#define TOWERSHOGI_PIECE_NONE 0
#define TOWERSHOGI_PIECE_KING 1
#define TOWERSHOGI_PIECE_PAWN 2
#define TOWERSHOGI_PIECE_KNIGHT 3
#define TOWERSHOGI_PIECE_BISHOP 4
#define TOWERSHOGI_PIECE_ROOK 5
#define TOWERSHOGI_PIECE_QUEEN 6
#define TOWERSHOGI_PIECE_AMOUNT 7

#define TOWERSHOGI_PLAYER_WHITE 0
#define TOWERSHOGI_PLAYER_BLACK 1

#define TOWERSHOGI_PIECE_(piece,player) (piece|(player*0x80))

#define TOWERSHOGI_STATE_NONE 0
#define TOWERSHOGI_STATE_HOVERED 1
#define TOWERSHOGI_STATE_HOVERED_AVAILIABLE 2
#define TOWERSHOGI_STATE_HOVERED_CAPTURE 3
#define TOWERSHOGI_STATE_SELECTED 4
#define TOWERSHOGI_STATE_SELECTED_UNAVAILIABLE 5
#define TOWERSHOGI_STATE_SELECTED_AVAILIABLE 6
#define TOWERSHOGI_STATE_SELECTED_CAPTURE 7

#define TOWERSHOGI_MOVEMENT_RANGE 2
#define TOWERSHOGI_MOVEMENT_SPAN (1+TOWERSHOGI_MOVEMENT_RANGE*2)
#define TOWERSHOGI_MOVEMENT_CENTER TOWERSHOGI_MOVEMENT_RANGE
#define TOWERSHOGI_MOVEMENT_UNLIMITED 0xff
#define TOWERSHOGI_MOVEMENT_CAPTURE 0xfe
#define TOWERSHOGI_MOVEMENT_FIRST 0xfd

#define TOWERSHOGI_BOARD_WIDTH 16
#define TOWERSHOGI_BOARD_HEIGHT 8
#define TOWERSHOGI_BOARD_SIZE (TOWERSHOGI_BOARD_WIDTH*TOWERSHOGI_BOARD_HEIGHT)




struct TOWERSHOGI_PIECE{
	unsigned int movement[TOWERSHOGI_MOVEMENT_SPAN][TOWERSHOGI_MOVEMENT_SPAN];
}static towershogi_piece[TOWERSHOGI_PIECE_AMOUNT] = {0};

struct TOWERSHOGI_TILE{
	unsigned int state;
	unsigned int piece, moved;
}static towershogi_board[TOWERSHOGI_BOARD_SIZE] = {0};

static struct TOWERSHOGI_TILE* towershogi_hovered = {0};
static struct TOWERSHOGI_TILE* towershogi_selected = {0};




static enum ISOLA_state towershogi_glstate = 0x00000001;
static unsigned int towershogi_shaderProg[1] = {0};
static unsigned int towershogi_vertArrObj[1] = {0};
static unsigned int towershogi_vertBufObj[1] = {0};
static unsigned int towershogi_texObj[1] = {0};
static unsigned char* towershogi_vertData = {0};




void towershogi_update(void){

	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[0]) )

	{int loc;
	float matProj[4*4] = {0};
	int lowRes;

	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio,0.25f,8.f,matProj);
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"matProj"); )
	if(loc == -1){ SDL_Log("bitfont: matProj not found"); }
	ISOLA_GLDBG_( glUniformMatrix4fv(loc,1,GL_FALSE,matProj); )
	}
}


void towershogi_create(void){

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

	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER][TOWERSHOGI_MOVEMENT_CENTER] = 0;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER] = 1;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER-1] = TOWERSHOGI_MOVEMENT_CAPTURE;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-1]
			[TOWERSHOGI_MOVEMENT_CENTER+1] = TOWERSHOGI_MOVEMENT_CAPTURE;
	towershogi_piece[TOWERSHOGI_PIECE_PAWN]
			.movement[TOWERSHOGI_MOVEMENT_CENTER-2]
			[TOWERSHOGI_MOVEMENT_CENTER] = TOWERSHOGI_MOVEMENT_FIRST;

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


	towershogi_selected = 0;
	towershogi_hovered = &towershogi_board[0];


	towershogi_vertData = SDL_calloc(TOWERSHOGI_BOARD_SIZE*6*2,
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

	{int loc;
	ISOLA_GLDBG_( loc = glGetUniformLocation(
			towershogi_shaderProg[0],"boardWidth"); )
	if(loc == -1){ SDL_Log("towershogi: boardWidth not found"); }
	ISOLA_GLDBG_( glUniform1i(loc,TOWERSHOGI_BOARD_WIDTH); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
				towershogi_shaderProg[0],"boardSpan"); )
	if(loc == -1){ SDL_Log("towershogi: boardSpan not found"); }
	ISOLA_GLDBG_( glUniform1i(loc,
			(TOWERSHOGI_BOARD_HEIGHT
			 *(TOWERSHOGI_BOARD_HEIGHT>TOWERSHOGI_BOARD_WIDTH)
			+TOWERSHOGI_BOARD_WIDTH
			*(TOWERSHOGI_BOARD_WIDTH>=TOWERSHOGI_BOARD_HEIGHT))); )

	ISOLA_GLDBG_( loc = glGetUniformLocation(
				towershogi_shaderProg[0],"pieceAtlas"); )
	if(loc == -1){ SDL_Log("towershogi: pieceAtlas not found"); }
	ISOLA_GLDBG_( glUniform1i(loc,0); )
	}
}


void towershogi_destroy(void){

	SDL_free(towershogi_vertData);


	ISOLA_GLDBG_( glUseProgram(0); )
	ISOLA_GLDBG_( glBindVertexArray(0); )

	ISOLA_GLDBG_( glDeleteProgram(towershogi_shaderProg[0]); )
	ISOLA_GLDBG_( glDeleteBuffers(1,towershogi_vertBufObj); )
	ISOLA_GLDBG_( glDeleteVertexArrays(1,towershogi_vertArrObj); )
}


void towershogi_step(void){
}


void towershogi_draw(void){

	ISOLA_GLDBG_( glBindVertexArray(towershogi_vertArrObj[0]); )

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
}




