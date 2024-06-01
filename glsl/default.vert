#version 150




uniform mat4 matProj;


in vec3 vertPosition;
in vec3 vertColor;


#define scale 0.0625;




void main(){


	gl_Position = matProj * vec4( position*scale,-1.,1.);
}




