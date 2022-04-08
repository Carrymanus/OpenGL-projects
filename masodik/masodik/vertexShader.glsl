#version 430

layout (location = 0) in vec3 aPos;

uniform bool isPoint;
uniform bool isPolygon;

out float myIsPoint;
out float myIsPolygon;

void main(void)
{
	gl_Position = vec4(aPos, 1.0);
	if(isPoint)
		myIsPoint = 1.0;
	else
		myIsPoint = 0.0;
	if(isPolygon)
		myIsPolygon = 1.0;
	else
		myIsPolygon = 0.0;
}