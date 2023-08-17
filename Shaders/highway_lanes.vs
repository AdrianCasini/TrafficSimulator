#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;


out vec2 TexCoord;



uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;                                


void main()
{
	vec3 vehicleCenter_worldspace = position.xyz;
	vec3 vertexPosition_worldspace = vehicleCenter_worldspace;
	gl_Position =  VP * vec4(vertexPosition_worldspace, 1.0f);

	TexCoord =  texCoord + vec2(0.5, 0.5);

}














