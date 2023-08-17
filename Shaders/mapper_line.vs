#version 330 core
layout (location = 0) in vec3 position;

out vec4 colorF;

uniform mat4 mVP; 
uniform vec3 mColor; 


void main()
{
	gl_Position =  mVP * vec4(position.xyz, 1.0f);
	colorF = vec4(mColor, 1.0f);
}














