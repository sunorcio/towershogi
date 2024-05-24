#version 130




in vec2 vfTexCoord;


out vec4 fragColor;


uniform sampler2D bitfontAtlas;




void main(){



	fragColor = vec4(texture(bitfontAtlas, vfTexCoord));
}




