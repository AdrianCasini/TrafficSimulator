#ifndef _MAPPER_H
#define _MAPPER_H

#include "Simulator.h"
#include "Controls.h"

class Mapper
{
	public:
		
		FILE  *  mFichero;
		Cadena   mFilename;
		GLuint   mType;

		int      mMAXPOINTS;
		int      mPoints;
		int      mTexture;

		glm::vec3  mVectColor;

		double*  mArray_buff[SIM_MAX_MAPPER];
		int      mArray_point[SIM_MAX_MAPPER];
		int      mArray_size[SIM_MAX_MAPPER];

		float*   mDRAW_BUFF;
		double*  mPtrOld;

		GLuint   mArray_VBO[SIM_MAX_MAPPER];
		GLuint   mVAO;

		Controls *mCtrl;

		map<Cadena, int>    mMapQuads_file_new;
		map<Cadena, int>    mMapQuads_file_old;
		Cadena      mQuads[SIM_MAX_MAPPER];


		int mSQsize;
		double mPosZ;
		double mPosZ_BTT;

		int mQuad_oldX;
		int mQuad_oldY;

		int mSizeTotal;

		Cadena mDirINIT;
		Cadena mDirGral;

		HANDLE mMutex;

		int mMAX_POINTS;

	public:
		//----------------------------------------
		Mapper();
		~Mapper();

		void init(Controls* ctrl, MapItem* mp, Cadena caddirinit);
		void need_new_quad();
		void getInfo(int,int);
		void getInfo_n_array(int indx, Cadena cadpath);

		void render(glm::mat4& viewprojectionmatrix, bool);

};

#endif

