#version 330 core

in vec4 colorF;
in vec2 textcoord;

out vec4 colorOutput;

uniform sampler2D Texture;

void main()
{
    colorOutput = texture( Texture,  textcoord) * colorF;
}
