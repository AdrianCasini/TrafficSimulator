#include "Routing.h"


//------------------------------------------------------------------------------
Routing::Routing()
{
	
}

//------------------------------------------------------------------------------
Routing::~Routing()
{
	mDB.desconectar();
}

//------------------------------------------------------------------------------
void Routing::init(Controls* ctrl, Drawing* drw, Cadena cadgral)
{
	mCtrl = ctrl;
	mDRW = drw;
	mPoint_init = false;
	mWayInfo.array_nodo_info = NULL;
	mActionActiveRoute = false;
	mActionActiveSnap  = false;
	mcadgral = cadgral;

	msCurlBuff.m_buff = new char[100000];
	msCurlBuff.m_indx = 0;


	reset_check_rep();

	if (mDB.conectar(SIM_DB_HOST, SIM_DB_PORT, SIM_DB_NAME, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true)
	{
		MessageBox(NULL, "", "There is a problem connecting with the database!", MB_OK);
	}
}

//------------------------------------------------------------------------------
void Routing::reset_check_rep()
{
	for (int i = 0; i < SIM_MAX_NODE_REP_WINDOW; i++)
	{
		mcheckRep[i] = 0;
	}
	mindex_rep = 0;
}

//------------------------------------------------------------------------------
bool Routing::is_active()
{
	return (mActionActiveRoute || mActionActiveSnap || mDRW->mPointIndex > 0 || mDRW->mPointIndex_route > 0);
}

//------------------------------------------------------------------------------
void Routing::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	mDRW->render(viewmatrix, viewprojectionmatrix);
}

//------------------------------------------------------------------------------
void Routing::add_point(double x, double y, double z)
{
	mDRW->add_point(x,y,z);
}

//------------------------------------------------------------------------------
void Routing::activate_route()
{
	mActionActiveRoute = true;
}

//------------------------------------------------------------------------------
void Routing::activate_snap()
{
	mActionActiveSnap = true;
}

//------------------------------------------------------------------------------
void Routing::deactivate_route()
{
	if (mDRW->mPointIndex >= 2)
	{
		route_from_draw();
	}
	else
	{
		MessageBox(NULL,  "The number of points have to be more than one", "Not enough points", MB_OK);
		clear();
	}
	mActionActiveRoute = false;
}

//------------------------------------------------------------------------------
void Routing::deactivate_snap()
{
	if (mDRW->mPointIndex >= 2)
	{
		snap_from_draw();
	}
	else
	{
		MessageBox(NULL, "The number of points have to be more than one", "Fewer points", MB_OK);
		clear();
	}
	mActionActiveSnap = false;
}

//------------------------------------------------------------------------------
void Routing::clear()
{
	mDRW->clear();
}

//------------------------------------------------------------------------------
void Routing::point_init(bool state)
{
	mPoint_init = state;
}

//------------------------------------------------------------------------------
void Routing::draw_route()
{
	mDRW->draw_route(&mWayInfo);
}

//------------------------------------------------------------------------------
void Routing::resize_line()
{
	mDRW->resize_line();
}


//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//                                     OSM ROUTE
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void::Routing::routes_from_file(Cadena cad_in_out)
{
	Cadena cadfileout, cadgpslist;
	Cadena cadx, cady, cadaux;
	vector<long long int> vectnodes;
	vectnodes.clear();

	cadgpslist = "";
	mRoutes.clear();
	
	if (mRoutes.get_data_from_txt(cad_in_out))
	{
		for (int i = 0; i < mRoutes.mMaxreg; i++)
		{
			cadfileout = mRoutes.mRegtable[i].reg["LABEL"].str;
			cadfileout.formCadena("%s%s%s.txt", mcadgral.getCadena(), SIM_DIR_TEST_ROUTES_, cadfileout.getCadena());

			cadgpslist = "";
			vectnodes.clear();

			for (int j = 1; j <= mRoutes.mRegtable[i].reg["NUM_POINTS"].num_i; j++)
			{
				cadx.formCadena("%s%d", "POSX", j);
				cady.formCadena("%s%d", "POSY", j);
				if (j == mRoutes.mRegtable[i].reg["NUM_POINTS"].num_i)
				{
					cadgpslist = cadgpslist + cadaux.formCadena("%lf,%lf", mRoutes.mRegtable[i].reg[cadx.getCadena()].num_d, mRoutes.mRegtable[i].reg[cady.getCadena()].num_d);
				}
				else
				{
					cadgpslist = cadgpslist + cadaux.formCadena("%lf,%lf;", mRoutes.mRegtable[i].reg[cadx.getCadena()].num_d, mRoutes.mRegtable[i].reg[cady.getCadena()].num_d);
				}
			}
			if (execute_OSM_route_service(cadgpslist))
			{
				msCurlBuff.m_buff[msCurlBuff.m_indx] = '\0';
				cout << msCurlBuff.m_buff << endl;
				get_nodes_from_osm_route_response(msCurlBuff.m_buff, msCurlBuff.m_indx, vectnodes);

				Simulator::get_info_nodes_from_vector(&mDB, &mWayInfo, vectnodes);
				cout << "Generating file:[" << cadfileout.getCadena() << "]" << endl;
				write_route(cadfileout);
			}
		}
	}
	else
	{
		cout << "Error: There is an error obtaining information from file:[" << cad_in_out.getCadena() << "]" << endl;
	}
}


