#ifndef _MENULEFT_H
#define _MENULEFT_H


#include "Simulator.h"

class MenuLeft
{
	public:

		enum eMenuLeft
		{
			SIM_BTT_ML_CREATE_ROUTES,
			SIM_BTT_ML_POINTOFINTEREST,
			SIM_BTT_ML_CPICKER,
			SIM_BTT_ML_TEXT,
			SIM_BTT_ML_NIGHT,
			SIM_MAX_BUTTON_ML
		};

		float mMENU_H;
		float mMENU_BTT_W;
		float mMENU_BTT_H;
		float mMENU_BTT_S;
		float mMENU_BTT_DX;
		float mMENU_BTT_DY;

		float mMENUpw_x;
		float mMENUpw_y;
		float mMENUw;
		float mMENUh;
		float mMENUs;
		float mMENUmv;
		float mMENUdx;
		float mMENUdy;

		int mBtt_pw_x;
		int mBtt_pw_y;
		int mBtt_w;
		int mBtt_h;
		int mBtt_s;
		int mBtt_mv;
		int mBtt_dx;
		int mBtt_dy;

		Button mButtons[SIM_MAX_BUTTON_ML];
		Menu_Bar mBar;
		Shader mShader;
		GLuint mVAO;
		GLuint mBtt_number;
		GLuint mTxBtt;
		int    mBttNumber;

		double mVertices[12];
		double mModel_x;
		double mModel_y;
		double mModel_z;

		float mMENU_Y_POS;
		float mMENU_X_POS;

		float mMENUpw_x_bar;
		float mMENUpw_y_bar;
		float mMENUw_bar;
		float mMENUh_bar;
		float mMENUs_bar;
		float mMENUmv_bar;
		float mMENUdx_bar;
		float mMENUdy_bar;

		Cadena mcadgral;

	public:

		void init(int screenWidth, int screenHeight, Cadena cadgral);
		void render();
		void refresh(int screenWidth, int screenHeight);
		void reset();
		void not_over();
		int get_button(double w, double h, double sep, double dx, double dy, double xpos, double ypos);

		MenuLeft();
		~MenuLeft();

};

#endif
