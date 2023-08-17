#include "Tools.h"


//------------------------------------------------------------------------------
Tools::Tools()
{
	//--------------------------------------------------------------
	//Here we create the general buffers for drawing purposes
	//In these bufers are all the needed points
	//These points are for a simple line
	mMP.mbuff_lines_in      = new double[SIM_MAX_POLYG_PNTS * SIM_MAX_POLYGONS * 3    ];
	//These points are needed for a surface line
	mMP.mbuff_lines_out     = new double[SIM_MAX_POLYG_PNTS * SIM_MAX_POLYGONS * 3 * 3];
	//These points are needed for drawing th eunion points
	mMP.mbuff_points_union  = new double[SIM_MAX_POLYG_PNTS * SIM_MAX_POLYGONS * 3    ];

	mColorBuff = new GLubyte[SIM_MAX_POLYG_PNTS * SIM_MAX_POLYGONS * 3 * 4];

	reset_MP();

}

//------------------------------------------------------------------------------
Tools::~Tools()
{
	delete mMP.mbuff_lines_in;
	delete mMP.mbuff_lines_out;
	delete mMP.mbuff_points_union;
}

//------------------------------------------------------------------------------
void Tools::init(Controls* ctrl, QuadMap* mquadmap, Cadena cadgral)
{

	mCtrl = ctrl;
	mQuadMap = mquadmap;
	mcadgral = cadgral;

	//---VAO---VBO--LINE---------------------
	glGenVertexArrays(1, &mVAO_line);
	glBindVertexArray(mVAO_line);

	glGenBuffers(1, &mVBO_lines_surface);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_POLYG_PNTS * 6, NULL, GL_STREAM_DRAW);

	//glGenBuffers(1, &mVBO_Color);
	//glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLubyte) * SIM_MAX_POLYG_PNTS * 6, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	//glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 0);
	//glVertexAttribDivisor(1, 0);

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
	mShader_line.inicializar(SIM_DIR_SHADERS_"tools_line.vs", SIM_DIR_SHADERS_"tools_line.frag");
	mShader_point.inicializar(SIM_DIR_SHADERS_"tools_point.vs", SIM_DIR_SHADERS_"tools_point.frag");

	//--shader variables LINE-------------------------
	mVP_l = glGetUniformLocation(mShader_line.mProgram, "VP");
	mColor_l = glGetUniformLocation(mShader_line.mProgram, "Color");

	//--shader variables POINT-------------------------
	mTextID_p  = glGetUniformLocation(mShader_point.mProgram, "texture_smp");
	mCameraR_p = glGetUniformLocation(mShader_point.mProgram, "CameraR");
	mCameraU_p = glGetUniformLocation(mShader_point.mProgram, "CameraU");
	mVP_p      = glGetUniformLocation(mShader_point.mProgram, "VP");
	mSize_p    = glGetUniformLocation(mShader_point.mProgram, "PointSize");
	mColor_p   = glGetUniformLocation(mShader_point.mProgram, "Color");

	//Simulator::texture_customize(mTextImg_p, "point.png", GL_CLAMP_TO_EDGE, GL_RGBA, SOIL_LOAD_AUTO);
	Simulator::texture_dds(mTextImg_p, "particle.DDS");

	mColor.r = 0.33;
	mColor.g = 0.22;
	mColor.b = 0.55;
	mColor.a = 0.88;

	refresh_draw();
}

//------------------------------------------------------------------------------
void Tools::tool_selected(int tool_selected)
{
	if (tool_selected > 0)
	{
		mMP.polygon[mMP.num_polygon].num_points = 0;
		mMP.polygon[mMP.num_polygon].bnew = true;
		mMP.polygon[mMP.num_polygon].type = tool_selected;
	}
}

//------------------------------------------------------------------------------
bool Tools::exist_polygon(int index)
{
	if (index < SIM_MAX_POLYGONS)
	{
		return (mMP.polygon[index].num_points >= 3);
	}
	return false;
}

//------------------------------------------------------------------------------
bool Tools::exist_any_polygon()
{
	return (mMP.num_polygon > 0);
}

//------------------------------------------------------------------------------
bool Tools::is_active()
{
	return ((mActionActive == true) || (mMP.num_polygon > 0));
}

//------------------------------------------------------------------------------
void Tools::activate()
{
	mActionActive = true;

	//Here we start a polygon
	mMP.polygon[mMP.num_polygon].bnew = true;

}

