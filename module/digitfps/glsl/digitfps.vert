#version 130




uniform mat4 matProj;
uniform int lowResolution;
uniform int pixelSize;


uniform int digitAmount;




#define pixelScale ( (float(pixelSize)/float(lowResolution))*2. )
#define screenPos vec2(1.,1.)
#define digitOffset vec2( -float(digitAmount)*1.25,-2.25 )


const vec2 digitVertex[15] = vec2[](
	vec2(0. ,0. ), vec2(0.5,0. ), vec2(1. ,0. ),
	vec2(0. ,0.5), vec2(0.5,0.5), vec2(1. ,0.5),
	vec2(0. ,1. ), vec2(0.5,1. ), vec2(1. ,1. ),
	vec2(0. ,1.5), vec2(0.5,1.5), vec2(1. ,1.5),
	vec2(0. ,2. ), vec2(0.5,2. ), vec2(1. ,2. )
	);




void main(){


	gl_Position = matProj * vec4(
					( digitVertex[gl_VertexID%15]
						+vec2((gl_VertexID/15)*1.25,0.)
						+digitOffset )
						*pixelScale,
					-1.,1. )
			+ vec4(screenPos,0.,0.);
}




