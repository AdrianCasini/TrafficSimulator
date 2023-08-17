#include "Drawing.h"


//------------------------------------------------------------------------------
Drawing::Drawing()
{
	
}

//------------------------------------------------------------------------------
Drawing::~Drawing()
{
	
}

//------------------------------------------------------------------------------
void Drawing::init(Controls* ctrl, QuadMap* mquadmap, Cadena cadgral)
{

	mcadgral = cadgral;
	mCtrl = ctrl;
	mQuadMap = mquadmap;

	mPointIndex = 0;
	mPointIndex_route = 0;

	mPoints_line=0;
	mPoints_line_out=0;

	mbuff_points_draw       = new double[3 * SIM_MAX_ROUTE_PNTS];
	mbuff_points_route_draw = new double[3 * SIM_MAX_ROUTE_PNTS_ROUTE];

	mbuff_lines_draw     = new double[3 * SIM_MAX_ROUTE_PNTS_ROUTE];
	mbuff_lines_draw_out = new double[3 * 6 * SIM_MAX_ROUTE_PNTS_ROUTE];

	mArrayPoints = new GPoint[3 * SIM_MAX_ROUTE_PNTS];


	render_setup();

}

//------------------------------------------------------------------------------
void Drawing::render_setup()
{

	//---VAO---VBO--POINT---------------------
	glGenVertexArrays(1, &mVAO_point);
	glBindVertexArray(mVAO_point);

	glGenBuffers(1, &mVBO_points_model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO_points_offset);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_ROUTE_PNTS, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);

	//---VAO---VBO--ROUTE---------------------
	glGenVertexArrays(1, &mVAO_point_route);
	glBindVertexArray(mVAO_point_route);

	glGenBuffers(1, &mVBO_points_model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO_points_offset_route);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset_route);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_ROUTE_PNTS_ROUTE, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset_route);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);

	//---VAO---VBO--LINE---------------------
	glGenVertexArrays(1, &mVAO_line);
	glBindVertexArray(mVAO_line);

	glGenBuffers(1, &mVBO_lines_surface);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_ROUTE_PNTS_ROUTE * 6, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribDivisor(0, 0);

	glBindVertexArray(0);

	
	//---shader----------------------------------
	mShader_line.inicializar(SIM_DIR_SHADERS_"drawing_line.vs", SIM_DIR_SHADERS_"drawing_line.frag");
	mShader_point.inicializar(SIM_DIR_SHADERS_"drawing_point.vs", SIM_DIR_SHADERS_"drawing_point.frag");

	//--shader variables POINT-------------------------
	mTextID_p  = glGetUniformLocation(mShader_point.mProgram, "texture_smp");
	mCameraR_p = glGetUniformLocation(mShader_point.mProgram, "CameraR");
	mCameraU_p = glGetUniformLocation(mShader_point.mProgram, "CameraU");
	mVP_p      = glGetUniformLocation(mShader_point.mProgram, "VP");
	mSize_p    = glGetUniformLocation(mShader_point.mProgram, "PointSize");
	mColor_p   = glGetUniformLocation(mShader_point.mProgram, "Color");

	Simulator::texture_customize(mTextImg_p, "point_circle.png", GL_CLAMP_TO_EDGE, GL_RGBA, SOIL_LOAD_AUTO);
	//Simulator::texture_dds(mTextImg_p, "particle.DDS");


	//--shader variables LINE-------------------------
	mVP_l = glGetUniformLocation(mShader_line.mProgram, "VP");
	mColor_l = glGetUniformLocation(mShader_line.mProgram, "Color");

	mColor.r = 0.33f;
	mColor.g = 0.22f;
	mColor.b = 0.55f;
	mColor.a = 0.88f;

	mColorRoute.r = 0.53f;
	mColorRoute.g = 0.53f;
	mColorRoute.b = 0.99f;
	mColorRoute.a = 0.86f;

}

//------------------------------------------------------------------------------
void Drawing::add_point(double x, double y, double z)
{
	add_point(x, y, z, mbuff_points_draw, mArrayPoints);
}

//------------------------------------------------------------------------------
void Drawing::add_point(double x, double y, double z, double* buff, GPoint* buffP)
{
	double yref, latref;
	double xref, lonref;
	double lat, lon;

	tranform(x, y, z);

	buff[3 * mPointIndex + 0] = x;
	buff[3 * mPointIndex + 1] = y;
	buff[3 * mPointIndex + 2] = z;

	mQuadMap->get_ref(x, y, xref, lonref, yref, latref, lat, lon);

	buffP[mPointIndex].lat = lat;
	buffP[mPointIndex].lon = lon;
	buffP[mPointIndex].X = x;
	buffP[mPointIndex].Y = y;

	if (mPointIndex < (SIM_MAX_ROUTE_PNTS - 1))
	{
		mPointIndex++;
		cout << "Points:[" << mPointIndex << "]" << endl;
	}
	else
	{
		cout << "Maximun number of Points reached" << endl;
		return;
	}

	refresh(); 

}

//------------------------------------------------------------------------------
void Drawing::tranform(double& x, double& y, double& z)
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
double Drawing::get_drawing_point_size()
{
	double element_size = SIM_DRAWING_POINT_ZIZE * mCtrl->mPosition.z;

	if (element_size < SIM_DRAWING_POINT_ZIZE)
	{
		element_size = SIM_DRAWING_POINT_ZIZE;
	}

	return element_size;
}