//------------------------------------------------------------------------------
void Routing::routes_from_segment(Cadena cadgpslist, vector<long long int>& vectnodes)
{
	if (execute_OSM_route_service(cadgpslist))
	{
		msCurlBuff.m_buff[msCurlBuff.m_indx] = '\0';
		cout << msCurlBuff.m_buff << endl;
		get_nodes_from_osm_route_response(msCurlBuff.m_buff, msCurlBuff.m_indx, vectnodes);
	}
}

//------------------------------------------------------------------------------
void Routing::route_from_draw()
{
	Cadena cadaux, cadfileout, cadgpslist;
	vector<long long int> vectnodes;
	vectnodes.clear();

	cadfileout.formCadena("%s%sroute_%s.txt", mcadgral.getCadena(), SIM_DIR_TEST_ROUTES_, Simulator::get_timestamp_format_YYMMDDhhmmss().getCadena());
	cadgpslist = "";

	get_list_gps_points_from_draw(cadgpslist);

	if (execute_OSM_route_service(cadgpslist))
	{
		msCurlBuff.m_buff[msCurlBuff.m_indx] = '\0';
		cout << msCurlBuff.m_buff << endl;
		get_nodes_from_osm_route_response(msCurlBuff.m_buff, msCurlBuff.m_indx, vectnodes);

		Simulator::get_info_nodes_from_vector(&mDB, &mWayInfo, vectnodes);
		write_route(cadfileout);
		mDRW->draw_route(&mWayInfo);
	}
}

//------------------------------------------------------------------------------
void Routing::get_list_gps_points_from_draw(Cadena& cadgpslist)
{
	Cadena cadaux;

	for (int i = 0; i < mDRW->mPointIndex; i++)
	{
		if (i == mDRW->mPointIndex - 1) { cadaux.formCadena("%lf,%lf", mDRW->mArrayPoints[i].lon, mDRW->mArrayPoints[i].lat); }
		else { cadaux.formCadena("%lf,%lf;", mDRW->mArrayPoints[i].lon, mDRW->mArrayPoints[i].lat); }
		cadgpslist = cadgpslist + cadaux;
	}
}

//------------------------------------------------------------------------------
bool Routing::execute_OSM_route_service(Cadena cadgpslist)
{
	Cadena cadurl, cad_header;

	cad_header = "";
	cadurl.formCadena("http://router.project-osrm.org/route/v1/driving/%s?skip_waypoints=false&snapping=default&alternatives=true&overview=full&geometries=geojson&annotations=true&continue_straight=default&steps=false", cadgpslist.getCadena());

	cout << "Get info from url:[" << cadurl.getCadena() << "]" << endl;
	msCurlBuff.m_indx = 0;

	return mCurl.curl_get(cadurl, cad_header, msCurlBuff);
}

