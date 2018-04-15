#version 330 core


out vec4 colour;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColour;
uniform vec3 lightColour; 

void main()
{
 
	//ambient lighting
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColour;

	// diffuse lighting 
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);
	float difference = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = difference * lightColour;


	//specular lighting
	float specularStrength = 1.0f;
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour;

	vec3 result = (ambient + diffuse + specular) * objectColour;
	colour = vec4(result, 1.0f);



}