//------------------------------------------------------------------------------
void Tools::deactivate()
{
	mActionActive = false;

	//Do we actually have a polygon?
	if (mMP.polygon[mMP.num_polygon].num_points > 2)
	{
		end_polygon();
	}
	//we have a only two points, this is not enough, delete it!
	else
	{
		delete_actual_polygon();
	}

	refresh();
	
}

//------------------------------------------------------------------------------
void Tools::reset_polygon(int indx)
{
	mMP.polygon[mMP.num_polygon].bnew = true;
	mMP.polygon[mMP.num_polygon].type = -1;
	mMP.polygon[mMP.num_polygon].num_points = 0;
}

//------------------------------------------------------------------------------
void Tools::delete_actual_polygon()
{
	reset_polygon(mMP.num_polygon);

	mMP.mpnts_points_union_now = mMP.mpnts_points_union;
	mMP.mpnts_line_in_now      = mMP.mpnts_line_in;
}

//------------------------------------------------------------------------------
void Tools::reset_MP()
{

	mSpeed = 100;
	mActionActive = false;
	mMP.num_polygon = 0;

	mMP.mpnts_points_union = 0;
	mMP.mpnts_line_in = 0;
	mMP.mpnts_line_out = 0;
	mMP.mpnts_points_union_now = 0;
	mMP.mpnts_line_in_now = 0;

	for (int i = 0; i < SIM_MAX_POLYGONS; i++)
	{
		mMP.polygon[i].bnew = true;
		mMP.polygon[i].num_points = 0;
		mMP.polygon[i].type = -1;
	}

	for (int i = 0; i < SIM_POLYGON_TYPE_MAX; i++)
	{
		for (int j = 0; j < SIM_MAX_POLYGONS; j++)
		{
			mMP.types[i].array_type[j] = -1;
		}
		mMP.types[i].num = 0;
	}
}

//------------------------------------------------------------------------------
void Tools::add_line(double x, double y, double z)
{
	double xpoint_now, ypoint_now, zpoint_now ;

	if (mActionActive == true)
	{
		tranform(x, y, z);

		if (mMP.polygon[mMP.num_polygon].bnew == true)
		{
			//Point Now
			xpoint_now = x;
			ypoint_now = y;
			zpoint_now = z;

			//Initial point of the polygon
			mXini = x;
			mYini = y;
			mZini = z;

			mMP.polygon[mMP.num_polygon].bnew = false;
		}
		else
		{
			//Need two follow the line
			xpoint_now = mXnext;
			ypoint_now = mYnext;
			zpoint_now = mZnext;
		}
		

		if (mMP.polygon[mMP.num_polygon].num_points < SIM_MAX_POLYG_PNTS)
		{
			//-----------------------------------------------------------------------------------------------
			//----------------THIS POINTS ARE NEEDED FOR CHECK IF SOMETHING IS INSIDE IT --------------------
			//-----------------------------------------------------------------------------------------------
			// ONLY ONE POINT
			mMP.polygon[mMP.num_polygon].buff_poly_points[2 * mMP.polygon[mMP.num_polygon].num_points + 0] = xpoint_now;
			mMP.polygon[mMP.num_polygon].buff_poly_points[2 * mMP.polygon[mMP.num_polygon].num_points + 1] = ypoint_now;

			mMP.polygon[mMP.num_polygon].buff_poly_x[mMP.polygon[mMP.num_polygon].num_points] = xpoint_now;
			mMP.polygon[mMP.num_polygon].buff_poly_y[mMP.polygon[mMP.num_polygon].num_points] = ypoint_now;
			
			mMP.polygon[mMP.num_polygon].num_points++;

			//-----------------------------------------------------------------------------------------------
			//------THIS POINTS ARE NEEDED TO MARK THE PLACE WHERE YOU JOINED THE LINES OF THE POLYGON-------
			//-----------------------------------------------------------------------------------------------
			//-------------------UNION POLYGON POINTS--------------------------------------------------------
			mMP.mbuff_points_union[3 * mMP.mpnts_points_union_now + 0] = xpoint_now;
			mMP.mbuff_points_union[3 * mMP.mpnts_points_union_now + 1] = ypoint_now;
			mMP.mbuff_points_union[3 * mMP.mpnts_points_union_now + 2] = zpoint_now;
			mMP.mpnts_points_union_now++;

			//-----------------------------------------------------------------------------------------------
			//------EVERY LINE  NEEDS 2 POINTS --------------------------------------------------------------
			//-----------------------------------------------------------------------------------------------
			
			//Point - The point marked with the mouse
			mMP.mbuff_lines_in[3 * mMP.mpnts_line_in_now + 0] = xpoint_now;
			mMP.mbuff_lines_in[3 * mMP.mpnts_line_in_now + 1] = ypoint_now;
			mMP.mbuff_lines_in[3 * mMP.mpnts_line_in_now + 2] = zpoint_now;

			mMP.mpnts_line_in_now++;

			//Point - Line Following: In the begining they are the same
			mMP.mbuff_lines_in[3 * mMP.mpnts_line_in_now + 0] = xpoint_now;
			mMP.mbuff_lines_in[3 * mMP.mpnts_line_in_now + 1] = ypoint_now;
			mMP.mbuff_lines_in[3 * mMP.mpnts_line_in_now + 2] = zpoint_now;

			mMP.mpnts_line_in_now++;

			refresh();

		}
		else
		{
			MessageBox(NULL, "", "You have reached the maximun number of polygon points", MB_OK);
		}
	}
}

