#version 330 core
struct Material
{
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shine;

};


struct Light
{

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};


out vec4 colour;
in vec3 FragPos;
in vec3 Normal;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
 
	//ambient lighting
	
	vec3 ambient = light.ambient * material.ambient;

	// diffuse lighting 
	vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(light.position - FragPos);
	float difference = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * (difference * material.diffuse);


	//specular lighting
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shine);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular);
	colour = vec4(result, 1.0f);



}