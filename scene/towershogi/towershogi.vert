#version 130




in int vertTilePiece;
in int vertTileState;


out vec2 vfTexCoord;
flat out int vfTileState;
flat out int vfTileCurrent;


uniform mat4 matProj;


uniform int boardWidth;
uniform int boardHeight;
uniform int currentPiece;




const vec2 towershogiVertPos[6] = vec2[](
	vec2(0. ,0. ),
	vec2(1. ,0. ),
	vec2(1. ,1. ),
	vec2(0. ,0. ),
	vec2(1. ,1. ),
	vec2(0. ,1. )
	);


#define pieceOffsetX 1./4
#define pieceOffsetY 1./4

const vec2 towershogiTexCoord[6] = vec2[](
	vec2(0*pieceOffsetX ,1*pieceOffsetY ),
	vec2(1*pieceOffsetX ,1*pieceOffsetY ),
	vec2(1*pieceOffsetX ,0*pieceOffsetY ),
	vec2(0*pieceOffsetX ,1*pieceOffsetY ),
	vec2(1*pieceOffsetX ,0*pieceOffsetY ),
	vec2(0*pieceOffsetX ,0*pieceOffsetY )
	);


#define tileGap 0.125




void main(){

	vfTileState = vertTileState;

	vfTexCoord = towershogiTexCoord[gl_VertexID%6] +
			vec2(vertTilePiece%4*pieceOffsetX,
			vertTilePiece/4*pieceOffsetY);

	vfTileCurrent = 0;
	if(gl_VertexID/6 == currentPiece){
		vfTileCurrent = 1;
	}


	float tileScale;
	if(boardWidth>boardHeight){
		tileScale = boardWidth;
	}else{
		tileScale = boardHeight;
	}
	tileScale = 2./(tileScale*(1.+tileGap));


	gl_Position = matProj * vec4(
			(towershogiVertPos[gl_VertexID%6]
					+ivec2( (gl_VertexID/6)%boardWidth,
							(gl_VertexID/6)/boardWidth )*(1.+tileGap)
					+vec2(tileGap/2.,tileGap/2.)
					)*tileScale
					+vec2(-1,-1),
			-4.,1.);
}




