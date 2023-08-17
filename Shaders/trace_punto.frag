#version 330 core


in vec2 UV;
in vec4 colorF;

out vec4 colorG;

uniform sampler2D Texture;

void main()
{
	colorG = texture( Texture, UV ) * colorF; 
}
