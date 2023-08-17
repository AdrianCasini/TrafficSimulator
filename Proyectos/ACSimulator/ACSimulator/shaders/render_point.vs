#version 330 core
layout (location = 0) in vec3 model;
layout (location = 1) in vec3 offset;

uniform vec3  camera_r;
uniform vec3  camera_u;
uniform mat4  vp;   
uniform float point_size;
uniform vec4  color_in;

out vec4 color_frag;
out vec2 uv;

void main()
{


	vec3 vertexPosition_worldspace = vec3(offset.x , offset.y , offset.z) + camera_r * model.x * point_size + camera_u * model.y * point_size ;

	gl_Position =  vp * vec4(vertexPosition_worldspace, 1.0f);

	uv = model.xy + vec2(0.5, 0.5);
	
	color_frag = color_in;

}











