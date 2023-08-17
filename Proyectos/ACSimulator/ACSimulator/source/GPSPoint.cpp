#include "GPSPoint.h"


//------------------------------------------------------------------------------
GPSPoint::GPSPoint()
{
	
}

//------------------------------------------------------------------------------
GPSPoint::~GPSPoint()
{

}

//------------------------------------------------------------------------------
void GPSPoint::init(Controls* ctrl, QuadMap* mquadmap, Cadena cadgral)
{
	mCtrl = ctrl;
	mQuadMap = mquadmap;
	mcadgral = cadgral;

	mPointIndex = 0;
	mbuff_points_draw = new double[SIM_MAX_GPSPOINTS];
	mArrayPoints      = new GPoint[SIM_MAX_GPSPOINTS];

	clear();
	render_setup();

}

//------------------------------------------------------------------------------
bool GPSPoint::open_file()
{

	Cadena cadtrip_name;
	time_t curr_time;
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	cadtrip_name.formCadena(SIM_DIR_TEST_CT_PNT_" / trip_ % 02d % 02d % 02d - % 02d % 02d % 02d.txt",
		tm_local->tm_mday, tm_local->tm_mon + 1, (tm_local->tm_year + 1900) % 100,
		tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);
	cadtrip_name = mcadgral + cadtrip_name;

	if (!mFile.is_open())
	{
		mFile.open(cadtrip_name.getCadena(), std::ios_base::app);
		if (!mFile.is_open()) { perror("error while opening file"); return false; }
	}


	return true;
}



//------------------------------------------------------------------------------
bool GPSPoint::button_active()
{
	return mFlagBtt;
}

//------------------------------------------------------------------------------
bool GPSPoint::button_not_active()
{
	return !mFlagBtt;
}

//------------------------------------------------------------------------------
bool GPSPoint::is_active()
{
	return (mPointIndex > 0);
}

//------------------------------------------------------------------------------
void GPSPoint::button_pressed(bool flag)
{
	mFlagBtt = flag;

	if (button_not_active() && is_active())
	{
		end_points();
	}
}

//------------------------------------------------------------------------------
void GPSPoint::end_points()
{
	if (open_file())
	{
		for (int i = 0; i < mPointIndex; i++)
		{
			mFile << mArrayPoints[i].lat << '#' << mArrayPoints[i].lon << '#' << mArrayPoints[i].Y << '#' << mArrayPoints[i].X << endl;
		}

		mFile.close();
	}
}

//------------------------------------------------------------------------------
double GPSPoint::get_point_size()
{
	double element_size = SIM_TOOLS_POINT_ZIZE * mCtrl->mPosition.z;

	if (element_size < SIM_TOOLS_POINT_ZIZE)
	{
		element_size = SIM_TOOLS_POINT_ZIZE;
	}

	return element_size;
}

//------------------------------------------------------------------------------
void GPSPoint::add_point(double x, double y, double z)
{
	double yref, latref;
	double xref, lonref;
	double lat, lon;


	if (button_active())
	{

		tranform(x, y, z);

		mbuff_points_draw[3 * mPointIndex + 0] = x;
		mbuff_points_draw[3 * mPointIndex + 1] = y;
		mbuff_points_draw[3 * mPointIndex + 2] = z;

		mQuadMap->get_ref(x, y, xref, lonref, yref, latref, lat, lon);

		mArrayPoints[mPointIndex].lat = lat;
		mArrayPoints[mPointIndex].lon = lon;
		mArrayPoints[mPointIndex].X = x;
		mArrayPoints[mPointIndex].Y = y;


		if (mPointIndex < (SIM_MAX_GPSPOINTS - 1))
		{
			mPointIndex++;
			cout << "Points:[" << mPointIndex << "]" << endl;
		}
		else
		{
			cout << "Maximun number of GPS Points reached" << endl;
			return;
		}

		refresh();

	}
}

//------------------------------------------------------------------------------
void GPSPoint::clear()
{
	mPointIndex = 0;
	memset(mbuff_points_draw, 0, SIM_MAX_GPSPOINTS * sizeof(double));
}

//------------------------------------------------------------------------------
void GPSPoint::tranform(double& x, double& y, double& z)
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
void GPSPoint::refresh()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * (mPointIndex), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * (mPointIndex), mbuff_points_draw);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------------------------
void GPSPoint::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	double sz;
	sz = get_point_size();
	mShader_point.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextImg_p);
	glUniform1i(mTextID_p, 0);

	glUniform3f(mCameraR_p, viewmatrix[0][0], viewmatrix[1][0], viewmatrix[2][0]);
	glUniform3f(mCameraU_p, viewmatrix[0][1], viewmatrix[1][1], viewmatrix[2][1]);
	glUniformMatrix4fv(mVP_p, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	//Point outside
	glUniform1f(mSize_p, sz);
	glUniform4f(mColor_p, mColor.r,mColor.g, mColor.b, mColor.a);

	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mPointIndex));
	glBindVertexArray(0);
	
	//Point inside
	glUniform1f(mSize_p, sz * SIM_TOOLS_POINT_IN_PW);
	glUniform4f(mColor_p, 1.0, 1.0, 1.0, 1.0);

	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mPointIndex));
	glBindVertexArray(0);
	

	glBindTexture(GL_TEXTURE_2D, 0);

}

//------------------------------------------------------------------------------
void GPSPoint::render_setup()
{


	//---VAO---VBO--POINT---------------------
	glGenVertexArrays(1, &mVAO_point);
	glBindVertexArray(mVAO_point);

	glGenBuffers(1, &mVBO_points_model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_model);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO_points_offset);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_GPSPOINTS, NULL, GL_STREAM_DRAW);

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
	mShader_point.inicializar(SIM_DIR_SHADERS_"tools_point.vs", SIM_DIR_SHADERS_"tools_point.frag");

	//--shader variables POINT-------------------------
	mTextID_p = glGetUniformLocation(mShader_point.mProgram, "texture_smp");
	mCameraR_p = glGetUniformLocation(mShader_point.mProgram, "CameraR");
	mCameraU_p = glGetUniformLocation(mShader_point.mProgram, "CameraU");
	mVP_p = glGetUniformLocation(mShader_point.mProgram, "VP");
	mSize_p = glGetUniformLocation(mShader_point.mProgram, "PointSize");
	mColor_p = glGetUniformLocation(mShader_point.mProgram, "Color");

	//Simulator::texture_customize(mTextImg_p, "point_.png", GL_CLAMP_TO_EDGE, GL_RGBA, SOIL_LOAD_AUTO);
	Simulator::texture_dds(mTextImg_p, "particle.DDS");

	mColor.r = 0.33;
	mColor.g = 0.22;
	mColor.b = 0.55;
	mColor.a = 0.88;
}
