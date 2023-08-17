#version 330 core


out vec4 colorFinal;
in vec2 UV;


uniform sampler2D textureBtt;

void main()
{
    colorFinal = texture( textureBtt, UV ) ; 

}

