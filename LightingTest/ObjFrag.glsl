#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 ViewPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 fColor;

void main()
{
	//Ambient
	vec3 ambient = LightColor * material.ambient;

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = LightColor * (diff * material.diffuse);

	//Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
	vec3 specular = LightColor * (spec * material.specular);

	vec3 result = (specular + ambient + diffuse);

	fColor = vec4(result, 1.0);
}