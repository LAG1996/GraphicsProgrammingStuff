#version 330

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	vec3 ambient;
	vec3 defaultDiffuse;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 position;
in vec2 texCoord;
in vec3 normal;
in vec4 weights;
in ivec4 ids;

out vec4 color;

uniform vec3 viewPos;

uniform int accessTextures;

uniform Material material;
uniform DirectionalLight directional_light;

void main()
{
	//Ambient
	vec3 ambient = directional_light.ambient + material.ambient;
	ambient = vec3(0.1) * ambient;

	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-directional_light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	if(accessTextures == 1)
		 diffuse = directional_light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	else
		diffuse = directional_light.diffuse * diff * material.defaultDiffuse;

	//Specular
	vec3 viewDir = normalize(viewPos - position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(0.0, 0.0, 0.0); 
	if(accessTextures == 1)
		 specular = directional_light.specular * spec * vec3(texture(material.specular, texCoord));

	color = vec4(diffuse + specular + ambient, 1.0);
}