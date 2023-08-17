#version 330 core
layout (location = 0) in vec3 model;
layout (location = 1) in mat4 instanceMatrix;


uniform mat4  VP;   
uniform float PointSize;
uniform vec4  Color;

out vec4 color_frag;
out vec2 UV;

void main()
{

	gl_Position =  VP * instanceMatrix * vec4(model * PointSize , 1.0f);


	UV = model.xy + vec2(0.5, 0.5);
	
	color_frag = Color;

}











