#version 130




smooth in vec2 vfQuadPos;


out vec4 fragColor;




void main(){

	if(vfQuadPos.x > (15./16.-0.01) || vfQuadPos.x < (1./16.) ||
			vfQuadPos.y > (15./16.) || vfQuadPos.y <= (1./16.+0.01) ){
		fragColor = vec4(vec3(0.75,0.75,0.75),1.);
	}else{
		fragColor = vec4(vec3(0.75,0.75,0.75),0.);
	}
}




