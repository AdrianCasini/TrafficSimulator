#version 330 core


in vec2 UV;
in vec4 color_frag;


out vec4 color_out;

uniform sampler2D texture_smp;

void main()
{
	color_out = texture( texture_smp, UV )  * color_frag; 
	//color_out = color_frag; 
}