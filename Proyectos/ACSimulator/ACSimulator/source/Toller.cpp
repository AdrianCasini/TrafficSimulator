#include "Toller.h"


//------------------------------------------------------------------------------
Toller::Toller()
{
	clear();
}

//------------------------------------------------------------------------------
Toller::~Toller()
{
	
}

//------------------------------------------------------------------------------
void Toller::init(Controls* ctrl)
{

	mCtrl = ctrl;

	//---VAO---VBO--LINE---------------------
	glGenVertexArrays(1, &mVAO_line);
	glBindVertexArray(mVAO_line);

	glGenBuffers(1, &mVBO_lines_surface);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_POLYG_PNTS * 6, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribDivisor(0, 0);

	glBindVertexArray(0);

	//---VAO---VBO--POINT---------------------
	glGenVertexArrays(1, &mVAO_point);
	glBindVertexArray(mVAO_point);

	glGenBuffers(1, &mVBO_points_model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);
	
	glGenBuffers(1, &mVBO_points_offset);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_POLYG_PNTS, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);

	//---shader----------------------------------
	mShader_line.inicializar(SIM_DIR_SHADERS_"toller_line.vs", SIM_DIR_SHADERS_"toller_line.frag");
	mShader_point.inicializar(SIM_DIR_SHADERS_"toller_point.vs", SIM_DIR_SHADERS_"toller_point.frag");

	//--shader variables LINE-------------------------
	mVP_l      = glGetUniformLocation(mShader_line.mProgram, "VP");
	mColor_l   = glGetUniformLocation(mShader_line.mProgram, "Color");

	//--shader variables POINT-------------------------
	mTextID_p   = glGetUniformLocation(mShader_point.mProgram, "texture_smp");
	mCameraR_p  = glGetUniformLocation(mShader_point.mProgram, "CameraR");
	mCameraU_p  = glGetUniformLocation(mShader_point.mProgram, "CameraU");
	mVP_p       = glGetUniformLocation(mShader_point.mProgram, "VP");
	mSize_p     = glGetUniformLocation(mShader_point.mProgram, "PointSize");
	mColor_p    = glGetUniformLocation(mShader_point.mProgram, "Color");

	//Simulator::texture_customize(mTextImg_p, "point.png", GL_CLAMP_TO_EDGE, GL_RGBA, SOIL_LOAD_AUTO);
	Simulator::texture_dds(mTextImg_p, "particle.DDS");

}

//------------------------------------------------------------------------------
bool Toller::button_active()
{
	return mFlagBtt;
}

//------------------------------------------------------------------------------
bool Toller::button_not_active()
{
	return !mFlagBtt;
}

//------------------------------------------------------------------------------
bool Toller::exist_polygon()
{
	return (mPoly_points > 0);
}

//------------------------------------------------------------------------------
bool Toller::is_payment_active()
{
	return (mPoly_points >= 10);
}

//------------------------------------------------------------------------------
bool Toller::is_active()
{
	return (mPoly_points > 0);
}

//------------------------------------------------------------------------------
void Toller::calculate_payment(double x, double x_old, double y, double y_old)
{
	mTrip = Simulator::distance(x, x_old, y, y_old) + mTrip;
	mText_km.formCadena("[%3.2f] Km", mTrip);
	mText_eu.formCadena("[%4.2f] euros", mTrip * SIM_TOLLER_TARIFA);
}

//------------------------------------------------------------------------------
bool Toller::inside_polygon(double x, double y)
{
	return (Simulator::pnpoly(mPoly_points, mbuff_poly_x, mbuff_poly_y,x, y));
}

//------------------------------------------------------------------------------
void Toller::button_pressed(bool flag)
{
	mFlagBtt = flag;

	if ((button_not_active()) && (exist_polygon()))
	{
		end_polygon();
	}
}

//------------------------------------------------------------------------------
bool Toller::is_first_point()
{
	return (mFirstPolygon_Point);
}

