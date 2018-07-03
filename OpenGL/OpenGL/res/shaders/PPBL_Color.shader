#shader vertex
#version 330 core

in vec4 position;
in vec2 texCoord;

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

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = vec4(1.0,0.0,0.0,1.0);
	color = texColor;
}