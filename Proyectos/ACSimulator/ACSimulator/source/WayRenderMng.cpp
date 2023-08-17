#include "WayRenderMng.h"



//------------------------------------------------------------------------------
WayRenderMng::WayRenderMng()
{

	m3Dactive = false;

}

//------------------------------------------------------------------------------
WayRenderMng::~WayRenderMng()
{

}

//------------------------------------------------------------------------------
void WayRenderMng::get_info()
{
	while (true)
	{
		//----------Lines--------------------------
		for (int i = 0; i < mCount_L; i++)
		{
			WR_L[i]->need_new_quad();
		}

		//----------Surface------------------------
		for (int i = 0; i < mCount_S; i++)
		{
			WR_S[i]->need_new_quad();
		}

		Sleep(200);
	}
}

//------------------------------------------------------------------------------
void WayRenderMng::render(glm::mat4& viewprojectionmatrix, bool flag)
{

	mVP = &viewprojectionmatrix;
	//----------Lines ---------------------------
	mShader_L.use();
	glUniformMatrix4fv(mVP_L, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	for (int i = 0; i < mCount_L; i++)
	{
		glUniform3f(mColor_L, WR_L[i]->mVectColor.r, WR_L[i]->mVectColor.g, WR_L[i]->mVectColor.b);
		WR_L[i]->render(viewprojectionmatrix, flag);
	}

	//---------Surfaces------------------------

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureImg_forest);
	glUniform1i(mTextureID_forest, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextureImg_building);
	glUniform1i(mTextureID_building, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mTextureImg_water);
	glUniform1i(mTextureID_water, 2);

	mShader_S.use();
	glUniformMatrix4fv(mVP_S, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	for (int i = 0; i < mCount_S; i++)
	{
		glUniform1f(mTextureID_index, WR_S[i]->mTexture);
		glUniform3f(mColor_S, WR_S[i]->mVectColor.r, WR_S[i]->mVectColor.g, WR_S[i]->mVectColor.b);
		WR_S[i]->render(viewprojectionmatrix, flag);
	}
}

//------------------------------------------------------------------------------
void WayRenderMng::init(Controls* ctrl, BaseDatos* pDB, Cadena cadgral)
{
	map<int, MapItem>::const_iterator it;
	mcadgral = cadgral;

	mCount_L = 0;
	mCount_S = 0;

	glm::vec3 clr;
	MapItem mi;

	mShader_L.inicializar(SIM_DIR_SHADERS_"mapper_line.vs", SIM_DIR_SHADERS_"mapper_line.frag");
	mShader_S.inicializar(SIM_DIR_SHADERS_"mapper_surface.vs", SIM_DIR_SHADERS_"mapper_surface.frag");


	mVP_L = glGetUniformLocation(mShader_L.mProgram, "mVP");
	mColor_L = glGetUniformLocation(mShader_L.mProgram, "mColor");

	mVP_S = glGetUniformLocation(mShader_S.mProgram, "mVP");
	mColor_S = glGetUniformLocation(mShader_S.mProgram, "mColor");

	mTextureID_forest   = glGetUniformLocation(mShader_S.mProgram, "Texture_f");
	mTextureID_building = glGetUniformLocation(mShader_S.mProgram, "Texture_b");
	mTextureID_water    = glGetUniformLocation(mShader_S.mProgram, "Texture_w");

	mTextureID_index = glGetUniformLocation(mShader_S.mProgram, "Txt_index");

	Simulator::texture_customize(mTextureImg_forest,   "forest2.png",  GL_REPEAT, GL_RGB, SOIL_LOAD_AUTO);
	Simulator::texture_customize(mTextureImg_building, "building.png", GL_REPEAT, GL_RGB, SOIL_LOAD_AUTO);
	Simulator::texture_customize(mTextureImg_water,    "water.jpg",    GL_REPEAT, GL_RGB, SOIL_LOAD_AUTO);

	get_file_mappers();

	for (it = mMapItem.begin(); it != mMapItem.end(); ++it)
	{
		mi = it->second;

		if (mi.type == "line")
		{
			WR_L[mCount_L] = new WayRender();
			WR_L[mCount_L]->init(ctrl, &mi, pDB);
			mCount_L++;
		}
		else //surface
		{
			WR_S[mCount_S] = new WayRender();
			WR_S[mCount_S]->init(ctrl, &mi, pDB);
			mCount_S++;
		}
	}
}

//------------------------------------------------------------------------------
void WayRenderMng::clearcolor()
{
	map<int, MapItem>::const_iterator it;
	MapItem mi;
	int count_fl = 0, count_fs = 0, count_line = 0, count_surface = 0;

	get_file_mappers();

	for (it = mMapItem.begin(); it != mMapItem.end(); ++it)
	{
		mi = it->second;

		if (mi.type == "line")
		{
			WR_L[count_line]->mVectColor.r = mi.clr_r;
			WR_L[count_line]->mVectColor.g = mi.clr_g;
			WR_L[count_line]->mVectColor.b = mi.clr_b;
			count_line++;
		}

		if (mi.type == "surface")
		{
			WR_S[count_surface]->mVectColor.r = mi.clr_r;
			WR_S[count_surface]->mVectColor.g = mi.clr_g;
			WR_S[count_surface]->mVectColor.b = mi.clr_b;
			count_surface++;
		}
	}
}

//------------------------------------------------------------------------------
void WayRenderMng::button_pressed_render3d()
{
	if (m3Dactive == true)
	{
		m3Dactive = false;
	}
	else
	{
		m3Dactive = true;
	}

	for (int i = 0; i < mCount_S; i++)
	{
		if (WR_S[i]->mDrawType == "building")
		{
			WR_S[i]->m3D = m3Dactive;
		}
	}

}

//------------------------------------------------------------------------------
void WayRenderMng::get_file_mappers()
{
	std::string line_str;
	string substr;
	vector<Cadena> result;
	MapItem mi;
	Cadena cad, cadfile;
	int count = 0;

	cadfile = mcadgral + SIM_DIR_ETC_CONFIG_"wayrender.txt"; 

	std::ifstream infile(cadfile.getCadena());
	if (!infile.is_open()) { cout << "Error obtaining info from file:[" << cadfile.getCadena() << "]" << endl; return; }
	else
	{

		while (std::getline(infile, line_str))
		{
			stringstream ss(line_str);
			result.clear();
			while (ss.good())
			{
				getline(ss, substr, ';');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();
				result.push_back(cad);
			}
			if ((result[0] == "file") || (result[0] == "db"))
			{
				mi.source = result[0];
				if ((result[1] == "line") || (result[1] == "surface") || (result[1] == "lane") || (result[1] == "building"))
				{
					mi.type   = result[1];
					mi.file   = result[2];
					mi.clr_r  = atof(result[4].c_str());
					mi.clr_g  = atof(result[5].c_str());
					mi.clr_b  = atof(result[6].c_str());
					mi.height = atof(result[7].c_str());
					mi.width  = atof(result[8].c_str());

					mi.quad_sz = atof(result[9].c_str());
					mi.distance = atof(result[10].c_str());

					if ((result[1] == "surface") || (result[1] == "lane") || (result[1] == "building"))
					{
						mi.texture = atof(result[11].c_str());
					}

					if (result[0] == "db")
					{
						mi.table = result[12];
					}

					mMapItem[count] = mi;
					count++;
				}
			}
		}
		infile.close();
	}
}