#ifndef _WAYRENDER_H
#define _WAYRENDER_H

#include "Simulator.h"
#include "Controls.h"


class WayRender
{
	public:

		int mMAXPOINTS;
		int mMAXWAYPoints;
		double* mBuffer_in;
		double* mBuffer_out;
		double* mBufferGral;
		MapWaysNodes mMapWayNodes;
		Cadena mTable;
		BaseDatos* mpDB;
		HANDLE mMutex;
		GLuint mType;
		Cadena mDrawType;

		bool m3D;

		int      mPoints;
		int      mTexture;

		glm::vec3  mVectColor;

		double*  mArray_buff[SIM_MAX_MAPPER_DB];
		int      mArray_point[SIM_MAX_MAPPER_DB];

		struct QuadDB
		{
			double quadX;
			double quadY;
		};


		GLuint   mArray_VBO[SIM_MAX_MAPPER_DB];
		GLuint   mVAO;
		Controls*  mCtrl;

		map<Cadena, int>    mMapQuads_DB_new;
		map<Cadena, int>    mMapQuads_DB_old;
		QuadDB      mQuadsDB[SIM_MAX_MAPPER_DB];
		Cadena      mQuadsDB_name[SIM_MAX_MAPPER_DB];
		bool        mQuadsDB_new[SIM_MAX_MAPPER_DB];


		double mHeight;
		double mWidth;
		double mSQsize;
		double mPosZ;
		double mPosZ_BTT;

		int mQuad_oldX;
		int mQuad_oldY;

		int mSizeTotal;

	public:
		//----------------------------------------
		WayRender();
		~WayRender();

		void setup_new_info();
		int generate_lines(int indx, map<long long, vectNode>* map_quad_nodes);
		int generate_surface(int indx, map<long long, vectNode>* map_quad_nodes);
		void get_nodes(int indx, map<long long, vectNode>* map_all_nodes);
		void getInfo_n_db(int indx);

		void init(Controls* ctrl, MapItem* mp, BaseDatos* pDB);
		void need_new_quad();
		void getInfo(int,int);

		void render(glm::mat4& viewprojectionmatrix, bool);
		void refresh();

};

#endif

