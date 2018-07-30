#shader vertex
#version 330 core

in vec4 position;
in vec2 texCoord;
in vec3 normals;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 WorldPosition;

uniform mat4 u_MVP;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Normal = (u_Model * vec4(normals, 0.0)).xyz;
	WorldPosition = (u_Model * vec4(position.xyz, 1.0)).xyz;
}

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 WorldPosition;

out vec4 color;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform vec3 eyePos;
uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;

uniform float specularIntensity;
uniform float specularExponent;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	direction = normalize(direction);
	float diffuseFactor = max(dot(normal, -direction), 0);
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor>0)
	{
		diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;
	}

	vec3 directionToEye = normalize(eyePos - WorldPosition);
	vec3 reflectDirection = normalize(reflect(direction, normal));

	float specularFactor = max(dot(directionToEye, reflectDirection), 0);
	specularFactor = pow(specularFactor, specularExponent);

	if (specularFactor>0)
	{
		specularColor = vec4(base.color, 1.0) * specularIntensity * specularFactor;
	}

	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calcLight(directionalLight.base, directionalLight.direction, normal);
}

void main()
{
	color = texture(u_Texture, v_TexCoord) * calcDirectionalLight(u_DirectionalLight, normalize(v_Normal));
}