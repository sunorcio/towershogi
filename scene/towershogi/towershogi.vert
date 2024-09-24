#version 130




in int vertTileState;


flat out vec3 vfTileColor;


uniform mat4 matProj;


uniform int boardWidth;
uniform int boardHeight;




const vec2 towershogiVertPos[6] = vec2[](
	vec2(0. ,0. ),
	vec2(1. ,0. ),
	vec2(1. ,1. ),
	vec2(0. ,0. ),
	vec2(1. ,1. ),
	vec2(0. ,1. )
	);


const vec3 towershogiStateColor[5] = vec3[](
	vec3(0.25,0.25,0.25),	/* void */
	vec3(0.0,0.0,0.75),	/* player 1 piece */
	vec3(0.5,0.0,0.5),	/* selected player 1 piece */
	vec3(0.0,0.75,0.0),	/* possible moves */
	vec3(0.75,0.0,0.0)	/* possible move that eats piece */
	);




void main(){

	vfTileColor = towershogiStateColor[vertTileState];


	float tileScale;
	if(boardWidth>boardHeight){
		tileScale = boardWidth;
	}else{
		tileScale = boardHeight;
	}
	tileScale = 2./(tileScale*1.25);


	gl_Position = matProj * vec4(
			(towershogiVertPos[gl_VertexID%6]
					+ivec2( (gl_VertexID/6)%boardWidth,
							(gl_VertexID/6)/boardWidth )*1.25
					+vec2(0.25/2.,0.25/2.)
					)*tileScale
					+vec2(-1,-1),
			-4.,1.);
}




