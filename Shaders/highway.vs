#version 330 core
layout (location = 0) in vec3 position;


out vec4 colorF;
out vec2 textcoord;

uniform mat4 VP; 
uniform float Transp; 

vec3 pos;
void main()
{


	gl_Position =  VP * vec4(position.xyz, 1.0f);

    textcoord = vec2(position.x, position.y) * 20.0 + vec2(0.5, 0.5);
	colorF    = vec4(0.9,0.9,0.9,Transp);
}














