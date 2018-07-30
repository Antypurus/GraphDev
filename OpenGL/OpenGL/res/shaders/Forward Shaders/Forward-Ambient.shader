#shader vertex
#version 330 core

in vec4 position;
in vec2 texCoord;
in vec3 Normals;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec3 AmbientIntensity;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord) * vec4(AmbientIntensity,1.0);
	color = texColor;
}