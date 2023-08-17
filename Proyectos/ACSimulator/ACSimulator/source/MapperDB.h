#ifndef _MAPPERDB_H
#define _MAPPERDB_H

#include "Simulator.h"
#include "Controls.h"


class MapperDB
{
	public:

		int mMAXPOINTS;
		double* mBuffer_in;
		double* mBuffer_out;
		double* mBufferGral;
		MapWaysNodes mMapWayNodes;
		Cadena mTable;
		BaseDatos* mpDB;
		//Cadena mDBName;

		GLuint   mType;


		int      mPoints;
		int      mTexture;

		glm::vec3  mVectColor;

		double*  mArray_buff[SIM_MAX_MAPPER];
		int      mArray_point[SIM_MAX_MAPPER];



		struct QuadDB
		{
			double quadX;
			double quadY;
		};


		GLuint   mArray_VBO[SIM_MAX_MAPPER];
		GLuint   mVAO;


		Controls*  mCtrl;

		map<Cadena, int>    mMapQuads_DB_new;
		map<Cadena, int>    mMapQuads_DB_old;
		QuadDB      mQuadsDB[SIM_MAX_MAPPER];
		Cadena      mQuadsDB_name[SIM_MAX_MAPPER];
		bool        mQuadsDB_new[SIM_MAX_MAPPER];


		double mSQsize;
		double mPosZ;
		double mPosZ_BTT;

		int mQuad_oldX;
		int mQuad_oldY;

		int mSizeTotal;





	public:
		//----------------------------------------
		MapperDB();
		~MapperDB();

		void setup_new_info();
		int generate_lines(int indx, map<long long, vectNode>* map_quad_nodes);
		void get_nodes(int indx, map<long long, vectNode>* map_all_nodes);
		void getInfo_n_db(int indx);

		void init(Controls* ctrl, MapItem* mp, BaseDatos* pDB);
		void need_new_quad();
		void getInfo(int,int);


		void render(glm::mat4& viewprojectionmatrix, bool);





};

#endif

