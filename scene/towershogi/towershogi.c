#include "towershogi.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>
#include <isola/input.h>




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


#define TOWERSHOGI_VB_SIZE (TOWERSHOGI_BOARD_SIZE*6*2)




struct TOWERSHOGI_board towershogi_board;


unsigned short towershogi_boardSize[2] = {8,8};

static unsigned char* towershogi_boardRenderState;




static enum ISOLA_state towershogi_state = 0x0000000d;
static unsigned int towershogi_shaderProg[1] = {0};
static unsigned int towershogi_vertArrObj[1] = {0};
static unsigned int towershogi_vertBufObj[1] = {0};
static unsigned int towershogi_texObj[1] = {0};
static unsigned char * towershogi_vertData = {0};




void towershogi_update(void){

	glBindVertexArray(towershogi_vertArrObj[0]);
	glBindBuffer(GL_ARRAY_BUFFER,towershogi_vertBufObj[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(towershogi_vertData),towershogi_vertData);


	glUseProgram(towershogi_shaderProg[0]);

	{int locProj;
	float matProj[4*4] = {0};
	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio,
			0.25,8.,matProj);
	locProj = glGetUniformLocation(towershogi_shaderProg[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}
}


void towershogi_create(void){

	SDL_memset(&towershogi_board,0,sizeof(struct TOWERSHOGI_board));
	towershogi_board.tile = SDL_calloc(TOWERSHOGI_BOARD_SIZE,
			sizeof(struct TOWERSHOGI_piece));
	towershogi_board.selectedTile = SELECTED_NONE;
	SDL_memset(towershogi_board.tile,0,
			sizeof(struct TOWERSHOGI_piece)*TOWERSHOGI_BOARD_SIZE);

	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		towershogi_board.tile[i].piece = PIECE_NONE;
		towershogi_board.tile[i].state = STATE_NONE;
	}}

	towershogi_board.tile[0].piece = towershogiPIECE(PIECE_KING,PLAYER_WHITE);
	{unsigned int i;
	for(i = 0;i<5;i++){
		{unsigned int j;
		for(j = 0;j<5;j++){
			if ((i!=2 || j!=2) && i!=4 && j!=4 && i!=0 && j!=0) {
				towershogi_board.tile[0].movement[i][j] = 1;
			}
		}}
	}}


	towershogi_vertData = SDL_calloc(TOWERSHOGI_VB_SIZE,sizeof(unsigned char));

	ISOLA_GLDBG_( glGenVertexArrays(1,&towershogi_vertArrObj[0]) )
	ISOLA_GLDBG_( glGenBuffers(1,&towershogi_vertBufObj[0]) )

	ISOLA_GLDBG_( glBindVertexArray(towershogi_vertArrObj[0]) )

	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,towershogi_vertBufObj[0]) )
	ISOLA_GLDBG_( glBufferData(GL_ARRAY_BUFFER,
				TOWERSHOGI_VB_SIZE*sizeof(towershogi_vertData[0]),
				towershogi_vertData,GL_DYNAMIC_DRAW) )

	ISOLA_GLDBG_( glEnableVertexAttribArray(0) )
	ISOLA_GLDBG_( glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,
			sizeof(towershogi_vertData[0])*2,
			(void*)(sizeof(towershogi_vertData[0])*0)) )
	ISOLA_GLDBG_( glEnableVertexAttribArray(1) )
	ISOLA_GLDBG_( glVertexAttribIPointer(1,1,GL_UNSIGNED_BYTE,
			sizeof(towershogi_vertData[0])*2,
			(void*)(sizeof(towershogi_vertData[0])*1)) )


	ISOLA_GLDBG_( glGenTextures(1,towershogi_texObj) )
	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0) )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,towershogi_texObj[0]) )

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


	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[0]) )

	{int loc;
	loc = glGetUniformLocation(towershogi_shaderProg[0],"boardWidth");
	if(loc == -1){ SDL_Log("towershogi: boardWidth not found"); }
	glUniform1i(loc,towershogi_boardSize[0]);

	loc = glGetUniformLocation(towershogi_shaderProg[0],"boardHeight");
	if(loc == -1){ SDL_Log("towershogi: boardHeight not found"); }
	glUniform1i(loc,towershogi_boardSize[1]);

	loc = glGetUniformLocation(towershogi_shaderProg[0],"pieceAtlas");
	if(loc == -1){ SDL_Log("towershogi: pieceAtlas not found"); }
	glUniform1i(loc,0);
	}
}


