#include "MenuRight.h"


//------------------------------------------------------------------------------
MenuRight::~MenuRight()
{

}

//------------------------------------------------------------------------------
MenuRight::MenuRight()
{

}

//------------------------------------------------------------------------------
void MenuRight::init(int screenWidth, int screenHeight, Cadena cadgral)
{
	mcadgral = cadgral;
	vector<Cadena> vec_btt;
	vector<Cadena>::const_iterator it;
	Cadena cad;
	int i;


	refresh(screenWidth, screenHeight);

	mShader.inicializar(SIM_DIR_SHADERS_"menu_right.vs", SIM_DIR_SHADERS_"menu_right.frag");
	mTxBtt      = glGetUniformLocation(mShader.mProgram, "textureBtt");
	mBtt_pw_x   = glGetUniformLocation(mShader.mProgram, "btt_pw_x");
	mBtt_pw_y   = glGetUniformLocation(mShader.mProgram, "btt_pw_y");
	mBtt_dx     = glGetUniformLocation(mShader.mProgram, "btt_dx");
	mBtt_dy     = glGetUniformLocation(mShader.mProgram, "btt_dy");
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);


	//Buttons List
	vec_btt.push_back("ml_r_up");
	vec_btt.push_back("ml_r_down");
	vec_btt.push_back("ml_r_right");
	vec_btt.push_back("ml_r_left");
	vec_btt.push_back("ml_r_rotate");

	vec_btt.push_back("ml_mv_up");
	vec_btt.push_back("ml_mv_down");
	vec_btt.push_back("ml_mv_right");
	vec_btt.push_back("ml_mv_left");
	vec_btt.push_back("ml_mv_move");

	mMENU_ROT_DX = 0.033;
	mMENU_ROT_DY = 0.053;
	mMENU_ROT_X = 0.90;
	mMENU_ROT_Y = 0.80;

	mMENU_MV_DX = 0.033;
	mMENU_MV_DY = 0.053;
	mMENU_MV_X = 0.90;
	mMENU_MV_Y = 0.60;


	mMENU_ZOOM_IN_X = 0.90;
	mMENU_ZOOM_IN_Y = 0.46;
	mMENU_ZOOM_OUT_X = 0.90;
	mMENU_ZOOM_OUT_Y = 0.40;


	position_btt_rotate(screenWidth, screenHeight);
	position_btt_move(screenWidth, screenHeight);
	position_btt_zoom(screenWidth, screenHeight);


	i = 0;
	for (it = vec_btt.begin(); it != vec_btt.end(); ++it)
	{

		mButtons[i].name   = cad.formCadena("menu_right/%s.png",   (*it).getCadena());
		mButtons[i].name_n = cad.formCadena("menu_right/%s_n.png", (*it).getCadena());
		mButtons[i].name_o = cad.formCadena("menu_right/%s_o.png", (*it).getCadena());

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
	

	mButtons[i].name = cad.formCadena("menu_right/%s.png", "zoom_in");
	mButtons[i].name_n = cad.formCadena("menu_right/%s_n.png", "zoom_in");
	mButtons[i].name_o = cad.formCadena("menu_right/%s_o.png", "zoom_in");

	mButtons[i].flag = false;
	mButtons[i].over = false;
	mButtons[i].ymv = 0.0;

	Simulator::texture_RGBA(mButtons[i].txt_id, mButtons[i].name);
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


	mButtons[i].name = cad.formCadena("menu_right/%s.png", "zoom_out");
	mButtons[i].name_n = cad.formCadena("menu_right/%s_n.png", "zoom_out");
	mButtons[i].name_o = cad.formCadena("menu_right/%s_o.png", "zoom_out");

	mButtons[i].flag = false;
	mButtons[i].over = false;
	mButtons[i].ymv = 0.0;

	Simulator::texture_RGBA(mButtons[i].txt_id, mButtons[i].name);
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


	glBindVertexArray(0);

}

//------------------------------------------------------------------------------
void MenuRight::position_btt_rotate(int screenWidth, int screenHeight)
{

	//ROTATE UP
	mButtons[SIM_BTT_UP_R].x = mMENU_ROT_X + 0 * mMENU_ROT_DX;
	mButtons[SIM_BTT_UP_R].y = mMENU_ROT_Y + 1 * mMENU_ROT_DY;

	//ROTATE RIGHT
	mButtons[SIM_BTT_RIGHT_R].x = mMENU_ROT_X + 1 * mMENU_ROT_DX;
	mButtons[SIM_BTT_RIGHT_R].y = mMENU_ROT_Y + 0 * mMENU_ROT_DY;

	//ROTATE DOWN
	mButtons[SIM_BTT_DOWN_R].x = mMENU_ROT_X +  0 * mMENU_ROT_DX;
	mButtons[SIM_BTT_DOWN_R].y = mMENU_ROT_Y -  1 * mMENU_ROT_DY;

	//ROTATE LEFT
	mButtons[SIM_BTT_LEFT_R].x = mMENU_ROT_X - 1 * mMENU_ROT_DX;
	mButtons[SIM_BTT_LEFT_R].y = mMENU_ROT_Y + 0 * mMENU_ROT_DY;

	//ROTATE
	mButtons[SIM_BTT_ROTATE_R].x = mMENU_ROT_X + 0 * mMENU_ROT_DX;
	mButtons[SIM_BTT_ROTATE_R].y = mMENU_ROT_Y + 0 * mMENU_ROT_DY;


}

