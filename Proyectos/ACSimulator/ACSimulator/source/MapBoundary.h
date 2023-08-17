#ifndef _MAPBOUNDARY_H
#define _MAPBOUNDARY_H

#include "Simulator.h"
#include "Controls.h"



#define SIM_MAX_BOUNDARY_MAPPER 100

class MapBoundary
{
	public:
		
		Controls* mCtrl;
		FILE*     mFichero;
		Cadena    mFilename;
		GLuint    mType;
		Cadena    mDirINIT;
		Cadena    mDirGral;
		HANDLE    mMutex;

		int      mMAXPOINTS;
		int      mPoints;
		int      mTexture;

		glm::vec3  mVectColor;
		glm::vec3  mVectColorNight;
		glm::vec3  mVectColorDay;

		double*  mArray_buff[SIM_MAX_BOUNDARY_MAPPER];
		int      mArray_point[SIM_MAX_BOUNDARY_MAPPER];
		int      mArray_size[SIM_MAX_BOUNDARY_MAPPER];

		GLuint   mArray_VBO[SIM_MAX_BOUNDARY_MAPPER];
		GLuint   mVAO;

		int    mSQsize;
		double mPosZ;
		double mPosZ_BTT;

		int mSizeTotal;


		bool mb_only_once;
		int  mmax_files;

	public:
		//----------------------------------------
		MapBoundary();
		~MapBoundary();

		void init(Controls* ctrl, MapItem* mp, Cadena caddirinit);
		void need_new_quad();
		void getInfo();
		int getInfo_n_array(int indx, Cadena cadpath);

		void set_color_night();
		void set_color_day();


		void render(glm::mat4& viewprojectionmatrix, bool);

};

#endif

