#ifndef _MAPMNG_H
#define _MAPMNG_H


#include "Simulator.h"
#include "Controls.h"
#include "Mapper.h"
#include "MapperDB.h"
#include "MapBoundary.h"

class MapMng
{
	public:
		Mapper* MPP_L[50];
		Mapper* MPP_S[50];

		MapperDB* MPPDB_L[50];
		MapperDB* MPPDB_S[50];

		MapBoundary* MPPBDRY;


		int mCountMDB_L;
		int mCountMDB_S;

		int mCountM_L;
		int mCountM_S;

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


	public:

		MapMng();
		~MapMng();
		void init(Controls* ctrl, BaseDatos* pDB, Cadena cadgral);
		void get_file_mappers();
		void render(glm::mat4& viewprojectionmatrix, bool);
		void clearcolor();
		void set_color_day();
		void set_color_night();

};

#endif

