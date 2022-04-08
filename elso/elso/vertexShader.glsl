#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float offsetX;
uniform float offsetY;
uniform float lineoffsetY;
uniform bool isLine;

out vec3 myColor;

void main(void)
{
	if(!isLine){
		if (offsetX < 0.54 && offsetX > -0.54 && offsetY < lineoffsetY + 0.21 && offsetY > lineoffsetY - 0.21)
			myColor = aColor;
		else
			myColor = vec3(aColor.y,aColor.x,aColor.z);
		gl_Position = vec4(aPos.x + offsetX, aPos.y + offsetY, aPos.z, 1.0);
	}
	else{
		gl_Position = vec4(aPos.x, aPos.y + lineoffsetY, aPos.z, 1.0);
		myColor = vec3(0.0,0.0,1.0);
	}
		
}