#version 330 core
layout (location = 0) in vec3 position;


out vec4 colorF;


uniform mat4 VP; 
uniform vec4 Color; 


void main()
{
	gl_Position =  VP * vec4(position.xyz, 1.0f);
	colorF = Color;

}














