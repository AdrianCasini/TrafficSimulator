#include "Trip.h"

//------------------------------------------------------------------------------
Trip::Trip()
{

}

//------------------------------------------------------------------------------
Trip::~Trip()
{

}

//------------------------------------------------------------------------------
void Trip::init(BaseDatos* mpdb, Cadena cadgral)
{
	mpDB     = mpdb;
	mcadgral = cadgral;

	mArrayWays       = new(std::nothrow) long long int[2000];       if (mArrayWays == NULL)       { cout << "Can`t get enough memory at the moment" << endl;  exit(1); }
	mArrayWaysPoints = new(std::nothrow) Cadena[1000];              if (mArrayWaysPoints == NULL) { cout << "Can`t get enough memory at the moment" << endl;  exit(1); }
	mArrayPoints     = new(std::nothrow) SPoint[SIM_MAX_GPSPOINTS]; if (mArrayPoints == NULL)     { cout << "Can`t get enough memory at the moment" << endl;  exit(1); }

}

//------------------------------------------------------------------------------
bool Trip::get_trip(Cadena file, Cadena cadtype, TripInfo* trip_info)
{
	Cadena cadfile;

	if (cadtype == "FILE_WAY")
	{
		cadfile.formCadena(SIM_DIR_TEST_CT_WAY_"%s", file.getCadena());
		cadfile = mcadgral + cadfile;
		return (get_trip_way(cadfile, trip_info));
	}
	else if (cadtype == "FILE_POINT")
	{
		cadfile.formCadena(SIM_DIR_TEST_CT_PNT_"%s", file.getCadena());
		cadfile = mcadgral + cadfile;
		return (get_trip_point(cadfile, trip_info, 0));
	}
	else if (cadtype == "TRIP")
	{
		cadfile.formCadena(SIM_DIR_TEST_ROUTES_"%s", file.getCadena());
		cadfile = mcadgral + cadfile;
		return (get_trip_route(cadfile, trip_info));
	}
	else if (cadtype == "SECTION")
	{
		cadfile.formCadena(SIM_DIR_TEST_SECTIONS_"%s", file.getCadena());
		cadfile = mcadgral + cadfile;
		return (get_trip_route(cadfile, trip_info));
	}

	return false;
}

//------------------------------------------------------------------------------
bool Trip::get_trip_route(Cadena cadfile, TripInfo* trip_info)
{
	Cadena cad;
	string line;
	string substr;
	vector<Cadena> result;
	int count_points = 0;
	

	ifstream f(cadfile.getCadena());
	if (!f.is_open()) { cout << "get_trip_route: There is no file with this name:[" << cadfile.getCadena() << "]" << endl;  return false; }

	while (getline(f, line))
	{
		if ((line[0] != '#') && (line[0] != ' ') && (strcmp(line.c_str(), "") == 1))
		{
			stringstream ss(line);
			result.clear();
			while (ss.good())
			{
				getline(ss, substr, '#');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();
				result.push_back(cad);
			}

			mArrayPoints[count_points].lat   = atof(result[0].c_str());
			mArrayPoints[count_points].lon   = atof(result[1].c_str());
			mArrayPoints[count_points].Y     = atof(result[2].c_str());
			mArrayPoints[count_points].X     = atof(result[3].c_str());
			mArrayPoints[count_points].lanes = atoi(result[4].c_str());
			mArrayPoints[count_points].nid   = atoll(result[5].c_str());

			trip_info->ntrip[count_points].nid = mArrayPoints[count_points].nid;

			count_points++;

			if (count_points > SIM_MAX_GPSPOINTS)
			{
				cout << "GPS POINTS HAS REACHED THE MAXIMUN" << endl;
				break;
			}
		}
	}

	f.close();

	//-----------Building trip info------------------------

	double deltamv;
	long long int nid, nid_next;
	double  x1, x2, y1, y2, dist, angle, deltamv_x, deltamv_y, delta_lane_x, delta_lane_y;

	for (int i = 0; i < (count_points - 1); i++)
	{
		nid = i;
		nid_next = i + 1;

		deltamv = (1 / SIM_SPEED_VH);
		x1 = mArrayPoints[i].X;
		y1 = mArrayPoints[i].Y;

		x2 = mArrayPoints[i + 1].X;
		y2 = mArrayPoints[i + 1].Y;

		dist = Simulator::distance(x1, x2, y1, y2);

		angle = 0.0;
		deltamv_x = 0.0;
		deltamv_y = 0.0;
		delta_lane_x = 0.0;
		delta_lane_y = 0.0;

		bool not_angle = false;
		angle = Simulator::getAngle(x1, x2, y1, y2, not_angle);

		if (!not_angle)
		{
			deltamv_x = deltamv * cos(angle);
			deltamv_y = deltamv * sin(angle);
		}

		delta_lane_x = SIM_HIGHWAY_W * cos(angle - (SIM_PI / 2.0));
		delta_lane_y = SIM_HIGHWAY_W * sin(angle - (SIM_PI / 2.0));


		trip_info->ntrip[i].angle = angle;
		trip_info->ntrip[i].dist = dist;
		trip_info->ntrip[i].deltamv = deltamv;
		trip_info->ntrip[i].deltamv_x = deltamv_x;
		trip_info->ntrip[i].deltamv_y = deltamv_y;
		trip_info->ntrip[i].delta_lane_x = delta_lane_x;
		trip_info->ntrip[i].delta_lane_y = delta_lane_y;
		trip_info->ntrip[i].lanes = mArrayPoints[i].lanes;
		trip_info->ntrip[i].lat = mArrayPoints[i].lat;
		trip_info->ntrip[i].lon = mArrayPoints[i].lon;
		trip_info->ntrip[i].X = mArrayPoints[i].X;
		trip_info->ntrip[i].Y = mArrayPoints[i].Y;
		trip_info->ntrip[i].nid = mArrayPoints[i].nid;

	}
	trip_info->max_nodos = count_points;

	get_way_info_from_nodes(trip_info);
	return true;

}

