#version 330 core


in vec2 TexCoords;
out vec4 color;
//in vec3 out_color;

uniform vec3 textColor;
uniform sampler2D text;


void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	color = vec4(textColor, 1.0) * sampled;
}