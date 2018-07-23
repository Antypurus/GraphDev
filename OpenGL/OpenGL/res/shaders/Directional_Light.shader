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
	v_Normal = (u_Model * vec4(normals,0.0)).xyz;
	WorldPosition = (u_Model * vec4(position.xyz,1.0)).xyz;
}

#shader fragment
#version 330 core

const int MAX_POINT_LIGHTS = 4;

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
	vec3 position;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponent;
};

struct PointLight
{
	BaseLight base;
	Attenuation atten;
	vec3 position;
};

uniform vec3 eyePos;
uniform vec3 u_BaseColor;
uniform vec3 u_AmbientLight;
uniform sampler2D u_Texture;
//uniform DirectionalLight u_DirectionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform float specularIntensity;
uniform float specularExponent;

vec4 calcLight(BaseLight base, vec3 position, vec3 normal)
{
	vec3 direction = normalize(WorldPosition - position);
	float diffuseFactor = max(dot(normal,-direction),0);
	vec4 diffuseColor = vec4(0,0,0,0);
	vec4 specularColor = vec4(0,0,0,0);
	
	if(diffuseFactor>0)
	{
		diffuseColor = vec4(base.color,1.0) * base.intensity * diffuseFactor;
	}

	vec3 directionToEye = normalize(eyePos - WorldPosition);
	vec3 reflectDirection = normalize(reflect(direction,normal));

	float specularFactor = max(dot(directionToEye,reflectDirection),0);
	specularFactor = pow(specularFactor,specularExponent);

	if(specularFactor>0)
	{
		specularColor = vec4(base.color,1.0) * specularIntensity * specularFactor;
	}

	return diffuseColor + specularColor;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight,vec3 normal)
{
	return calcLight(directionalLight.base,directionalLight.position,normal);
}

vec4 calcPointLight(PointLight pointLight, vec3 normal)
{
	float distanceToPoint = length(WorldPosition - pointLight.position);

	vec4 color = calcLight(pointLight.base,pointLight.position,normal);

	float attenuation = pointLight.atten.constant + pointLight.atten.linear * distanceToPoint + pointLight.atten.exponent * distanceToPoint * distanceToPoint;

	if (attenuation < 1)
	{
		attenuation = 1;
	}

	return color/ attenuation;
}

void main()
{
	vec4 totalLight = vec4(0,0,0,1);
	vec4 endColor = vec4(u_BaseColor,1);
	vec4 texture = texture(u_Texture, v_TexCoord);

	vec3 normal = normalize(v_Normal);
	
	for(int i=0 ; i<MAX_POINT_LIGHTS ; i++)
	{
		totalLight += calcPointLight(pointLights[i],normal);
	}

	endColor *= texture;

	color =  endColor * totalLight;
}