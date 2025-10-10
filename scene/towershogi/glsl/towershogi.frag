#version 130




in vec2 vfTexCoord;
flat in int vfState;


out vec4 fragColor;


uniform sampler2D pieceAtlas;




const vec3 towershogiStateColor[] = vec3[](
	vec3(0.375,0.375,0.375), /* none */
	vec3(0.25,0.25,0.375), /* hovered */
	vec3(0.25,0.375,0.25), /* availiable while hovered */
	vec3(0.375,0.25,0.25), /* capture while hovered */
	vec3(0.125,0.125,0.5), /* selected */
	vec3(0.125,0.25,0.125), /* unavailiable while selected */
	vec3(0.125,0.5,0.125), /* availiable while selected */
	vec3(0.5,0.125,0.125) /* capture while selected */
	);




void main(){

	vec4 texFrag = texture(pieceAtlas,vfTexCoord);


/* 	if(texFrag.w == 0.){
		fragColor = vec4(towershogiStateColor[vfState],1.);
	}else{
		fragColor = texFrag;
	} */
	fragColor = vec4(
					towershogiStateColor[vfState]*float(!bool(texFrag.w))
					+texFrag.xyz*texFrag.w
			,1.);
}




