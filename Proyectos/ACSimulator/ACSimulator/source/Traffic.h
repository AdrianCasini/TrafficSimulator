#ifndef _TRAFFIC_H
#define _TRAFFIC_H

#include "Simulator.h"
#include "Tools.h"
#include "Menu.h"
#include "Test.h"
#include "PofI.h"
#include "Model.h"
#include "Overlapper.h"
#include "Logic.h"


#define SIM_TIME_IN_VEH  60000
#define SIM_VEHICLE_SIZE 0.0023
#define SIM_DIST_PW      0.003
#define SIM_CHANGELANE_TIME 60

#define SIM_TR_STOP_TIMEOUT  100  //mseconds
#define SIM_TR_PAUSE_TIMEOUT 100  //mseconds

#define SIM_LANE_CNT_PASS 5
#define SIM_TR_LANE_NEW 100

class Traffic
{
	public:

		enum sim_tr_method_play
		{
			SIM_TR_METHOD_PLAY_FROMSAVEDFILE,
			SIM_TR_METHOD_PLAY_FROM_SOURCETRIP
		};

		enum sim_tr_state 
		{ 
			SIM_TR_STATE_PLAY, 
			SIM_TR_STATE_PAUSE,
			SIM_TR_STATE_STOP 
		};

		enum sim_tr_type_alg 
		{ 
			SIM_TR_MOVE_SIMPLE, 
			SIM_TR_MOVE_ALG 
		};

		enum sim_tr_render_3d 
		{ 
			SIM_TR_RENDER_SIMPLE, 
			SIM_TR_RENDER_3D,
			SIM_TR_RENDER_3D_MAX
		};

		enum sim_trvh_alg
		{
			SIM_TR_ALG_NOT_ALIVE,
			SIM_TR_ALG_NORMAL,
			SIM_TR_ALG_CHANGING_NODE,
			SIM_TR_ALG_OVERLAPING,
			SIM_TR_ALG_STATE_NEW_LANE,
			SIM_TR_ALG_EXIT,
			SIM_TR_ALG_CHANGING_LANE_PASS,
			SIM_TR_ALG_CH_LANE_MANDATORY,
			SIM_TR_ALG_CH_LANE,
			SIM_TR_ALG_LANE_RETURN,
			SIM_TR_ALG_CHANGING_LANE_DISTURBING,
		};


		//---Simulator Clases---
		TestHandle mTH;
		Overlapper mOV;
		Logic*     mpLG;
		Test*      mpTST;
		Menu*      mpMN;
		Tools*     mpTLS;
		PofI*      mpPOI;
		
		//---Render Variables----
		Shader mShader;
		GLuint mTextureID_car;
		GLuint mTextureImg_car;

		GLuint mTextureID_truck;
		GLuint mTextureImg_truck;

		GLuint mTextureID_motorcycle;
		GLuint mTextureImg_motorcycle;

		GLuint mVP;
		GLuint mPW;
		GLuint mTxIndex;
		double mVHSize;

		GLuint mVAO;
		GLuint mVBO_Model;
		GLuint mVBO_Model_Tri;
		GLuint mVBO_Color;
		GLuint mVBO_Scale;
		GLuint mVBO_Tranformation;

		//---Gral Core Array Variavles---
		GLdouble*  mVhMove;
		GLdouble*  mVhScale;
		GLdouble*  mVhAngle;
		GLubyte*   mVhColor;
		glm::mat4* mModelMatrices;


		//--gral stuff---
		bool   mDebug;
		GLuint mMODEL_3D;
		Model  mOurModel;
		point3 mMODEL_3DP[SIM_MAX_3D_MODEL_POINTS];
		int    mModelCount;


		int mTurbo;
		bool mNightColor;

		Cadena mcadgral;

	public:

		Traffic();
		~Traffic();

		void init(int fecha, Tools* tls, Menu* mn, PofI* poi, BaseDatos* db, Cadena cadgral);
		void reset();

		void get_info_traffic();
		void play_simulation(Cadena cadfile, enum sim_tr_method_play emethod);
		void vehicle_out(Vehiculo* pVH);
		void finish_move(int& index, Vehiculo* pVH);
		void clear();
		void clear_polygon();
		void turbo();

		//----Movement----------------------
		void do_move();

		//----------------------------------
		void pause();
		void alg();
		void render3d();
		void render3d_max();
		void send_cloud(bool flag);
		void send_file(bool flag);

		//----------------------------------
		void setup_matrix(int index, Vehiculo* pVH);
		void setup_tools(int index, Vehiculo* pVH);
		void send_info(bool active);
		void save_test(Cadena cadfile);
		void get_speed(Vehiculo* pVH);

		//------Render------------------------
		void setup_render();
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_mv(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void set_vh_size(double size);

		void set_night();
		void set_day();
};

#endif

