#include "MapperDB.h"


//------------------------------------------------------------------------------
MapperDB::MapperDB()
{
	
}

//------------------------------------------------------------------------------
MapperDB::~MapperDB()
{

}

//------------------------------------------------------------------------------
void MapperDB::init(Controls* ctrl, MapItem* mp, BaseDatos* pdb)
{
	mCtrl = ctrl;
	mpDB = pdb;


	if (mp->type == "line")
	{
		mType = GL_LINES;
	}
	else
	{
		mType = GL_TRIANGLES;
	}

	mVectColor.r = mp->clr_r;
	mVectColor.g = mp->clr_g;
	mVectColor.b = mp->clr_b;
	mSQsize      = double(mp->quad_sz);
	mPosZ        = mp->distance;
	mTexture     = mp->texture;
	mTable       = mp->table;
	mPosZ_BTT    = mCtrl->mPosition.z;

	mQuad_oldX = -100000;
	mQuad_oldY = -100000;

	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		mArray_point[i] = 0;
		mArray_buff[i] = NULL;
	}


	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		glGenBuffers(1, &mArray_VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
	}
	glBindVertexArray(0);

	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		mQuadsDB_name[i] = "";
		mQuadsDB_new[i] = false;
	}
	mMapQuads_DB_new.clear();
    mMapQuads_DB_old.clear();

	mMapWayNodes.clear();

	mMAXPOINTS = 200000;
	mBuffer_in  = new double[mMAXPOINTS * 3];
	mBuffer_out = new double[mMAXPOINTS * 3];
	mBufferGral = new double[mMAXPOINTS * 3 * 2];

	memset(mBuffer_in, 0, mMAXPOINTS * 3 * sizeof(double));
	memset(mBuffer_out, 0, mMAXPOINTS * 3 * sizeof(double));
	memset(mBufferGral, 0, mMAXPOINTS * 3 * 2 * sizeof(double));
}

//------------------------------------------------------------------------------
void MapperDB::render(glm::mat4& viewprojectionmatrix, bool flag)
{
	mPosZ_BTT = mCtrl->mPosition.z;
	if (flag)
	{
		mPosZ_BTT = 100;
	}


	if (mPosZ_BTT < mPosZ)
	{
		glBindVertexArray(mVAO);
		for (int i = 0; i < SIM_MAX_MAPPER; i++)
		{
			if ((mArray_point[i] != 0) && (mArray_buff[i] != NULL))
			{
				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
				glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

				glDrawArrays(mType, 0, mArray_point[i]);
				glVertexAttribDivisor(0, 0);
			}
		}
		glBindVertexArray(0);
	}
}