//------------------------------------------------------------------------------
double Drawing::get_drawing_line_size()
{
	double element_size = SIM_DRAWING_LINE_WIDTH * mCtrl->mPosition.z;

	if (element_size < SIM_DRAWING_LINE_WIDTH)
	{
		element_size = SIM_DRAWING_LINE_WIDTH;
	}

	return element_size;
}

//------------------------------------------------------------------------------
void Drawing::draw_route(WayInfo* wayinfo)
{
	mPointIndex_route = 0;

	for (int i = 0; i < wayinfo->max_nodos; i++)
	{
		mbuff_points_route_draw[3 * i + 0] = (wayinfo->array_nodo_info)[i].X;
		mbuff_points_route_draw[3 * i + 1] = (wayinfo->array_nodo_info)[i].Y;
		mbuff_points_route_draw[3 * i + 2] = 0.0;
		mPointIndex_route++;
	}

	mPoints_line = 0;
	mPoints_line_out = 0;
	
	for (int i = 0; i < wayinfo->max_nodos - 1; i++)
	{
		//-------------------LINE POINTS---------------------------------------------
		//Point - Line
		mbuff_lines_draw[3 * mPoints_line + 0] = mbuff_points_route_draw[3 * i + 0];
		mbuff_lines_draw[3 * mPoints_line + 1] = mbuff_points_route_draw[3 * i + 1];
		mbuff_lines_draw[3 * mPoints_line + 2] = mbuff_points_route_draw[3 * i + 2];
		mPoints_line++;

		//Point - Line Following
		mbuff_lines_draw[3 * mPoints_line + 0] = mbuff_points_route_draw[3 * (i + 1) + 0];
		mbuff_lines_draw[3 * mPoints_line + 1] = mbuff_points_route_draw[3 * (i + 1) + 1];
		mbuff_lines_draw[3 * mPoints_line + 2] = mbuff_points_route_draw[3 * (i + 1) + 2];
		mPoints_line++;

	}

	refresh();
}

//------------------------------------------------------------------------------
void Drawing::clear()
{
	mPointIndex = 0;
	mPointIndex_route = 0;
	mPoints_line = 0;
	mPoints_line_out = 0;

	memset(mbuff_points_draw,       0, 3 * SIM_MAX_ROUTE_PNTS *           sizeof(double));
	memset(mbuff_points_route_draw, 0, 3 * SIM_MAX_ROUTE_PNTS_ROUTE *     sizeof(double));
	memset(mbuff_lines_draw,        0, 3 * SIM_MAX_ROUTE_PNTS_ROUTE *     sizeof(double));
	memset(mbuff_lines_draw_out,    0, 3 * 6 * SIM_MAX_ROUTE_PNTS_ROUTE * sizeof(double));
	memset(mArrayPoints,            0, 3 * SIM_MAX_ROUTE_PNTS *           sizeof(GPoint));
}

//------------------------------------------------------------------------------
void Drawing::refresh()
{
	glBindVertexArray(mVAO_point);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * (mPointIndex), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * (mPointIndex), mbuff_points_draw);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(mVAO_point_route);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset_route);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * mPointIndex_route, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mPointIndex_route, mbuff_points_route_draw);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Simulator::build_LINE_RECT(get_drawing_line_size(), mbuff_lines_draw, mPoints_line, mbuff_lines_draw_out, 3 * 6 * SIM_MAX_ROUTE_PNTS_ROUTE, &mPoints_line_out);

	glBindVertexArray(mVAO_line);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * mPoints_line_out, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mPoints_line_out, mbuff_lines_draw_out);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

//------------------------------------------------------------------------------
void Drawing::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	render_line_surface(viewprojectionmatrix);
	render_point(viewmatrix, viewprojectionmatrix);
}

//------------------------------------------------------------------------------
void Drawing::render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{

	double sz = get_drawing_point_size();
	mShader_point.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextImg_p);
	glUniform1i(mTextID_p, 0);

	glUniform3f(mCameraR_p, viewmatrix[0][0], viewmatrix[1][0], viewmatrix[2][0]);
	glUniform3f(mCameraU_p, viewmatrix[0][1], viewmatrix[1][1], viewmatrix[2][1]);
	glUniformMatrix4fv(mVP_p, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	
	//Point Start-End in
	/*
	glUniform1f(mSize_p, sz);
	glUniform4f(mColor_p, mColor.r, mColor.g, mColor.b, mColor.a);
	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mPointIndex));
	glBindVertexArray(0);
	*/

	//Point Start-End out
	
	glUniform1f(mSize_p, sz * SIM_TOOLS_POINT_IN_PW);
	glUniform4f(mColor_p, 1.0, 1.0, 1.0, 1.0);
	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mPointIndex));
	glBindVertexArray(0);
	

	//Point Route
	glUniform1f(mSize_p, sz/3.0);
	glUniform4f(mColor_p, mColorRoute.r, mColorRoute.g, mColorRoute.b, mColorRoute.a);
	glBindVertexArray(mVAO_point_route);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mPointIndex_route));
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

}

//------------------------------------------------------------------------------
void Drawing::render_line_surface(glm::mat4& viewprojectionmatrix)
{
	
	mShader_line.use();

	glUniformMatrix4fv(mVP_l, 1, GL_FALSE, &viewprojectionmatrix[0][0]);
	glUniform4f(mColor_l, mColorRoute.r, mColorRoute.g, mColorRoute.b, mColorRoute.a);

	glBindVertexArray(mVAO_line);
	glDrawArrays(GL_TRIANGLES, 0, mPoints_line_out);
	glBindVertexArray(0);
	
}

//------------------------------------------------------------------------------
void Drawing::resize_line()
{
	refresh();

}

