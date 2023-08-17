#version 330 core
layout (location = 0) in vec3 model;
layout (location = 1) in vec3 model_tri;
layout (location = 2) in vec4 color;
layout (location = 3) in float scale;
layout (location = 4) in mat4 instanceMatrix;

out vec2 textcoord;
out vec4 colorF;
flat out int txt_index_out;


uniform mat4  VP;     
uniform float PW;
uniform bool MODEL_3D;

void main()
{
	if (scale < 1)
	{
		txt_index_out = 0;
	}
	else if ( scale > 1)
	{
		txt_index_out = 2;
	}
	else 
	{
		txt_index_out = 1;
	}

	if (MODEL_3D == false)
	{
		gl_Position =  VP * instanceMatrix * vec4(model * PW * scale, 1.0f);
		textcoord = vec2(model.x*0.5, model.y)  + vec2(0.5, 0.5);
	}
	else
	{
		gl_Position =  VP * instanceMatrix * vec4(model_tri * PW * scale, 1.0f);
		textcoord = vec2(model_tri.x*0.5, model_tri.y)  + vec2(0.5, 0.5);
		txt_index_out=10;
	}

	colorF = color;

}











