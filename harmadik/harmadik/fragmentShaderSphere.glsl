#version 430

in vec3 myColorSphere;

out vec4 color;

void main(void)
{
	
    color = vec4(myColorSphere, 1.0);
}