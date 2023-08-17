#version 330 core
layout (location = 0) in vec3 position;

out vec2 UV;

uniform float btt_pw_x;
uniform float btt_pw_y;
uniform float btt_dx;
uniform float btt_dy;

void main()
{

	gl_Position = vec4(position.x * btt_pw_x + btt_dx, position.y * btt_pw_y + btt_dy, position.z, 1.0f);
	UV = position.xy + vec2(0.5, 0.5);
}











