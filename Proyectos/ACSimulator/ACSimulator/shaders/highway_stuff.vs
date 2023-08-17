#version 330 core
layout (location = 0) in vec3 position;


out vec4 colorFF;


uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;                                


void main()
{
	vec3 vehicleCenter_worldspace = position.xyz;
	vec3 vertexPosition_worldspace = vehicleCenter_worldspace ;
	gl_Position =  VP * vec4(vertexPosition_worldspace, 1.0f);

	colorFF = vec4(0.99f, 0.88f, 0.88f, 1.0f);

}