//------------------------------------------------------------------------------
void Trip::get_way_info_from_nodes(TripInfo* trip_info)
{
	Cadena query, name;
	int count_ways = 0;
	long long int wid_old, wid = 0;

	//---- Get possible Ways from every Node-------------------------
	for (int i = 0; i < (trip_info->max_nodos); i++)
	{
		query.formCadena("SELECT wid FROM Way_nodes WHERE nid = %lld ORDER by ver ASC", trip_info->ntrip[i].nid);
		mpDB->ejecutarConsulta(query);
		count_ways = mpDB->getNumRegistros();
		if (count_ways == 0)
		{
			trip_info->ntrip[i].max_ways = 0;
			trip_info->ntrip[i].array_way_links = NULL;
		}
		else
		{
			trip_info->ntrip[i].array_way_links = new NodeWayLink[count_ways];
			trip_info->ntrip[i].max_ways = count_ways;
			for (int j = 0; j < count_ways; j++)
			{
				trip_info->ntrip[i].array_way_links[j].wid = long long(mpDB->getDouble("wid", j));
			}
		}
	}

	//---- Match ways between Nodes--------------------------------
	for (int i = 0; i < (trip_info->max_nodos - 1); i++)
	{
		trip_info->ntrip[i].wid = 0;
		for (int j = 0; j < trip_info->ntrip[i].max_ways; j++)
		{
			for (int s = 0; s < trip_info->ntrip[i+1].max_ways; s++)
			{
				if (trip_info->ntrip[i].array_way_links[j].wid == trip_info->ntrip[i + 1].array_way_links[s].wid)
				{
					trip_info->ntrip[i].wid = trip_info->ntrip[i].array_way_links[j].wid;
					break;
				}
			}
		}
	}

	//---- Get informaction from every Way-------------------------
	wid_old = 0 ;
	for (int i = 0; i < (trip_info->max_nodos - 1); i++)
	{

		if (wid_old != trip_info->ntrip[i].wid)
		{
			wid_old = trip_info->ntrip[i].wid;

			//--------------------- LANES of the way--------------------------------------------------------------------
			query.formCadena("SELECT value FROM Way_tags WHERE wid = %lld AND key_tag = 'lanes'", trip_info->ntrip[i].wid);
			mpDB->ejecutarConsulta(query);
			if (mpDB->getNumRegistros() == 0)
			{
				trip_info->ntrip[i].lanes = 1;
			}
			else
			{
				trip_info->ntrip[i].lanes = mpDB->getInt("value", 0);
			}

			//--------------------- NAME of the way----------------------------------------------------------------------
			query.formCadena("SELECT value FROM Way_tags WHERE wid = %lld AND key_tag = 'name'", trip_info->ntrip[i].wid);
			mpDB->ejecutarConsulta(query);
			if (mpDB->getNumRegistros() == 0)
			{
				strcpy(trip_info->ntrip[i].name, "");
				trip_info->ntrip[i].name_long = 0;
			}
			else
			{
				name = mpDB->getString("value", 0);
				trip_info->ntrip[i].name_long = name.getLong();
				strcpy(trip_info->ntrip[i].name, name.getCadena());
				trip_info->ntrip[i].name[name.getLong()] = '\0';
			}

			//--------------------- HIGHWAY of the way--------------------------------------------------------------------
			query.formCadena("SELECT value FROM Way_tags WHERE wid = %lld AND key_tag = 'highway'", trip_info->ntrip[i].wid);
			mpDB->ejecutarConsulta(query);
			if (mpDB->getNumRegistros() == 0)
			{
				trip_info->ntrip[i].hwtype = SIM_ESCAPE;
			}
			else
			{
				trip_info->ntrip[i].hwtype = Simulator::get_highway_type(mpDB->getString("value", 0));
			}
		}
		else
		{
			//lanes
			trip_info->ntrip[i].lanes = trip_info->ntrip[i - 1].lanes;
			//name
			strncpy(trip_info->ntrip[i].name, trip_info->ntrip[i - 1].name, trip_info->ntrip[i - 1].name_long);
			trip_info->ntrip[i].name[trip_info->ntrip[i - 1].name_long] = '\0';
			trip_info->ntrip[i].name_long = trip_info->ntrip[i - 1].name_long;
			//highway type
			trip_info->ntrip[i].hwtype = trip_info->ntrip[i - 1].hwtype;
		}
	}
}

