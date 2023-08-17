#include "Menu.h"


//------------------------------------------------------------------------------
Menu::~Menu()
{

}

//------------------------------------------------------------------------------
Menu::Menu()
{

}

//------------------------------------------------------------------------------
void Menu::init(int screenWidth, int screenHeight, Cadena cadgral)
{
	vector<Cadena> vec_btt;
	vector<Cadena>::const_iterator it;
	Cadena cad;
	int count;

	mcadgral = cadgral;

	//The order doesn't matter
	mMapButton[SIM_BTT_FIND]      = "find";
	mMapButton[SIM_BTT_PLAY]      = "play";
	mMapButton[SIM_BTT_PAUSE]     = "pause";
	mMapButton[SIM_BTT_ALGORITHM] = "algorithm";
	mMapButton[SIM_BTT_SAVE_TEST] = "save_test";
	mMapButton[SIM_BTT_SET_TEST]  = "set_test";
	mMapButton[SIM_BTT_SEND_INFO] = "send_info";
	mMapButton[SIM_BTT_TOOLS]     = "tools";

	mMapButton[SIM_BTT_ROUTE]     = "route";
	mMapButton[SIM_BTT_SNAP]      = "snap";
	mMapButton[SIM_BTT_SNAPFILE]  = "snapfile";
	mMapButton[SIM_BTT_TRACE]     = "trace";


	mMapButton[SIM_BTT_TURBO]     = "turbo";
	mMapButton[SIM_BTT_3D]        = "3d";
	mMapButton[SIM_BTT_3D_MAX]    = "3d_max";
	mMapButton[SIM_BTT_CLEAN]     = "clean";

	mMapButton[SIM_BTT_SECMNGR]   = "sectionmngr";
	mMapButton[SIM_BTT_INRIX_SPEED_BUCKET] = "inrix_speed_bucket";
	mMapButton[SIM_BTT_INRIX_SPEED]        = "inrix_speed";

	mMapButton[SIM_BTT_DUM_1]    = "transparent";
	mMapButton[SIM_BTT_DUM_2]    = "transparent";
	mMapButton[SIM_BTT_DUM_3]    = "transparent";
	mMapButton[SIM_BTT_DUM_4]    = "transparent";


	//The order doesn't matter
	mMapButton_tt[SIM_BTT_FIND]       = "Find location";
	mMapButton_tt[SIM_BTT_PLAY]       = "Play";
	mMapButton_tt[SIM_BTT_PAUSE]      = "Pause";
	mMapButton_tt[SIM_BTT_ALGORITHM]  = "Apply overlaping algorythm";
	mMapButton_tt[SIM_BTT_SAVE_TEST]  = "Save the test";
	mMapButton_tt[SIM_BTT_SET_TEST]   = "Set a test";
	mMapButton_tt[SIM_BTT_SEND_INFO]  = "Send information to HUB Autopistas";
	mMapButton_tt[SIM_BTT_TOOLS]      = "Tools";

	mMapButton_tt[SIM_BTT_ROUTE]      = "Route: [Choose points to make a route]\nOSM route Service";
	mMapButton_tt[SIM_BTT_SNAP]       = "Snap To Route [Choose a set of point]\nOSM match Service";
	mMapButton_tt[SIM_BTT_SNAPFILE]   = "Snap To Route from file";
	mMapButton_tt[SIM_BTT_TRACE]      = "Draw a predefined Trip";


	mMapButton_tt[SIM_BTT_TURBO]      = "Prediction in 30 minutes";
	mMapButton_tt[SIM_BTT_3D]         = "3D ";
	mMapButton_tt[SIM_BTT_3D_MAX]     = "3D Real";
	mMapButton_tt[SIM_BTT_CLEAN]      = "Clean elements";

	mMapButton_tt[SIM_BTT_SECMNGR]    = "Section Manager";
	mMapButton_tt[SIM_BTT_INRIX_SPEED_BUCKET] = "INRIX Speed bucket";
	mMapButton_tt[SIM_BTT_INRIX_SPEED] = "INRIX Speed";

	mMapButton_tt[SIM_BTT_DUM_1] = "";
	mMapButton_tt[SIM_BTT_DUM_2] = "";
	mMapButton_tt[SIM_BTT_DUM_3] = "";
	mMapButton_tt[SIM_BTT_DUM_4] = "";




	for (int i = 0; i < SIM_MAX_BUTTON; i++)
	{
		vec_btt.push_back(mMapButton[eMenuTop(i)]);
	}


	refresh(screenWidth, screenHeight);

	//---Gral Buttons----------------------------

	mShader.inicializar(SIM_DIR_SHADERS_"menu.vs", SIM_DIR_SHADERS_"menu.frag");
	mTxBtt      = glGetUniformLocation(mShader.mProgram, "textureBtt");
	mBtt_number = glGetUniformLocation(mShader.mProgram, "btt_number");
	mBtt_pw_x   = glGetUniformLocation(mShader.mProgram, "btt_pw_x");
	mBtt_pw_y   = glGetUniformLocation(mShader.mProgram, "btt_pw_y");
	mBtt_w      = glGetUniformLocation(mShader.mProgram, "btt_w");
	mBtt_h      = glGetUniformLocation(mShader.mProgram, "btt_h");
	mBtt_s      = glGetUniformLocation(mShader.mProgram, "btt_s");
	mBtt_mv     = glGetUniformLocation(mShader.mProgram, "btt_mv");
	mBtt_dx     = glGetUniformLocation(mShader.mProgram, "btt_dx");
	mBtt_dy     = glGetUniformLocation(mShader.mProgram, "btt_dy");



	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	count = 0;
	for (it = vec_btt.begin(); it != vec_btt.end(); ++it)
	{

		mButtons[count].name   = cad.formCadena("menu_top/%s.png",   (*it).getCadena());
		mButtons[count].name_n = cad.formCadena("menu_top/%s_n.png", (*it).getCadena());
		mButtons[count].name_o = cad.formCadena("menu_top/%s_o.png", (*it).getCadena());

		mButtons[count].tooltip = mMapButton_tt[eMenuTop(count)];


		mButtons[count].flag = false;
		mButtons[count].over = false;
		mButtons[count].ymv = 0.0;

		Simulator::texture_RGBA(mButtons[count].txt_id,   mButtons[count].name);
		Simulator::texture_RGBA(mButtons[count].txt_id_n, mButtons[count].name_n);
		Simulator::texture_RGBA(mButtons[count].txt_id_o, mButtons[count].name_o);

		glGenBuffers(1, &mButtons[count].vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, mButtons[count].vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MODEL_4P), MODEL_4P, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mButtons[count].vbo_id);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
		glVertexAttribDivisor(0, 0);
		count++;
	}

	//---Menu Bar----------------------------
	mBar.name   = "menu_top/menutop_bar.png";
	mBar.name_n = "menu_top/menutop_bar.png";
	mBar.name_o = "menu_top/menutop_bar.png";

	mButtons[count].flag = false;
	mButtons[count].over = false;
	mButtons[count].ymv = 0.0;

	Simulator::texture_RGBA(mBar.txt_id, mBar.name);
	Simulator::texture_RGBA(mBar.txt_id_n, mBar.name_n);
	Simulator::texture_RGBA(mBar.txt_id_o, mBar.name_o);

	glGenBuffers(1, &mBar.vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, mBar.vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MODEL_4P), MODEL_4P, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mBar.vbo_id);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribDivisor(0, 0);

	
	mBttMax = vec_btt.size();
	glBindVertexArray(0);

}