//------------------------------------------------------------------------------
void Routing::get_nodes_from_osm_route_response(char* buff, int total, vector<long long int>& vectnodes)
{
	int idx_i=0, idx_f=0;
	char cbuff[20000];
	string line, substr;
	bool inside_nodes = false, nodes_catched = false;

	//cout << "----------------------------------------------------------" << endl;
	//cout << "---------------------    NODES   -------------------------" << endl;
	for (int i = 0; i < total; i++)
	{
		//"nodes":[383818105,14452786,1445278828,1445278836,144527884....],"distance":"
		if (buff[i] == 'n')
		{
			if ((buff[i + 1] == 'o') &&
			    (buff[i + 2] == 'd') &&
			    (buff[i + 3] == 'e') &&
			    (buff[i + 4] == 's'))
			{
				i = i + 8;
				idx_i = i;
				inside_nodes = true;
			}
		}
		else if ((buff[i] == ']') && (inside_nodes))
		{
			idx_f = i;
			nodes_catched = true;
		}

		if (nodes_catched == true)
		{

			memcpy(cbuff, buff + idx_i, idx_f - idx_i);
			cbuff[idx_f - idx_i] = '\0';
			stringstream ss(cbuff);

			while (ss.good()) 
			{ 
				getline(ss, substr, ','); 
				vectnodes.push_back(atoll(substr.c_str()));
				cout << "node: [" << substr.c_str() << "]" << endl; 
			}
			//cout << "-------------" << endl;
			idx_f = 0;
			idx_i = 0;
			inside_nodes = false;
			nodes_catched = false;
		}
	}
	//cout << "----------------------------------------------------------" << endl;
}


//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//                                     SNAP ROUTE
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Routing::set_snap_files_names()
{
	//mCadTimeStamp = Simulator::get_timestamp_format_YYMMDDhhmmss();
	//mCadFileOSMResult.formCadena("%s%s%s.txt", mcadgral.getCadena(), SIM_DIR_TEST_OSM_SNP_"snap_file_result_", mCadTimeStamp.getCadena());
	//mCadFileOSMNodes.formCadena("%s%s%s.txt", mcadgral.getCadena(),  SIM_DIR_TEST_OSM_SNP_"snap_nodes_", mCadTimeStamp.getCadena());
	//mCadFileOut.formCadena("%s%ssnap_%s.txt", mcadgral.getCadena(), SIM_DIR_TEST_SNAPS_, mCadTimeStamp.getCadena());
	//mCadFileGpsPoints.formCadena("%s%s%s.txt", mcadgral.getCadena(), SIM_DIR_TEST_OSM_SNP_"snap_gps_points_", mCadTimeStamp.getCadena());
}

//------------------------------------------------------------------------------
void Routing::snap_from_draw()
{
	set_snap_files_names();

	cout << "------------------------------------------------------------------" << endl;
	//create_gpspoints_file(false, "");
	//execute_OSM_snap_service(false, "");
	//obtaing_OSM_snap_nodes();
	Simulator::get_info_nodes_from_file(&mDB, &mWayInfo, mCadFileOSMNodes.getCadena());
	draw_snap();
	write_snap(mCadFileOut);
	cout << "------------------------------------------------------------------" << endl;
}