//------------------------------------------------------------------------------
bool Trip::get_trip_point(Cadena cadfile, TripInfo* trip_info, int count_nodes_ext)
{
	//--------------------------------- Get ways Info from File---------------------------------
	Cadena cad;
	string line;
	string substr;
	vector<Cadena> result;
	int count_points = 0;
		

	ifstream f(cadfile.getCadena());
	if (!f.is_open()) { cout << "There is no file with this name:[" << cadfile.getCadena() << endl;  return false; }

	while (getline(f, line))
	{
		if ((line[0] != '#') && (line[0] != ' ') && (strcmp(line.c_str(),"") == 1))
		{
			stringstream ss(line);
			result.clear();
			while (ss.good())
			{
				getline(ss, substr, '#');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();
				result.push_back(cad);
			}
			mArrayPoints[count_points].lat = atof(result[0].c_str());
			mArrayPoints[count_points].lon = atof(result[1].c_str());
			mArrayPoints[count_points].Y   = atof(result[2].c_str());
			mArrayPoints[count_points].X   = atof(result[3].c_str());

			count_points++;

			if (count_points > SIM_MAX_GPSPOINTS)
			{
				cout << "GPS POINTS HAS REACHED THE MAXIMUN" << endl;
				break;
			}
		}
	}
	f.close();

	//-----------Building trip info

	double deltamv;
	long long int nid, nid_next;
	double  x1,x2,y1,y2,dist,angle,deltamv_x, deltamv_y, delta_lane_x, delta_lane_y;

	for (int i = 0; i < (count_points - 1); i++)
	{
		nid = i;
		nid_next = i + 1;

		deltamv = (1 / SIM_SPEED_VH);
		x1 = mArrayPoints[i].X;
		y1 = mArrayPoints[i].Y;

		x2 = mArrayPoints[i+1].X;
		y2 = mArrayPoints[i+1].Y;

		dist = Simulator::distance(x1, x2, y1, y2);

		angle = 0.0;
		deltamv_x = 0.0;
		deltamv_y = 0.0;
		delta_lane_x = 0.0;
		delta_lane_y = 0.0;

		bool not_angle = false;
		angle = Simulator::getAngle(x1, x2, y1, y2, not_angle);

		if (!not_angle)
		{
			deltamv_x = deltamv * cos(angle);
			deltamv_y = deltamv * sin(angle);
		}

		delta_lane_x = SIM_HIGHWAY_W * cos(angle - (SIM_PI / 2.0));
		delta_lane_y = SIM_HIGHWAY_W * sin(angle - (SIM_PI / 2.0));

		int index = i + count_nodes_ext;
		trip_info->ntrip[index].angle = angle;
		trip_info->ntrip[index].dist = dist;
		trip_info->ntrip[index].deltamv = deltamv;
		trip_info->ntrip[index].deltamv_x = deltamv_x;
		trip_info->ntrip[index].deltamv_y = deltamv_y;
		trip_info->ntrip[index].delta_lane_x = delta_lane_x;
		trip_info->ntrip[index].delta_lane_y = delta_lane_y;
		trip_info->ntrip[index].lanes = 1;
		trip_info->ntrip[index].lat = mArrayPoints[i].lat;
		trip_info->ntrip[index].lon = mArrayPoints[i].lon;
		trip_info->ntrip[index].X = mArrayPoints[i].X;
		trip_info->ntrip[index].Y = mArrayPoints[i].Y;

	}

	trip_info->max_nodos = count_points;

	return true;
}

