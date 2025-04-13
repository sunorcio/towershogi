

#include "towershogi_render.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>

#include "towershogi.h"




#define TOWERSHOGI_VB_SIZE (TOWERSHOGI_BOARD_SIZE*6*2)




static ISOLA_state towershogiState = 0x0000000d;
static unsigned int towershogiSP[1] = {0};
static unsigned int towershogiVAO[1] = {0};
static unsigned int towershogiVBO[1] = {0};
static unsigned int towershogiTO[1] = {0};
static unsigned char * towershogiVD = {0};




void updateTowershogiRender(void){

	glBindVertexArray(towershogiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(towershogiVD),towershogiVD);


	glUseProgram(towershogiSP[0]);

	{int locProj;
	const char* strProj = "matProj";
	float matProj[4*4] = {0};
	isola_mut_glproj_ortho(-isola_info_window.xRatio,isola_info_window.xRatio,
			-isola_info_window.yRatio,isola_info_window.yRatio,
			0.25,8.,matProj);
	locProj = glGetUniformLocation(towershogiSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}
}


void createTowershogiRender(void){

	towershogiVD = calloc(sizeof(unsigned char),TOWERSHOGI_VB_SIZE);


	glGenVertexArrays(1,&towershogiVAO[0]);
	glGenBuffers(1,&towershogiVBO[0]);

	glBindVertexArray(towershogiVAO[0]);


	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER,TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]),
			towershogiVD,GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,
			sizeof(towershogiVD[0])*2,(void*)(sizeof(towershogiVD[0])*0));
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1,1,GL_UNSIGNED_BYTE,
			sizeof(towershogiVD[0])*2,(void*)(sizeof(towershogiVD[0])*1));


	glGenTextures(1,towershogiTO);
	glActiveTexture(GL_TEXTURE0+0);
	glBindTexture(GL_TEXTURE_2D,towershogiTO[0]);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	{void* imagedata;
	long filesize;
	FILE* imagefile = fopen("scene/towershogi/16x16p_4x4_chess.data","rb");
	fseek(imagefile,0,SEEK_END);
	filesize = ftell(imagefile);
	fseek(imagefile,0,SEEK_SET);
	imagedata = malloc(filesize+1);
	fread(imagedata,filesize,1,imagefile);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,16*4,16*4,
			0,GL_RGBA,GL_UNSIGNED_BYTE,(char*)imagedata);
	fclose(imagefile);
	free(imagedata);
	}


	towershogiSP[0] = isola_shader_buildProgram("scene/towershogi/towershogi.vert",
			"scene/towershogi/towershogi.frag");

	glUseProgram(towershogiSP[0]);

	{int locBrdWdth;
	const char* strBrdWdth = "boardWidth";
	locBrdWdth = glGetUniformLocation(towershogiSP[0],strBrdWdth);
	if(locBrdWdth == -1){
		SDL_Log("%s not found : %s", strBrdWdth, __FUNCTION__);
	}
	glUniform1i(locBrdWdth,towershogiBoardSize[0]);
	}

	{int locBrdHght;
	const char* strBrdHght = "boardHeight";
	locBrdHght = glGetUniformLocation(towershogiSP[0],strBrdHght);
	if(locBrdHght == -1){
		SDL_Log("%s not found : %s", strBrdHght, __FUNCTION__);
	}
	glUniform1i(locBrdHght,towershogiBoardSize[1]);
	}

	{int locPatl;
	const char* strPatl = "pieceAtlas";
	locPatl = glGetUniformLocation(towershogiSP[0],strPatl);
	if(locPatl == -1){
		SDL_Log("%s not found : %s", strPatl, __FUNCTION__);
	}
	glUniform1i(locPatl,0);
	}


	updateTowershogiRender();
}


void destroyTowershogiRender(void){

	glUseProgram(0);
	glBindVertexArray(0);

	glDeleteProgram(towershogiSP[0]);
	glDeleteBuffers(1,towershogiVBO);
	glDeleteVertexArrays(1,towershogiVAO);
}


void drawTowershogiRender(void){

	glBindVertexArray(towershogiVAO[0]);

	isola_set_state(towershogiState);
	glUseProgram(towershogiSP[0]);

	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);


	memset(towershogiVD,0,TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]));
	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		{unsigned int v;
		for(v = 0;v<6;v++){
			towershogiVD[i*6*2+v*2] = towershogiBoard.tile[i].piece;
			towershogiVD[i*6*2+v*2+1] = towershogiBoard.tile[i].state;
		}}
	}}

	glBufferSubData(GL_ARRAY_BUFFER,sizeof(towershogiVD[0])*0,
			TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]),towershogiVD);


	{int locCpz;
	const char* strCpz = "currentPiece";
	locCpz = glGetUniformLocation(towershogiSP[0],strCpz);
	if(locCpz == -1){
		SDL_Log("%s not found : %s", strCpz, __FUNCTION__);
	}
	glUniform1i(locCpz,towershogiBoard.currentTile);
	}


	glDrawArrays(GL_TRIANGLES,0,TOWERSHOGI_VB_SIZE);
}




