#version 330 core

in vec2 TexCoord;
out vec4 colorOutput;

uniform sampler2D TxHW_lanes;

void main()
{
    colorOutput = texture( TxHW_lanes,  TexCoord);
}