//------------------------------------------------------------------------------
bool Trip::get_trip_way(Cadena cadfile, TripInfo* trip_info)
{

	//--------------------------------- Get ways Info from File---------------------------------
	Cadena cad, cad_filename;
	string line;
	string substr;
	vector<Cadena> result;
	vector<long long int>::const_iterator itv;
	long long int wid;
	int count_ways = 0;


	ifstream f(cadfile.getCadena());
	if (!f.is_open()) { cout << "There is no file with this name:[" << cadfile.getCadena() << endl;  return false; }

	while (getline(f, line))
	{
		if ((line[0] != '#') && (line[0] != ' ') && (strcmp(line.c_str(), "") == 1))
		{

			stringstream ss(line);
			result.clear();
			while (ss.good())
			{
				getline(ss, substr, '#');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();
				result.push_back(cad);
			}
			
			wid = atoll(result[0].c_str());
			mArrayWays[count_ways] = wid;
			//cout << "wid:[" << wid << "]" << endl;

			if (wid < 0)
			{
				cad_filename = result[1].c_str();
				mArrayWaysPoints[count_ways] = cad_filename;
			}

			count_ways++;
		}
	}
	f.close();
	//---------------------------------------------------------------------------------------------

	int order = 0;
	bool way_catched = false;
	bool forward = false;
	int count_nodes = 0;
	int count_nodes_old = 0;
	trip_info->max_nodos = 0;

	for (int i = 0; i < count_ways; i++)
	{
		cout << "=====================================================" << endl;
		cout << "        [" << mArrayWays[i] << "]" << endl;
		cout << "=====================================================" << endl;

		if (mArrayWays[i] > 0)
		{
			get_way_db(mArrayWays[i], &mWayInfo);

			way_catched = false;
			forward = false;

			//Look for the direction where the next way appear
			//---------- Forward Direction Count Up---------------

			cout << "mWayInfo.max_nodos:[" << mWayInfo.max_nodos << "]" << endl;
			if (mWayInfo.max_nodos > 0)
			{

				for (int j = order; j < mWayInfo.max_nodos; j++)
				{
					trip_info->ntrip[count_nodes] = mWayInfo.array_nodo_info[j];
					trip_info->sense[count_nodes] = true;
					count_nodes++;



						//Looking for the next exit FORWARD
						if (mWayInfo.array_nodo_info[j].max_ways > 0)
						{
							for (int z = 0; z < mWayInfo.array_nodo_info[j].max_ways; z++)
							{
								if (mWayInfo.array_nodo_info[j].array_way_links[z].wid == mArrayWays[i + 1])
								{
									order = mWayInfo.array_nodo_info[j].array_way_links[z].order;
									cout << "catched new order:[" << order << "]" << endl;
									way_catched = true;
									count_nodes--;
									break;
								}
							}
						}

						if (way_catched) break;
					


				}
				if (mArrayWays[i + 1] < 0)
				{
					way_catched = true;
					count_nodes--;
					order = 0;
				}

				if (way_catched == true)
				{
					trip_info->max_nodos = count_nodes;
					count_nodes_old = count_nodes;
					cout << "FORWARD trip_info->max_nodos:[" << trip_info->max_nodos << "]" << endl;
				}
				else
				{
					count_nodes = count_nodes_old;
					way_catched = false;

					get_way_reverse(&mWayInfo, &order);


					//Looking for the next exit REVERSE
					for (int j = order; j < mWayInfo.max_nodos; j++)
					{

						trip_info->ntrip[count_nodes] = mWayInfo.array_nodo_info[j];
						trip_info->sense[count_nodes] = false;
						count_nodes++;

						if (mWayInfo.array_nodo_info[j].max_ways > 0)
						{
							for (int z = 0; z < mWayInfo.array_nodo_info[j].max_ways; z++)
							{
								if (mWayInfo.array_nodo_info[j].array_way_links[z].wid == mArrayWays[i + 1])
								{
									order = mWayInfo.array_nodo_info[j].array_way_links[z].order;
									cout << "catched new order:[" << order << "]" << endl;
									way_catched = true;
									count_nodes--;
									break;
								}
							}
						}

						if (way_catched) break;

					}

					if (way_catched == true)
					{
						trip_info->max_nodos = count_nodes;
						count_nodes_old = count_nodes;
						cout << "REVERSE trip_info->max_nodos:[" << trip_info->max_nodos << "]" << endl;
					}
					else
					{
						cout << "Can't find it to the right or to the left" << endl;
						order = 0;
						i++;
					}
				}


			}
			else
			{
				cout << "Unknown" << endl;
				order = 0;
				i++;
			}
		}
		else
		{
			int count_nodes_temp = trip_info->max_nodos;
			get_trip_point(mArrayWaysPoints[i], trip_info, count_nodes);
			trip_info->max_nodos = trip_info->max_nodos + count_nodes_temp -1;
			count_nodes = trip_info->max_nodos;
			order = 0;
			cout << "count_nodes[" << count_nodes << "]" << endl;

		}
	}

	return true;
}

