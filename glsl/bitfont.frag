#version 130




in vec2 vfTexCoord;


out vec4 fragColor;


uniform sampler2D bitfontAtlas;

uniform vec4 fontColor;
uniform vec4 backColor;




void main(){


	if (texture(bitfontAtlas, vfTexCoord).w == 0.){
		fragColor = backColor;
	}else{
		fragColor = fontColor;
	}
}




