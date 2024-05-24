#version 130




uniform mat4 matProj;


const vec2 digitVertex[15] = vec2[](
	vec2(0. ,0. ), vec2(0.5,0. ), vec2(1. ,0. ),
	vec2(0. ,0.5), vec2(0.5,0.5), vec2(1. ,0.5),
	vec2(0. ,1. ), vec2(0.5,1. ), vec2(1. ,1. ),
	vec2(0. ,1.5), vec2(0.5,1.5), vec2(1. ,1.5),
	vec2(0. ,2. ), vec2(0.5,2. ), vec2(1. ,2. )
	);


#define scale 24./768.




void main(){

	vec2 position = digitVertex[gl_VertexID%15] * scale
		+ vec2( -1/matProj[0][0] + (gl_VertexID/15)*1.25*scale 
		+ 0.25*scale, 1-2.25*scale);


	gl_Position = matProj * vec4( position,-1.,1.);
}




