#version 430

in vec3 Normal;
in vec3 FragPos;
in vec3 myColor;

uniform vec3 lightPos;

out vec4 color;

void main(void)
{
	// AMBIENT
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 0.0);
  	
    // DIFFUSE
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

    vec3 colorAttribute = myColor;        
    vec3 result = (ambient + diffuse) * colorAttribute;
    color = vec4(result, 1.0);
}