#version 430

layout (location = 0) in vec3 aPosSphere;
layout (location = 1) in vec3 aNormalSphere;

out vec3 NormalSphere;
out vec3 FragPosSphere;
out vec3 myColorSphere;

uniform float lightX;
uniform float lightY;
uniform float lightZ;

uniform mat4 modelSphere;
uniform mat4 viewSphere;
uniform mat4 projectionSphere;
uniform mat4 invTMatrixSphere;

void main(void)
{
	//NormalSphere = normalize(vec3(invTMatrixSphere * vec4(aNormalSphere, 0.0)));
	NormalSphere = vec3(invTMatrixSphere * vec4(aNormalSphere, 0.0));
	FragPosSphere = vec3(viewSphere * modelSphere * vec4(vec3(aPosSphere.x + lightX,aPosSphere.y + lightY,aPosSphere.z + lightZ), 1.0));
	//FragPosSphere = vec3(viewSphere * modelSphere * vec4(vec3(aPosSphere.x,aPosSphere.y,aPosSphere.z), 1.0));

	myColorSphere = vec3(1.0,1.0,0.0);

	gl_Position =  projectionSphere * vec4(FragPosSphere, 1.0);
	//gl_Position =  vec4(aPosSphere.x,aPosSphere.y,aPosSphere.z, 1.0);

}
