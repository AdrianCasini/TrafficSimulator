#include "MenuLeft.h"


//------------------------------------------------------------------------------
MenuLeft::~MenuLeft()
{

}

//------------------------------------------------------------------------------
MenuLeft::MenuLeft()
{

}

//------------------------------------------------------------------------------
void MenuLeft::init(int screenWidth, int screenHeight, Cadena cadgral)
{
	mcadgral = cadgral;
	vector<Cadena> vec_btt;
	vector<Cadena>::const_iterator it;
	Cadena cad;
	int i;


	refresh(screenWidth, screenHeight);


	//---Gral Buttons----------------------------
	mShader.inicializar(SIM_DIR_SHADERS_"menu_left.vs", SIM_DIR_SHADERS_"menu_left.frag");
	mTxBtt      = glGetUniformLocation(mShader.mProgram, "texture_btt"); 
	mBtt_number = glGetUniformLocation(mShader.mProgram, "btt_number");
	mBtt_pw_x   = glGetUniformLocation(mShader.mProgram, "btt_pw_x");
	mBtt_pw_y   = glGetUniformLocation(mShader.mProgram, "btt_pw_y");
	mBtt_w      = glGetUniformLocation(mShader.mProgram, "btt_w");
	mBtt_h      = glGetUniformLocation(mShader.mProgram, "btt_h");
	mBtt_s      = glGetUniformLocation(mShader.mProgram, "btt_s");
	mBtt_mv     = glGetUniformLocation(mShader.mProgram, "btt_mv");
	mBtt_dx     = glGetUniformLocation(mShader.mProgram, "btt_dx");
	mBtt_dy     = glGetUniformLocation(mShader.mProgram, "btt_dy");

	vec_btt.push_back("create_trip");
	vec_btt.push_back("detector");
	vec_btt.push_back("color_picker");
	vec_btt.push_back("text");
	vec_btt.push_back("night");

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	i = 0;

	for (it = vec_btt.begin(); it != vec_btt.end(); ++it)
	{

		mButtons[i].name   = cad.formCadena("menu_left/%s.png",   (*it).getCadena());
		mButtons[i].name_n = cad.formCadena("menu_left/%s_n.png", (*it).getCadena());
		mButtons[i].name_o = cad.formCadena("menu_left/%s_o.png", (*it).getCadena());

		mButtons[i].flag = false;
		mButtons[i].over = false;
		mButtons[i].ymv = 0.0;

		Simulator::texture_RGBA(mButtons[i].txt_id,   mButtons[i].name);
		Simulator::texture_RGBA(mButtons[i].txt_id_n, mButtons[i].name_n);
		Simulator::texture_RGBA(mButtons[i].txt_id_o, mButtons[i].name_o);

		glGenBuffers(1, &mButtons[i].vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, mButtons[i].vbo_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MODEL_4P), MODEL_4P, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mButtons[i].vbo_id);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
		glVertexAttribDivisor(0, 0);
		i++;
	}

	//---Menu Bar----------------------------
	mBar.name   = "menu_left/menuleft_bar.png";
	mBar.name_n = "menu_left/menuleft_bar.png";
	mBar.name_o = "menu_left/menuleft_bar.png";

	mButtons[i].flag = false;
	mButtons[i].over = false;
	mButtons[i].ymv = 0.0;

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

		
	mBttNumber = vec_btt.size();
	glBindVertexArray(0);

	mMENU_X_POS = 0.05f * screenWidth;
	mMENU_Y_POS = 0.02f * screenHeight;

	refresh(screenWidth, screenHeight);
}
//------------------------------------------------------------------------------
void MenuLeft::reset()
{
	for (int i = 0; i < mBttNumber; i++)
	{
		mButtons[i].flag = false;
		mButtons[i].over = false;
		mButtons[i].ymv = 0.0;
	}

	mBar.flag = false;
	mBar.over = false;
	mBar.ymv  = 0.0;
}

//------------------------------------------------------------------------------
void MenuLeft::not_over()
{
	for (int i = 0; i < mBttNumber; i++)
	{
		mButtons[i].over = false;
		mButtons[i].ymv  = 0.0;
	}

	mBar.flag = false;
	mBar.over = false;
	mBar.ymv = 0.0;
}


//------------------------------------------------------------------------------
int MenuLeft::get_button(double w, double h, double sep, double dx, double dy, double xpos, double ypos)
{

	for (int i = 0; i < SIM_MAX_BUTTON_ML; i++)
	{
		double index = double(i);
		if ((ypos > ((h * index + sep * index) + dy)) && (ypos < ((h * (index + 1) + (sep * index))) + dy) && (xpos < w + dx) && (xpos > dx))
		{
			return i;
		}
	}
	return -1;
}

//------------------------------------------------------------------------------
void MenuLeft::refresh(int screenWidth, int screenHeight)
{
	float sc_w = float(screenWidth);
	float sc_h = float(screenHeight);
	float fact_scw = float(SIM_SCREEN_W) / float(screenWidth);
	float fact_sch = float(SIM_SCREEN_H) / float(screenHeight);

	/*  (screen)(opengl)
	   +--------------------------------------------------------
	   + (0,0)(-1.0,1.0)                            (1920,0)(1.0,1.0)
	   +
	   +
	   +
	   +
	   +
	   + (0,1080)(-1.0,-1.0)                       (1920,1080)(1.0,-1.0)
	   +--------------------------------------------------------
	*/

	//--gral buttons-------------------
	//---opengl distances-----------------
	mMENUpw_x = 0.035f * fact_scw;
	mMENUpw_y = 0.045f * fact_sch;

	mMENUw  = mMENUpw_x;
	mMENUh  = mMENUpw_y;
	mMENUs  = (float)SIM_BUTTON_SEP * mMENUh;
	mMENUmv = 0.0f;
	mMENUdx = 0.0145f * fact_scw;
	mMENUdy = 0.2f    * fact_sch;

	//---screen  distances-----------------
	mMENU_X_POS = 0.05f * sc_w;
	mMENU_Y_POS = 0.02f * sc_h;

	mMENU_BTT_W  = mMENUpw_x * sc_w * 0.5f ;
	mMENU_BTT_H  = mMENUpw_y * sc_h * 0.5f ;
	mMENU_BTT_S  = mMENUs    * sc_h * 0.5f ;
	mMENU_BTT_DX = mMENUdx * sc_w * 0.5f;
	mMENU_BTT_DY = mMENUdy * sc_h * 0.5f;

	//---menu bar--------------------
	mMENUpw_x_bar = 0.040f * fact_scw;
	mMENUpw_y_bar = 0.6f   * fact_sch;
	mMENUw_bar = mMENUpw_x_bar;
	mMENUh_bar = mMENUpw_y_bar;
	mMENUs_bar = 0.0f;
	mMENUmv_bar = 0.0f;
	mMENUdx_bar = 0.0138f * fact_scw;
	mMENUdy_bar = 0.18f   * fact_sch;

}

//------------------------------------------------------------------------------
void MenuLeft::render()
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


	for (int i = 0; i < mBttNumber; i++)
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
		//glUniform1f(mBtt_mv, mMENUmv * mButtons[i].ymv);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
}