//------------------------------------------------------------------------------
void MapperDB::need_new_quad()
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
void MapperDB::getInfo(int quadCX, int quadCY)
{

	QuadDB quadsDB_[SIM_MAX_MAPPER];
	Cadena quadsDB_name_[SIM_MAX_MAPPER];
	QuadDB qdb;
	Cadena cad_map;
	double quadx, quady;
	int count = 0;


	for (int i = 0; i < SIM_MAX_MAPPER_X; i++)
	{
		int x = 1 + i - int(SIM_MAX_MAPPER_X / 2);

		for (int j = 0; j < SIM_MAX_MAPPER_Y; j++)
		{
			int y =   1 + j - int(SIM_MAX_MAPPER_Y / 2);
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
	for (int i = 0; i < SIM_MAX_MAPPER; i++)
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
	setup_new_info();
}

//------------------------------------------------------------------------------
void MapperDB::setup_new_info()
{
	for (int i = 0; i < SIM_MAX_MAPPER ; i++)
	{
		if ((mQuadsDB_new[i]) && (mArray_point[i] != 0) && (mArray_buff[i] != NULL))
		{
			glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * mArray_point[i], mArray_buff[i], GL_DYNAMIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mArray_point[i], mArray_buff[i]);
		}
	}
}


//------------------------------------------------------------------------------
void MapperDB::getInfo_n_db(int indx)
{
	//query DB + build Line/Surface for every quad (indx)
	int count_points = 0;

	mMapWayNodes.clear();
	get_nodes(indx, &mMapWayNodes);
	count_points = generate_lines(indx, &mMapWayNodes);

	mArray_point[indx] = 0;
	delete mArray_buff[indx];
	mArray_buff[indx] = NULL;
	mArray_buff[indx] = new double[count_points * 3 * sizeof(double)];

	memcpy(mArray_buff[indx], mBufferGral, count_points * 3 * sizeof(double));
	mArray_point[indx] = count_points;

}

//------------------------------------------------------------------------------
void MapperDB::get_nodes(int indx, map<long long, vectNode>* map_all_nodes)
{
	map<long long, int>::const_iterator it;
	vectNode vectorNodos;
	Cadena consulta;
	Node nod;
	long long wid, wid_old;
	double rx1, rx2, ry1, ry2;
	int count_ways = 0;
	int cant_nodos, min_nodes, max_nodes;
	min_nodes = 0;

	rx1 = mQuadsDB[indx].quadX - mSQsize / 2;
	rx2 = mQuadsDB[indx].quadX + mSQsize / 2;
	ry1 = mQuadsDB[indx].quadY - mSQsize / 2;
	ry2 = mQuadsDB[indx].quadY + mSQsize / 2;

	consulta.formCadena("SELECT wid, nid, norder, xlon, ylat FROM %s WHERE xlon >= %lf AND xlon <= %lf AND ylat >= %lf and ylat <= %lf ORDER BY wid, norder ASC", mTable.getCadena(), rx1, rx2, ry1, ry2);
	cout << consulta.getCadena() << endl;
	mpDB->ejecutarConsulta(consulta);


	vectorNodos.clear();
	max_nodes = mpDB->getNumRegistros();

	cant_nodos = 0;
	int i = 0;


	if (max_nodes > 0)
	{
		wid = long long(mpDB->getDouble("wid", i));
		wid_old = wid;
		nod.nid = long long(mpDB->getDouble("nid", i));
		nod.order = mpDB->getInt("norder", i);
		nod.X = (double)mpDB->getDouble("xlon", i);
		nod.Y = (double)mpDB->getDouble("ylat", i);
		vectorNodos.push_back(nod);
		cant_nodos++;
		max_nodes--;
		i++;

		do
		{
			if (cant_nodos >= min_nodes)
			{
				wid = long long(mpDB->getDouble("wid", i));
				if (wid != wid_old)
				{
					(*map_all_nodes)[wid_old] = vectorNodos;

					count_ways++;
					cant_nodos = 0;
					vectorNodos.clear();
					wid_old = wid;
				}
			}

			nod.nid = long long(mpDB->getDouble("nid", i));
			nod.order = mpDB->getInt("norder", i);
			nod.X = (double)mpDB->getDouble("xlon", i);
			nod.Y = (double)mpDB->getDouble("ylat", i);
			vectorNodos.push_back(nod);
			cant_nodos++;

			i++;
			max_nodes--;
		} while (max_nodes);


		(*map_all_nodes)[wid] = vectorNodos;
	}
	cout << "----------------------------------------------------------------------------------------------------------------" << endl;
}

//------------------------------------------------------------------------------
int MapperDB::generate_lines(int indx, map<long long, vectNode>* map_quad_nodes)
{
	MapWaysNodes::const_iterator itways;
	vectNode::const_iterator itnodes;
	int indice = 0;
	int count_points = 0;
	int total_out = 0;
	int count_ways = 0;

	memset(mBuffer_in, 0, mMAXPOINTS * 3 * sizeof(double));
	memset(mBuffer_out, 0, mMAXPOINTS * 3 * sizeof(double));
	memset(mBufferGral, 0, mMAXPOINTS * 3 * 2 * sizeof(double));

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
			mBuffer_in[3 * indice + 2] = 0.0;
			indice++;
		}
	
		Simulator::build_LINE_SIMPLE(mBuffer_in, 3, indice, mBuffer_out, mMAXPOINTS, &total_out);

		if (total_out > 1)
		{
			memcpy(ptr, mBuffer_out, total_out * 3 * sizeof(double));
			count_points = count_points + total_out;
			ptr = (ptr + total_out * 3);

			memset(mBuffer_in, 0, mMAXPOINTS * 3 * sizeof(double));
			memset(mBuffer_out, 0, mMAXPOINTS * 3 * sizeof(double));
			indice = 0;
			total_out = 0;
		}
	}
	return count_points;
}