//------------------------------------------------------------------------------
void Trip::get_way_reverse(WayInfo* way_info, int* order)
{
	/*typedef struct
	{
		long long int wid;
		enumHWType    hwtype;
		int           lanes;

		int           max_nodos;
		char          name[100];
		int           name_long;
		char          href[100];
		int           href_long;

		NodoInfo* array_nodo_info;
	} WayInfo;*/


	int count = 0;

	for (int i = way_info->max_nodos-1; i > 0; i--)
	{
		mNodoInfo[count].angle        = way_info->array_nodo_info[i - 1].angle + SIM_PI;
		mNodoInfo[count].deltamv      = way_info->array_nodo_info[i - 1].deltamv;
		mNodoInfo[count].deltamv_x    = -way_info->array_nodo_info[i - 1].deltamv_x;
		mNodoInfo[count].deltamv_y    = -way_info->array_nodo_info[i - 1].deltamv_y;
		mNodoInfo[count].delta_lane_x = -way_info->array_nodo_info[i - 1].delta_lane_x;
		mNodoInfo[count].delta_lane_y = -way_info->array_nodo_info[i - 1].delta_lane_y;
		mNodoInfo[count].dist         = way_info->array_nodo_info[i - 1].dist;
		mNodoInfo[count].lanes        = way_info->array_nodo_info[i - 1].lanes;


		mNodoInfo[count].lat = way_info->array_nodo_info[i].lat;
		mNodoInfo[count].lon = way_info->array_nodo_info[i].lon;
		mNodoInfo[count].X = way_info->array_nodo_info[i].X;
		mNodoInfo[count].Y = way_info->array_nodo_info[i].Y;
		mNodoInfo[count].order = count;
		mNodoInfo[count].array_way_links = way_info->array_nodo_info[i].array_way_links;
		mNodoInfo[count].nid = way_info->array_nodo_info[i].nid;
		mNodoInfo[count].max_ways = way_info->array_nodo_info[i].max_ways;

		count++;
	}

	mNodoInfo[count].lat = way_info->array_nodo_info[0].lat;
	mNodoInfo[count].lon = way_info->array_nodo_info[0].lon;
	mNodoInfo[count].X = way_info->array_nodo_info[0].X;
	mNodoInfo[count].Y = way_info->array_nodo_info[0].Y;
	mNodoInfo[count].order = count;
	mNodoInfo[count].array_way_links = way_info->array_nodo_info[0].array_way_links;
	mNodoInfo[count].nid = way_info->array_nodo_info[0].nid;
	mNodoInfo[count].max_ways = way_info->array_nodo_info[0].max_ways;

	*order = way_info->max_nodos - 1 - (*order);


	for (int i = 0; i < way_info->max_nodos;  i++)
	{
		way_info->array_nodo_info[i] = mNodoInfo[i];
	}


}