//------------------------------------------------------------------------------
int Menu::get_button(double w, double h, double sep, double dx, double dy, double xpos, double ypos)
{

	for (int i = 0; i < SIM_MAX_BUTTON; i++)
	{
		if ((xpos > ((w + sep) * i) + dx) && (xpos < ((w * (i + 1) + (sep * i))) + dx) && (ypos < h + dy))
		{
			return i;
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
void Menu::reset()
{
	for (int i = 0; i < mBttMax; i++)
	{
		mButtons[i].flag = false;
		mButtons[i].over = false;
		mButtons[i].ymv = 0.0;
	}

	mBar.flag = false;
	mBar.over = false;
	mBar.ymv = 0.0;
}

//------------------------------------------------------------------------------
void Menu::not_over()
{
	for (int i = 0; i < mBttMax; i++)
	{
		mButtons[i].over = false;
		mButtons[i].ymv  = 0.0;
	}

	mBar.flag = false;
	mBar.over = false;
	mBar.ymv = 0.0;
}

//------------------------------------------------------------------------------
void Menu::refresh(int screenWidth, int screenHeight)
{
	mMENUpw_x = 0.035 * float(SIM_SCREEN_W) / float(screenWidth);
	mMENUpw_y = 0.045 * float(SIM_SCREEN_H) / float(screenHeight);

	mMENUw = (0.5 * 2) * mMENUpw_x;
	mMENUh = (0.5 * 2) * mMENUpw_y;
	mMENUs  = SIM_BUTTON_SEP * mMENUw;
	mMENUmv = SIM_BUTTON_YMV * mMENUh;
	mMENUdx = SIM_BUTTON_DX  * mMENUw;
	mMENUdy = SIM_BUTTON_DY  * mMENUh;
	
	mMENU_BTT_W  = screenWidth  * 0.5 * mMENUpw_x; 
	mMENU_BTT_H  = screenHeight * 0.5 * mMENUpw_y;
	mMENU_H      = screenHeight * 0.5 * mMENUpw_y;
	mMENU_BTT_S  = SIM_BUTTON_SEP * mMENU_BTT_W;
	mMENU_BTT_DX = SIM_BUTTON_DX * mMENU_BTT_W;
	mMENU_BTT_DY = SIM_BUTTON_DY * mMENU_BTT_H;


	//---menu bar--------------------
	mMENUpw_x_bar = 3.5 * float(SIM_SCREEN_H) / float(screenHeight);
	mMENUpw_y_bar = 0.09 * float(SIM_SCREEN_H) / float(screenHeight);
	mMENUw_bar = 0.0;
	mMENUh_bar = 0.0;
	mMENUs_bar = 0.0;
	mMENUmv_bar = 0.0;
	mMENUdx_bar = 0.332;
	mMENUdy_bar = 0.02;
}

//------------------------------------------------------------------------------
void Menu::render()
{

	glBindVertexArray(mVAO);
	mShader.use();

	//---menu bar--------------------
	glUniform1f(mBtt_pw_x, mMENUpw_x_bar);
	glUniform1f(mBtt_pw_y, mMENUpw_y_bar);
	glUniform1f(mBtt_w, mMENUw_bar);
	glUniform1f(mBtt_h, mMENUh_bar);
	glUniform1f(mBtt_s, mMENUs_bar);
	glUniform1f(mBtt_dx, mMENUdx_bar);
	glUniform1f(mBtt_dy, mMENUdy_bar);
	glUniform1i(mTxBtt, 0);
	glUniform1f(mBtt_number, 0);
	glUniform1f(mBtt_mv, 0.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mBar.txt_id);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	//--gral buttons-------------------
	glUniform1f(mBtt_pw_x, mMENUpw_x);
	glUniform1f(mBtt_pw_y, mMENUpw_y);
	glUniform1f(mBtt_w,    mMENUw);
	glUniform1f(mBtt_h,    mMENUh);
	glUniform1f(mBtt_s,    mMENUs);
	glUniform1f(mBtt_dx,   mMENUdx);
	glUniform1f(mBtt_dy,   mMENUdy);
	glUniform1i(mTxBtt,    0);


	for (int i = 0; i < mBttMax; i++)
	{
		glUniform1f(mBtt_number, i);


		glActiveTexture(GL_TEXTURE0);

		if (mButtons[i].over == true)
		{
			glBindTexture(GL_TEXTURE_2D, mButtons[i].txt_id_o);
		}
		else
		{
		
			if (mButtons[i].flag == true)
			{
				glBindTexture(GL_TEXTURE_2D, mButtons[i].txt_id_n);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, mButtons[i].txt_id);
			}
		}

		if ((i == SIM_BTT_DUM_1) || 
			(i == SIM_BTT_DUM_2) || 
			(i == SIM_BTT_DUM_3) || 
			(i == SIM_BTT_DUM_4))
		{
			mButtons[i].ymv = 0.0;
		}
		glUniform1f(mBtt_mv, mMENUmv * mButtons[i].ymv);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
}