#version 130




in int vertPiece;
in int vertState;


out vec2 vfTexCoord;
flat out int vfState;


uniform mat4 projection;
uniform float tileScale;
uniform float gapOffset;
uniform vec2 screenOffset;


uniform int boardWidth;




#define texOffsetX (1./4.)
#define texOffsetY (1./4.)




const vec2 towershogiVertPos[] = vec2[](
		vec2(0. ,0. ),
		vec2(1. ,0. ),
		vec2(1. ,1. ),
		vec2(0. ,0. ),
		vec2(1. ,1. ),
		vec2(0. ,1. )
		);

const vec2 towershogiTexCoord[] = vec2[](
		vec2(0*texOffsetX ,1*texOffsetY ),
		vec2(1*texOffsetX ,1*texOffsetY ),
		vec2(1*texOffsetX ,0*texOffsetY ),
		vec2(0*texOffsetX ,1*texOffsetY ),
		vec2(1*texOffsetX ,0*texOffsetY ),
		vec2(0*texOffsetX ,0*texOffsetY )
		);




void main(){

	vfTexCoord = towershogiTexCoord[gl_VertexID%6]
			+vec2(vertPiece%4*texOffsetX,vertPiece/4*texOffsetY);


	vfState = vertState;


	gl_Position = projection
			*vec4(
				( towershogiVertPos[gl_VertexID%6]
					+ivec2( (gl_VertexID/6)%boardWidth,
					(gl_VertexID/6)/boardWidth )*(1.+gapOffset) )*tileScale,
				-4.,1.)
			+vec4(screenOffset,0.,0.);
}




