#version 330 core
layout (location = 0) in vec3 position;

out vec4 colorF;
out vec2 textcoord;

uniform mat4 mVP; 
uniform vec3 mColor; 


void main()
{

	gl_Position =  mVP * vec4(position.xyz , 1.0f);
	textcoord = vec2(position.x, position.y) * 3.0 + vec2(0.5, 0.5);
	colorF = vec4(mColor, 1.0f);
}