//------------------------------------------------------------------------------
void Routing::snap_from_file(Cadena cadfile)
{
	cout << "------------------------------------------------------------------" << endl;
	Cadena cadgpspoints;
	char* GPS_points;


	GPS_points = new char[50000];


	int gps_number = 0;

	cout << cadfile.getCadena() << endl;

	get_gps_points(cadfile, GPS_points, 20000, gps_number);

	cout << GPS_points << endl;
	cout << gps_number << endl;


	execute_OSM_snap_service(GPS_points, gps_number);
	//obtaing_OSM_snap_nodes(OSM_resp);
	//Simulator::get_info_nodes(&mDB, &mWayInfo, mCadFileOSMNodes.getCadena());
	//draw_snap();
	//write_snap(mCadFileOut);
	
	cout << "------------------------------------------------------------------" << endl;

	if (GPS_points != NULL) {delete GPS_points; }

}
//------------------------------------------------------------------------------
void Routing::execute_OSM_snap_service(char* buffgpspoints, int gps_number)
{

	int block = 0, charindx_ini =0, charindx_last=0;
	vector<long long int> vectnodes;
	Cadena cadfileout, cadurl, cad_header = "";
	Cadena cadgpsblock, cadradblock;

	cout << "------------------------------------------------------------------" << endl;
	//cadfileout.formCadena("%s%s%s.txt", mcadgral.getCadena(), SIM_DIR_TEST_SNAPS_, cadroutename.getCadena());
	//cout << "Generating route for:[" << cadroutename.getCadena() << "]" << endl;

	while (block < gps_number)
	{
		
		cadgpsblock = get_gps_block(charindx_ini, charindx_last, buffgpspoints);
		charindx_ini = charindx_last;
		charindx_last = 0;
		block = block + SIM_BLOCK_POINTS_SNAP;
		//cadradblock = get_rad_block(block, gpspoints);

		cout << "[" << cadgpsblock.getCadena() << "]" << endl;

		/*cadurl.formCadena("'http://router.project-osrm.org/match/v1/car/%s?radiuses=%s&annotations=true&tidy=true&geometries=geojson&generate_hints=false'", cadgpsblock.getCadena(), cadradblock.getCadena());

		msCurlBuff.m_indx = 0;
		cout << "Get info from url:[" << cadurl.getCadena() << "]" << endl;
		if (mCurl.curl_get(cadurl, cad_header, msCurlBuff) == true)
		{
			msCurlBuff.m_buff[msCurlBuff.m_indx] = '\0';
			cout << msCurlBuff.m_buff << endl;
			get_nodes_from_osm_route_response(msCurlBuff.m_buff, msCurlBuff.m_indx, vectnodes);
		}*/
	}
	//Simulator::get_info_nodes_from_vector(&mDB, &mWayInfo, vectnodes);
	//write_route(cadfileout);
}	

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Cadena Routing::get_gps_block(int charindx_ini, int& charindx_last, char* ptr )
{
	char buff[3000];
	Cadena cadblock;
	int count_lonlat = 0, count_gps_point=0;
	int charindx_end = charindx_ini;

	cout << "[" << charindx_ini << "]" << endl;

	while (true)
	{
		if (*(ptr + charindx_end) == ',')
		{
			count_lonlat++;
			if (count_lonlat == 2)
			{
				count_gps_point++;
				count_lonlat = 0;
			}
		}
		else if (*(ptr + charindx_end) == '\0')
		{
			charindx_end++;
			charindx_last = charindx_end+1;
			break;
		}
		charindx_end++;
		charindx_last = charindx_end+1;

		if (count_gps_point >= SIM_BLOCK_POINTS_SNAP)
		{
			break;
		}

	}
	
	memset(buff, 0, 3000);
	memcpy(buff, ptr + charindx_ini, charindx_end - charindx_ini-1);
	buff[charindx_end - charindx_ini] = '\0';

	return cadblock.formCadena("%s", buff);

}

//------------------------------------------------------------------------------
Cadena Routing::get_rad_block(int block, char* ptr)
{
	return "";
}


//------------------------------------------------------------------------------
void Routing::write_route(Cadena cadfileout)
{
	map<long long int, int> mapNodoRep;
	Cadena cadaux;

	reset_check_rep();

	mFile.open(cadfileout.getCadena(), std::ios_base::out | std::ios_base::app | std::ios_base::in);
	if (!mFile.is_open()) 
	{ 
		cout << "Error: while opening file:[" << cadfileout.getCadena() << "]" << endl;
	}
	else
	{
		for (int i = 0; i < mWayInfo.max_nodos; i++)
		{
			if (!check_rep((mWayInfo.array_nodo_info)[i].nid))
			{
				mcheckRep[mindex_rep] = (mWayInfo.array_nodo_info)[i].nid;
				mindex_rep++;
				if (mindex_rep >= SIM_MAX_NODE_REP_WINDOW)
				{
					mindex_rep = 0;
				}
				cadaux.formCadena("%lf#%lf#%lf#%lf#%d#%lld\n", (mWayInfo.array_nodo_info)[i].lat, (mWayInfo.array_nodo_info)[i].lon, (mWayInfo.array_nodo_info)[i].Y, (mWayInfo.array_nodo_info)[i].X, (mWayInfo.array_nodo_info)[i].lanes, (mWayInfo.array_nodo_info)[i].nid);
				mFile << cadaux.getCadena();
			}
		}
		mFile.close();
	}
}