//------------------------------------------------------------------------------
void MenuRight::position_btt_move(int screenWidth, int screenHeight)
{

	//MOVE UP
	mButtons[SIM_BTT_UP_MV].x = mMENU_MV_X + 0 * mMENU_MV_DX;
	mButtons[SIM_BTT_UP_MV].y = mMENU_MV_Y + 1 * mMENU_MV_DY;

	//MOVE DOWN
	mButtons[SIM_BTT_DOWN_MV].x = mMENU_MV_X + 0 * mMENU_MV_DX;
	mButtons[SIM_BTT_DOWN_MV].y = mMENU_MV_Y - 1 * mMENU_MV_DY;

	//MOVE RIGHT
	mButtons[SIM_BTT_RIGHT_MV].x = mMENU_MV_X + 1 * mMENU_MV_DX;
	mButtons[SIM_BTT_RIGHT_MV].y = mMENU_MV_Y + 0 * mMENU_MV_DY;

	//MOVE LEFT
	mButtons[SIM_BTT_LEFT_MV].x = mMENU_MV_X - 1 * mMENU_MV_DX;
	mButtons[SIM_BTT_LEFT_MV].y = mMENU_MV_Y + 0 * mMENU_MV_DY;

	//MOVE
	mButtons[SIM_BTT_MOVE_MV].x = mMENU_MV_X + 0 * mMENU_MV_DX;
	mButtons[SIM_BTT_MOVE_MV].y = mMENU_MV_Y + 0 * mMENU_MV_DY;
}

//------------------------------------------------------------------------------
void MenuRight::position_btt_zoom(int screenWidth, int screenHeight)
{
	//ZOOM_IN
	mButtons[SIM_BTT_ZOOM_IN].x = mMENU_ZOOM_IN_X;
	mButtons[SIM_BTT_ZOOM_IN].y = mMENU_ZOOM_IN_Y;

	//ZOOM_OUT
	mButtons[SIM_BTT_ZOOM_OUT].x = mMENU_ZOOM_OUT_X;
	mButtons[SIM_BTT_ZOOM_OUT].y = mMENU_ZOOM_OUT_Y;


}




//------------------------------------------------------------------------------
int MenuRight::get_button(Button* ptrB, double xpos, double ypos)
{

	for (int i = 0; i < SIM_MAX_BUTTON_MR; i++)
	{
		if ((ptrB[i].x_screen > (xpos - ptrB[i].w / 2)) && (ptrB[i].x_screen < (xpos + ptrB[i].w / 2)) &&
			(ptrB[i].y_screen > (ypos - ptrB[i].h / 2)) && (ptrB[i].y_screen < (ypos + ptrB[i].h / 2)))
		{
			return i;
		}
	}
	return -1;
}


//------------------------------------------------------------------------------
void MenuRight::reset()
{
	for (int i = 0; i < SIM_MAX_BUTTON_MR; i++)
	{
		mButtons[i].flag = false;
		mButtons[i].over = false;
	}
}

//------------------------------------------------------------------------------
void MenuRight::not_over()
{
	for (int i = 0; i < SIM_MAX_BUTTON_MR; i++)
	{
		mButtons[i].over = false;
	}
}

//------------------------------------------------------------------------------
void MenuRight::refresh(int screenWidth, int screenHeight)
{
	mMENUpw_x = 0.04 ;
	mMENUpw_y = 0.05 ;


	for (int i = 0; i < SIM_MAX_BUTTON_MR; i++)
	{
		mButtons[i].w =  0.04 * float(screenWidth) / 2;
		mButtons[i].h =  0.05 * float(screenHeight) / 2;

		mButtons[i].x_screen = mButtons[i].x * float(screenWidth) / 2 + float(screenWidth) / 2;
		mButtons[i].y_screen = (screenHeight / 2) - mButtons[i].y * float(screenHeight) / 2 ;

	}

	mMENU_X_POS = 0.90 * float(screenWidth);

}

//------------------------------------------------------------------------------
void MenuRight::render()
{

	glBindVertexArray(mVAO);
	mShader.use();

	glUniform1f(mBtt_pw_x, mMENUpw_x);
	glUniform1f(mBtt_pw_y, mMENUpw_y);
	glUniform1i(mTxBtt,    0);


	for (int i = 0; i < SIM_MAX_BUTTON_MR; i++)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1f(mBtt_dx, mButtons[i].x);
		glUniform1f(mBtt_dy, mButtons[i].y);

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

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glBindVertexArray(0);
}