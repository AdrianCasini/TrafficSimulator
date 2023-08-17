#include "WayRender.h"


//------------------------------------------------------------------------------
WayRender::WayRender()
{
	
}

//------------------------------------------------------------------------------
WayRender::~WayRender()
{

}

//------------------------------------------------------------------------------
void WayRender::init(Controls* ctrl, MapItem* mp, BaseDatos* pdb)
{
	mCtrl = ctrl;
	mpDB = pdb;


	if (mp->type == "line")
	{
		mType = GL_LINES;
		mDrawType = "line";
	}
	else if (mp->type == "surface")
	{
		mType = GL_TRIANGLES;
		mDrawType = "surface";
	}
	else if(mp->type == "lane")
	{
		mType = GL_TRIANGLES;
		mDrawType = "lane";
	}
	else
	{
		mType = GL_TRIANGLES;
		mDrawType = "building";
	}

	mVectColor.r = mp->clr_r;
	mVectColor.g = mp->clr_g;
	mVectColor.b = mp->clr_b;
	mSQsize      = double(mp->quad_sz);
	mPosZ        = mp->distance;
	mTexture     = mp->texture;
	mTable       = mp->table;
	mHeight      = mp->height;
	mWidth       = mp->width;
	
	mPosZ_BTT    = mCtrl->mPosition.z;

	mQuad_oldX = -100000;
	mQuad_oldY = -100000;

	mMAXPOINTS    = 100000;
	mMAXWAYPoints = 3000;

	for (int i = 0; i < SIM_MAX_MAPPER_DB; i++)
	{
		mArray_point[i] = 0;
		mArray_buff[i]  = NULL;
	}

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	for (int i = 0; i < SIM_MAX_MAPPER_DB; i++)
	{
		glGenBuffers(1, &mArray_VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
		//glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * mMAXPOINTS, mArray_buff[i], GL_STREAM_DRAW);
	}
	glBindVertexArray(0);

	for (int i = 0; i < SIM_MAX_MAPPER_DB; i++)
	{
		mQuadsDB_name[i] = "";
		mQuadsDB_new[i] = false;
	}
	mMapQuads_DB_new.clear();
    mMapQuads_DB_old.clear();

	mMapWayNodes.clear();
	//cout << "iniciando WRM" << endl;

	mBuffer_in  = new double[mMAXWAYPoints * 4];
	mBuffer_out = new double[mMAXWAYPoints * 4 * 30];
	mBufferGral = new double[mMAXPOINTS * 3];

	m3D = false;
	 
	if ((mBuffer_in == NULL) || (mBuffer_out == NULL) || (mBufferGral == NULL))
	{
		cout << "WayRender::init:Not enough memory to process info" << endl;
		exit(1);
	}

	mMutex = CreateMutex(NULL,FALSE,NULL);
	if (mMutex == NULL) {cout << "WayRender::init:CreateMutex error" << endl;}

}

//------------------------------------------------------------------------------
void WayRender::render(glm::mat4& viewprojectionmatrix, bool flag)
{
	DWORD dwWaitResult;

	mPosZ_BTT = mCtrl->mPosition.z;
	if (flag)
	{
		mPosZ_BTT = 100;
	}

	glEnable(GL_POLYGON_SMOOTH );

	if (mPosZ_BTT < mPosZ)
	{

		dwWaitResult = WaitForSingleObject(mMutex, INFINITE);
		switch (dwWaitResult)
		{
			case WAIT_OBJECT_0:

				glBindVertexArray(mVAO);
	
				for (int i = 0; i < SIM_MAX_MAPPER_DB; i++)
				{
					try
					{
						if (/*(mQuadsDB_new[i]) && */ (mArray_point[i] != 0) && (mArray_buff[i] != NULL))
						{
							glEnableVertexAttribArray(0);
							glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
							glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * mArray_point[i], mArray_buff[i], GL_STREAM_DRAW);
							glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
							glVertexAttribDivisor(0, 0);
							if (mDrawType == "building") glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							glDrawArrays(mType, 0, mArray_point[i]);
							if (mDrawType == "building") glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						}
						else
						{
							//
						}
					}
					catch (...)
					{
						cout << "Error:WayRender::render" << endl;
					}
				}
	
				glBindVertexArray(0);
				break;
			default:
				cout << "WayRender::Unexpected behaviour of mutex" << endl;
				break;

		}	
		ReleaseMutex(mMutex);
	}
}

