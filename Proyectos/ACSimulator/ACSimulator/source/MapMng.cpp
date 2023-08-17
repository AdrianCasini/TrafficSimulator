#include "MapMng.h"


//------------------------------------------------------------------------------
MapMng::MapMng()
{
	MPPBDRY = NULL;
}

//------------------------------------------------------------------------------
MapMng::~MapMng()
{
	
}

//------------------------------------------------------------------------------
void MapMng::render(glm::mat4& viewprojectionmatrix, bool flag)
{
	//---------Surfaces------------------------
	mShader_S.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureImg_forest);
	glUniform1i(mTextureID_forest, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextureImg_building);
	glUniform1i(mTextureID_building, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mTextureImg_water);
	glUniform1i(mTextureID_water, 2);

	glUniformMatrix4fv(mVP_S, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	if (MPPBDRY != NULL)
	{
		MPPBDRY->need_new_quad();
		glUniform1f(mTextureID_index, MPPBDRY->mTexture);
		glUniform3f(mColor_S, MPPBDRY->mVectColor.r, MPPBDRY->mVectColor.g, MPPBDRY->mVectColor.b);
		MPPBDRY->render(viewprojectionmatrix, flag);
	}


	for (int i = 0; i < mCountM_S; i++)
	{
		MPP_S[i]->need_new_quad();

		glUniform1f(mTextureID_index, MPP_S[i]->mTexture);
		glUniform3f(mColor_S, MPP_S[i]->mVectColor.r, MPP_S[i]->mVectColor.g, MPP_S[i]->mVectColor.b);
		MPP_S[i]->render(viewprojectionmatrix, flag);
	}

	//----------Lines File---------------------
	mShader_L.use();
	glUniformMatrix4fv(mVP_L, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	for (int i = 0; i < mCountM_L; i++)
	{
		MPP_L[i]->need_new_quad();

		glUniform3f(mColor_L, MPP_L[i]->mVectColor.r, MPP_L[i]->mVectColor.g, MPP_L[i]->mVectColor.b);
		MPP_L[i]->render(viewprojectionmatrix, flag);
	}


}

//------------------------------------------------------------------------------
void MapMng::set_color_night()
{
	if (MPPBDRY != NULL)
	{
		MPPBDRY->set_color_night();
	}
}

//------------------------------------------------------------------------------
void MapMng::set_color_day()
{
	if (MPPBDRY != NULL)
	{
		MPPBDRY->set_color_day();
	}
}

//------------------------------------------------------------------------------
void MapMng::init(Controls* ctrl, BaseDatos* pDB, Cadena cadgral)
{
	map<int, MapItem>::const_iterator it;
	mCountM_L = 0;
	mCountM_S = 0;
	mCountMDB_L = 0;
	mCountMDB_S = 0;

	mcadgral = cadgral;
	glm::vec3 clr;
	MapItem mi;

	mShader_L.inicializar(SIM_DIR_SHADERS_"mapper_line.vs",    SIM_DIR_SHADERS_"mapper_line.frag");
	mShader_S.inicializar(SIM_DIR_SHADERS_"mapper_surface.vs", SIM_DIR_SHADERS_"mapper_surface.frag");


	mVP_L      = glGetUniformLocation(mShader_L.mProgram, "mVP");
	mColor_L   = glGetUniformLocation(mShader_L.mProgram, "mColor");

	mVP_S      = glGetUniformLocation(mShader_S.mProgram, "mVP");
	mColor_S   = glGetUniformLocation(mShader_S.mProgram, "mColor");

	mTextureID_forest   = glGetUniformLocation(mShader_S.mProgram, "Texture_f");
	mTextureID_building = glGetUniformLocation(mShader_S.mProgram, "Texture_b");
	mTextureID_water    = glGetUniformLocation(mShader_S.mProgram, "Texture_w");

	mTextureID_index    = glGetUniformLocation(mShader_S.mProgram, "Txt_index");

	Simulator::texture_customize(mTextureImg_forest,   "forest2.png",  GL_REPEAT, GL_RGB, SOIL_LOAD_AUTO);
	Simulator::texture_customize(mTextureImg_building, "building.png", GL_REPEAT, GL_RGB, SOIL_LOAD_AUTO);
	Simulator::texture_customize(mTextureImg_water,    "water.jpg",    GL_REPEAT, GL_RGB, SOIL_LOAD_AUTO);


	get_file_mappers();

	for (it = mMapItem.begin(); it != mMapItem.end(); ++it)
	{
		mi = it->second;

		if (mi.source == "db")
		{
			if (mi.type == "line")
			{
				MPPDB_L[mCountMDB_L] = new MapperDB();
				MPPDB_L[mCountMDB_L]->init(ctrl, &mi, pDB);
				mCountMDB_L++;
			}
			else //surface
			{
				MPPDB_S[mCountMDB_S] = new MapperDB();
				MPPDB_S[mCountMDB_S]->init(ctrl, &mi, pDB);
				mCountMDB_S++;
			}
		}
		else if (mi.source == "file")
		{
			if (mi.type == "line")
			{
				MPP_L[mCountM_L] = new Mapper();
				MPP_L[mCountM_L]->init(ctrl, &mi, mcadgral);
				mCountM_L++;
			}
			else //surface
			{
				MPP_S[mCountM_S] = new Mapper();
				MPP_S[mCountM_S]->init(ctrl, &mi, mcadgral);
				mCountM_S++;
			}
		}
		else if (mi.source == "bound")
		{
			if (mi.type == "surface")
			{
				MPPBDRY = new MapBoundary();
				MPPBDRY->init(ctrl, &mi, mcadgral);
			}
		}
	}
}

//------------------------------------------------------------------------------
void MapMng::clearcolor()
{
	map<int, MapItem>::const_iterator it;
	MapItem mi;
	int count_fl = 0, count_fs = 0, count_dbl=0;

	get_file_mappers();

	for (it = mMapItem.begin(); it != mMapItem.end(); ++it)
	{
		mi = it->second;

		if (mi.source == "db")
		{
			if (mi.type == "line")
			{
				MPPDB_L[count_dbl]->mVectColor.r = mi.clr_r;
				MPPDB_L[count_dbl]->mVectColor.g = mi.clr_g;
				MPPDB_L[count_dbl]->mVectColor.b = mi.clr_b;
				count_dbl++;
			}
		}
		else if (mi.source == "file")
		{
			if (mi.type == "line")
			{
				MPP_L[count_fl]->mVectColor.r = mi.clr_r;
				MPP_L[count_fl]->mVectColor.g = mi.clr_g;
				MPP_L[count_fl]->mVectColor.b = mi.clr_b;
				count_fl++;
			}
		}
		else if (mi.source == "bound")
		{
			if (mi.type == "surface")
			{
				MPPBDRY->mVectColor.r = mi.clr_r;
				MPPBDRY->mVectColor.g = mi.clr_g;
				MPPBDRY->mVectColor.b = mi.clr_b;
			}
		}
	}
}

//------------------------------------------------------------------------------
void MapMng::get_file_mappers()
{
	std::string line_str;
	string substr;
	vector<Cadena> result;
	MapItem mi;
	Cadena cad, cadfile;
	int count=0;

	cadfile = mcadgral + SIM_DIR_ETC_CONFIG_"mapper.txt";

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
			if ((result[0] == "file") || (result[0] == "db") || (result[0] == "bound"))
			{
				mi.source = result[0];
				if ((result[1] == "line") || (result[1] == "surface"))
				{
					mi.type = result[1];
					mi.file = result[2];

					mi.clr_r = atof(result[3].c_str());
					mi.clr_g = atof(result[4].c_str());
					mi.clr_b = atof(result[5].c_str());

					mi.quad_sz = atof(result[6].c_str());
					mi.distance = atof(result[7].c_str());

					if (result[1] == "surface")
					{
						mi.texture = atof(result[8].c_str());
					}
					if (result[0] == "db")
					{
						mi.table = result[9];
					}

					mMapItem[count] = mi;
					count++;
				}
			}
		}
		infile.close();
	}
}