#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 color_frag;
uniform mat4 vp; 


void main()
{
	gl_Position =  vp * vec4(position.xyz , 1.0f);
	color_frag = color;
}














