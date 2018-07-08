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

uniform mat4 u_Model;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;

	v_SurfaceNormal = (u_Model * vec4(normals, 0.0)).xyz;
	v_ToLightVector = u_LPosition - (u_Model * position).xyz;
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightVector;

uniform vec3 u_LColor;
uniform sampler2D u_Texture;

void main()
{
	vec3 unitNormal = normalize(v_SurfaceNormal);
	vec3 unitLightVector = normalize(v_ToLightVector);

	float dotp = dot(unitNormal, unitLightVector);
	float bright = max(dotp, 0.0);
	vec3 diffuse = bright * u_LColor;

	vec3 ambient = 0.1f * u_LColor;

	vec4 texColor = vec4((ambient + diffuse), 1.0f) * texture(u_Texture, v_TexCoord);
	color = texColor;
}