//------------------------------------------------------------------------------
void WayRender::need_new_quad()
{
	if (mCtrl->mPosition.z < mPosZ)
	{
		int quad_center_x = int(mCtrl->mPosition.x / (mSQsize)); if (mCtrl->mPosition.x >= 0) { quad_center_x = quad_center_x + 1; }
		int quad_center_y = int(mCtrl->mPosition.y / (mSQsize)); if (mCtrl->mPosition.y <= 0) { quad_center_y = quad_center_y - 1; }

		if (mQuad_oldX != quad_center_x)
		{
			getInfo(quad_center_x, quad_center_y);
			mQuad_oldX = quad_center_x;
		}
		else if (mQuad_oldY != quad_center_y)
		{
			getInfo(quad_center_x, quad_center_y);
			mQuad_oldY = quad_center_y;
		}
	}
}

//------------------------------------------------------------------------------
void WayRender::refresh()
{
	if (mCtrl->mPosition.z < mPosZ)
	{
		int quad_center_x = int(mCtrl->mPosition.x / (mSQsize)); if (mCtrl->mPosition.x >= 0) { quad_center_x = quad_center_x + 1; }
		int quad_center_y = int(mCtrl->mPosition.y / (mSQsize)); if (mCtrl->mPosition.y <= 0) { quad_center_y = quad_center_y - 1; }
		getInfo(quad_center_x, quad_center_y);
	}
}

//------------------------------------------------------------------------------
void WayRender::getInfo(int quadCX, int quadCY)
{

	QuadDB quadsDB_[SIM_MAX_MAPPER_DB];
	Cadena quadsDB_name_[SIM_MAX_MAPPER_DB];
	QuadDB qdb;
	Cadena cad_map;
	double quadx, quady;
	int count = 0;


	for (int i = 0; i < SIM_MAX_MAPPER_DB_X; i++)
	{
		int x = 1 + i - int(SIM_MAX_MAPPER_DB_X / 2);

		for (int j = 0; j < SIM_MAX_MAPPER_DB_Y; j++)
		{
			int y =   1 + j - int(SIM_MAX_MAPPER_DB_Y / 2);
			quadx = quadCX * mSQsize - x * mSQsize;
			quady = quadCY * mSQsize + y * mSQsize;

			cad_map.formCadena("%lf_%lf", quadx, quady);
			
			qdb.quadX = quadx;
			qdb.quadY = quady;

			mMapQuads_DB_new[cad_map] = 9999;
			if (mMapQuads_DB_old[cad_map] != 9999)
			{
				quadsDB_name_[count] = cad_map;
				quadsDB_[count]      = qdb;

				count++;
			}
		}
	}

	mMapQuads_DB_old.clear();
	mMapQuads_DB_old = mMapQuads_DB_new;

	count = 0;
	for (int i = 0; i < SIM_MAX_MAPPER_DB; i++)
	{
		//Existia?
		if (mMapQuads_DB_new[mQuadsDB_name[i]] != 9999)
		{
			//Pick one file brand new
			mQuadsDB_name[i] = quadsDB_name_[count];
			mQuadsDB[i]      = quadsDB_[count];
			count++;

		    getInfo_n_db(i);
			mQuadsDB_new[i] = true;
		}
	}

	mMapQuads_DB_new.clear();
}

