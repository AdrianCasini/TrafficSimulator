#version 330 core
layout (location = 0) in vec3 model;
layout (location = 1) in vec4 color;
layout (location = 2) in mat4 instanceMatrix;


out vec2 UV;
out vec4 colorF;

uniform mat4  VP;     
uniform float PW;


void main()
{

	gl_Position =  VP * instanceMatrix * vec4(model * PW, 1.0f);
	UV = model.xy + vec2(0.5, 0.5);
	colorF = color;
}













