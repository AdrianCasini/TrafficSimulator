#version 330 core


out vec4 colorFinal;
in vec2 UV;


uniform sampler2D texture_btt;

void main()
{
    colorFinal = texture( texture_btt, UV ) ; 

}

