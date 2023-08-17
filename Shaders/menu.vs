#version 330 core
layout (location = 0) in vec3 position;

out vec2 UV;
uniform float btt_number;
uniform float btt_pw_x;
uniform float btt_pw_y;
uniform float btt_w;
uniform float btt_h;
uniform float btt_s;
uniform float btt_mv;
uniform float btt_dx;
uniform float btt_dy;

void main()
{
    gl_Position =  vec4(position.x * btt_pw_x, position.y * btt_pw_y, position.z, 1.0f)  + vec4 (-1.0 + btt_w/2 + (btt_w + btt_s)* btt_number +btt_dx , 1.0 - btt_h/2-btt_dy-btt_mv, 0.0f, 0.0f);
	UV = position.xy + vec2(0.5, 0.5);
}











