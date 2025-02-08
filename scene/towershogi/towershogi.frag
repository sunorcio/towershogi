#version 130




in vec2 vfTexCoord;
flat in int vfTileState;
flat in int vfTileCurrent;


out vec4 fragColor;


uniform sampler2D pieceAtlas;




const vec3 towershogiStateColor[4] = vec3[](
	vec3(0.375,0.375,0.375),	/* void */
	vec3(0.0,0.0,0.375),	/* selected piece */
	vec3(0.0,0.375,0.0),	/* possible move */
	vec3(0.375,0.0,0.0)		/* possible eat */
	);




void main(){

	vec4 texFrag = texture(pieceAtlas,vfTexCoord);


	if(texFrag.w == 0.){
		fragColor = vec4(
				towershogiStateColor[vfTileState]
						*(1.+vfTileCurrent*1.)
				,1.);
	}else{
		fragColor = texFrag;
	}
}