//------------------------------------------------------------------------------
void Tools::follow_line(double x, double y, double z)
{

	if (mMP.polygon[mMP.num_polygon].bnew == false)
	{
		tranform(x, y, z);

		//Here we are modifyng the last point in the add_line function
		mMP.mbuff_lines_in[3 * (mMP.mpnts_line_in_now - 1) + 0] = x;
		mMP.mbuff_lines_in[3 * (mMP.mpnts_line_in_now - 1) + 1] = y;
		mMP.mbuff_lines_in[3 * (mMP.mpnts_line_in_now - 1) + 2] = z;

		mXnext = x;
		mYnext = y;
		mZnext = z;

		refresh();

	}
}

//------------------------------------------------------------------------------
void Tools::end_polygon()
{
	int type;

	mMP.mbuff_lines_in[3 * (mMP.mpnts_line_in_now - 1) + 0] = mXini;
	mMP.mbuff_lines_in[3 * (mMP.mpnts_line_in_now - 1) + 1] = mYini;
	mMP.mbuff_lines_in[3 * (mMP.mpnts_line_in_now - 1) + 2] = mZini;

	type = mMP.polygon[mMP.num_polygon].type;
	mMP.types[type].array_type[mMP.types[type].num] = mMP.num_polygon;
	mMP.types[type].num++;

	mMP.num_polygon++;
	mMP.mpnts_points_union = mMP.mpnts_points_union_now;
	mMP.mpnts_line_in = mMP.mpnts_line_in_now;

}

//------------------------------------------------------------------------------
void Tools::save_polygon(Cadena cadfile)
{
	double xref, lonref, yref, latref, lat, lon;
	int index_ply = mMP.num_polygon - 1;

	mFilePolygons.open(cadfile.getCadena(), std::ios_base::out);
	if (!mFilePolygons.is_open())
	{
		cout << "Error: Creating polygon file:[" << cadfile.getCadena() << "]" <<  endl;
	}
	else
	{
		//mFilePolygons << "lat,lon"  << endl;
		for (int i = 0; i < mMP.polygon[index_ply].num_points; i++)
		{
			mQuadMap->get_ref(mMP.polygon[index_ply].buff_poly_x[i], mMP.polygon[index_ply].buff_poly_y[i], xref, lonref, yref, latref, lat, lon);
			mFilePolygons << std::setprecision(10) << lat << "," << lon << endl;
		}
		mFilePolygons.flush();
		mFilePolygons.close();
	}
}

//------------------------------------------------------------------------------
void Tools::refresh()
{
	Simulator::build_LINE_RECT_POLY(mLineSize, mMP.mbuff_lines_in, mMP.mpnts_line_in_now, mMP.mbuff_lines_out, SIM_MAX_POLYG_PNTS * 3 * SIM_MAX_POLYGONS, &mMP.mpnts_line_out);
	
	//glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(GLubyte) * mMP.mpnts_line_out, mColorBuff);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_points_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_POLYG_PNTS * SIM_MAX_POLYGONS * 3, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mMP.mpnts_points_union_now, mMP.mbuff_points_union);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_lines_surface);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_POLYG_PNTS * SIM_MAX_POLYGONS * 6, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mMP.mpnts_line_out, mMP.mbuff_lines_out);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//------------------------------------------------------------------------------
void Tools::clear()
{
	mXnext = 0;
	mYnext = 0;
	mZnext = 0;

	mXini = 0;
	mYini = 0;
	mZini = 0;

	reset_MP();
}

//------------------------------------------------------------------------------
void Tools::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	render_line_surface(viewprojectionmatrix);
	render_point(viewmatrix, viewprojectionmatrix);
}