//------------------------------------------------------------------------------
void Toller::resize_line()
{
	if (exist_polygon())
	{
		refresh();
	}
}

//------------------------------------------------------------------------------
double Toller::get_toller_point_size()
{
	double element_size = SIM_TOLLER_POINT_ZIZE * mCtrl->mPosition.z;

	if (element_size < SIM_TOLLER_POINT_ZIZE)
	{
		element_size = SIM_TOLLER_POINT_ZIZE;
	}

	return element_size;
}

//------------------------------------------------------------------------------
double Toller::get_toller_line_size()
{
	double element_size = SIM_TOLLER_LINE_WIDTH * mCtrl->mPosition.z;

	if (element_size < SIM_TOLLER_LINE_WIDTH)
	{
		element_size = SIM_TOLLER_LINE_WIDTH;
	}

	return element_size;
}

//------------------------------------------------------------------------------
void Toller::end_polygon()
{
	if (exist_polygon())
	{

		mbuff_lines_draw[3 * (mPoints_line - 1) + 0] = mIX;
		mbuff_lines_draw[3 * (mPoints_line - 1) + 1] = mIY;
		mbuff_lines_draw[3 * (mPoints_line - 1) + 2] = mIZ;
		refresh();

	}
}

//------------------------------------------------------------------------------
void Toller::add_line(double x, double y, double z)
{
	double xx, yy, zz;

	if (button_active())
	{
		tranform(x, y, z);

		if (mPoints_line == 0)
		{
			xx = x;
			yy = y;
			zz = z;

			mIX = x;
			mIY = y;
			mIZ = z;
		}
		else
		{
			xx = mX;
			yy = mY;
			zz = mZ;
		}


		//-------------------UNION POLYGON POINTS----------------------------------
		mbuff_points_draw_union[3 * mPoints_point_draw + 0] = xx;
		mbuff_points_draw_union[3 * mPoints_point_draw + 1] = yy;
		mbuff_points_draw_union[3 * mPoints_point_draw + 2] = zz;

		if (mPoints_point_draw < (SIM_MAX_POLYG_PNTS - 1))
			mPoints_point_draw++;

		//-------------------POLYGON POINTS-----------------------------------------
		mbuff_poly_points[2 * mPoly_points + 0] = xx;
		mbuff_poly_points[2 * mPoly_points + 1] = yy;

		mbuff_poly_x[mPoly_points] = xx;
		mbuff_poly_y[mPoly_points] = yy;
		

		if (mPoly_points < (SIM_MAX_POLYG_PNTS - 1))
		{
			mPoly_points++;
			//cout << "Polygon Points:" << mPoly_points << endl;
		}

		//-------------------LINE POINTS---------------------------------------------
		//Point - Line
		mbuff_lines_draw[3 * mPoints_line + 0] = xx;
		mbuff_lines_draw[3 * mPoints_line + 1] = yy;
		mbuff_lines_draw[3 * mPoints_line + 2] = zz;
		if (mPoints_line < (SIM_MAX_POLYG_PNTS - 1))
			mPoints_line++;

		//Point - Line Following
		mbuff_lines_draw[3 * mPoints_line + 0] = x;
		mbuff_lines_draw[3 * mPoints_line + 1] = y;
		mbuff_lines_draw[3 * mPoints_line + 2] = z;
		if (mPoints_line < (SIM_MAX_POLYG_PNTS - 1))
			mPoints_line++;

		refresh();

	}
}

//------------------------------------------------------------------------------
void Toller::follow_line(double x, double y, double z)
{

	if (exist_polygon())
	{
		tranform(x, y, z);

		mbuff_lines_draw[3 * (mPoints_line-1) + 0] = x;
		mbuff_lines_draw[3 * (mPoints_line-1) + 1] = y;
		mbuff_lines_draw[3 * (mPoints_line-1) + 2] = z;

		mX = x;
		mY = y;
		mZ = z;

		refresh();

	}
}

