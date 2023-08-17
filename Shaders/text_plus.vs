#version 330 core
layout (location = 0) in vec3  model;
layout (location = 1) in float scale_w;
layout (location = 2) in float scale_h;
layout (location = 3) in int   text_index;
layout (location = 4) in vec3  color;
layout (location = 5) in mat4  instanceMatrix;

out vec2 TexCoords;
uniform mat4  VP;     
flat out int text_index_out;
out vec3 out_color;

void main()
{
	out_color = color;
	text_index_out = text_index;
	gl_Position =  VP * instanceMatrix * vec4(model.x * scale_w, model.y * scale_h, model.z,  1.0f);
	TexCoords = vec2(-model.x, model.y)  + vec2(0.5, 0.5);
}



