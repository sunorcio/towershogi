

#include "towershogi_render.h"
#include "towershogi_render_logic.h"




#include <isola/isola.h>
#include <isola/mutil.h>
#include <isola/misc.h>

#include "towershogi.h"




#define TOWERSHOGI_VB_SIZE (TOWERSHOGI_BOARD_SIZE*6)




unsigned char * towershogiBoardstate;


static ISOLA_State towershogiState = 0x0000000d;
static unsigned int towershogiSP[1] = {0};
static unsigned int towershogiVAO[1] = {0};
static unsigned int towershogiVBO[1] = {0};
static unsigned char * towershogiVD = {0};




void updateTowershogiRender(void){

	glBindVertexArray(towershogiVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(towershogiVD),towershogiVD);


	glUseProgram(towershogiSP[0]);

	{int locProj;
	const char* strProj = "matProj";
	float matProj[4*4] = {0};
	mut_proj_glortho(-isolaInfoWindow.xratio,isolaInfoWindow.xratio,
			-isolaInfoWindow.yratio,isolaInfoWindow.yratio,
			0.25,8.,matProj);
	locProj = glGetUniformLocation(towershogiSP[0],"matProj");
	if(locProj == -1){SDL_Log("matProj not found in shader %d",0);}
	glUniformMatrix4fv(locProj,1,GL_FALSE,matProj);
	}
}


void createTowershogiRender(void){

	towershogiBoardstate = calloc(sizeof(unsigned char),TOWERSHOGI_BOARD_SIZE);
	towershogiVD = calloc(sizeof(unsigned char),TOWERSHOGI_BOARD_SIZE*6);


	glGenVertexArrays(1,&towershogiVAO[0]);
	glGenBuffers(1,&towershogiVBO[0]);

	glBindVertexArray(towershogiVAO[0]);


	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER,TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]),
			towershogiVD,GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,
			sizeof(towershogiVD[0]),(void*)0);


	towershogiSP[0] = isolaShaderProgram("scene/towershogi/towershogi.vert",
			"scene/towershogi/towershogi.frag");

	glUseProgram(towershogiSP[0]);

	{int locBrdWdth;
	const char* strBrdWdth = "boardWidth";
	locBrdWdth = glGetUniformLocation(towershogiSP[0],"boardWidth");
	if(locBrdWdth == -1){SDL_Log("%s not found in shader %d",strBrdWdth,0);}
	glUniform1i(locBrdWdth,towershogiBoardSize[0]);
	}

	{int locBrdHght;
	const char* strBrdHght = "boardHeight";
	locBrdHght = glGetUniformLocation(towershogiSP[0],"boardHeight");
	if(locBrdHght == -1){SDL_Log("%s not found in shader %d",strBrdHght,0);}
	glUniform1i(locBrdHght,towershogiBoardSize[1]);
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

	isolaSetState(towershogiState);
	glUseProgram(towershogiSP[0]);

	glBindBuffer(GL_ARRAY_BUFFER,towershogiVBO[0]);


	memset(towershogiVD,0,TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]));
	{unsigned int i;
	for(i = 0;i<TOWERSHOGI_BOARD_SIZE;i++){
		{unsigned int v;
		for(v = 0;v<6;v++){
			towershogiVD[i*6+v] = towershogiBoardstate[i];
		}}
	}}

	glBufferSubData(GL_ARRAY_BUFFER,sizeof(towershogiVD[0])*0,
			TOWERSHOGI_VB_SIZE*sizeof(towershogiVD[0]),towershogiVD);


	glDrawArrays(GL_TRIANGLES,0,TOWERSHOGI_VB_SIZE);
}