//------------------------------------------------------------------------------
bool Routing::check_rep(long long int nid)
{
	for (int i = 0; i < SIM_MAX_NODE_REP_WINDOW; i++)
	{
		if (nid == mcheckRep[i])
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
void Routing::create_simulation_file(Cadena cadfilesource, Cadena cadfilesimulation)
{
	Cadena cadaux;

	if (is_active())
	{
		write_route(mCadFileOutSim);

		mFile.open(cadfilesimulation.getCadena(), std::ios_base::out | std::ios_base::app | std::ios_base::in);
		if (!mFile.is_open())
		{
			cout << "Error: while opening file:[" << cadfilesimulation.getCadena() << "]" << endl;
		}
		else
		{
			mFile << "#--------------------------------------------------------------------------------------------------------------------" << endl;
			mFile << "TYPE; SOURCETRIP                                                                                                     " << endl;
			mFile << "FORMAT; STR; SOURCE_FILE; STR; SOURCE_FILE; STR; TYPE; STR; TRIP_FILE; STR; EST_IN; STR; EST_OUT; INT; ITERATION     " << endl;
			mFile << "#--------------------------------------------------------------------------------------------------------------------" << endl;

			cadfilesource = Simulator::get_filename_clean(cadfilesource);
			cadaux.formCadena("SOURCE;%s;TRIP;route_%s.txt;ent;sal;2000", cadfilesource.getCadena(), mCadTimeStamp.getCadena());
			mFile << cadaux.getCadena() << endl;
			mFile.close();

		}
	}
}

//------------------------------------------------------------------------------
void Routing::draw_snap()
{
	mDRW->draw_route(&mWayInfo);
}

//------------------------------------------------------------------------------
void Routing::write_snap(Cadena cadfileout)
{
	map<long long int, int> mapNodoRep;
	Cadena cadaux, cadexe;

	reset_check_rep();

	mFile.open(cadfileout.getCadena(), std::ios_base::out | std::ios_base::app | std::ios_base::in);
	if (!mFile.is_open())
	{
		cout << "Error: while opening file:[" << cadfileout.getCadena() << "]" << endl;
	}
	else
	{
		for (int i = 0; i < mWayInfo.max_nodos; i++)
		{
			if (!check_rep((mWayInfo.array_nodo_info)[i].nid))
			{
				mcheckRep[mindex_rep] = (mWayInfo.array_nodo_info)[i].nid;
				mindex_rep++;
				if (mindex_rep >= SIM_MAX_NODE_REP_WINDOW)
				{
					mindex_rep = 0;
				}
				cadaux.formCadena("%lf#%lf#%lf#%lf#%d#%lld\n", (mWayInfo.array_nodo_info)[i].lat, (mWayInfo.array_nodo_info)[i].lon, (mWayInfo.array_nodo_info)[i].Y, (mWayInfo.array_nodo_info)[i].X, (mWayInfo.array_nodo_info)[i].lanes, (mWayInfo.array_nodo_info)[i].nid);
				mFile << cadaux.getCadena();
			}
		}
		mFile.close();
	}
}


//------------------------------------------------------------------------------
void Routing::get_gps_points(Cadena cadfile, char* ptrgps, int max, int& gps_number)
{
	vector<Cadena> result;
	string line;
	string substr;
	Cadena cadgps;
	double lon,lat;
	char sep_char;
	int indx=0;

	gps_number = 0;

	ifstream f(cadfile.getCadena());
	if (!f.is_open())
	{
		MessageBox(NULL, cadfile.getCadena(), "You haven't picked up any file", MB_OK);
		cout << "Error while opening file:[" << cadfile.getCadena() << "]" << endl;
		return;
	}

	while (getline(f, line))
	{
		sep_char = ',';
		if (line.find_first_of('#', 0) != string::npos) { sep_char = '#'; }
		if (line.find_first_of(',', 0) != string::npos) { sep_char = ','; }
		if (line.find_first_of(';', 0) != string::npos) { sep_char = ';'; }

		if ((line[0] != '#') && (line[0] != ' '))
		{
			stringstream ss(line);
			result.clear();
			while (ss.good())
			{
				//41.123456,2.123456
				getline(ss, substr, sep_char);
				cadgps = substr.c_str();
				cadgps.rTrim();
				cadgps.lTrim();
				result.push_back(cadgps);
			}

			lat = atof(result[0].c_str());
			lon = atof(result[1].c_str());
			cadgps.formCadena("%lf,%lf,",  lat, lon );

			//std::cout << " GPS points:[" << cadgps.getCadena() << "]" << endl;
			memcpy(ptrgps + indx, cadgps.getCadena(), cadgps.getLong());
			indx = indx + cadgps.getLong();

			gps_number++;
		}


		if (gps_number >= SIM_MAX_SNAP_POINTS)
		{
			std::cout << "You have reached the maximun number of GPS points:[" << gps_number << "]" << endl;
			MessageBox(NULL, "You have reached the maximun number of GPS points", "Limit Reached", MB_OK);
			break;
		}
		else if (indx > max)
		{
			std::cout << "You have reached the maximun size of GPS points information:[" << indx << "]" << endl;
			MessageBox(NULL, "You have reached the maximun size of GPS points information", "Size Limit Reached", MB_OK);
			break;
		}
		
	}
	ptrgps[indx - 1] = '\0';//Delete the last "'" character
}

