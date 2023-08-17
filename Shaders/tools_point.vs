#version 330 core
layout (location = 0) in vec3 model;
layout (location = 1) in vec3 offset;

uniform vec3  CameraR;
uniform vec3  CameraU;
uniform mat4  VP;   
uniform float PointSize;
uniform vec4  Color;

out vec4 color_frag;
out vec2 UV;

void main()
{


	vec3 vertexPosition_worldspace = vec3(offset.x , offset.y , offset.z) + CameraR * model.x * PointSize + CameraU * model.y * PointSize ;

	gl_Position =  VP * vec4(vertexPosition_worldspace, 1.0f);

	UV = model.xy + vec2(0.5, 0.5);
	
	color_frag = Color;

}











