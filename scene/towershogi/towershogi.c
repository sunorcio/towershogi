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




struct TOWERSHOGI_board towershogiBoard;


unsigned short towershogiBoardSize[2] = {8,8};

unsigned char* towershogiBoardRenderState;




static enum ISOLA_state towershogiState = 0x0000000d;
static unsigned int towershogiSP[1] = {0};
static unsigned int towershogiVAO[1] = {0};
static unsigned int towershogiVBO[1] = {0};
static unsigned int towershogiTO[1] = {0};
static unsigned char * towershogiVD = {0};




void updateTowershogi(void){

	glBindVertexArray(towershogiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(towershogiVD),towershogiVD);


	glUseProgram(towershogiSP[0]);

	{int locProj;
	float matProj[4*4] = {0};
	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio,
			0.25,8.,matProj);
	locProj = glGetUniformLocation(towershogiSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}
}


void createTowershogi(void){

	SDL_memset(&towershogiBoard,0,sizeof(struct TOWERSHOGI_board));
	towershogiBoard.tile = SDL_calloc(TOWERSHOGI_BOARD_SIZE,
			sizeof(struct TOWERSHOGI_piece));
	towershogiBoard.selectedTile = SELECTED_NONE;
	SDL_memset(towershogiBoard.tile,0,
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


	towershogiVD = SDL_calloc(TOWERSHOGI_VB_SIZE,sizeof(unsigned char));

	ISOLA_GLDBG_( glGenVertexArrays(1,&towershogiVAO[0]) )
	ISOLA_GLDBG_( glGenBuffers(1,&towershogiVBO[0]) )

	ISOLA_GLDBG_( glBindVertexArray(towershogiVAO[0]) )

	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]) )
	ISOLA_GLDBG_( glBufferData(GL_ARRAY_BUFFER,
				TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]),
				towershogiVD,GL_DYNAMIC_DRAW) )

	ISOLA_GLDBG_( glEnableVertexAttribArray(0) )
	ISOLA_GLDBG_( glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,
			sizeof(towershogiVD[0])*2,(void*)(sizeof(towershogiVD[0])*0)) )
	ISOLA_GLDBG_( glEnableVertexAttribArray(1) )
	ISOLA_GLDBG_( glVertexAttribIPointer(1,1,GL_UNSIGNED_BYTE,
			sizeof(towershogiVD[0])*2,(void*)(sizeof(towershogiVD[0])*1)) )


	ISOLA_GLDBG_( glGenTextures(1,towershogiTO) )
	ISOLA_GLDBG_( glActiveTexture(GL_TEXTURE0+0) )
	ISOLA_GLDBG_( glBindTexture(GL_TEXTURE_2D,towershogiTO[0]) )

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


	towershogiSP[0] = isola_shader_buildProgram(
			"scene/towershogi/glsl/towershogi.vert",
			"scene/towershogi/glsl/towershogi.frag");


	ISOLA_GLDBG_( glUseProgram(towershogiSP[0]) )

	{int loc;
	loc = glGetUniformLocation(towershogiSP[0],"boardWidth");
	if(loc == -1){ SDL_Log("towershogi: boardWidth not found"); }
	glUniform1i(loc,towershogiBoardSize[0]);

	loc = glGetUniformLocation(towershogiSP[0],"boardHeight");
	if(loc == -1){ SDL_Log("towershogi: boardHeight not found"); }
	glUniform1i(loc,towershogiBoardSize[1]);

	loc = glGetUniformLocation(towershogiSP[0],"pieceAtlas");
	if(loc == -1){ SDL_Log("towershogi: pieceAtlas not found"); }
	glUniform1i(loc,0);
	}
}


void destroyTowershogi(void){

	SDL_free(towershogiBoard.tile);


	ISOLA_GLDBG_( glUseProgram(0) )
	ISOLA_GLDBG_( glBindVertexArray(0) )

	ISOLA_GLDBG_( glDeleteProgram(towershogiSP[0]) )
	ISOLA_GLDBG_( glDeleteBuffers(1,towershogiVBO) )
	ISOLA_GLDBG_( glDeleteVertexArrays(1,towershogiVAO) )
}


void stepTowershogi(void){

	if (isola_keyState[SDL_SCANCODE_J] && !isola_keyRepeat[SDL_SCANCODE_J]) {
		if (towershogiBoard.currentTile >= towershogiBoardSize[0]) {
			towershogiBoard.currentTile -= towershogiBoardSize[0];
		}
	}
	if (isola_keyState[SDL_SCANCODE_K] && !isola_keyRepeat[SDL_SCANCODE_K]) {
		if (towershogiBoard.currentTile < TOWERSHOGI_BOARD_SIZE 
				-towershogiBoardSize[0]) {
			towershogiBoard.currentTile += towershogiBoardSize[0];
		}
	}
	if (isola_keyState[SDL_SCANCODE_L] && !isola_keyRepeat[SDL_SCANCODE_L]) {
		if ((towershogiBoard.currentTile+1)%towershogiBoardSize[0]) {
			towershogiBoard.currentTile += 1;
		}
	}
	if (isola_keyState[SDL_SCANCODE_H] && !isola_keyRepeat[SDL_SCANCODE_H]) {
		if (towershogiBoard.currentTile%towershogiBoardSize[0]) {
			towershogiBoard.currentTile -= 1;
		}
	}
	if (isola_keyState[SDL_SCANCODE_SPACE] &&
			!isola_keyRepeat[SDL_SCANCODE_SPACE]) {
		if(towershogiBoard.selectedTile == SELECTED_NONE){
			towershogiBoard.tile[towershogiBoard.currentTile].state =
					STATE_SELECTED;
			towershogiBoard.selectedTile = towershogiBoard.currentTile;
		}
	}
	if (isola_keyState[SDL_SCANCODE_Q] && !isola_keyRepeat[SDL_SCANCODE_Q]) {
		if(towershogiBoard.selectedTile != SELECTED_NONE){
			towershogiBoard.currentTile = towershogiBoard.selectedTile;
			towershogiBoard.tile[towershogiBoard.selectedTile].state =
					STATE_NONE;
			towershogiBoard.selectedTile = SELECTED_NONE;
		}
	}
}


void drawTowershogi(void){

	ISOLA_GLDBG_( glBindVertexArray(towershogiVAO[0]) )

	isola_set_state(towershogiState);
	ISOLA_GLDBG_( glUseProgram(towershogiSP[0]) )

	ISOLA_GLDBG_( glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]) )

	SDL_memset(towershogiVD,0,TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]));
	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		{unsigned int v;
		for(v = 0;v<6;v++){
			towershogiVD[i*6*2+v*2] = towershogiBoard.tile[i].piece;
			towershogiVD[i*6*2+v*2+1] = towershogiBoard.tile[i].state;
		}}
	}}

	ISOLA_GLDBG_( glBufferSubData(GL_ARRAY_BUFFER,sizeof(towershogiVD[0])*0,
			TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]),towershogiVD) )


	{int loc;
	loc = glGetUniformLocation(towershogiSP[0],"currentPiece");
	if(loc == -1){ SDL_Log("currentPiece not found"); }
	glUniform1i(loc,towershogiBoard.currentTile);
	}


	ISOLA_GLDBG_( glDrawArrays(GL_TRIANGLES,0,TOWERSHOGI_VB_SIZE) )
}




