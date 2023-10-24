#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 view, proj, model,negyzet_model;
uniform bool padloe;
uniform bool negyzete;

out vec3 colour;

void main() {
	if(padloe){
		colour = vec3(0.54,0.27,0.0);
		gl_Position = proj * view  * vec4(vertex_position, 1.0);
	}
	else{
		if(negyzete){
			colour = vec3(1.0,0.0,1.0);
			gl_Position = proj * view * negyzet_model * vec4(vertex_position, 1.0);
		}
		else{
			colour = vertex_colour;
			gl_Position = proj * view * model * vec4(vertex_position, 1.0);
		}
	}
	
}
