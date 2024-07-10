#version 130




uniform mat4 matProj;
uniform float lowResolution;

uniform int pixelSize;




#define scale pixelSize/lowResolution
#define screenPos vec2(-1.,1.)


const vec2 digitVertex[15] = vec2[](
	vec2(0. ,0. ), vec2(0.5,0. ), vec2(1. ,0. ),
	vec2(0. ,0.5), vec2(0.5,0.5), vec2(1. ,0.5),
	vec2(0. ,1. ), vec2(0.5,1. ), vec2(1. ,1. ),
	vec2(0. ,1.5), vec2(0.5,1.5), vec2(1. ,1.5),
	vec2(0. ,2. ), vec2(0.5,2. ), vec2(1. ,2. )
	);




void main(){


	gl_Position = matProj * 
		( vec4( (digitVertex[gl_VertexID%15]
		+vec2((gl_VertexID/15)*1.25,0.))*scale,
		-1.,1. )
		+ vec4(vec2(0.25*scale,-2.25*scale),0.,0.) )
		+ vec4(screenPos,0.,0.);
}




