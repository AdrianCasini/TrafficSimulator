#version 330 core


in vec2 textcoord;
in vec4 colorF;
flat in int txt_index_out;

out vec4 colorG;

uniform sampler2D Texture_car;
uniform sampler2D Texture_truck;
uniform sampler2D Texture_motorcycle;

void main()
{

	switch (txt_index_out)
	{

		case 0:
			colorG = texture( Texture_motorcycle, textcoord ) * colorF;
			break;

		case 1:
			colorG = texture( Texture_car, textcoord ) * colorF;
			break;

		case 2:
			colorG = texture( Texture_truck, textcoord ) * colorF;
			break;



		default:
			colorG = colorF; 
			break;
	}
}

