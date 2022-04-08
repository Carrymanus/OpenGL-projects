#version 430

layout (location = 0) in vec3 aPos;

uniform bool isPoint;

out float myIsPoint;

void main(void)
{
	gl_Position = vec4(aPos, 1.0);
	if(isPoint)
		myIsPoint = 1.0;
	else
		myIsPoint = 0.0;
}