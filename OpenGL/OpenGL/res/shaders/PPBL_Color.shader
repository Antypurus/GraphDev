#shader vertex
#version 330 core

in vec4 position;
in vec2 texCoord;
in vec3 normals;

out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_ToLightVector;
out vec3 v_ToCameravector;

uniform mat4 u_MVP;
uniform vec3 u_LPosition;
uniform mat4 u_View;
uniform vec3 u_CameraPos;

uniform mat4 u_Model;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;

	v_SurfaceNormal = (u_Model * vec4(normals,0.0)).xyz;
	v_ToLightVector = u_LPosition - (u_Model * position).xyz;
	v_ToCameravector = u_CameraPos - (u_Model * position).xyz;
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightVector;
in vec3 v_ToCameravector;

uniform vec4 u_Color;
uniform vec3 u_LColor;

uniform float shineDamper;
uniform float reflectivity;

void main()
{
	vec3 unitNormal = normalize(v_SurfaceNormal);
	vec3 unitLightVector = normalize(v_ToLightVector);

	float dotp = dot(unitNormal, unitLightVector);
	float bright = max(dotp, 0.0);

	vec3 diffuse = bright * u_LColor;
	vec3 ambient = 0.1f * u_LColor;

	vec3 unitVectorToCamera = normalize(v_ToCameravector);
	vec3 lighDirection = -unitLightVector;
	vec3 reflectedLigthDirection = reflect(lighDirection, unitNormal);

	float specularFactor = dot(reflectedLigthDirection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);
	float dampedfactor = pow(specularFactor, shineDamper);
	vec3 finalSpecular = dampedfactor * reflectivity * u_LColor;

	vec4 texColor = vec4((ambient + diffuse),1.0f) * vec4(1.0, 0.1, 0.1, 1.0) + vec4(finalSpecular,1.0);
	color = texColor;
}