//------------------------------------------------------------------------------
void Tools::render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	mShader_point.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextImg_p);
	glUniform1i(mTextID_p, 0);

	glUniform3f(mCameraR_p, viewmatrix[0][0], viewmatrix[1][0], viewmatrix[2][0]);
	glUniform3f(mCameraU_p, viewmatrix[0][1], viewmatrix[1][1], viewmatrix[2][1]);
	glUniformMatrix4fv(mVP_p, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	//Point outside
	glUniform1f(mSize_p, mPointSize);
	glUniform4f(mColor_p, mColor.r,mColor.g, mColor.b, mColor.a);

	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mMP.mpnts_points_union_now);
	glBindVertexArray(0);
	
	//Point inside
	glUniform1f(mSize_p, mPointSize * SIM_TOOLS_POINT_IN_PW);
	glUniform4f(mColor_p, 1.0, 1.0, 1.0, 1.0);

	glBindVertexArray(mVAO_point);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mMP.mpnts_points_union_now);
	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);

}

//------------------------------------------------------------------------------
void Tools::render_line_surface(glm::mat4& viewprojectionmatrix)
{
	mShader_line.use();

	glUniformMatrix4fv(mVP_l, 1, GL_FALSE, &viewprojectionmatrix[0][0]);
	glUniform4f(mColor_l, mColor.r, mColor.g, mColor.b, mColor.a);

	glBindVertexArray(mVAO_line);
	glDrawArrays(GL_TRIANGLES, 0, mMP.mpnts_line_out);
	glBindVertexArray(0);

}

//------------------------------------------------------------------------------
void Tools::tranform(double& x, double& y, double& z)
{
	double factor = 0.7377049;
	//cout << "----------------------------------------------------------- " << endl;
	//cout << "x " << x << endl;
	//cout << "y " << y << endl;

	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat  winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)y;
	winY = (float)viewport[3] - winY;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	//cout << "posX " << posX << endl;
	//cout << "posY " << posY << endl;
	//cout << "posZ " << posZ << endl;

	posX = posX * mCtrl->mPosition.z * factor;
	posY = posY * mCtrl->mPosition.z * factor;

	//cout << "posX " << posX << endl;
	//cout << "posY " << posY << endl;


	x = posX + mCtrl->mPosition.x;
	y = posY  * ((9.0 / 16.0))  + mCtrl->mPosition.y;
	z = 0.0;


	//cout << "posX " << x << endl;
	//cout << "posY " << y << endl;


	//cout << "mCtrl->mPosition.x " << mCtrl->mPosition.x << endl;
	//cout << "mCtrl->mPosition.y " << mCtrl->mPosition.y << endl;

}
//------------------------------------------------------------------------------
void Tools::refresh_draw()
{
	mPointSize = get_tools_point_size();
	mLineSize  = get_tools_line_size();
	if (exist_any_polygon())
	{
		refresh();
	}
}

//------------------------------------------------------------------------------
double Tools::get_tools_point_size()
{
	return (SIM_TOOLS_POINT_ZIZE * mCtrl->mPosition.z);
}

//------------------------------------------------------------------------------
double Tools::get_tools_line_size()
{
	return (SIM_TOOLS_LINE_WIDTH * mCtrl->mPosition.z);
}

//------------------------------------------------------------------------------
bool Tools::exist_type_polygon(enum eTypePolygon etype)
{
	return (mMP.types[etype].num > 0);
}

//------------------------------------------------------------------------------
void Tools::calculate_payment(double x, double x_old, double y, double y_old)
{
	mTrip = Simulator::distance(x, x_old, y, y_old) + mTrip;
	mText_km.formCadena("[%3.2f] Km", mTrip);
	mText_eu.formCadena("[%4.2f] euros", mTrip * SIM_TOLLER_TARIFA);
}

//------------------------------------------------------------------------------
bool Tools::inside_polygon(enum eTypePolygon etype, double x, double y)
{
	bool resp = false;
	int index_type;

	for (int i = 0; i < mMP.types[etype].num; i++)
	{
		index_type = mMP.types[etype].array_type[i];
		resp = (Simulator::pnpoly(mMP.polygon[index_type].num_points, mMP.polygon[index_type].buff_poly_x, mMP.polygon[index_type].buff_poly_y, x, y));
		if (resp == true)
		{
			return true;
		}
	}

	return false;
}