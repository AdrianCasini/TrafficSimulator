#include "PofI.h"


//------------------------------------------------------------------------------
PofI::PofI()
{
	
}

//------------------------------------------------------------------------------
PofI::~PofI()
{
	
}

//------------------------------------------------------------------------------
void PofI::init(Controls* ctrl, BaseDatos* db, Cadena cadgral)
{
	mpDB = db;
	mCtrl = ctrl;
	mcadgral = cadgral;
	mItemIndex = 0;
	mb_node_detection = false;
	mbuff_item_draw   = new double[3 * SIM_MAX_ITEM_PNTS];

	render_setup();

}

//------------------------------------------------------------------------------
void PofI::render_setup()
{

	//---VAO---VBO--POINT---------------------
	glGenVertexArrays(1, &mVAO_item);
	glBindVertexArray(mVAO_item);

	glGenBuffers(1, &mVBO_item_model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_item_model);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO_item_offset);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_item_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_ITEM_PNTS, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_item_model);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_item_offset);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);

	glBindVertexArray(0);

	//---shader----------------------------------
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

	mColor.r = 0.99;
	mColor.g = 0.22;
	mColor.b = 0.55;
	mColor.a = 0.58;

}

//------------------------------------------------------------------------------
void PofI::generate_draw(Cadena file_name)
{
	long long int nid;
	double x, y;

	mItems.clear();

	if (!mItems.get_data_from_txt(file_name))
	{
		cout << "Error: There is an error obtaining information from file:[" << file_name.getCadena() << "]" << endl;
		return;
	}

	if (mItems.mType == "DETECTORS")
	{
		for (int i = 0; i < mItems.mMaxreg; i++)
		{
			if (mItemIndex < SIM_MAX_ITEM_PNTS)
			{
				mbuff_item_draw[3 * mItemIndex + 0] = Simulator::getXfromLon(mItems.mRegtable[i].reg["LON"].num_d);
				mbuff_item_draw[3 * mItemIndex + 1] = Simulator::getYfromLat(mItems.mRegtable[i].reg["LAT"].num_d);
				mbuff_item_draw[3 * mItemIndex + 2] = 18.0;
				mItemIndex++;
			}
			else
			{
				MessageBox(NULL, file_name.getCadena(), "You have exceeded the maximun number of Point of Interest", MB_OK);
				return;
			}
		}

	}
	else if (mItems.mType == "DETECTORS_NODE")
	{
		for (int i = 0; i < mItems.mMaxreg; i++)
		{
			if (mItemIndex < SIM_MAX_ITEM_PNTS)
			{
				nid = mItems.mRegtable[i].reg["NODE"].num_li;
				Simulator::get_XY_from_node(mpDB, x,y,nid );
				mNodes[nid].num = -1;
				mNodes[nid].name = mItems.mRegtable[i].reg["NAME"].str;

				mbuff_item_draw[3 * mItemIndex + 0] = x;
				mbuff_item_draw[3 * mItemIndex + 1] = y;
				mbuff_item_draw[3 * mItemIndex + 2] = 18.0;
				mItemIndex++;
				mb_node_detection = true;
			}
			else
			{
				MessageBox(NULL, file_name.getCadena(), "You have exceeded the maximun number of Point of Interest", MB_OK);
				return;
			}
		}

	}



	refresh();
}

//------------------------------------------------------------------------------
double PofI::get_point_size()
{
	double element_size = SIM_ITEM_POINT_ZIZE * mCtrl->mPosition.z;

	if (element_size < SIM_ITEM_POINT_ZIZE)
	{
		element_size = SIM_ITEM_POINT_ZIZE;
	}

	return element_size;
}

//------------------------------------------------------------------------------
bool PofI::is_active()
{
	return (mItemIndex > 0 );
}

//------------------------------------------------------------------------------
void PofI::clear()
{
	mItemIndex = 0;
}

//------------------------------------------------------------------------------
void PofI::refresh()
{
	glBindVertexArray(mVAO_item);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_item_offset);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * (mItemIndex), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * (mItemIndex), mbuff_item_draw);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

//------------------------------------------------------------------------------
void PofI::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	render_point(viewmatrix, viewprojectionmatrix);

	if (mbuff_item_draw[2] > 0.0005)
	{
		for (int i = 0; i < mItemIndex; i++)
		{
			mbuff_item_draw[3 * i + 2] = mbuff_item_draw[3 * i + 2] - 0.15;
		}
		refresh();
	}

}

//------------------------------------------------------------------------------
void PofI::render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{

	double sz = get_point_size();
	mShader_point.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextImg_p);
	glUniform1i(mTextID_p, 0);

	glUniform3f(mCameraR_p, viewmatrix[0][0], viewmatrix[1][0], viewmatrix[2][0]);
	glUniform3f(mCameraU_p, viewmatrix[0][1], viewmatrix[1][1], viewmatrix[2][1]);
	glUniformMatrix4fv(mVP_p, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	
	//Point Start-End in
	glUniform1f(mSize_p, sz);
	glUniform4f(mColor_p, mColor.r, mColor.g, mColor.b, mColor.a);
	glBindVertexArray(mVAO_item);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mItemIndex));
	glBindVertexArray(0);

	//Point Start-End out
	glUniform1f(mSize_p, sz * SIM_TOOLS_POINT_IN_PW);
	glUniform4f(mColor_p, 1.0, 1.0, 0.2, 1.0);
	glBindVertexArray(mVAO_item);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, (mItemIndex));
	glBindVertexArray(0);


	glBindTexture(GL_TEXTURE_2D, 0);

}