//------------------------------------------------------------------------------
void Toller::clear()
{
	mPoints_line       = 0;
	mPoints_line_out   = 0;
	mPoints_point_draw = 0;
	mPoly_points       = 0;

	mTrip = 0;
	mText_km = "";
	mText_eu = "";

	mX = 0;
	mY = 0;
	mZ = 0;

	mIX = 0;
	mIY = 0;
	mIZ = 0;

	memset(mbuff_lines_draw, 0,        SIM_MAX_POLYG_PNTS * sizeof(double));
	memset(mbuff_lines_draw_out, 0,    SIM_MAX_POLYG_PNTS * sizeof(double) * 6);
	memset(mbuff_points_draw_union, 0, SIM_MAX_POLYG_PNTS * sizeof(double));
	memset(mbuff_poly_points, 0,       SIM_MAX_POLYG_PNTS * sizeof(double));

	memset(mbuff_poly_x, 0, SIM_MAX_POLYG_PNTS * sizeof(double));
	memset(mbuff_poly_x, 0, SIM_MAX_POLYG_PNTS * sizeof(double));
	
}

//------------------------------------------------------------------------------
void Toller::tranform(double& x, double& y, double& z)
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat  winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	posX = posX * mCtrl->mPosition.z * ((9.0 / 16.0) * (1.0 / 0.7625));
	posY = posY * mCtrl->mPosition.z * ((9.0 / 16.0) * (1.0 / 0.7625));

	x = posX + mCtrl->mPosition.x;
	y = posY * ((9.0 / 16.0)) + mCtrl->mPosition.y;
	z = 0.0;

}

//------------------------------------------------------------------------------
void Toller::refresh()
{
	Simulator::build_LINE_RECT(get_toller_line_size(), mbuff_lines_draw, mPoints_line, mbuff_lines_draw_out, SIM_MAX_POLYG_PNTS * 6, &mPoints_line_out);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * mPoints_point_draw, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mPoints_point_draw, mbuff_points_draw_union);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * mPoints_line_out, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mPoints_line_out, mbuff_lines_draw_out);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------------------------
void Toller::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	render_line_surface(viewprojectionmatrix);
	render_point(viewmatrix, viewprojectionmatrix);
}

//------------------------------------------------------------------------------
void Toller::render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{

	double sz;
	sz = get_toller_point_size();
	mShader_point.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextImg_p);
	glUniform1i(mTextID_p, 0);

	glUniform3f(mCameraR_p, viewmatrix[0][0], viewmatrix[1][0], viewmatrix[2][0]);
	glUniform3f(mCameraU_p, viewmatrix[0][1], viewmatrix[1][1], viewmatrix[2][1]);
	glUniformMatrix4fv(mVP_p, 1, GL_FALSE, &viewprojectionmatrix[0][0]);


	//Point outside
	glUniform1f(mSize_p, sz);
	glUniform4f(mColor_p, 0.0, 170.0 / 255.0, 228.0 / 255.0, 1.0);
	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mPoints_point_draw);

	//Point inside
	glUniform1f(mSize_p, sz * SIM_TOLLER_POINT_IN_PW);
	glUniform4f(mColor_p, 1.0, 1.0, 1.0, 1.0);
	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mPoints_point_draw);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

//------------------------------------------------------------------------------
void Toller::render_line_surface(glm::mat4& viewprojectionmatrix)
{
	mShader_line.use();

	glUniformMatrix4fv(mVP_l, 1, GL_FALSE, &viewprojectionmatrix[0][0]);
	glUniform4f(mColor_l, 0.0, 170.0/255.0, 228.0/255.0, 0.88);

	glBindVertexArray(mVAO_line);
	glDrawArrays(GL_TRIANGLES, 0, mPoints_line_out);
	glBindVertexArray(0);
}




