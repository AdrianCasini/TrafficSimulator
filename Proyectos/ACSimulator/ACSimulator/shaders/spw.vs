#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 colorF;


uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;                                


void main()
{
	vec3 vehicleCenter_worldspace = position.xyz;
	
	vec3 vertexPosition_worldspace = vehicleCenter_worldspace + CameraRight_worldspace * position.x  + CameraUp_worldspace * position.y ;

	gl_Position =  VP * vec4(vertexPosition_worldspace, 1.0f);


	colorF = vec4(0.25f, 0.25f, 0.25f, 1.0f);

}