//------------------------------------------------------------------------------
bool Trip::get_way_db(long long wid, WayInfo* way_info)
{
	/*
	typedef struct
	{
		long long int wid;
		enumHWType    hwtype;
		int           lanes;

		int           max_nodos;
		char          name[100];
		int           name_long;
		char          href[100];
		int           href_long;

		NodoInfo      *array_nodo_info;
	}
	*/


	map<long long int, Way>::const_iterator itm;
	Cadena consulta;
	NodoInfo nod;
	long long nid = 0;
	long long nid_next;
	long long wid_link;

	double x1, x2, y1, y2, deltamv_x, deltamv_y;
	double angle;
	double deltamv, dist;
	double delta_lane_x = 0, delta_lane_y = 0;

	int cant_ways = 0;
	int lanes = 1;
	int w_norder = 0;

	way_info->hwtype = enumHWType(SIM_ESCAPE);
	way_info->wid = wid;

	//-----------------Obtaining Lanes of a way-------------------------------
	consulta.formCadena("SELECT wid, value from Way_tags WHERE key_tag = 'lanes' AND wid = %lld", wid);
	mpDB->ejecutarConsulta(consulta);
	for (int i = 0; i < mpDB->getNumRegistros(); i++)
	{
		lanes = mpDB->getInt("value", i);
	}
	way_info->lanes = lanes;
	//cout << "ways/lanes:[" << wid << "]:[" << lanes << "]" << endl;

	//-----------------Obtaining Name of a way-------------------------------
	consulta.formCadena("SELECT wid, value from Way_tags WHERE key_tag = 'name' AND wid = %lld", wid);
	Cadena name;
	mpDB->ejecutarConsulta(consulta);
	for (int i = 0; i < mpDB->getNumRegistros(); i++)
	{
		name = mpDB->getString("value", i);
	}

	if (name.getLong() < 100)
	{
		strncpy(way_info->name, name.getCadena(), name.getLong());

		way_info->name_long = name.getLong();
	}
	else
	{
		strncpy(way_info->name, name.getCadena(), 99);
		way_info->name_long = 99;
	}

	//cout << "Name:[" << wid << "]:[" << name.getCadena() << "]" << endl;


	//-----------------Obtaining Ref of a way-------------------------------
	consulta.formCadena("SELECT wid, value from Way_tags WHERE key_tag = 'ref' AND wid = %lld", wid);
	Cadena href;
	mpDB->ejecutarConsulta(consulta);
	for (int i = 0; i < mpDB->getNumRegistros(); i++)
	{
		href = mpDB->getString("value", i);
	}

	if (href.getLong() < 100)
	{
		strncpy(way_info->href, href.getCadena(), href.getLong());
		way_info->href_long = href.getLong();
	}
	else
	{
		strncpy(way_info->href, href.getCadena(), 99);
		way_info->href_long = 99;
	}

	//cout << "Ref:[" << wid << "]:[" << href.getCadena() << "]" << endl;


	//-----------------Obtaining nodes from way-------------------------------
	//cout << "Obtaining nodes from way" << endl;
	consulta.formCadena("SELECT Way_nodes.wid, Way_nodes.norder, Way_nodes.nid, Node_pos.lon, Node_pos.lat FROM Way_nodes LEFT OUTER JOIN Node_pos ON(Way_nodes.nid = Node_pos.nid) WHERE Way_nodes.wid = %lld ORDER BY Way_nodes.norder ASC", wid);
	mpDB->ejecutarConsulta(consulta);
	way_info->max_nodos = mpDB->getNumRegistros();

	//cout << "Max Nodos:[" << wid << "]:[" << mWayInfo->max_nodos << "]" << endl;
	way_info->array_nodo_info = new NodoInfo[way_info->max_nodos];
	for (int i = 0; i < way_info->max_nodos; i++)
	{

		nod.nid = long long(mpDB->getDouble("nid", i));
		nod.order = mpDB->getInt("norder", i);
		nod.lon = mpDB->getDouble("lon", i);
		nod.lat = mpDB->getDouble("lat", i);
		nod.X = Simulator::getXfromLon(nod.lon);
		nod.Y = Simulator::getYfromLat(nod.lat);
		nod.lanes = lanes;

		way_info->array_nodo_info[i].nid = nod.nid;
		way_info->array_nodo_info[i].order = nod.order;
		way_info->array_nodo_info[i].lon = nod.lon;
		way_info->array_nodo_info[i].lat = nod.lat;
		way_info->array_nodo_info[i].X = nod.X;
		way_info->array_nodo_info[i].Y = nod.Y;
		way_info->array_nodo_info[i].lanes = nod.lanes;

		//cout << "----->Nodo:[" << way_info->array_nodo_info[i].nid << "]" << endl;

	}


	for (int i = 0; i < way_info->max_nodos; i++)
	{
		way_info->array_nodo_info[i].max_ways = 0;
		consulta.formCadena("SELECT * FROM Node_ways WHERE nid =  \'%lld\'", way_info->array_nodo_info[i].nid);
		mpDB->ejecutarConsulta(consulta);


		for (int j = 0; j < mpDB->getNumRegistros(); j++)
		{
			wid_link = long long(mpDB->getDouble("wid", j));

			//itm = mMapWaysFree_ALL.find(wid_link);
			if (wid != wid_link) //&& (itm != mMapWaysFree_ALL.end()))
			{
				way_info->array_nodo_info[i].max_ways++;
			}
		}

		//cout << "----------->array_way_links NID:[" << mWayInfo->array_nodo_info[i].nid << "] [" << mWayInfo->array_nodo_info[i].max_ways << "]" << endl;

		if (way_info->array_nodo_info[i].max_ways != 0)
		{
			way_info->array_nodo_info[i].array_way_links = new NodeWayLink[way_info->array_nodo_info[i].max_ways];

			int count = 0;
			for (int j = 0; j < mpDB->getNumRegistros(); j++)
			{
				wid_link = long long(mpDB->getDouble("wid", j));
				w_norder = mpDB->getInt("w_norder", j);

				//itm = mMapWaysFree_ALL.find(wid_link);
				if (wid != wid_link) // && (itm != mMapWaysFree_ALL.end()))
				{

					way_info->array_nodo_info[i].array_way_links[count].wid = wid_link;
					way_info->array_nodo_info[i].array_way_links[count].order = w_norder;

					way_info->array_nodo_info[i].array_way_links[count].max_nodos = 0;// mMapWaysFree_ALL[wid_link].max_nodos;
					way_info->array_nodo_info[i].array_way_links[count].lanes = 0;//mMapWaysFree_ALL[wid_link].lanes;
					//way_info->array_nodo_info[i].array_way_links[count].hwtype = 0;//mMapWaysFree_ALL[wid_link].etype;
					//way_info->array_nodo_info[i].array_way_links[count].sense = 0;//mMapWaysFree_ALL[wid_link].eSense;

					//cout << "------------->array_way_links:[" << mWayInfo->array_nodo_info[i].array_way_links[count].wid << "]" << endl;
					count++;

				}
			}
		}
	}




	//Becareful: The last has not a following node
	for (int i = 0; i < (way_info->max_nodos - 1); i++)
	{
		nid = way_info->array_nodo_info[i].nid;
		nid_next = way_info->array_nodo_info[i + 1].nid;

		deltamv = (1 / SIM_SPEED_VH);
		x1 = way_info->array_nodo_info[i].X;
		y1 = way_info->array_nodo_info[i].Y;

		x2 = way_info->array_nodo_info[i + 1].X;
		y2 = way_info->array_nodo_info[i + 1].Y;

		dist = Simulator::distance(x1, x2, y1, y2);

		angle = 0;
		deltamv_x = 0;
		deltamv_y = 0;
		delta_lane_x = 0;
		delta_lane_y = 0;

		bool not_angle = false;
		angle = Simulator::getAngle(x1, x2, y1, y2, not_angle);

		if (!not_angle)
		{
			deltamv_x = deltamv * cos(angle);
			deltamv_y = deltamv * sin(angle);
		}

		delta_lane_x = SIM_HIGHWAY_W * cos(angle - (SIM_PI / 2.0));
		delta_lane_y = SIM_HIGHWAY_W * sin(angle - (SIM_PI / 2.0));

		way_info->array_nodo_info[i].angle = angle;
		way_info->array_nodo_info[i].dist = dist;
		way_info->array_nodo_info[i].deltamv = deltamv;
		way_info->array_nodo_info[i].deltamv_x = deltamv_x;
		way_info->array_nodo_info[i].deltamv_y = deltamv_y;
		way_info->array_nodo_info[i].delta_lane_x = delta_lane_x;
		way_info->array_nodo_info[i].delta_lane_y = delta_lane_y;

	}

	return true;
		
}

