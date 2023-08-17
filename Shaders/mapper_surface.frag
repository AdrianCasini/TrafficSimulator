#version 330 core

in vec4 colorF;
in vec2 textcoord;


out vec4 colorOutput;

uniform sampler2D Texture_f;
uniform sampler2D Texture_b;
uniform sampler2D Texture_w;

uniform float Txt_index;

void main()
{
	if (Txt_index == 2.0)
	{
		colorOutput = texture( Texture_f, textcoord ) * colorF;
	}
	else if (Txt_index == 3.0)
	{
		colorOutput = texture( Texture_w, textcoord ) * colorF;
	}
	else
	{
		colorOutput =  colorF;
	}
}
