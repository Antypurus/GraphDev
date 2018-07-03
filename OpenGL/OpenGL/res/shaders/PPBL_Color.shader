#shader vertex
#version 330 core

in vec4 position;
in vec2 texCoord;
in vec3 normals;

out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_ToLightVector;

uniform mat4 u_MVP;
uniform vec3 u_LPosition;

void main()
{
	// this is missing the accounting of transformations in the normals position outputs since this shader is not ready for that
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;

	v_SurfaceNormal = normals;
	v_ToLightVector = u_LPosition - position.xyz;
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightVector;

uniform vec4 u_Color;
uniform vec3 u_LColor;

void main()
{
	vec3 unitNormal = normalize(v_SurfaceNormal);
	vec3 unitLightVector = normalize(v_ToLightVector);

	float dotp = dot(unitNormal, unitLightVector);
	float bright = max(dotp, 0.0);

	vec3 diffuse = bright * u_LColor;

	vec4 texColor = vec4(diffuse, 1.0) * vec4(1.0, 0.0, 0.0, 1.0);
	color = texColor;
}