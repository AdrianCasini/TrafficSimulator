#version 330 core

in vec3 out_color;
in vec2 TexCoords;
flat in int text_index_out;

out vec4 color;

uniform sampler2D text0;
uniform sampler2D text1;
uniform sampler2D text2;
uniform sampler2D text3;
uniform sampler2D text4;
uniform sampler2D text5;
uniform sampler2D text6;
uniform sampler2D text7;
uniform sampler2D text8;
uniform sampler2D text9;
uniform sampler2D text10;
uniform sampler2D text11;
uniform sampler2D text12;
uniform sampler2D text13;
uniform sampler2D text14;
uniform sampler2D text16;
uniform sampler2D text17;
uniform sampler2D text15;
uniform sampler2D text18;
uniform sampler2D text19;
uniform sampler2D text20;
uniform sampler2D text21;
uniform sampler2D text22;
uniform sampler2D text23;
uniform sampler2D text24;
uniform sampler2D text25;
uniform sampler2D text26;
uniform sampler2D text27;
uniform sampler2D text28;
uniform sampler2D text29;
uniform sampler2D text30;
uniform sampler2D text31;




void main()
{
	vec4 sampled ;

	switch (text_index_out)
	{

	    //-------------------- from [ ] to [?]----------------------
		case 32:
			sampled = vec4(1.0, 1.0, 1.0, texture(text0, TexCoords).r);
			break;
		case 33:
			sampled = vec4(1.0, 1.0, 1.0, texture(text1, TexCoords).r);
			break;
		case 34:
			sampled = vec4(1.0, 1.0, 1.0, texture(text2, TexCoords).r);
			break;
		case 35:
			sampled = vec4(1.0, 1.0, 1.0, texture(text3, TexCoords).r);
			break;
		case 36:
			sampled = vec4(1.0, 1.0, 1.0, texture(text4, TexCoords).r);
			break;

		case 37:
			sampled = vec4(1.0, 1.0, 1.0, texture(text5, TexCoords).r);
			break;
		case 38:
			sampled = vec4(1.0, 1.0, 1.0, texture(text6, TexCoords).r);
			break;
		case 39:
			sampled = vec4(1.0, 1.0, 1.0, texture(text7, TexCoords).r);
			break;
		case 40:
			sampled = vec4(1.0, 1.0, 1.0, texture(text8, TexCoords).r);
			break;
		case 41:
			sampled = vec4(1.0, 1.0, 1.0, texture(text9, TexCoords).r);
			break;


		case 42:
			sampled = vec4(1.0, 1.0, 1.0, texture(text10, TexCoords).r);
			break;
		case 43:
			sampled = vec4(1.0, 1.0, 1.0, texture(text11, TexCoords).r);
			break;
		case 44:
			sampled = vec4(1.0, 1.0, 1.0, texture(text12, TexCoords).r);
			break;
		case 45:
			sampled = vec4(1.0, 1.0, 1.0, texture(text13, TexCoords).r);
			break;
		case 46:
			sampled = vec4(1.0, 1.0, 1.0, texture(text14, TexCoords).r);
			break;


		case 47:
			sampled = vec4(1.0, 1.0, 1.0, texture(text15, TexCoords).r);
			break;
		case 48:
			sampled = vec4(1.0, 1.0, 1.0, texture(text16, TexCoords).r);
			break;
		case 49:
			sampled = vec4(1.0, 1.0, 1.0, texture(text17, TexCoords).r);
			break;
		case 50:
			sampled = vec4(1.0, 1.0, 1.0, texture(text18, TexCoords).r);
			break;
		case 51:
			sampled = vec4(1.0, 1.0, 1.0, texture(text19, TexCoords).r);
			break;


		case 52:
			sampled = vec4(1.0, 1.0, 1.0, texture(text20, TexCoords).r);
			break;
		case 53:
			sampled = vec4(1.0, 1.0, 1.0, texture(text21, TexCoords).r);
			break;
		case 54:
			sampled = vec4(1.0, 1.0, 1.0, texture(text22, TexCoords).r);
			break;
		case 55:
			sampled = vec4(1.0, 1.0, 1.0, texture(text23, TexCoords).r);
			break;
		case 56:
			sampled = vec4(1.0, 1.0, 1.0, texture(text24, TexCoords).r);
			break;


		case 57:
			sampled = vec4(1.0, 1.0, 1.0, texture(text25, TexCoords).r);
			break;
		case 58:
			sampled = vec4(1.0, 1.0, 1.0, texture(text26, TexCoords).r);
			break;
		case 59:
			sampled = vec4(1.0, 1.0, 1.0, texture(text27, TexCoords).r);
			break;
		case 60:
			sampled = vec4(1.0, 1.0, 1.0, texture(text28, TexCoords).r);
			break;
		case 61:
			sampled = vec4(1.0, 1.0, 1.0, texture(text29, TexCoords).r);
			break;
		case 62:
			sampled = vec4(1.0, 1.0, 1.0, texture(text30, TexCoords).r);
			break;
		case 63:
			sampled = vec4(1.0, 1.0, 1.0, texture(text31, TexCoords).r);
			break;


		//-------------------- from [@] to [_]----------------------
		case 64:
			sampled = vec4(1.0, 1.0, 1.0, texture(text0, TexCoords).r);
			break;
		case 65:
			sampled = vec4(1.0, 1.0, 1.0, texture(text1, TexCoords).r);
			break;
		case 66:
			sampled = vec4(1.0, 1.0, 1.0, texture(text2, TexCoords).r);
			break;
		case 67:
			sampled = vec4(1.0, 1.0, 1.0, texture(text3, TexCoords).r);
			break;
		case 68:
			sampled = vec4(1.0, 1.0, 1.0, texture(text4, TexCoords).r);
			break;

		case 69:
			sampled = vec4(1.0, 1.0, 1.0, texture(text5, TexCoords).r);
			break;
		case 70:
			sampled = vec4(1.0, 1.0, 1.0, texture(text6, TexCoords).r);
			break;
		case 71:
			sampled = vec4(1.0, 1.0, 1.0, texture(text7, TexCoords).r);
			break;
		case 72:
			sampled = vec4(1.0, 1.0, 1.0, texture(text8, TexCoords).r);
			break;
		case 73:
			sampled = vec4(1.0, 1.0, 1.0, texture(text9, TexCoords).r);
			break;


		case 74:
			sampled = vec4(1.0, 1.0, 1.0, texture(text10, TexCoords).r);
			break;
		case 75:
			sampled = vec4(1.0, 1.0, 1.0, texture(text11, TexCoords).r);
			break;
		case 76:
			sampled = vec4(1.0, 1.0, 1.0, texture(text12, TexCoords).r);
			break;
		case 77:
			sampled = vec4(1.0, 1.0, 1.0, texture(text13, TexCoords).r);
			break;
		case 78:
			sampled = vec4(1.0, 1.0, 1.0, texture(text14, TexCoords).r);
			break;


		case 79:
			sampled = vec4(1.0, 1.0, 1.0, texture(text15, TexCoords).r);
			break;
		case 80:
			sampled = vec4(1.0, 1.0, 1.0, texture(text16, TexCoords).r);
			break;
		case 81:
			sampled = vec4(1.0, 1.0, 1.0, texture(text17, TexCoords).r);
			break;
		case 82:
			sampled = vec4(1.0, 1.0, 1.0, texture(text18, TexCoords).r);
			break;
		case 83:
			sampled = vec4(1.0, 1.0, 1.0, texture(text19, TexCoords).r);
			break;


		case 84:
			sampled = vec4(1.0, 1.0, 1.0, texture(text20, TexCoords).r);
			break;
		case 85:
			sampled = vec4(1.0, 1.0, 1.0, texture(text21, TexCoords).r);
			break;
		case 86:
			sampled = vec4(1.0, 1.0, 1.0, texture(text22, TexCoords).r);
			break;
		case 87:
			sampled = vec4(1.0, 1.0, 1.0, texture(text23, TexCoords).r);
			break;
		case 88:
			sampled = vec4(1.0, 1.0, 1.0, texture(text24, TexCoords).r);
			break;


		case 89:
			sampled = vec4(1.0, 1.0, 1.0, texture(text25, TexCoords).r);
			break;
		case 90:
			sampled = vec4(1.0, 1.0, 1.0, texture(text26, TexCoords).r);
			break;
		case 91:
			sampled = vec4(1.0, 1.0, 1.0, texture(text27, TexCoords).r);
			break;
		case 92:
			sampled = vec4(1.0, 1.0, 1.0, texture(text28, TexCoords).r);
			break;
		case 93:
			sampled = vec4(1.0, 1.0, 1.0, texture(text29, TexCoords).r);
			break;
		case 94:
			sampled = vec4(1.0, 1.0, 1.0, texture(text30, TexCoords).r);
			break;

		case 95:
			sampled = vec4(1.0, 1.0, 1.0, texture(text31, TexCoords).r);
			break;


	    //-------------------- from [a] to [DEL]----------------------
		case 97:
			sampled = vec4(1.0, 1.0, 1.0, texture(text0, TexCoords).r);
			break;
		case 98:
			sampled = vec4(1.0, 1.0, 1.0, texture(text1, TexCoords).r);
			break;
		case 99:
			sampled = vec4(1.0, 1.0, 1.0, texture(text2, TexCoords).r);
			break;
		case 100:
			sampled = vec4(1.0, 1.0, 1.0, texture(text3, TexCoords).r);
			break;
		case 101:
			sampled = vec4(1.0, 1.0, 1.0, texture(text4, TexCoords).r);
			break;


		case 102:
			sampled = vec4(1.0, 1.0, 1.0, texture(text5, TexCoords).r);
			break;
		case 103:
			sampled = vec4(1.0, 1.0, 1.0, texture(text6, TexCoords).r);
			break;
		case 104:
			sampled = vec4(1.0, 1.0, 1.0, texture(text7, TexCoords).r);
			break;
		case 105:
			sampled = vec4(1.0, 1.0, 1.0, texture(text8, TexCoords).r);
			break;
		case 106:
			sampled = vec4(1.0, 1.0, 1.0, texture(text9, TexCoords).r);
			break;


		case 107:
			sampled = vec4(1.0, 1.0, 1.0, texture(text10, TexCoords).r);
			break;
		case 108:
			sampled = vec4(1.0, 1.0, 1.0, texture(text11, TexCoords).r);
			break;
		case 109:
			sampled = vec4(1.0, 1.0, 1.0, texture(text12, TexCoords).r);
			break;
		case 110:
			sampled = vec4(1.0, 1.0, 1.0, texture(text13, TexCoords).r);
			break;
		case 111:
			sampled = vec4(1.0, 1.0, 1.0, texture(text14, TexCoords).r);
			break;


		case 112:
			sampled = vec4(1.0, 1.0, 1.0, texture(text15, TexCoords).r);
			break;
		case 113:
			sampled = vec4(1.0, 1.0, 1.0, texture(text16, TexCoords).r);
			break;
		case 114:
			sampled = vec4(1.0, 1.0, 1.0, texture(text17, TexCoords).r);
			break;
		case 115:
			sampled = vec4(1.0, 1.0, 1.0, texture(text18, TexCoords).r);
			break;
		case 116:
			sampled = vec4(1.0, 1.0, 1.0, texture(text19, TexCoords).r);
			break;


		case 117:
			sampled = vec4(1.0, 1.0, 1.0, texture(text20, TexCoords).r);
			break;
		case 118:
			sampled = vec4(1.0, 1.0, 1.0, texture(text21, TexCoords).r);
			break;
		case 119:
			sampled = vec4(1.0, 1.0, 1.0, texture(text22, TexCoords).r);
			break;
		case 120:
			sampled = vec4(1.0, 1.0, 1.0, texture(text23, TexCoords).r);
			break;
		case 121:
			sampled = vec4(1.0, 1.0, 1.0, texture(text24, TexCoords).r);
			break;


		case 122:
			sampled = vec4(1.0, 1.0, 1.0, texture(text25, TexCoords).r);
			break;
		case 123:
			sampled = vec4(1.0, 1.0, 1.0, texture(text26, TexCoords).r);
			break;
		case 124:
			sampled = vec4(1.0, 1.0, 1.0, texture(text27, TexCoords).r);
			break;
		case 125:
			sampled = vec4(1.0, 1.0, 1.0, texture(text28, TexCoords).r);
			break;
		case 126:
			sampled = vec4(1.0, 1.0, 1.0, texture(text29, TexCoords).r);
			break;
		case 127:
			sampled = vec4(1.0, 1.0, 1.0, texture(text30, TexCoords).r);
			break;


	    //-------------------- from [128] to [160]----------------------
		case 128:
			sampled = vec4(1.0, 1.0, 1.0, texture(text0, TexCoords).r);
			break;
		case 129:
			sampled = vec4(1.0, 1.0, 1.0, texture(text1, TexCoords).r);
			break;
		case 130:
			sampled = vec4(1.0, 1.0, 1.0, texture(text2, TexCoords).r);
			break;
		case 131:
			sampled = vec4(1.0, 1.0, 1.0, texture(text3, TexCoords).r);
			break;
		case 132:
			sampled = vec4(1.0, 1.0, 1.0, texture(text4, TexCoords).r);
			break;


		case 133:
			sampled = vec4(1.0, 1.0, 1.0, texture(text5, TexCoords).r);
			break;
		case 134:
			sampled = vec4(1.0, 1.0, 1.0, texture(text6, TexCoords).r);
			break;
		case 135:
			sampled = vec4(1.0, 1.0, 1.0, texture(text7, TexCoords).r);
			break;
		case 136:
			sampled = vec4(1.0, 1.0, 1.0, texture(text8, TexCoords).r);
			break;
		case 137:
			sampled = vec4(1.0, 1.0, 1.0, texture(text9, TexCoords).r);
			break;


		case 138:
			sampled = vec4(1.0, 1.0, 1.0, texture(text10, TexCoords).r);
			break;
		case 139:
			sampled = vec4(1.0, 1.0, 1.0, texture(text11, TexCoords).r);
			break;
		case 140:
			sampled = vec4(1.0, 1.0, 1.0, texture(text12, TexCoords).r);
			break;
		case 141:
			sampled = vec4(1.0, 1.0, 1.0, texture(text13, TexCoords).r);
			break;
		case 142:
			sampled = vec4(1.0, 1.0, 1.0, texture(text14, TexCoords).r);
			break;


		case 143:
			sampled = vec4(1.0, 1.0, 1.0, texture(text15, TexCoords).r);
			break;
		case 144:
			sampled = vec4(1.0, 1.0, 1.0, texture(text16, TexCoords).r);
			break;
		case 145:
			sampled = vec4(1.0, 1.0, 1.0, texture(text17, TexCoords).r);
			break;
		case 146:
			sampled = vec4(1.0, 1.0, 1.0, texture(text18, TexCoords).r);
			break;
		case 147:
			sampled = vec4(1.0, 1.0, 1.0, texture(text19, TexCoords).r);
			break;


		case 148:
			sampled = vec4(1.0, 1.0, 1.0, texture(text20, TexCoords).r);
			break;
		case 149:
			sampled = vec4(1.0, 1.0, 1.0, texture(text21, TexCoords).r);
			break;
		case 150:
			sampled = vec4(1.0, 1.0, 1.0, texture(text22, TexCoords).r);
			break;
		case 151:
			sampled = vec4(1.0, 1.0, 1.0, texture(text23, TexCoords).r);
			break;
		case 152:
			sampled = vec4(1.0, 1.0, 1.0, texture(text24, TexCoords).r);
			break;


		case 153:
			sampled = vec4(1.0, 1.0, 1.0, texture(text25, TexCoords).r);
			break;
		case 154:
			sampled = vec4(1.0, 1.0, 1.0, texture(text26, TexCoords).r);
			break;
		case 155:
			sampled = vec4(1.0, 1.0, 1.0, texture(text27, TexCoords).r);
			break;
		case 156:
			sampled = vec4(1.0, 1.0, 1.0, texture(text28, TexCoords).r);
			break;
		case 157:
			sampled = vec4(1.0, 1.0, 1.0, texture(text29, TexCoords).r);
			break;
		case 158:
			sampled = vec4(1.0, 1.0, 1.0, texture(text30, TexCoords).r);
			break;
		case 159:
			sampled = vec4(1.0, 1.0, 1.0, texture(text31, TexCoords).r);
			break;


	    //-------------------- from [190] to [222]----------------------
		case 190:
			sampled = vec4(1.0, 1.0, 1.0, texture(text0, TexCoords).r);
			break;
		case 191:
			sampled = vec4(1.0, 1.0, 1.0, texture(text1, TexCoords).r);
			break;
		case 192:
			sampled = vec4(1.0, 1.0, 1.0, texture(text2, TexCoords).r);
			break;
		case 193:
			sampled = vec4(1.0, 1.0, 1.0, texture(text3, TexCoords).r);
			break;
		case 194:
			sampled = vec4(1.0, 1.0, 1.0, texture(text4, TexCoords).r);
			break;


		case 195:
			sampled = vec4(1.0, 1.0, 1.0, texture(text5, TexCoords).r);
			break;
		case 196:
			sampled = vec4(1.0, 1.0, 1.0, texture(text6, TexCoords).r);
			break;
		case 197:
			sampled = vec4(1.0, 1.0, 1.0, texture(text7, TexCoords).r);
			break;
		case 198:
			sampled = vec4(1.0, 1.0, 1.0, texture(text8, TexCoords).r);
			break;
		case 199:
			sampled = vec4(1.0, 1.0, 1.0, texture(text9, TexCoords).r);
			break;


		case 200:
			sampled = vec4(1.0, 1.0, 1.0, texture(text10, TexCoords).r);
			break;
		case 201:
			sampled = vec4(1.0, 1.0, 1.0, texture(text11, TexCoords).r);
			break;
		case 202:
			sampled = vec4(1.0, 1.0, 1.0, texture(text12, TexCoords).r);
			break;
		case 203:
			sampled = vec4(1.0, 1.0, 1.0, texture(text13, TexCoords).r);
			break;
		case 204:
			sampled = vec4(1.0, 1.0, 1.0, texture(text14, TexCoords).r);
			break;


		case 205:
			sampled = vec4(1.0, 1.0, 1.0, texture(text15, TexCoords).r);
			break;
		case 206:
			sampled = vec4(1.0, 1.0, 1.0, texture(text16, TexCoords).r);
			break;
		case 207:
			sampled = vec4(1.0, 1.0, 1.0, texture(text17, TexCoords).r);
			break;
		case 208:
			sampled = vec4(1.0, 1.0, 1.0, texture(text18, TexCoords).r);
			break;
		case 209:
			sampled = vec4(1.0, 1.0, 1.0, texture(text19, TexCoords).r);
			break;


		case 210:
			sampled = vec4(1.0, 1.0, 1.0, texture(text20, TexCoords).r);
			break;
		case 211:
			sampled = vec4(1.0, 1.0, 1.0, texture(text21, TexCoords).r);
			break;
		case 212:
			sampled = vec4(1.0, 1.0, 1.0, texture(text22, TexCoords).r);
			break;
		case 213:
			sampled = vec4(1.0, 1.0, 1.0, texture(text23, TexCoords).r);
			break;
		case 214:
			sampled = vec4(1.0, 1.0, 1.0, texture(text24, TexCoords).r);
			break;


		case 215:
			sampled = vec4(1.0, 1.0, 1.0, texture(text25, TexCoords).r);
			break;
		case 216:
			sampled = vec4(1.0, 1.0, 1.0, texture(text26, TexCoords).r);
			break;
		case 217:
			sampled = vec4(1.0, 1.0, 1.0, texture(text27, TexCoords).r);
			break;
		case 218:
			sampled = vec4(1.0, 1.0, 1.0, texture(text28, TexCoords).r);
			break;
		case 219:
			sampled = vec4(1.0, 1.0, 1.0, texture(text29, TexCoords).r);
			break;
		case 220:
			sampled = vec4(1.0, 1.0, 1.0, texture(text30, TexCoords).r);
			break;
		case 221:
			sampled = vec4(1.0, 1.0, 1.0, texture(text31, TexCoords).r);
			break;


	    //-------------------- from [X] to [X]----------------------
		case 223:
			sampled = vec4(1.0, 1.0, 1.0, texture(text0, TexCoords).r);
			break;
		case 224:
			sampled = vec4(1.0, 1.0, 1.0, texture(text1, TexCoords).r);
			break;
		case 225:
			sampled = vec4(1.0, 1.0, 1.0, texture(text2, TexCoords).r);
			break;
		case 226:
			sampled = vec4(1.0, 1.0, 1.0, texture(text3, TexCoords).r);
			break;
		case 227:
			sampled = vec4(1.0, 1.0, 1.0, texture(text4, TexCoords).r);
			break;


		case 228:
			sampled = vec4(1.0, 1.0, 1.0, texture(text5, TexCoords).r);
			break;
		case 229:
			sampled = vec4(1.0, 1.0, 1.0, texture(text6, TexCoords).r);
			break;
		case 230:
			sampled = vec4(1.0, 1.0, 1.0, texture(text7, TexCoords).r);
			break;
		case 231:
			sampled = vec4(1.0, 1.0, 1.0, texture(text8, TexCoords).r);
			break;
		case 232:
			sampled = vec4(1.0, 1.0, 1.0, texture(text9, TexCoords).r);
			break;


		case 233:
			sampled = vec4(1.0, 1.0, 1.0, texture(text10, TexCoords).r);
			break;
		case 234:
			sampled = vec4(1.0, 1.0, 1.0, texture(text11, TexCoords).r);
			break;
		case 235:
			sampled = vec4(1.0, 1.0, 1.0, texture(text12, TexCoords).r);
			break;
		case 236:
			sampled = vec4(1.0, 1.0, 1.0, texture(text13, TexCoords).r);
			break;
		case 237:
			sampled = vec4(1.0, 1.0, 1.0, texture(text14, TexCoords).r);
			break;


		case 238:
			sampled = vec4(1.0, 1.0, 1.0, texture(text15, TexCoords).r);
			break;
		case 239:
			sampled = vec4(1.0, 1.0, 1.0, texture(text16, TexCoords).r);
			break;
		case 240:
			sampled = vec4(1.0, 1.0, 1.0, texture(text17, TexCoords).r);
			break;
		case 241:
			sampled = vec4(1.0, 1.0, 1.0, texture(text18, TexCoords).r);
			break;
		case 242:
			sampled = vec4(1.0, 1.0, 1.0, texture(text19, TexCoords).r);
			break;


		case 243:
			sampled = vec4(1.0, 1.0, 1.0, texture(text20, TexCoords).r);
			break;
		case 244:
			sampled = vec4(1.0, 1.0, 1.0, texture(text21, TexCoords).r);
			break;
		case 245:
			sampled = vec4(1.0, 1.0, 1.0, texture(text22, TexCoords).r);
			break;
		case 246:
			sampled = vec4(1.0, 1.0, 1.0, texture(text23, TexCoords).r);
			break;
		case 247:
			sampled = vec4(1.0, 1.0, 1.0, texture(text24, TexCoords).r);
			break;


		case 248:
			sampled = vec4(1.0, 1.0, 1.0, texture(text25, TexCoords).r);
			break;
		case 249:
			sampled = vec4(1.0, 1.0, 1.0, texture(text26, TexCoords).r);
			break;
		case 250:
			sampled = vec4(1.0, 1.0, 1.0, texture(text27, TexCoords).r);
			break;
		case 251:
			sampled = vec4(1.0, 1.0, 1.0, texture(text28, TexCoords).r);
			break;
		case 252:
			sampled = vec4(1.0, 1.0, 1.0, texture(text29, TexCoords).r);
			break;
		case 253:
			sampled = vec4(1.0, 1.0, 1.0, texture(text30, TexCoords).r);
			break;
		case 254:
			sampled = vec4(1.0, 1.0, 1.0, texture(text31, TexCoords).r);
			break;


		default:
			break;
	
	}

   color = vec4(out_color, 1.0) * sampled;
}