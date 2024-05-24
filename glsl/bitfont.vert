#version 130




out vec2 vfTexCoord;


uniform mat4 matProj;


const vec2 bitfontVertex[4] = vec2[](
	vec2(0. ,0. ),
	vec2(1. ,0. ),
	vec2(1. ,1. ),
	vec2(0. ,1. )
	);


#define scale 1.
/* 	24./768. */




void main(){

	vec2 position = bitfontVertex[gl_VertexID%4];


	vfTexCoord = position;


	gl_Position = matProj * vec4( position*scale,-1.,1.);
}