//------------------------------------------------------------------------------
bool Trip::get_way(long long wid, WayInfo* way_info)
{
	/*
	Cadena cadpath;
	Cadena cadname;
	Cadena cadhref;
	int r = 0;


	//XXXXXXXXXXXXXXX  OBSOLETO
	//cadpath.formCadena(SIM_DIR_WAYBINES_"way_%lld", wid);
	mpFichero = fopen(cadpath.getCadena(), "rb");

	if (mpFichero == NULL)
	{ 
		cout << "Error: Opening:[" << cadpath.getCadena() << "]" << endl;
		return false; 
	}
	else
	{
		try
		{
			r = fread(way_info, sizeof(WayInfo), 1, mpFichero);
			cadname.setCharArray(way_info->name, way_info->name_long);
			cadhref.setCharArray(way_info->href, way_info->href_long);

			cout << "Way ID:[" << wid << "]" << endl;
			cout << "-->lanes:[" << way_info->lanes << "]" << endl;
			cout << "-->name:[" << cadname.getCadena() << "]" << endl;
			cout << "-->href:[" << cadhref.getCadena() << "]" << endl;
			cout << "-->hwtype:[" << enumHWType (way_info->hwtype) << "]" << endl;
			cout << "-->max_nodos:[" << way_info->max_nodos << "]" << endl;
			cout << "-->|" << endl;
			cout << "-->|" << endl;

			way_info->array_nodo_info = new NodoInfo[way_info->max_nodos];

			for (int i = 0; i < way_info->max_nodos; i++)
			{
				r = fread(&way_info->array_nodo_info[i], sizeof(NodoInfo), 1, mpFichero);
				cout << "    -->Nodo:[" << way_info->array_nodo_info[i].nid << "] Max ways: [" << way_info->array_nodo_info[i].max_ways << "]" << endl;

				if (way_info->array_nodo_info[i].max_ways != 0)
				{
					way_info->array_nodo_info[i].array_way_links = new NodeWayLink[way_info->array_nodo_info[i].max_ways];

					for (int j = 0; j < way_info->array_nodo_info[i].max_ways; j++)
					{
						r = fread(&way_info->array_nodo_info[i].array_way_links[j], sizeof(NodeWayLink), 1, mpFichero);
						cout << "       |-->way:[" << way_info->array_nodo_info[i].array_way_links[j].wid << "]" << endl;
						cout << "       |-->order:[" << way_info->array_nodo_info[i].array_way_links[j].order << "]" << endl;
					}
				}
			}
		}
		catch (...)
		{
			cout << "Error: Reading:[" << cadpath.getCadena() << "]" << endl;
			return false;
		}
	}
	fclose(mpFichero);


	*/

	return true;



}