//------------------------------------------------------------------------------
void WayRender::getInfo_n_db(int indx)
{
	//query DB + build Line/Surface for every quad (indx)
	DWORD dwWaitResult;
	int count_points = 0;

	mMapWayNodes.clear();
	get_nodes(indx, &mMapWayNodes);

	if (mType == GL_LINES)
	{
		count_points = generate_lines(indx, &mMapWayNodes);
	}
	else
	{
		count_points = generate_surface(indx, &mMapWayNodes);
	}
	

	dwWaitResult = WaitForSingleObject(mMutex, INFINITE);
	switch  (dwWaitResult)
	{
		case WAIT_OBJECT_0:
			mArray_point[indx] = 0;
			delete mArray_buff[indx];
			mArray_buff[indx] = NULL;

			if (count_points < mMAXPOINTS)
			{
				try
				{
					mArray_buff[indx] = new double[count_points * 3];
				}
				catch (...)
				{
					cout << "WayRender::getInfo_n_db: Not enough memory" << endl;
					mArray_point[indx] = 0;
					delete mArray_buff[indx];
					mArray_buff[indx] = NULL;
					count_points = 0;
				}

				if (mArray_buff[indx] != NULL)
				{
					memcpy(mArray_buff[indx], mBufferGral, count_points * 3 * sizeof(double));
					mArray_point[indx] = count_points;		
				}
			}
		

			break;

		default:
			cout << "WayRender::getInfo_n_db: Unexpected behaviour of mutex" << endl;
			break;

	}
	ReleaseMutex(mMutex);
}

//------------------------------------------------------------------------------
void WayRender::get_nodes(int indx, map<long long, vectNode>* map_all_nodes)
{
	map<long long, int>::const_iterator it;
	map<long long int, int> map_ways;
	map<long long int, int>::const_iterator it_ways;
	vectNode vectorNodos;
	Cadena consulta;
	Node nod;
	long long wid;
	double rx1, rx2, ry1, ry2;
	int ver, count_ways = 0;
	int min_nodes;
	min_nodes = 0;

	rx1 = mQuadsDB[indx].quadX - mSQsize / 2;
	rx2 = mQuadsDB[indx].quadX + mSQsize / 2;
	ry1 = mQuadsDB[indx].quadY - mSQsize / 2;
	ry2 = mQuadsDB[indx].quadY + mSQsize / 2;

	consulta.formCadena("SELECT wid, ver  FROM %s WHERE xlon >= %lf AND xlon <= %lf AND ylat >= %lf and ylat <= %lf ORDER BY wid, norder ASC", mTable.getCadena(), rx1, rx2, ry1, ry2);
	//cout << consulta.getCadena() << endl;

	map_ways.clear();
	mpDB->ejecutarConsulta(consulta);
	for (int i = 0; i < mpDB->getNumRegistros(); i++)
	{
		wid = long long(mpDB->getDouble("wid", i));
		ver = int(mpDB->getDouble("ver", i));
		map_ways[wid] = ver;
	}


	for (it_ways = map_ways.begin(); it_ways != map_ways.end(); ++it_ways)
	{
		wid = it_ways->first;
		consulta.formCadena("SELECT nid, norder, ylat, xlon FROM %s WHERE wid = %lld AND ver = %d ORDER BY norder ASC", mTable.getCadena(), wid, map_ways[wid]);
		mpDB->ejecutarConsulta(consulta);
		vectorNodos.clear();

		if (mpDB->getNumRegistros() >= min_nodes)
		{

			for (int i = 0; i < mpDB->getNumRegistros(); i++)
			{
				nod.wid   = wid;
				nod.nid   = long long(mpDB->getDouble("nid", i));
				nod.order = mpDB->getInt("norder", i);
				nod.X     = mpDB->getDouble("xlon", i);
				nod.Y     = mpDB->getDouble("ylat", i);
				vectorNodos.push_back(nod);
			}

			(*map_all_nodes)[wid] = vectorNodos;
		}
	}
}

