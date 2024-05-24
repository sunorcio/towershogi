#version 130




<<<<<<< HEAD
uniform mat4 matProj;


=======
in vec3 vertPosition;
in vec3 vertColor;
>>>>>>> 78c117ba3555663f33ab0277793cd4d3b921a663


void main(){

	float scale = 0.0625;


<<<<<<< HEAD
	gl_Position = matProj * vec4( position,-1.,1.);
=======
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProj;
uniform vec3 position;




void main(){

	vfColor = vertColor;
/* 	vfColor = pow(vec3(vertPosition.x+position.x,
					(vertPosition.y+position.y)*sqrt(2)/(2.*cos(asin(0.5))),
					vertPosition.z+position.z)/16,
					vec3(1,1,1)); */


	gl_Position = matProj * matView * matModel *
				vec4(vertPosition.x+position.x,
					(vertPosition.y+position.y)*sqrt(2)/(2.*cos(asin(0.5))),
					vertPosition.z+position.z,
					1.);
>>>>>>> 78c117ba3555663f33ab0277793cd4d3b921a663
}