void towershogi_destroy(void){

	SDL_free(towershogi_vertData);
	SDL_free(towershogi_board.tile);


	ISOLA_GLDBG_( glUseProgram(0) )
	ISOLA_GLDBG_( glBindVertexArray(0) )

	ISOLA_GLDBG_( glDeleteProgram(towershogi_shaderProg[0]) )
	ISOLA_GLDBG_( glDeleteBuffers(1,towershogi_vertBufObj) )
	ISOLA_GLDBG_( glDeleteVertexArrays(1,towershogi_vertArrObj) )
}


void towershogi_step(void){

	if (isola_keyState[SDL_SCANCODE_J] && !isola_keyRepeat[SDL_SCANCODE_J]) {
		if (towershogi_board.currentTile >= towershogi_boardSize[0]) {
			towershogi_board.currentTile -= towershogi_boardSize[0];
		}
	}
	if (isola_keyState[SDL_SCANCODE_K] && !isola_keyRepeat[SDL_SCANCODE_K]) {
		if (towershogi_board.currentTile < TOWERSHOGI_BOARD_SIZE 
				-towershogi_boardSize[0]) {
			towershogi_board.currentTile += towershogi_boardSize[0];
		}
	}
	if (isola_keyState[SDL_SCANCODE_L] && !isola_keyRepeat[SDL_SCANCODE_L]) {
		if ((towershogi_board.currentTile+1)%towershogi_boardSize[0]) {
			towershogi_board.currentTile += 1;
		}
	}
	if (isola_keyState[SDL_SCANCODE_H] && !isola_keyRepeat[SDL_SCANCODE_H]) {
		if (towershogi_board.currentTile%towershogi_boardSize[0]) {
			towershogi_board.currentTile -= 1;
		}
	}
	if (isola_keyState[SDL_SCANCODE_SPACE] &&
			!isola_keyRepeat[SDL_SCANCODE_SPACE]) {
		if(towershogi_board.selectedTile == SELECTED_NONE){
			towershogi_board.tile[towershogi_board.currentTile].state =
					STATE_SELECTED;
			towershogi_board.selectedTile = towershogi_board.currentTile;
		}
	}
	if (isola_keyState[SDL_SCANCODE_Q] && !isola_keyRepeat[SDL_SCANCODE_Q]) {
		if(towershogi_board.selectedTile != SELECTED_NONE){
			towershogi_board.currentTile = towershogi_board.selectedTile;
			towershogi_board.tile[towershogi_board.selectedTile].state =
					STATE_NONE;
			towershogi_board.selectedTile = SELECTED_NONE;
		}
	}
}


void towershogi_draw(void){

	ISOLA_GLDBG_( glBindVertexArray(towershogi_vertArrObj[0]) )

	isola_set_state(towershogi_state);
	ISOLA_GLDBG_( glUseProgram(towershogi_shaderProg[0]) )

	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,towershogi_vertBufObj[0]) )

	SDL_memset(towershogi_vertData,0,
			TOWERSHOGI_VB_SIZE*sizeof(towershogi_vertData[0]));
	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		{unsigned int v;
		for(v = 0;v<6;v++){
			towershogi_vertData[i*6*2+v*2] = towershogi_board.tile[i].piece;
			towershogi_vertData[i*6*2+v*2+1] = towershogi_board.tile[i].state;
		}}
	}}

	ISOLA_GLDBG_( glBufferSubData(GL_ARRAY_BUFFER,
			sizeof(towershogi_vertData[0])*0,
			TOWERSHOGI_VB_SIZE*sizeof(towershogi_vertData[0]),
			towershogi_vertData) )


	{int loc;
	loc = glGetUniformLocation(towershogi_shaderProg[0],"currentPiece");
	if(loc == -1){ SDL_Log("currentPiece not found"); }
	glUniform1i(loc,towershogi_board.currentTile);
	}


	ISOLA_GLDBG_( glDrawArrays(GL_TRIANGLES,0,TOWERSHOGI_VB_SIZE) )
}