//------------------------------------------------------------------------------
int WayRender::generate_lines(int indx, map<long long, vectNode>* map_quad_nodes)
{
	MapWaysNodes::const_iterator itways;
	vectNode::const_iterator itnodes;
	int indice = 0;
	int count_points = 0;
	int total_out = 0;
	int count_ways = 0;


	double* ptr = mBufferGral;

	for (itways = map_quad_nodes->begin(); itways != map_quad_nodes->end(); ++itways)
	{
		indice = 0;
		count_ways++;

		//Cada uno de los nodos de un way
		for (itnodes = itways->second.begin(); itnodes != itways->second.end(); ++itnodes)
		{
			mBuffer_in[3 * indice + 0] = itnodes->X;
			mBuffer_in[3 * indice + 1] = itnodes->Y;
			mBuffer_in[3 * indice + 2] = mHeight;

			indice++;
			if (indice >= mMAXWAYPoints)
			{
				cout << "WayRender::generate_lines: Not enough memory to process all points" << endl;
				return 0;
				break;
			}
		}
	
	
		Simulator::build_LINE_SIMPLE(mBuffer_in, 3, indice, mBuffer_out, mMAXWAYPoints, &total_out);


		if (total_out > 0)
		{
			count_points = count_points + total_out;
			if (count_points > mMAXPOINTS)
			{
				cout << "Error WayRender::generate_lines: Not enough memory count_points:[" << count_points << "] > mMAXPOINTS:[" << mMAXPOINTS << "] mTable " << mTable.getCadena() << endl;
				return 0;// (count_points - total_out);
			}
			memcpy(ptr, mBuffer_out, total_out * 3 * sizeof(double));
			ptr = (ptr + total_out * 3);
			total_out = 0;
		}
	}

	return count_points;
}


//------------------------------------------------------------------------------
int WayRender::generate_surface(int indx, map<long long, vectNode>* map_quad_nodes)
{
	MapWaysNodes::const_iterator itways;
	vectNode::const_iterator itnodes;
	int indice = 0;
	int count_points = 0;
	int total_out = 0;
	Cadena consulta;
	int lanes;

	double* ptr = mBufferGral;

	for (itways = map_quad_nodes->begin(); itways != map_quad_nodes->end(); ++itways)
	{
		indice = 0;
		total_out = 0;
		lanes = 1;


		consulta.formCadena("SELECT wid, value FROM Way_tags WHERE wid = %lld AND key_tag = 'lanes'", itways->first);
		//cout << consulta.getCadena() << endl;
		mpDB->ejecutarConsulta(consulta);
		if (mpDB->getNumRegistros() > 0)
		{
			lanes = long long(mpDB->getInt("value", 0));
		}

		//Cada uno de los nodos de un way
		for (itnodes = itways->second.begin(); itnodes != itways->second.end(); ++itnodes)
		{
			if (indice < mMAXWAYPoints)
			{
				mBuffer_in[4 * indice + 0] = itnodes->X;
				mBuffer_in[4 * indice + 1] = itnodes->Y;
				mBuffer_in[4 * indice + 2] = mHeight;
				mBuffer_in[4 * indice + 3] = lanes;

				indice++;
			}
			else
			{
				cout << "WayRender::generate_surface: Not enough points allocations" << endl;
				break;
			}
		}

		if (mDrawType == "surface")
		{
			Simulator::build_LINE_RECT_TRG_LANES( /*mWidth*/SIM_HIGHWAY_W, mBuffer_in, indice, mBuffer_out, mMAXWAYPoints * 6, &total_out);
		}
		else if (mDrawType == "lane")
		{
			Simulator::build_LINE_RECT_DASHED(SIM_HIGHWAY_W, mBuffer_in, indice, mBuffer_out, mMAXWAYPoints * 6, &total_out);
		}
		else //"building"
		{
			if (m3D)
			{
				Simulator::build_BUILDING(mHeight, mBuffer_in, indice, mBuffer_out, mMAXWAYPoints * 6, &total_out);
			}		
		}


		if (total_out > 0)
		{
			count_points = count_points + total_out;
			if (count_points >= mMAXPOINTS)
			{
				cout << "Error WayRender::generate_surface: Not enough memory count_points:[" << count_points << "] > mMAXPOINTS:[" << mMAXPOINTS << "] mTable " << mTable.getCadena() <<  endl;
				return 0;// (count_points - total_out);
			}
			memcpy(ptr, mBuffer_out, total_out * 3 * sizeof(double));
			ptr = (ptr + total_out * 3);
		}
	}

	return count_points;
}




