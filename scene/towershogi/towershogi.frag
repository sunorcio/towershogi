#version 130




flat in vec3 vfTileColor;


out vec4 fragColor;




void main(){

	fragColor = vec4(vfTileColor,1.);
}




