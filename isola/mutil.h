#ifndef MUTIL_H
#define MUTIL_H




#include <math.h>


#include <SDL2/SDL.h>




static void mut_proj_glortho(float left, float right,
		float bottom, float top, float nearZ, float farZ, float dest[4*4]){

	float rl,tb,fn;

	rl = 1/(right-left);
	tb = 1/(top-bottom);
	fn = -1/(farZ-nearZ);

	dest[0*4+0] = 2*rl;
	dest[1*4+1] = 2*tb;
	dest[2*4+2] = 2*fn;
	dest[3*4+0] = -(right+left)*rl;
	dest[3*4+1] = -(top+bottom)*tb;
	dest[3*4+2] = (farZ+nearZ)*fn;
	dest[3*4+3] = 1.;
}

static unsigned int mut_gcd(int a, int b){

	int t;
	while(b!=0){
		t=b;
		b=a%b;
		a=t;
	}
	return abs(a);
}

static void mut_print_m4(float m[4*4]){

	int i;
	SDL_Log("\n");
	for(i = 0;i<4;i++){
		SDL_Log("%f,%f,%f,%f,\n",m[0*4+i],m[1*4+i],m[2*4+i],m[3*4+i]);
	}
	SDL_Log("\n");
}

static void mut_print_m3(float m[3*3]){

	int i;
	SDL_Log("\n");
	for(i = 0;i<3;i++){
		SDL_Log("%f,%f,%f,\n",m[0*3+i],m[1*3+i],m[2*3+i]);
	}
	SDL_Log("\n");
}

static void mut_m3_cp(float src[3*3],float dest[3*3]){

	dest[0*3+0] = src[0*3+0];
	dest[0*3+1] = src[0*3+1];
	dest[0*3+2] = src[0*3+2];
	dest[1*3+0] = src[1*3+0];
	dest[1*3+1] = src[1*3+1];
	dest[1*3+2] = src[1*3+2];
	dest[2*3+0] = src[2*3+0];
	dest[2*3+1] = src[2*3+1];
	dest[2*3+2] = src[2*3+2];
}

static void mut_m3_cp_m4(float src[3*3],float dest[4*4]){

	dest[0*4+0] = src[0*3+0];
	dest[0*4+1] = src[0*3+1];
	dest[0*4+2] = src[0*3+2];
	dest[1*4+0] = src[1*3+0];
	dest[1*4+1] = src[1*3+1];
	dest[1*4+2] = src[1*3+2];
	dest[2*4+0] = src[2*3+0];
	dest[2*4+1] = src[2*3+1];
	dest[2*4+2] = src[2*3+2];
}

static void mut_m3_mul(float a[3*3],float b[3*3],float dest[3*3]){

	float c[3*3] = {0};
	c[0*3+0] = a[0*3+0]*b[0*3+0] + a[1*3+0]*b[0*3+1] + a[2*3+0]*b[0*3+2];
	c[0*3+1] = a[0*3+1]*b[0*3+0] + a[1*3+1]*b[0*3+1] + a[2*3+1]*b[0*3+2];
	c[0*3+2] = a[0*3+2]*b[0*3+0] + a[1*3+2]*b[0*3+1] + a[2*3+2]*b[0*3+2];
	c[1*3+0] = a[0*3+0]*b[1*3+0] + a[1*3+0]*b[1*3+1] + a[2*3+0]*b[1*3+2];
	c[1*3+1] = a[0*3+1]*b[1*3+0] + a[1*3+1]*b[1*3+1] + a[2*3+1]*b[1*3+2];
	c[1*3+2] = a[0*3+2]*b[1*3+0] + a[1*3+2]*b[1*3+1] + a[2*3+2]*b[1*3+2];
	c[2*3+0] = a[0*3+0]*b[2*3+0] + a[1*3+0]*b[2*3+1] + a[2*3+0]*b[2*3+2];
	c[2*3+1] = a[0*3+1]*b[2*3+0] + a[1*3+1]*b[2*3+1] + a[2*3+1]*b[2*3+2];
	c[2*3+2] = a[0*3+2]*b[2*3+0] + a[1*3+2]*b[2*3+1] + a[2*3+2]*b[2*3+2];
	mut_m3_cp(c,dest);
}




#endif
