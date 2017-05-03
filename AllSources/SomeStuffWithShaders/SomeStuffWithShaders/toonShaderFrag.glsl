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

in vec3 normal;
in vec2 texCoord;

out vec4 f_color;

uniform int accessTextures;
uniform Material material;
uniform DirectionalLight directional_light;

void main()
{
	float intensity = dot(normalize(directional_light.direction), normalize(normal));

	if(accessTextures == 1)
	{
		
		f_color = vec4(directional_light.diffuse * vec3(texture(material.diffuse, texCoord)), 1.0);

		if(intensity > .8)
		{
			f_color = f_color * vec4(vec3(1.2), 1.0);
		}
		else if(intensity < .25)
		{
			f_color = f_color * vec4(vec3(.7), 1.0);
		}
	}
	else
	{
		f_color = vec4(material.defaultDiffuse, 1.0);

		if(intensity > .8)
		{
			f_color = f_color * vec4(vec3(1.2), 1.0);
		}
		else if(intensity < .25)
		{
			f_color = f_color * vec4(vec3(.7), 1.0);
		}
	}
}