#ifndef _WAYRENDERMNG_H
#define _WAYRENDERMNG_H


#include "Simulator.h"
#include "Controls.h"
#include "WayRender.h"


class WayRenderMng
{
	public:


		WayRender* WR_L[50];
		WayRender* WR_S[50];

		int mCount_L;
		int mCount_S;

		GLuint mVP_L; 
		GLuint mColor_L; 

		GLuint mVP_S;
		GLuint mColor_S;

		GLuint mTextureID_forest;
		GLuint mTextureID_building;
		GLuint mTextureID_water;
		GLuint mTextureID_index;

		GLuint mTextureImg_forest;
		GLuint mTextureImg_building;
		GLuint mTextureImg_water;

		Shader mShader_L;
		Shader mShader_S;

		map<int, MapItem> mMapItem;

		double mSize;

		Cadena mcadgral;

		bool m3Dactive;
		glm::mat4* mVP;


	public:

		WayRenderMng();
		~WayRenderMng();
		void init(Controls* ctrl, BaseDatos* pDB, Cadena cadgral);
		void get_file_mappers();
		void get_info();
		void render(glm::mat4& viewprojectionmatrix, bool);
		void clearcolor();
		void button_pressed_render3d();

};

#endif

