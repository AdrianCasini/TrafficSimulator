#ifndef _MENURIGHT_H
#define _MENURIGHT_H


#include "Simulator.h"

class MenuRight
{
	public:

		enum eMenuRight
		{
			SIM_BTT_UP_R,
			SIM_BTT_DOWN_R,
			SIM_BTT_RIGHT_R,
			SIM_BTT_LEFT_R,
			SIM_BTT_ROTATE_R,

			SIM_BTT_UP_MV,
			SIM_BTT_DOWN_MV,
			SIM_BTT_RIGHT_MV,
			SIM_BTT_LEFT_MV,
			SIM_BTT_MOVE_MV,

			SIM_BTT_ZOOM_IN,
			SIM_BTT_ZOOM_OUT,

			SIM_MAX_BUTTON_MR
		};




		double mMENU_ROT_X;
		double mMENU_ROT_Y;
		double mMENU_ROT_DX;
		double mMENU_ROT_DY;

		double mMENU_MV_X;
		double mMENU_MV_Y;
		double mMENU_MV_DX;
		double mMENU_MV_DY;


		double mMENU_ZOOM_IN_X;
		double mMENU_ZOOM_IN_Y;
		double mMENU_ZOOM_OUT_X;
		double mMENU_ZOOM_OUT_Y;



		double mMENU_X_POS;


		double mMENUpw_x;
		double mMENUpw_y;


		double mBtt_pw_x;
		double mBtt_pw_y;
		double mBtt_dx;
		double mBtt_dy;

		Button mButtons[SIM_MAX_BUTTON_MR];
		Shader mShader;
		GLuint mVAO;
		GLuint mTxBtt;

		Cadena mcadgral;


	public:

		void init(int screenWidth, int screenHeight, Cadena cadgral);
		void refresh(int screenWidth, int screenHeight);
		void render();
		int get_button(Button* ptrB, double xpos, double ypos);


		void position_btt_rotate(int screenWidth, int screenHeight);
		void position_btt_move(int screenWidth, int screenHeight);
		void position_btt_zoom(int screenWidth, int screenHeight);
		
		void reset();
		void not_over();
		

		MenuRight();
		~MenuRight();

};

#endif
