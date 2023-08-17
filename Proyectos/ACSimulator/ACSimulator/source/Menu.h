#ifndef _MENU_H
#define _MENU_H


#include "Simulator.h"

class Menu
{
	public:

		
		enum eMenuTop
		{
			SIM_BTT_PLAY,
			SIM_BTT_PAUSE,
			SIM_BTT_ALGORITHM,
			SIM_BTT_DUM_1,
			SIM_BTT_SAVE_TEST,
			SIM_BTT_SET_TEST,
			SIM_BTT_CLEAN,
			SIM_BTT_DUM_2,
			SIM_BTT_SEND_INFO,
			SIM_BTT_TOOLS,
			SIM_BTT_DUM_3,
			SIM_BTT_ROUTE,
			SIM_BTT_SNAP,
			SIM_BTT_SNAPFILE,
			SIM_BTT_TRACE,
			SIM_BTT_DUM_4,
			SIM_BTT_3D,
			SIM_BTT_3D_MAX,
			SIM_BTT_TURBO,
			SIM_BTT_FIND,
			SIM_BTT_SECMNGR,
			SIM_BTT_INRIX_SPEED_BUCKET,
			SIM_BTT_INRIX_SPEED,
			SIM_MAX_BUTTON,
			SIM_NO_BUTTON
		};

		double mMENU_H;
		double mMENU_BTT_W;
		double mMENU_BTT_H;
		double mMENU_BTT_S;
		double mMENU_BTT_DX;
		double mMENU_BTT_DY;

		double mMENUpw_x;
		double mMENUpw_y;
		double mMENUw;
		double mMENUh;
		double mMENUs;
		double mMENUmv;
		double mMENUdx;
		double mMENUdy;

		double mBtt_pw_x;
		double mBtt_pw_y;
		double mBtt_w;
		double mBtt_h;
		double mBtt_s;
		double mBtt_mv;
		double mBtt_dx;
		double mBtt_dy;

		map<enum eMenuTop, Cadena> mMapButton;
		map<enum eMenuTop, Cadena> mMapButton_tt;
		Button mButtons[SIM_MAX_BUTTON];
		Menu_Bar mBar;
		Shader mShader;
		GLuint mVAO;
		GLuint mBtt_number;
		GLuint mTxBtt;
		int    mBttMax;

		double mVertices[12];
		double mModel_x;
		double mModel_y;
		double mModel_z;

		double mMENUpw_x_bar;
		double mMENUpw_y_bar;
		double mMENUw_bar;
		double mMENUh_bar;
		double mMENUs_bar;
		double mMENUmv_bar;
		double mMENUdx_bar;
		double mMENUdy_bar;

		Cadena mcadgral;


	public:

		int get_button(double w, double h, double sep, double dx, double dy, double xpos, double ypos);
		void init(int screenWidth, int screenHeight, Cadena cadgral);
		void render();
		void refresh(int screenWidth, int screenHeight);
		void reset();
		void not_over();
		

		Menu();
		~Menu();

};

#endif
