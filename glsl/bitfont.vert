#version 130




in int vertCharIndex;


out vec2 vfTexCoord;
flat out int vfCharIndex;


uniform mat4 matProj;
uniform float lowResolution;

uniform vec2 screenPos;
uniform int pixelSize;
uniform int charWrap;




#define scale (2.*pixelSize/lowResolution)

#define charOffsetX 1/16.
#define charOffsetY 1./8.


const vec2 bitfontVertPos[6] = vec2[](
	vec2(0. ,0. ),
	vec2(1. ,0. ),
	vec2(1. ,2. ),
	vec2(0. ,0. ),
	vec2(1. ,2. ),
	vec2(0. ,2. )
	);

const vec2 bitfontTexCoord[6] = vec2[](
	vec2(0*charOffsetX ,1*charOffsetY ),
	vec2(1*charOffsetX ,1*charOffsetY ),
	vec2(1*charOffsetX ,0*charOffsetY ),
	vec2(0*charOffsetX ,1*charOffsetY ),
	vec2(1*charOffsetX ,0*charOffsetY ),
	vec2(0*charOffsetX ,0*charOffsetY )
	);




void main(){

	vfCharIndex = vertCharIndex;

	vfTexCoord = bitfontTexCoord[gl_VertexID%6] +
			vec2(vertCharIndex%16*charOffsetX,
			vertCharIndex/16*charOffsetY);


	gl_Position = matProj * 
			vec4( (bitfontVertPos[gl_VertexID%6]
			+ ivec2( (gl_VertexID/6)%charWrap, -2*(gl_VertexID/6/charWrap))
			+ vec2(0.,-2.))*scale
			,-2.,1. )
			+ vec4(screenPos,0.,0.);
}




