#version 130




smooth out vec2 vfQuadPos;


uniform mat4 projection;
uniform float tileScale;
uniform float gapOffset;
uniform vec2 screenOffset;


uniform ivec2 boardPos;




const vec2 towershogiVertPos[] = vec2[](
		vec2(0. ,0. ),
		vec2(1. ,0. ),
		vec2(1. ,1. ),
		vec2(0. ,0. ),
		vec2(1. ,1. ),
		vec2(0. ,1. )
		);




void main(){

	vfQuadPos = towershogiVertPos[gl_VertexID];

	gl_Position = projection*
			vec4( ( towershogiVertPos[gl_VertexID]
					+boardPos*(1.+gapOffset) )*tileScale,
				-4.,1.)
			+vec4(screenOffset,0.,0.);
}




