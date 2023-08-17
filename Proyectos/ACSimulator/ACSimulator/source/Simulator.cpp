#include "Simulator.h"



//------------------------------------------------------------------------------
void Simulator::get_XY_from_node(BaseDatos* pdb,  double& x, double& y, long long int ni)
{
	double lon, lat;
	Cadena consulta;
	consulta.formCadena("SELECT * FROM Node_pos WHERE nid = %lld", ni);
	pdb->ejecutarConsulta(consulta);
	if (pdb->getNumRegistros() > 0)
	{
		lon = pdb->getDouble("lon", 0);
		lat = pdb->getDouble("lat", 0);
		x = Simulator::getXfromLon(lon);
		y = Simulator::getYfromLat(lat);

	}
	else
	{
		cout << "The following node doesn't exist:[" << ni << "]" << endl;
	}
}


//------------------------------------------------------------------------------
void Simulator::init_fonts_plus(std::map<unsigned char, Character>& mfont)
{

	FT_Library ft;
	GLuint texture;

	if (FT_Init_FreeType(&ft)) { std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl; }
	FT_Face face;
	if (FT_New_Face(ft, SIM_DIR_FONTS_"verdana.ttf", 0, &face)) { std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl; }
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);   // FT_ENCODING_NONE
	FT_Set_Pixel_Sizes(face, 0, 128);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char i = 0; i < 255; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER )) //  FT_GLYPH_FORMAT_OUTLINE
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
			continue;
		}
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
		Character character =
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		mfont[i] = character;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

//------------------------------------------------------------------------------
double Simulator::get_speed_factor_from_hwtype(enumHWType ehwtype)
{
	double factor = 1.0;

	switch (ehwtype)
	{
		case SIM_MOTORWAY:
			factor = 1.0;
			break;
		case SIM_MOTORWAY_LINK:
			factor = 0.9;
			break;
		case SIM_TRUNK:
			factor = 1.0;
			break;
		case SIM_TRUNK_LINK:
			factor = 0.8;
			break;
		case SIM_PRIMARY:
			factor = 1.0;
			break;
		case SIM_PRIMARY_LINK:
			factor = 0.8;
			break;
		case SIM_SECONDARY:
			factor = 0.80;
			break;
		case SIM_SECONDARY_LINK:
			factor = 0.70;
			break;
		case SIM_TERTIARY:
			factor = 0.70;
			break;
		case SIM_TERTIARY_LINK:
			factor = 0.65;
			break;
		case SIM_RESIDENTIAL:
			factor = 0.45;
			break;
		case SIM_UNCLASSIFIED:
			factor = 0.65;
			break;
		case SIM_SERVICE:
			factor = 0.65;
			break;
		case SIM_TRACK:
			factor = 0.35;
			break;
		case SIM_LIVING_STREET:
			factor = 0.45;
			break;
		case SIM_ROAD:
			factor = 0.45;
			break;
		case SIM_ESCAPE:
			factor = 0.45;
			break;
		default:
			factor = 1.0;
			break;
	}

	return factor;
}

//------------------------------------------------------------------------------
enumHWType Simulator::get_highway_type(Cadena cadtype)
{
	if      (cadtype == "motorway")       { return SIM_MOTORWAY;}
	else if (cadtype == "motorway_link")  { return SIM_MOTORWAY_LINK; }
	else if (cadtype == "trunk")          { return SIM_TRUNK; }
	else if (cadtype == "trunk_link")     { return SIM_TRUNK_LINK; }
	else if (cadtype == "primary")        { return SIM_PRIMARY; }
	else if (cadtype == "primary_link")   { return SIM_PRIMARY_LINK; }
	else if (cadtype == "secondary")      { return SIM_SECONDARY; }
	else if (cadtype == "secondary_link") { return SIM_SECONDARY_LINK; }
	else if (cadtype == "tertiary")       { return SIM_TERTIARY; }
	else if (cadtype == "tertiary_link")  { return SIM_TERTIARY_LINK; }
	else if (cadtype == "residential")    { return SIM_RESIDENTIAL; }
	else if (cadtype == "unclassified")   { return SIM_UNCLASSIFIED; }
	else if (cadtype == "service")        { return SIM_SERVICE; }
	else if (cadtype == "track")          { return SIM_TRACK; }
	else if (cadtype == "living_street")  { return SIM_LIVING_STREET; }
	else if (cadtype == "road")           { return SIM_ROAD; }
	else if (cadtype == "escape")         { return SIM_ESCAPE; }


	return SIM_WAY_TYPE_END;

}

//------------------------------------------------------------------------------
void Simulator::get_info_nodes_from_vector(BaseDatos* db, WayInfo* way_info, vector<long long int>& vectnodes)
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
	string line;
	string substr;
	vector<long long int> result;
	vector<long long int>::const_iterator itv;
	map<long long int, Way>::const_iterator itm;
	Cadena consulta, cadaux, name, href;
	NodoInfo nod;
	long long nid_next, wid, nid = 0;
	int lanes, count_nodes = 0;
	double x1, x2, y1, y2, deltamv_x, deltamv_y;
	double angle;
	double deltamv, dist;
	double delta_lane_x = 0, delta_lane_y = 0;

	way_info->wid = 0;
	way_info->hwtype = SIM_WAY_TYPE_END;
	way_info->lanes = 1;
	way_info->max_nodos = 0;
	strcpy(way_info->name, "");
	way_info->name_long = 0;
	strcpy(way_info->href, "");
	way_info->href_long = 0;
	if (way_info->array_nodo_info != NULL)
	{
		delete(way_info->array_nodo_info);
		way_info->array_nodo_info = NULL;
	}


	name = "";
	href = "";
	way_info->hwtype = enumHWType(SIM_ESCAPE);
	way_info->wid = 0;
	way_info->lanes = 1;
	strncpy(way_info->name, name.getCadena(), name.getLong());
	way_info->name_long = name.getLong();
	strncpy(way_info->href, href.getCadena(), href.getLong());
	way_info->href_long = href.getLong();
	way_info->max_nodos = vectnodes.size();
	way_info->array_nodo_info = new NodoInfo[way_info->max_nodos];

	//-----------------Obtaining info from node-------------------------
	cout << "Obtaining info from Node" << endl;
	int i = 0;
	for (itv = vectnodes.begin(); itv != vectnodes.end(); ++itv)
	{
		cout << "Nodos--:[" << *itv << "]" << endl;

		consulta.formCadena("SELECT * FROM Node_pos WHERE nid = %lld ORDER BY ver DESC", *itv);
		db->ejecutarConsulta(consulta);
		if (db->getNumRegistros() == 0)
		{
			cout << "No existe nodo:[" << *itv << "]" << endl;
		}
		else
		{
			nod.nid = long long(db->getDouble("nid", 0));
			nod.lon = db->getDouble("lon", 0);
			nod.lat = db->getDouble("lat", 0);
			nod.X = Simulator::getXfromLon(nod.lon);
			nod.Y = Simulator::getYfromLat(nod.lat);
			nod.lanes = 1;

			way_info->array_nodo_info[i].nid = nod.nid;
			way_info->array_nodo_info[i].order = 0;
			way_info->array_nodo_info[i].lon = nod.lon;
			way_info->array_nodo_info[i].lat = nod.lat;
			way_info->array_nodo_info[i].X = nod.X;
			way_info->array_nodo_info[i].Y = nod.Y;
			way_info->array_nodo_info[i].lanes = nod.lanes;
			i++;

		}
	}

	way_info->max_nodos = i;

	//-----------------Obtaining info from node-------------------------
	//cout << "Obtaining wid info from Node" << endl;
	i = 0;

	for (int i = 0; i < way_info->max_nodos; i++)
	{
		//cout << "Nodos:[" << *itv << "]" << endl;

		wid = 0;
		lanes = 1;
		consulta.formCadena("SELECT wid FROM Node_ways WHERE nid = %lld", way_info->array_nodo_info[i].nid);
		db->ejecutarConsulta(consulta);
		if (db->getNumRegistros() == 0)
		{
			//cout << "No existe wid para este nodo:[" << *itv << "]" << endl;
		}
		else
		{
			wid = long long(db->getDouble("wid", 0));
		}

		consulta.formCadena("SELECT value FROM Way_tags WHERE wid = %lld AND key_tag = 'lanes'", wid);
		db->ejecutarConsulta(consulta);
		if (db->getNumRegistros() == 0)
		{
			//cout << "No existe informacion de Lane:[" << *itv << "]" << endl;
		}
		else
		{
			lanes = long long(db->getDouble("value", 0));
		}
		way_info->array_nodo_info[i].lanes = lanes;
		//cout << "Lanes :[" << *itv << "]  [" << lanes << "]" << endl;
	}


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

	cout << "Max Nodos:[" << way_info->max_nodos << "]" << endl;

}


//------------------------------------------------------------------------------
void Simulator::get_info_nodes_from_file(BaseDatos* db, WayInfo* way_info, Cadena file)
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
	string line;
	string substr;
	vector<long long int> result;
	vector<long long int>::const_iterator itv;
	map<long long int, Way>::const_iterator itm;
	Cadena consulta, cadaux, name, href;
	NodoInfo nod;
	long long nid_next, wid,  nid = 0;
	int lanes, count_nodes = 0;
	double x1, x2, y1, y2, deltamv_x, deltamv_y;
	double angle;
	double deltamv, dist;
	double delta_lane_x = 0, delta_lane_y = 0;

	way_info->wid=0;
	way_info->hwtype= SIM_WAY_TYPE_END;
	way_info->lanes = 1;
	way_info->max_nodos=0;
    strcpy(way_info->name,"");
	way_info->name_long=0;
	strcpy(way_info->href, "");
	way_info->href_long = 0;
	if (way_info->array_nodo_info != NULL)
	{
		delete(way_info->array_nodo_info);
		way_info->array_nodo_info = NULL;
	}

	ifstream f(file.getCadena());
	if (!f.is_open()) { cout << "There is no file with this name:[" << file.getCadena() << "]" << endl;  return; }

	while (getline(f, line))
	{
		if ((line[0] != '#') && (line[0] != ' ') && (strcmp(line.c_str(), "") == 1))
		{

			cadaux = line.c_str();
			cadaux.rTrim();
			cadaux.lTrim();
			nid = atoll(cadaux.getCadena());
			result.push_back(nid);
			//cout << "Node:[" << nid << "]" << endl;
			count_nodes++;
		}
	}
	f.close();

	name = "";
	href = "";
	way_info->hwtype = enumHWType(SIM_ESCAPE);
	way_info->wid = 0;
	way_info->lanes = 1;
	strncpy(way_info->name, name.getCadena(), name.getLong());
	way_info->name_long = name.getLong();
	strncpy(way_info->href, href.getCadena(), href.getLong());
	way_info->href_long = href.getLong();
	way_info->max_nodos = count_nodes;
	way_info->array_nodo_info = new NodoInfo[way_info->max_nodos];

	//-----------------Obtaining info from node-------------------------
	//cout << "Obtaining info from Node" << endl;
	int i = 0;
	for ( itv = result.begin(); itv != result.end(); ++itv)
	{
		//cout << "Nodos:[" << *itv << "]" << endl;

		consulta.formCadena("SELECT * FROM Node_pos WHERE nid = %lld ORDER BY ver DESC", *itv);
		db->ejecutarConsulta(consulta);
		if (db->getNumRegistros() == 0)
		{
			cout << "No existe nodo:[" << *itv << "]" << endl;
		}
		else
		{
			nod.nid = long long(db->getDouble("nid", 0));
			nod.lon = db->getDouble("lon", 0);
			nod.lat = db->getDouble("lat", 0);
			nod.X = Simulator::getXfromLon(nod.lon);
			nod.Y = Simulator::getYfromLat(nod.lat);
			nod.lanes = 1;

			way_info->array_nodo_info[i].nid = nod.nid;
			way_info->array_nodo_info[i].order = 0;
			way_info->array_nodo_info[i].lon = nod.lon;
			way_info->array_nodo_info[i].lat = nod.lat;
			way_info->array_nodo_info[i].X = nod.X;
			way_info->array_nodo_info[i].Y = nod.Y;
			way_info->array_nodo_info[i].lanes = nod.lanes;
			i++;

		}
	}

	way_info->max_nodos = i;

	//-----------------Obtaining info from node-------------------------
	//cout << "Obtaining wid info from Node" << endl;
	i = 0;

	for (int i=0; i < way_info->max_nodos; i++)
	{
		//cout << "Nodos:[" << *itv << "]" << endl;

		wid = 0;
		lanes = 1;
		consulta.formCadena("SELECT wid FROM Node_ways WHERE nid = %lld", way_info->array_nodo_info[i].nid);
		db->ejecutarConsulta(consulta);
		if (db->getNumRegistros() == 0)
		{
			//cout << "No existe wid para este nodo:[" << *itv << "]" << endl;
		}
		else
		{
			wid = long long(db->getDouble("wid", 0));
		}

		consulta.formCadena("SELECT value FROM Way_tags WHERE wid = %lld AND key_tag = 'lanes'", wid );
		db->ejecutarConsulta(consulta);
		if (db->getNumRegistros() == 0)
		{
			//cout << "No existe informacion de Lane:[" << *itv << "]" << endl;
		}
		else
		{
			lanes = long long(db->getDouble("value", 0));
		}
		way_info->array_nodo_info[i].lanes = lanes;
		//cout << "Lanes :[" << *itv << "]  [" << lanes << "]" << endl;
	}


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

	cout << "Max Nodos:[" << way_info->max_nodos << "]" << endl;

}

//------------------------------------------------------------------------------
Cadena Simulator::get_filename_clean(Cadena cadfile)
{

	string substr;
	vector<Cadena> result;
	vector<Cadena>::const_iterator it;
	Cadena cad;
	int count=0;

	stringstream ss(cadfile.getCadena());
	result.clear();
	while (ss.good())
	{
		getline(ss, substr, '\\');
		cad = substr.c_str();
		cad.rTrim();
		cad.lTrim();
		result.push_back(cad);
		count++;
	}
		
	return result[count-1];
}

//------------------------------------------------------------------------------
Cadena Simulator::get_timestamp_format_YYMMDDhhmmss()
{
	time_t curr_time;
	Cadena cad;

	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	cad.formCadena("%02d%02d%02d%02d%02d%02d",
		(tm_local->tm_year + 1900) % 100, tm_local->tm_mon + 1, tm_local->tm_mday,
		tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);

	return cad;
}

//------------------------------------------------------------------------------
Cadena Simulator::get_timestamp_format_YYYYMMDD_T_hhmmss()
{
	time_t curr_time;
	Cadena cad;

	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	cad.formCadena("%04d-%02d-%02dT%02d:%02d:%02d",
		tm_local->tm_year + 1900, tm_local->tm_mon + 1, tm_local->tm_mday,
		tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);

	return cad;
}

//------------------------------------------------------------------------------
Cadena Simulator::get_timestamp_GMT_format_YYYYMMDD_X_hhmmss()
{
	time_t raw_time;
	struct tm* ptm;
	Cadena cad;
	int min=0;

	time(&raw_time);
	ptm = gmtime(&raw_time);

	min = ptm->tm_min - 5;
	if (min < 0) {min = 0;}

	cad.formCadena("%04d-%02d-%02d%%20%02d:%02d:00",ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,ptm->tm_hour, min);

	return cad;
}

//------------------------------------------------------------------------------
double Simulator::diff_angle(double angle_1, double angle_2)
{
	angle_1 = rad2deg(angle_1);
	angle_2 = rad2deg(angle_2);
	if (abs(angle_1) > 180) { angle_1 = abs(angle_1 - 2 * 180); }
	if (abs(angle_2) > 180) { angle_2 = abs(angle_2 - 2 * 180); }

	return abs(angle_1 - angle_2);
}

//------------------------------------------------------------------------------
int Simulator::Simulator::get_overlap_avrg(double speed, Cadena category)
{
	int overlap;
	if ((category == "T2") || (category == "T3") || (category == "T4") || (category == "M2") || (category == "M3"))
	{
		if (speed > 100)
		{
			overlap = 80;
		}
		else
		{
			overlap = 110;
		}
	}
	else
	{
		if (speed > 90)
		{
			overlap = 120;
		}
		else
		{
			overlap = 140;
		}

	}

	return overlap;
}

//------------------------------------------------------------------------------
int Simulator::Simulator::get_lanereturn_avrg(double speed, Cadena category)
{
	int lanereturn;
	if ((category == "T2") || (category == "T3") || (category == "T4") || (category == "M2") || (category == "M3"))
	{
		if (speed > 100)
		{
			lanereturn = 1200;
		}
		else
		{
			lanereturn = 1000;
		}
	}
	else
	{
		if (speed > 90)
		{
			lanereturn = 700;
		}
		else
		{
			lanereturn = 500;
		}
		
	}

	return lanereturn;
}

//------------------------------------------------------------------------------
int Simulator::get_delta_lane_return(int lane_now)
{
	int new_lane = lane_now -1;

	if (new_lane == 0)
	{
		new_lane = 1;
	}

	return new_lane;
}

//------------------------------------------------------------------------------
int Simulator::get_new_lane(int lane_now, int lanes)
{
	int new_lane = 1;

	if (lane_now == lanes)
	{
		new_lane = lanes - 1;
		if (lane_now == 0) new_lane = 1;
	}
	else if (lanes > lane_now)
	{
		new_lane = lane_now + 1;
	}


	return new_lane;
}
//------------------------------------------------------------------------------
int Simulator::get_delta_lane_ch(int lane_now, int new_lane)
{
	return (new_lane - lane_now);
}

//------------------------------------------------------------------------------
double Simulator::get_delta_lane(int lane, int max_lanes)
{
	return  double((max_lanes - 1) / 2.0) - double((max_lanes - lane));
}

//------------------------------------------------------------------------------
int Simulator::get_lane_default(double speed, int lanes, int lane_type)
{
	int lane;
	return 1;

	//cout << "LANES  ==> [" << lanes << "]" << endl;

	switch (lanes)
	{
		case 1:
			lane = 1;
			break;
		case 2:
			if (speed > 90)
			{
				lane = 2;
			}
			else
			{
				lane = 1;
			}
			break;
		case 3:
			if (lane_type == 1)
			{ 
				if (speed > 100)
				{
					lane = 3;
				}
				else if ((speed > 70) && (speed <= 100))
				{
					lane = 2;
				}
				else
				{
					lane = 1;
				}
			}
			else
			{
				if (speed > 100) 
				{
					lane = 2;
				}
				else
				{
					lane = 1;
				}
			}
			break;
		case 4:
			if (speed > 120)
			{
				lane = 4;
			}
			else if ((speed > 100) && (speed <= 120))
			{
				lane = 3;
			}
			else if ((speed > 80) && (speed <= 100))
			{
				lane = 2;
			}
			else
			{
				lane = 1;
			}
			break;
		case 5:
			if (speed > 130)
			{
				lane = 5;
			}
			else if ((speed > 110) && (speed <= 130))
			{
				lane = 4;
			}
			else if ((speed > 90) && (speed <= 110))
			{
				lane = 3;
			}
			else if ((speed > 70) && (speed <= 90))
			{
				lane = 2;
			}
			else
			{
				lane = 1;
			}
			break;
		case 6:
			if (speed > 130)
			{
				lane = 6;
			}
			else if ((speed > 115) && (speed <= 130))
			{
				lane = 5;
			}
			else if ((speed > 90) && (speed <= 115))
			{
				lane = 4;
			}
			else if ((speed > 70) && (speed <= 90))
			{
				lane = 3;
			}
			else if ((speed > 50) && (speed <= 70))
			{
				lane = 2;
			}
			else
			{
				lane = 1;
			}
			break;
		case 7:
		case 8:
		case 9:
		case 10:
			if (speed > 130)
			{
				lane = 7;
			}
			else if ((speed > 120) && (speed <= 130))
			{
				lane = 6;
			}
			else if ((speed > 110) && (speed <= 120))
			{
				lane = 5;
			}
			else if ((speed > 100) && (speed <= 110))
			{
				lane = 4;
			}
			else if ((speed > 80) && (speed <= 100))
			{
				lane = 3;
			}
			else if ((speed > 60) && (speed <= 80))
			{
				lane = 2;
			}
			else
			{
				lane = 1;
			}
			break;
	
		default:
			lane = 2;
	}

	return lane;
}

//------------------------------------------------------------------------------
double Simulator::get_speed(double posS, double posE, Cadena mnt_sal, Cadena mnt_ent)
{

	double km = 1000.0;
	double hr = 3600.0;
	double speed;

	speed = abs((abs((posS - posE)) / km) * hr / Simulator::get_segs(mnt_ent, mnt_sal));

	if (speed < 35.0) speed = 35.0;
	return speed;

}

//------------------------------------------------------------------------------
void Simulator::get_category_stuff(Cadena category, double& size, double& speed_pw, int& lane_type, Color& color)
{

	int alfa = 255;
	color.r = 200 + rand() % 55;
	color.g = 100 + rand() % 55;
	color.b = 100 + rand() % 75;

	if ((category == "M2") || (category == "M3"))
	{
		color.f = alfa;
		size = 0.88;
		speed_pw = 1.0;
		lane_type = 1;
	}
	else if ((category == "T2") || (category == "T3"))
	{
		color.f = alfa;
		size = 1.0;
		speed_pw = 1.0;
		lane_type = 1;
	}
	else if ((category == "F2") || (category == "F3"))
	{
		color.f = alfa;
		size = 1.2;
		speed_pw = 1.0;
		lane_type = 2;
	}
	else if ((category == "C2") || (category == "C3"))
	{
		color.f = alfa;
		size = 1.3;
		speed_pw = 0.9;
		lane_type = 2;
	}
	else if ((category == "C4") || (category == "C5"))
	{
		color.f = alfa;
		size = 1.4;
		speed_pw = 0.8;
		lane_type = 2;
	}
	else
	{
		color.r = 20;
		color.g = 20;
		color.b = 20;
		color.f = alfa;
		size = 1.0;
		speed_pw = 1.0;
		lane_type = 1;

	}
}
//------------------------------------------------------------------------------
//        NODES - WAYS
//------------------------------------------------------------------------------
void Simulator::get_nodes_from_way(BaseDatos* db, long long wid, Cadena order,  vectNode* vn)
{
	Cadena consulta;
	Node nod;
	consulta.formCadena("SELECT Way_nodes.wid, Way_nodes.norder, Way_nodes.nid, Node_pos.lon, Node_pos.lat FROM Way_nodes LEFT OUTER JOIN Node_pos ON(Way_nodes.nid = Node_pos.nid) WHERE Way_nodes.wid = %lld ORDER BY Way_nodes.norder %s", wid, order.getCadena());
	db->ejecutarConsulta(consulta);

	for (int i = 0; i < db->getNumRegistros(); i++)
	{

		nod.nid = long long(db->getDouble("nid", i));
		nod.order = db->getInt("norder", i);
		nod.lon = db->getDouble("lon", i);
		nod.lat = db->getDouble("lat", i);

		nod.X = Simulator::getXfromLon(nod.lon);
		nod.Y = Simulator::getYfromLat(nod.lat);

		vn->push_back(nod);
	}
}

//------------------------------------------------------------------------------
double Simulator::smoother_z(double position)
{
	double delta = 0.0;

	if (position <= 1.0)                            	{delta =  0.05;}
	else if ((position > 1.0)  && (position <= 5.0)) 	{delta =  0.30;}
	else if ((position > 5.0)  && (position <= 10.0))	{delta =  1.20;}
	else if ((position > 10.0) && (position <= 50.0))	{delta =  2.00;}
	else if ((position > 50.0) && (position <= 100.0))	{delta =  6.00;}
	else if (position > 100.0)                       	{delta = 15.00;}
	
	if ((position <= SIM_MIN_POSITION_Z) || (position >= SIM_MAX_POSITION_Z))
	{
		delta = 0.0;
	}
	
	return delta;
}


//------------------------------------------------------------------------------
time_t Simulator::get_time_now_ms()
{
	return (std::time(nullptr) * 1000);
}

//------------------------------------------------------------------------------
double Simulator::get_segs(Cadena cad_e, Cadena cad_s)
{
	tm tm1;
	int diaEnt, horaEnt, minEnt;
	int segSal, segEnt;
	
	// 2017-06-29 10:02:48
	sscanf(cad_s.getCadena(), "%4d-%2d-%2d %2d:%2d:%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday, &tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);
	if      (cad_e.getLong() == 7) {sscanf(cad_e.getCadena(), "%3d%2d%2d", &diaEnt, &horaEnt, &minEnt);}
	else if (cad_e.getLong() == 6) {sscanf(cad_e.getCadena(), "%2d%2d%2d", &diaEnt, &horaEnt, &minEnt);}
	else                           {cout << "Error en formato de momento de entrada: [" << cad_e.getCadena() << "] " << endl;}

	segSal = Simulator::getSegOut(tm1);
	segEnt = diaEnt * 24 * 60 * 60 + horaEnt * 60 * 60 + minEnt * 60;

	return (segSal - segEnt);
}

//------------------------------------------------------------------------------
int Simulator::getSegOut(tm& t)
{
	int seg;
	seg = (t.tm_mon - 1) * 30 * 24 * 60 * 60 + (t.tm_mday + 1) * 24 * 60 * 60 + t.tm_hour * 60 * 60 + t.tm_min * 60 + t.tm_sec;
	return seg;
}

//------------------------------------------------------------------------------
void Simulator::get_new_moment(int* mnt)
{
	int min, hour, days, year = 0;
	Cadena cadmmt;

	cadmmt.formCadena("%07d", *mnt);
	sscanf(cadmmt.getCadena(), "%3d%2d%2d", &days, &hour, &min);

	min++;
	if (min > 59)
	{
		min = 0;
		hour++;
		if (hour > 23)
		{
			hour = 0;
			days++;
			if (days > 365)
			{
				year++;
			}
		}
	}

	cadmmt.formCadena("%03d%02d%02d", days, hour, min);
	*mnt = cadmmt.toUDWordType();
}

//------------------------------------------------------------------------------
double Simulator::distance(double x1, double x2, double y1, double y2)
{
	return (std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

//------------------------------------------------------------------------------
int Simulator::pnpoly(int npol, double *xp, double *yp, double x, double y)
{
	int i, j, c = 0;
	for (i = 0, j = npol - 1; i < npol; j = i++)
	{
		if ((((yp[i] <= y) && (y < yp[j])) || ((yp[j] <= y) && (y < yp[i]))) && (x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))
		{
			c = !c;
		}

	}
	return c;
}

//------------------------------------------------------------------------------
void Simulator::texture_dds(GLuint& id, Cadena file)
{
	Cadena cad;
	cad.formCadena(SIM_DIR_IMAGES_"%s", file.getCadena());
    id = loadDDS(cad.getCadena());
}

//------------------------------------------------------------------------------
void Simulator::texture(GLuint& id, Cadena file)
{
	unsigned char* image;
	int width, height;
	Cadena cadpath;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	cadpath.formCadena(SIM_DIR_IMAGES_"%s", file.getCadena());
	image = SOIL_load_image(cadpath.getCadena(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width,height, GL_TRUE);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//------------------------------------------------------------------------------
void Simulator::texture_customize(GLuint& id, Cadena file, GLuint typeWrap, GLuint typeColor, GLuint typeLoad)
{
	unsigned char* image;
	int width, height;
	Cadena cadpath;

	try
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		cadpath.formCadena(SIM_DIR_IMAGES_"%s", file.getCadena());
		image = SOIL_load_image(cadpath.getCadena(), &width, &height, 0, typeLoad);
		if (image != NULL)
		{

			glTexImage2D(GL_TEXTURE_2D, 0, typeColor, width, height, 0, typeColor, GL_UNSIGNED_BYTE, image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, typeWrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, typeWrap);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			cout << "Error getting image:[" << file.getCadena() << "]" << endl;
		}
	}
	catch (...)
	{
		cout << "Error getting texture customized" << endl;
	}


}

//------------------------------------------------------------------------------
void Simulator::texture_RGBA(GLuint& id, Cadena file)
{
	unsigned char* image;
	int width, height;
	Cadena cadpath;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	cadpath.formCadena(SIM_DIR_IMAGES_"%s", file.getCadena());
	image = SOIL_load_image(cadpath.getCadena(), &width, &height, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //GL_LINEAR_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR_MIPMAP_NEAREST
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}


//------------------------------------------------------------------------------
void Simulator::texture_RGBA_repeat(GLuint& id, Cadena file)
{
	unsigned char* image;
	int width, height;
	Cadena cadpath;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	cadpath.formCadena(SIM_DIR_IMAGES_"%s", file.getCadena());
	image = SOIL_load_image(cadpath.getCadena(), &width, &height, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //GL_LINEAR_MIPMAP_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR_MIPMAP_NEAREST
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}


//------------------------------------------------------------------------------
void Simulator::spin_model(double* array_points, int size, glm::mat4* modelMatrices)
{
	float angle;
	int inext = 0;
	int index_model = 0;
	double x1, y1, z1;
	double x2, y2, z2;
	bool not_angle;

	for (int i = 0; i < size; i++)
	{
		glm::mat4 model;
		not_angle = false;

		x1 = array_points[3 * i + 0];
		y1 = array_points[3 * i + 1];
		z1 = array_points[3 * i + 2];
		
		inext = i + 1;

		x2 = array_points[3 * inext + 0];
		y2 = array_points[3 * inext + 1];
		z2 = array_points[3 * inext + 2];

		angle = float(Simulator::getAngle(x1, x2, y1, y2, not_angle));
		//cout << "Angle:[" << angle << "]" << endl;
		//if (!not_angle) {}

		model = glm::translate(model, glm::vec3(x1, y1, z1));
		model = glm::scale(model, glm::vec3(0.02));
		model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));
 	  	  	 
		modelMatrices[i] = model;
	}
}

//------------------------------------------------------------------------------
void Simulator::spin_model_vh(double* array_points, double* array_scale, double* array_angles, int size, glm::mat4* modelMatrices)
{
	double x1, y1, z1;
	float angle;
	float scale;


	for (int i = 0; i < size; i++)
	{
		glm::mat4 model;

		x1 = array_points[3 * i + 0];
		y1 = array_points[3 * i + 1];
		z1 = array_points[3 * i + 2];

		angle = array_angles[i];
		scale = array_scale[i];

		model = glm::translate(model, glm::vec3(x1, y1, z1));
		model = glm::scale(model, glm::vec3(scale));
		model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));

		modelMatrices[i] = model;

	}
}

//------------------------------------------------------------------------------
void Simulator::build_LINE_SIMPLE(double* array_in, int ncoord, int total_in, double* array_out, int max_out, int* total_out)
{
	int index_out = 0;
	for (int i = 0; i < total_in; i++)
	{
		if ((i == 0) || (i == total_in - 1)/*LAST*/)
		{
			array_out[ncoord * index_out + 0] = array_in[ncoord * i + 0];
			array_out[ncoord * index_out + 1] = array_in[ncoord * i + 1];
			array_out[ncoord * index_out + 2] = array_in[ncoord * i + 2];
			index_out++;
			if (index_out >= max_out) return;
		}
		else
		{

			array_out[ncoord * index_out + 0] = array_in[ncoord * i + 0];
			array_out[ncoord * index_out + 1] = array_in[ncoord * i + 1];
			array_out[ncoord * index_out + 2] = array_in[ncoord * i + 2];
			index_out++;
			if (index_out >= max_out) return;

			array_out[ncoord * index_out + 0] = array_in[ncoord * i + 0];
			array_out[ncoord * index_out + 1] = array_in[ncoord * i + 1];
			array_out[ncoord * index_out + 2] = array_in[ncoord * i + 2];
			index_out++;
			if (index_out >= max_out) return;
		}
	}
	*total_out = index_out;
}

//------------------------------------------------------------------------------
void Simulator::build_LINE_RECT_POLY(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double xu1, yu1, zu1, xo1, yo1, zo1, xd1, yd1, zd1;
	double xu2, yu2, zu2, xo2, yo2, zo2, xd2, yd2, zd2;
	double xx1, xx2, yy1, yy2, angle, xtemp, ytemp;


	int indice = 0;
	int inext = 0;
	int index_out = 0;

	bool first_point = true;
	bool not_angle = false;

	angle = 0;
	xtemp = 0;
	ytemp = 0;



	for (indice = 0; indice < total_in; indice++)
	{

		inext = indice + 1;

		xx1 = array_in[3 * indice + 0];
		xx2 = array_in[3 * inext + 0];
		yy1 = array_in[3 * indice + 1];
		yy2 = array_in[3 * inext + 1];

		xtemp = 0;
		ytemp = 0;
		angle = 0;


		angle = Simulator::getAngle(xx1, xx2, yy1, yy2, not_angle);

		if (!not_angle)
		{
			xtemp = road_w * cos(angle - SIM_PI / 2);
			ytemp = road_w * sin(angle - SIM_PI / 2);
		}




		//---Rectangle Points Setup----------

		xu1 = array_in[3 * indice + 0] - xtemp;
		yu1 = array_in[3 * indice + 1] - ytemp;
		zu1 = array_in[3 * indice + 2];

		xo1 = array_in[3 * indice + 0];
		yo1 = array_in[3 * indice + 1];
		zo1 = array_in[3 * indice + 2];

		xd1 = array_in[3 * indice + 0] + xtemp;
		yd1 = array_in[3 * indice + 1] + ytemp;
		zd1 = array_in[3 * indice + 2];

		xu2 = array_in[3 * inext + 0] - xtemp;
		yu2 = array_in[3 * inext + 1] - ytemp;
		zu2 = array_in[3 * inext + 2];

		xo2 = array_in[3 * inext + 0];
		yo2 = array_in[3 * inext + 1];
		zo2 = array_in[3 * inext + 2];

		xd2 = array_in[3 * inext + 0] + xtemp;
		yd2 = array_in[3 * inext + 1] + ytemp;
		zd2 = array_in[3 * inext + 2];


		//----------OUT-------------------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);


		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

		indice++;

	}

	

	*total_out = index_out;
}





//------------------------------------------------------------------------------
void Simulator::build_LINE_RECT(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double xu1, yu1, zu1, xo1, yo1, zo1, xd1, yd1, zd1;
	double xu2, yu2, zu2, xo2, yo2, zo2, xd2, yd2, zd2;
	double xx1, xx2, yy1, yy2, angle, xtemp, ytemp;


	int index_out = 0;
	int inext = 0;
	bool first_point = true;
	bool not_angle = false;

	angle = 0;
	xtemp = 0;
	ytemp = 0;

	for (int i = 0; i < total_in - 1; i++)
	{

		inext = i + 1;

		xx1 = array_in[3 * i + 0];
		xx2 = array_in[3 * inext + 0];
		yy1 = array_in[3 * i + 1];
		yy2 = array_in[3 * inext + 1];

		xtemp = 0;
		ytemp = 0;
		angle = 0;


		angle = Simulator::getAngle(xx1, xx2, yy1, yy2, not_angle);

		if (!not_angle)
		{
			xtemp = road_w * cos(angle - SIM_PI / 2);
			ytemp = road_w * sin(angle - SIM_PI / 2);
		}




		//---Rectangle Points Setup----------

		xu1 = array_in[3 * i + 0] - xtemp;
		yu1 = array_in[3 * i + 1] - ytemp;
		zu1 = array_in[3 * i + 2];

		xo1 = array_in[3 * i + 0];
		yo1 = array_in[3 * i + 1];
		zo1 = array_in[3 * i + 2];

		xd1 = array_in[3 * i + 0] + xtemp;
		yd1 = array_in[3 * i + 1] + ytemp;
		zd1 = array_in[3 * i + 2];

		xu2 = array_in[3 * inext + 0] - xtemp;
		yu2 = array_in[3 * inext + 1] - ytemp;
		zu2 = array_in[3 * inext + 2];

		xo2 = array_in[3 * inext + 0];
		yo2 = array_in[3 * inext + 1];
		zo2 = array_in[3 * inext + 2];

		xd2 = array_in[3 * inext + 0] + xtemp;
		yd2 = array_in[3 * inext + 1] + ytemp;
		zd2 = array_in[3 * inext + 2];


		//----------OUT-------------------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);


		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

	}

	*total_out = index_out;
}

//------------------------------------------------------------------------------
void Simulator::build_LINE_RENDER(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double xu1, yu1, zu1, xo1, yo1, zo1, xd1, yd1, zd1;
	double xu2, yu2, zu2, xo2, yo2, zo2, xd2, yd2, zd2;
	double xx1, xx2, yy1, yy2, angle, xtemp, ytemp;
	double xtrg1, xtrg2, xtrg3, ytrg1, ytrg2, ytrg3;

	int index_out = 0;
	int inext = 0;
	bool first_point = true;
	bool not_angle = false;

	angle = 0;
	xtemp = 0;
	ytemp = 0;

	for (int i = 0; i < total_in - 1; i++)
	{
		inext = i + 1;

		xx1 = array_in[3 * i + 0];
		xx2 = array_in[3 * inext + 0];
		yy1 = array_in[3 * i + 1];
		yy2 = array_in[3 * inext + 1];

		xtemp = 0;
		ytemp = 0;
		angle = 0;

		angle = Simulator::getAngle(xx1, xx2, yy1, yy2, not_angle);

		if (!not_angle)
		{
			xtemp = road_w * cos(angle - SIM_PI / 2);
			ytemp = road_w * sin(angle - SIM_PI / 2);
		}

		xtrg1 = road_w * cos(angle);
		ytrg1 = road_w * sin(angle);

		xtrg2 = road_w * cos((SIM_PI / 4) - angle);
		ytrg2 = road_w * sin((SIM_PI / 4) - angle);

		xtrg3 = road_w * cos(angle + SIM_PI / 4);
		ytrg3 = road_w * sin(angle + SIM_PI / 4);


		//---Rectangle Points Setup----------
		xu1 = array_in[3 * i + 0] - xtemp;
		yu1 = array_in[3 * i + 1] - ytemp;
		zu1 = array_in[3 * i + 2];

		xo1 = array_in[3 * i + 0];
		yo1 = array_in[3 * i + 1];
		zo1 = array_in[3 * i + 2];

		xd1 = array_in[3 * i + 0] + xtemp;
		yd1 = array_in[3 * i + 1] + ytemp;
		zd1 = array_in[3 * i + 2];

		xu2 = array_in[3 * inext + 0] - xtemp;
		yu2 = array_in[3 * inext + 1] - ytemp;
		zu2 = array_in[3 * inext + 2];

		xo2 = array_in[3 * inext + 0];
		yo2 = array_in[3 * inext + 1];
		zo2 = array_in[3 * inext + 2];

		xd2 = array_in[3 * inext + 0] + xtemp;
		yd2 = array_in[3 * inext + 1] + ytemp;
		zd2 = array_in[3 * inext + 2];

		//----------OUT-------------------
		//First Triangle------------------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		//Second Triangle------------------
		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

		//------------------------------------
		// Terminaciones
		//------------------------------------
		//--------1 LEFT----------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg2;
		array_out[3 * index_out + 1] = yo1 + ytrg2;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------2 LEFT----------
		array_out[3 * index_out + 0] = xo1 - xtrg2;
		array_out[3 * index_out + 1] = yo1 + ytrg2;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg1;
		array_out[3 * index_out + 1] = yo1 - ytrg1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------3 LEFT----------
		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg3;
		array_out[3 * index_out + 1] = yo1 - ytrg3;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------4 LEFT----------
		array_out[3 * index_out + 0] = xo1 - xtrg3;
		array_out[3 * index_out + 1] = yo1 - ytrg3;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg1;
		array_out[3 * index_out + 1] = yo1 - ytrg1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//=====================RIGHT===============
		//--------1 RIGHT----------
		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg3;
		array_out[3 * index_out + 1] = yo2 + ytrg3;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------2 RIGHT----------
		array_out[3 * index_out + 0] = xo2 + xtrg3;
		array_out[3 * index_out + 1] = yo2 + ytrg3;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg1;
		array_out[3 * index_out + 1] = yo2 + ytrg1;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------3 RIGHT----------
		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg2;
		array_out[3 * index_out + 1] = yo2 - ytrg2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------4 RIGHT----------
		array_out[3 * index_out + 0] = xo2 + xtrg1;
		array_out[3 * index_out + 1] = yo2 + ytrg1;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg2;
		array_out[3 * index_out + 1] = yo2 - ytrg2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

	}

	*total_out = index_out;
}



//------------------------------------------------------------------------------
void Simulator::build_LINE_RECT_TRG(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double xu1, yu1, zu1, xo1, yo1, zo1, xd1, yd1, zd1;
	double xu2, yu2, zu2, xo2, yo2, zo2, xd2, yd2, zd2;
	double xx1, xx2, yy1, yy2, angle, xtemp, ytemp;
	double xtrg1, xtrg2, xtrg3, ytrg1, ytrg2, ytrg3;

	int index_out = 0;
	int inext = 0;
	bool first_point = true;
	bool not_angle = false;

	angle = 0;
	xtemp = 0;
	ytemp = 0;

	for (int i = 0; i < total_in - 1; i++)
	{
		inext = i + 1;

		xx1 = array_in[4 * i + 0];
		xx2 = array_in[4 * inext + 0];
		yy1 = array_in[4 * i + 1];
		yy2 = array_in[4 * inext + 1];

		xtemp = 0;
		ytemp = 0;
		angle = 0;

		angle = Simulator::getAngle(xx1, xx2, yy1, yy2, not_angle);

		if (!not_angle)
		{
			xtemp = road_w * cos(angle - SIM_PI / 2);
			ytemp = road_w * sin(angle - SIM_PI / 2);
		}

		xtrg1 = road_w * cos(angle);
		ytrg1 = road_w * sin(angle);

		xtrg2 = road_w * cos((SIM_PI / 4) - angle);
		ytrg2 = road_w * sin((SIM_PI / 4) - angle);

		xtrg3 = road_w * cos(angle + SIM_PI / 4);
		ytrg3 = road_w * sin(angle + SIM_PI / 4);


		//---Rectangle Points Setup----------
		xu1 = array_in[4 * i + 0] - xtemp;
		yu1 = array_in[4 * i + 1] - ytemp;
		zu1 = array_in[4 * i + 2];

		xo1 = array_in[4 * i + 0];
		yo1 = array_in[4 * i + 1];
		zo1 = array_in[4 * i + 2];

		xd1 = array_in[4 * i + 0] + xtemp;
		yd1 = array_in[4 * i + 1] + ytemp;
		zd1 = array_in[4 * i + 2];

		xu2 = array_in[4 * inext + 0] - xtemp;
		yu2 = array_in[4 * inext + 1] - ytemp;
		zu2 = array_in[4 * inext + 2];

		xo2 = array_in[4 * inext + 0];
		yo2 = array_in[4 * inext + 1];
		zo2 = array_in[4 * inext + 2];

		xd2 = array_in[4 * inext + 0] + xtemp;
		yd2 = array_in[4 * inext + 1] + ytemp;
		zd2 = array_in[4 * inext + 2];

		//----------OUT-------------------
		//First Triangle------------------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		//Second Triangle------------------
		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

		//------------------------------------
		// Terminaciones
		//------------------------------------
		//--------1 LEFT----------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg2;
		array_out[3 * index_out + 1] = yo1 + ytrg2;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------2 LEFT----------
		array_out[3 * index_out + 0] = xo1 - xtrg2;
		array_out[3 * index_out + 1] = yo1 + ytrg2;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg1;
		array_out[3 * index_out + 1] = yo1 - ytrg1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------3 LEFT----------
		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg3;
		array_out[3 * index_out + 1] = yo1 - ytrg3;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------4 LEFT----------
		array_out[3 * index_out + 0] = xo1 - xtrg3;
		array_out[3 * index_out + 1] = yo1 - ytrg3;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg1;
		array_out[3 * index_out + 1] = yo1 - ytrg1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//=====================RIGHT===============
		//--------1 RIGHT----------
		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg3;
		array_out[3 * index_out + 1] = yo2 + ytrg3;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------2 RIGHT----------
		array_out[3 * index_out + 0] = xo2 + xtrg3;
		array_out[3 * index_out + 1] = yo2 + ytrg3;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg1;
		array_out[3 * index_out + 1] = yo2 + ytrg1;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------3 RIGHT----------
		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg2;
		array_out[3 * index_out + 1] = yo2 - ytrg2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------4 RIGHT----------
		array_out[3 * index_out + 0] = xo2 + xtrg1;
		array_out[3 * index_out + 1] = yo2 + ytrg1;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg2;
		array_out[3 * index_out + 1] = yo2 - ytrg2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

	}

	*total_out = index_out;
}


//------------------------------------------------------------------------------
void Simulator::build_LINE_RECT_TRG_LANES(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double xu1, yu1, zu1, xo1, yo1, zo1, xd1, yd1, zd1;
	double xu2, yu2, zu2, xo2, yo2, zo2, xd2, yd2, zd2;
	double xx1, xx2, yy1, yy2, angle, xtemp, ytemp;
	double xtrg1, xtrg2, xtrg3, ytrg1, ytrg2, ytrg3;

	int index_out = 0;
	int inext = 0;
	bool first_point = true;
	bool not_angle = false;
	int lanes = array_in[4 * 0 + 3];
	double delta_lane;

	angle = 0;
	xtemp = 0;
	ytemp = 0;

	for (int i = 0; i < total_in - 1; i++)
	{

		inext = i + 1;

		xx1 = array_in[4 * i + 0];
		xx2 = array_in[4 * inext + 0];
		yy1 = array_in[4 * i + 1];
		yy2 = array_in[4 * inext + 1];

		xtemp = 0;
		ytemp = 0;
		angle = 0;


		angle = Simulator::getAngle(xx1, xx2, yy1, yy2, not_angle);
		delta_lane = (0.5) * lanes;


		if (!not_angle)
		{
			xtemp = road_w * cos(angle - SIM_PI / 2) * delta_lane;
			ytemp = road_w * sin(angle - SIM_PI / 2) * delta_lane;
		}

		xtrg1 = road_w * cos(angle) * delta_lane;
		ytrg1 = road_w * sin(angle) * delta_lane;

		xtrg2 = road_w * cos((SIM_PI / 4) - angle) * delta_lane;
		ytrg2 = road_w * sin((SIM_PI / 4) - angle) * delta_lane;

		xtrg3 = road_w * cos(angle + SIM_PI / 4) * delta_lane;
		ytrg3 = road_w * sin(angle + SIM_PI / 4) * delta_lane;


		//---Rectangle Points Setup----------

		xu1 = array_in[4 * i + 0] - xtemp;
		yu1 = array_in[4 * i + 1] - ytemp;
		zu1 = array_in[4 * i + 2];

		xo1 = array_in[4 * i + 0];
		yo1 = array_in[4 * i + 1];
		zo1 = array_in[4 * i + 2];

		xd1 = array_in[4 * i + 0] + xtemp;
		yd1 = array_in[4 * i + 1] + ytemp;
		zd1 = array_in[4 * i + 2];

		xu2 = array_in[4 * inext + 0] - xtemp;
		yu2 = array_in[4 * inext + 1] - ytemp;
		zu2 = array_in[4 * inext + 2];

		xo2 = array_in[4 * inext + 0];
		yo2 = array_in[4 * inext + 1];
		zo2 = array_in[4 * inext + 2];

		xd2 = array_in[4 * inext + 0] + xtemp;
		yd2 = array_in[4 * inext + 1] + ytemp;
		zd2 = array_in[4 * inext + 2];

		//----------OUT-------------------
		//First Triangle------------------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);


		//Second Triangle------------------
		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);


		//------------------------------------
		// Terminaciones
		//------------------------------------
		//--------1 LEFT----------
		array_out[3 * index_out + 0] = xu1;
		array_out[3 * index_out + 1] = yu1;
		array_out[3 * index_out + 2] = zu1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg2;
		array_out[3 * index_out + 1] = yo1 + ytrg2;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);


		//--------2 LEFT----------
		array_out[3 * index_out + 0] = xo1 - xtrg2;
		array_out[3 * index_out + 1] = yo1 + ytrg2;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg1;
		array_out[3 * index_out + 1] = yo1 - ytrg1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//--------3 LEFT----------
		array_out[3 * index_out + 0] = xd1;
		array_out[3 * index_out + 1] = yd1;
		array_out[3 * index_out + 2] = zd1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg3;
		array_out[3 * index_out + 1] = yo1 - ytrg3;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);


		//--------4 LEFT----------
		array_out[3 * index_out + 0] = xo1 - xtrg3;
		array_out[3 * index_out + 1] = yo1 - ytrg3;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1;
		array_out[3 * index_out + 1] = yo1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo1 - xtrg1;
		array_out[3 * index_out + 1] = yo1 - ytrg1;
		array_out[3 * index_out + 2] = zo1;
		index_out = get_new_index(index_out, max_out);

		//=====================RIGHT===============
		//--------1 RIGHT----------
		array_out[3 * index_out + 0] = xu2;
		array_out[3 * index_out + 1] = yu2;
		array_out[3 * index_out + 2] = zu2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg3;
		array_out[3 * index_out + 1] = yo2 + ytrg3;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);


		//--------2 RIGHT----------
		array_out[3 * index_out + 0] = xo2 + xtrg3;
		array_out[3 * index_out + 1] = yo2 + ytrg3;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg1;
		array_out[3 * index_out + 1] = yo2 + ytrg1;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------3 RIGHT----------
		array_out[3 * index_out + 0] = xd2;
		array_out[3 * index_out + 1] = yd2;
		array_out[3 * index_out + 2] = zd2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg2;
		array_out[3 * index_out + 1] = yo2 - ytrg2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		//--------4 RIGHT----------
		array_out[3 * index_out + 0] = xo2 + xtrg1;
		array_out[3 * index_out + 1] = yo2 + ytrg1;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2;
		array_out[3 * index_out + 1] = yo2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);

		array_out[3 * index_out + 0] = xo2 + xtrg2;
		array_out[3 * index_out + 1] = yo2 - ytrg2;
		array_out[3 * index_out + 2] = zo2;
		index_out = get_new_index(index_out, max_out);
	}

	*total_out = index_out;
}


//------------------------------------------------------------------------------
void Simulator::build_LINE_RECT_DASHED(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double xx1, xx2, yy1, yy2, angle, xtemp, ytemp;
	double x, x1, y, y1, z, z1;
	double delta_lane, delta_x, delta_y, delta_xlane, delta_ylane, lane_init, distance;

	bool first_point = true;
	bool not_angle = false;
	int index_out = 0;
	int inext = 0;
	int lanes, loop, loop_dash;
	double lane_line_lenght = 0.0013;
	double width = 0.00012;

	angle = 0;
	xtemp = 0;
	ytemp = 0;

	lanes = array_in[4 * 0 + 3];
	loop = lanes - 1;
	delta_lane = double(loop) / 2.0 - 0.5;
	

	//Each lane (e.g.: For 3 lane road, 2 lanes lines)
	for (int j = 0; j < loop; j++)
	{
		for (int i = 0; i < total_in - 1; i++)
		{
			
			xtemp = 0;
			ytemp = 0;
			angle = 0;
			delta_xlane = 0.0;
			delta_ylane = 0.0;
			delta_x = 0.0;
			delta_y = 0.0;

			inext = i + 1;

			xx1 = array_in[4 * i + 0];
			xx2 = array_in[4 * inext + 0];
			yy1 = array_in[4 * i + 1];
			yy2 = array_in[4 * inext + 1];

			distance = std::sqrt((xx1 - xx2) * (xx1 - xx2) + (yy1 - yy2) * (yy1 - yy2));
			loop_dash = int(distance / (lane_line_lenght * 2));
			angle = Simulator::getAngle(xx1, xx2, yy1, yy2, not_angle);
			
			if (!not_angle)
			{
				xtemp = road_w * cos(angle - SIM_PI / 2) * delta_lane;
				ytemp = road_w * sin(angle - SIM_PI / 2) * delta_lane;

				delta_xlane = width * cos(angle - SIM_PI / 2);
				delta_ylane = width * sin(angle - SIM_PI / 2);

				delta_x = lane_line_lenght * cos(angle);
				delta_y = lane_line_lenght * sin(angle);
			}
			
			x  = xx1 - xtemp;
			x1 = x + delta_x;
			y  = yy1 - ytemp;
			y1 = y + delta_y;
			z  = array_in[4 * i + 2];
			z1 = array_in[4 * i + 2];

			for (int w = 0; w < loop_dash + 1; w++)
			{
				Simulator::build_rectangle(x, y, z, x1, y1, z1, delta_xlane, delta_ylane, index_out, array_out);
				*total_out = *total_out + 6;
				x  = x  + 2 * delta_x;
				x1 = x1 + 2 * delta_x;
				y  = y  + 2 * delta_y;
				y1 = y1 + 2 * delta_y;
			}

			//cout << "total out -------------------  " << *total_out << endl;
		}

		delta_lane = delta_lane - 1.0;

	}


}

//------------------------------------------------------------------------------
void Simulator::build_BUILDING(double height, double* array_in, int total_in, double* array_out, int max_out, int* total_out)
{
	double x1, y1, z1, x2, y2, z2;
	int index_out = 0;
	int inext = 0;


	if (total_in > 2)
	{


		for (int i = 0; i < total_in - 1; i++)
		{
			//For every line, we have to cretae a rectangle, but vertical
			inext = i + 1;

			x1 = array_in[4 * i + 0];
			y1 = array_in[4 * i + 1];
			z1 = 0.0;

			x2 = array_in[4 * inext + 0];
			y2 = array_in[4 * inext + 1];
			z2 = 0.0;

			build_rectangle_vertical(height, x1, y1, z1, x2, y2, z2, index_out, array_out);

		}

	}

	*total_out = index_out;

}

//------------------------------------------------------------------------------
void Simulator::build_rectangle_vertical(double height, double x1, double y1, double z1, double x2, double y2, double z2, int& index_out, double* array_out)
{
	double x3, y3, x4, y4, z3, z4;
	x3 = x1;
	y3 = y1;
	z3 = z1  +  height;

	x4 = x2;
	y4 = y2;
	z4 = z2 + height;

	//First triangle
	array_out[3 * index_out + 0] = x1;
	array_out[3 * index_out + 1] = y1;
	array_out[3 * index_out + 2] = z1;
	index_out++;

	array_out[3 * index_out + 0] = x2;
	array_out[3 * index_out + 1] = y2;
	array_out[3 * index_out + 2] = z2;
	index_out++;

	array_out[3 * index_out + 0] = x3;
	array_out[3 * index_out + 1] = y3;
	array_out[3 * index_out + 2] = z3;
	index_out++;

	//Second triangle
	array_out[3 * index_out + 0] = x2;
	array_out[3 * index_out + 1] = y2;
	array_out[3 * index_out + 2] = z2;
	index_out++;

	array_out[3 * index_out + 0] = x3;
	array_out[3 * index_out + 1] = y3;
	array_out[3 * index_out + 2] = z3;
	index_out++;

	array_out[3 * index_out + 0] = x4;
	array_out[3 * index_out + 1] = y4;
	array_out[3 * index_out + 2] = z4;
	index_out++;

}

//------------------------------------------------------------------------------
void Simulator::build_rectangle(double x, double y, double z, double x1, double y1, double z1, double delta_xlane, double delta_ylane, int& index_out, double* array_out)
{
	array_out[3 * index_out + 0] = x - delta_xlane;
	array_out[3 * index_out + 1] = y - delta_ylane;
	array_out[3 * index_out + 2] = z;
	index_out = index_out + 1;
	//cout << "index_out out -------------------  " << index_out << endl;

	array_out[3 * index_out + 0] = x + delta_xlane;
	array_out[3 * index_out + 1] = y + delta_ylane;;
	array_out[3 * index_out + 2] = z;
	index_out = index_out + 1;

	array_out[3 * index_out + 0] = x1 + delta_xlane;
	array_out[3 * index_out + 1] = y1 + delta_ylane;
	array_out[3 * index_out + 2] = z1;
	index_out = index_out + 1;

	array_out[3 * index_out + 0] = x - delta_xlane;
	array_out[3 * index_out + 1] = y - delta_ylane;
	array_out[3 * index_out + 2] = z;
	index_out = index_out + 1;

	array_out[3 * index_out + 0] = x1 - delta_xlane;
	array_out[3 * index_out + 1] = y1 - delta_ylane;
	array_out[3 * index_out + 2] = z1;
	index_out = index_out + 1;

	array_out[3 * index_out + 0] = x1 + delta_xlane;
	array_out[3 * index_out + 1] = y1 + delta_ylane;
	array_out[3 * index_out + 2] = z1;
	index_out = index_out + 1;

}

//------------------------------------------------------------------------------
void Simulator::set_triangle(double* ptr, int& index, int max_out, double x1, double y1, double z1,
	                                                               double x2, double y2, double z2,
	                                                               double x3, double y3, double z3)
{
	ptr[3 * index + 0] = x1;
	ptr[3 * index + 1] = y1;
	ptr[3 * index + 2] = z3;
	index = get_new_index(index, max_out);

	ptr[3 * index + 0] = x2;
	ptr[3 * index + 1] = y2;
	ptr[3 * index + 2] = z3;
	index = get_new_index(index, max_out);

	ptr[3 * index + 0] = x3;
	ptr[3 * index + 1] = y3;
	ptr[3 * index + 2] = z3;
	index = get_new_index(index, max_out);
}

//------------------------------------------------------------------------------
int Simulator::get_new_index(int index, int max)
{
	index++;

	if (index >= max) 	{cout << "Error: Max Index Catched [Simulator::get_new_index]!!" << endl;}
	return index;
}

//------------------------------------------------------------------------------
double Simulator::getXfromLon(double lon)
{
	double x;
	x = Simulator::distanceEarth(SIM_LAT_REF, SIM_LON_REF, SIM_LAT_REF, lon);
	if (SIM_LON_REF > lon) {x = -x;}

	return x;
}

//------------------------------------------------------------------------------
double Simulator::getYfromLat(double lat)
{
	double y;
	y = Simulator::distanceEarth(SIM_LAT_REF, SIM_LON_REF, lat, SIM_LON_REF);
	if (SIM_LAT_REF > lat) {y = -y;}

	return y;
}

//------------------------------------------------------------------------------
double Simulator::get_delta_lat(double lat_i, double deltay)
{
	double lat_o;

	lat_o = lat_i + Simulator::rad2deg(2.0 * sin(deltay / (2.0 * SIM_EARTH_RADIUS_KM)));


	return lat_o;
}

//------------------------------------------------------------------------------
double Simulator::get_delta_lon(double lon_i, double lat_i, double lat_i2, double deltax)
{
	double lon_o, dum;
	dum = sin(deltax / (2.0 * SIM_EARTH_RADIUS_KM));

	if (deltax > 0.0)
	{
		lon_o = lon_i + Simulator::rad2deg(2.0 * std::sqrt((dum * dum) / (cos(Simulator::deg2rad(lat_i)) * cos(Simulator::deg2rad(lat_i2)))));
	}
	else
	{
		lon_o = lon_i - Simulator::rad2deg(2.0 * std::sqrt((dum * dum) / (cos(Simulator::deg2rad(lat_i)) * cos(Simulator::deg2rad(lat_i2)))));

	}
	return lon_o;
} 

//------------------------------------------------------------------------------
void Simulator::get_lat_lon(double lat_i, double lon_i, double distance, double angle, double& lat_o, double& lon_o)
{
	double delta = distance / SIM_EARTH_RADIUS_KM;
	double theta = deg2rad(angle);
	double zeta1 = deg2rad(lat_i);
	double lambda1 = deg2rad(lon_i);

	double sin_zeta2 = sin(zeta1) * cos(delta) + cos(zeta1) * sin(delta) * cos(theta);
	double zeta2 = asin(sin_zeta2);
	double y = sin(theta) * sin(delta) * cos(zeta1);
	double x = cos(delta) - sin(zeta1) * sin_zeta2;
	double lambda2 = lambda1 + atan2(y, x);

	lat_o = rad2deg(zeta2);
	lon_o = rad2deg(lambda2);

}

//------------------------------------------------------------------------------
double Simulator::deg2rad(double deg)
{
	return (deg * SIM_PI / 180.0);
}

//------------------------------------------------------------------------------ 
double Simulator::rad2deg(double rad)
{
	return (rad * 180.0 / SIM_PI);
}

//------------------------------------------------------------------------------
double Simulator::distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d)
{
	double lat1r, lon1r, lat2r, lon2r, u, v;
	lat1r = deg2rad(lat1d);
	lon1r = deg2rad(lon1d);
	lat2r = deg2rad(lat2d);
	lon2r = deg2rad(lon2d);
	u = sin((lat2r - lat1r) / 2.0);
	v = sin((lon2r - lon1r) / 2.0);
	return 2.0 * SIM_EARTH_RADIUS_KM * asin(std::sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));

	//double dum = std::sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v);
	//return (2.0 * SIM_EARTH_RADIUS_KM * atan2(dum, std::sqrt(1 - dum * dum)));

}

//------------------------------------------------------------------------------
double Simulator::getAngle(double x1, double x2, double y1, double y2, bool& not_angle)
{

	double angle = 0;
	double diff_x = x2 - x1;
	double diff_y = y2 - y1;
	double atang = 0;
	not_angle = false;

	if ((x2 == x1) && (y2 == y1))
	{
		not_angle = true;
	}
	else if (x2 == x1)
	{
		angle = SIM_PI - SIM_PI / 2.0f * (diff_y / abs(diff_y));
	}
	else if (y2 == y1)
	{
		angle = (SIM_PI / 2.0f) - (SIM_PI / 2.0f) * (diff_x / abs(diff_x));
	}
	else
	{
		atang = atan(diff_y / diff_x);

		if ((diff_x > 0) && (diff_y > 0))
		{
			angle = atang;
		}
		else if ((diff_x < 0) && (diff_y > 0))
		{
			angle = atang + SIM_PI;
		}
		else if ((diff_x < 0) && (diff_y < 0))
		{
			angle = atang + SIM_PI;
		}
		else if ((diff_x > 0) && (diff_y < 0))
		{
			angle = atang + 2 * SIM_PI;
		}
	}

	return angle;

}

//------------------------------------------------------------------------------
// Each time the renderer calls us back we just push another span entry on
// our list.

void Simulator::RasterCallback(const int y,	const int count,const FT_Span * const spans,void * const user)
{
	Spans *sptr = (Spans *)user;
	for (int i = 0; i < count; ++i)
		sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
}

//------------------------------------------------------------------------------
// Set up the raster parameters and render the outline.

void Simulator::RenderSpans(FT_Library &library,FT_Outline * const outline,	Spans *spans)
{
	FT_Raster_Params params;
	memset(&params, 0, sizeof(params));
	params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
	params.gray_spans = RasterCallback;
	params.user = spans;

	FT_Outline_Render(library, outline, &params);
}

//------------------------------------------------------------------------------
// Render the specified character as a colored glyph with a colored outline
// and dump it to a TGA.

void Simulator::WriteGlyphAsTGA(FT_Library &library,	const std::string &fileName,	wchar_t ch,	FT_Face &face,	int size,	const Pixel32 &fontCol,	const Pixel32 outlineCol,	float outlineWidth)
{
	// Set the size to use.
	if (FT_Set_Char_Size(face, size << 6, size << 6, 90, 90) == 0)
	{
		// Load the glyph we are looking for.
		FT_UInt gindex = FT_Get_Char_Index(face, ch);
		if (FT_Load_Glyph(face, gindex, FT_LOAD_NO_BITMAP) == 0)
		{
			// Need an outline for this to work.
			if (face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
			{
				// Render the basic glyph to a span list.
				Spans spans;
				RenderSpans(library, &face->glyph->outline, &spans);

				// Next we need the spans for the outline.
				Spans outlineSpans;

				// Set up a stroker.
				FT_Stroker stroker;
				FT_Stroker_New(library, &stroker);
				FT_Stroker_Set(stroker,
					(int)(outlineWidth * 64),
					FT_STROKER_LINECAP_ROUND,
					FT_STROKER_LINEJOIN_ROUND,
					0);

				FT_Glyph glyph;
				if (FT_Get_Glyph(face->glyph, &glyph) == 0)
				{
					FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
					// Again, this needs to be an outline to work.
					if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
					{
						// Render the outline spans to the span list
						FT_Outline *o =
							&reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
						RenderSpans(library, o, &outlineSpans);
					}

					// Clean up afterwards.
					FT_Stroker_Done(stroker);
					FT_Done_Glyph(glyph);

					// Now we need to put it all together.
					if (!spans.empty())
					{
						// Figure out what the bounding rect is for both the span lists.
						Rect rect(spans.front().x,
							spans.front().y,
							spans.front().x,
							spans.front().y);
						for (Spans::iterator s = spans.begin(); s != spans.end(); ++s)
						{
							rect.Include(Vec2(s->x, s->y));
							rect.Include(Vec2(s->x + s->width - 1, s->y));
						}
						for (Spans::iterator s = outlineSpans.begin(); s != outlineSpans.end(); ++s)
						{
							rect.Include(Vec2(s->x, s->y));
							rect.Include(Vec2(s->x + s->width - 1, s->y));
						}

#if 0
						// This is unused in this test but you would need this to draw
						// more than one glyph.
						float bearingX = face->glyph->metrics.horiBearingX >> 6;
						float bearingY = face->glyph->metrics.horiBearingY >> 6;
						float advance = face->glyph->advance.x >> 6;
#endif

						// Get some metrics of our image.
						int imgWidth = rect.Width(),
							imgHeight = rect.Height(),
							imgSize = imgWidth * imgHeight;

						// Allocate data for our image and clear it out to transparent.
						Pixel32 *pxl = new Pixel32[imgSize];
						memset(pxl, 0, sizeof(Pixel32) * imgSize);

						// Loop over the outline spans and just draw them into the
						// image.
						for (Spans::iterator s = outlineSpans.begin();
							s != outlineSpans.end(); ++s)
							for (int w = 0; w < s->width; ++w)
								pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth
									+ s->x - rect.xmin + w)] =
								Pixel32(outlineCol.r, outlineCol.g, outlineCol.b,
									s->coverage);

						// Then loop over the regular glyph spans and blend them into
						// the image.
						for (Spans::iterator s = spans.begin();
							s != spans.end(); ++s)
							for (int w = 0; w < s->width; ++w)
							{
								Pixel32 &dst =
									pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth
										+ s->x - rect.xmin + w)];
								Pixel32 src = Pixel32(fontCol.r, fontCol.g, fontCol.b,
									s->coverage);
								dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
								dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
								dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
								dst.a = MIN(255, dst.a + src.a);
							}

						// Dump the image to disk.
						WriteTGA(fileName, pxl, imgWidth, imgHeight);

						delete[] pxl;
					}
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
bool Simulator::WriteTGA(const std::string &filename, const Pixel32 *pxl, uint16 width,	uint16 height)
{
	std::ofstream file(filename.c_str(), std::ios::binary);
	if (file)
	{
		TGAHeader header;
		memset(&header, 0, sizeof(TGAHeader));
		header.imageType = 2;
		header.width = width;
		header.height = height;
		header.depth = 32;
		header.descriptor = 0x20;

		file.write((const char *)&header, sizeof(TGAHeader));
		file.write((const char *)pxl, sizeof(Pixel32) * width * height);

		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void Simulator::tranform_stations(int& estent, int& estsal)
{

	if (
		((estent == 115) && (estsal == 114)) ||
		((estent == 114) && (estsal == 115)) ||
		(estent == 116) || (estsal == 116) ||
		(estent == 107) || (estsal == 107)

		)
	{
		estent = 113;
		estsal = 102;
	}
	else if (estent == 110)
	{
		if (estsal == 118)
		{
			estent = 114;
			estsal = 106;
		}
		else if (estsal == 119)
		{
			estent = 110;
			estsal = 114;
		}
		else if (estsal == 117)
		{
			estent = 113;
			estsal = 105;
		}
		else if (estsal == 109)
		{
			estent = 112;
			estsal = 102;
		}
	}
	else if (estent == 115)
	{
		if (estsal == 113)
		{
			estent = 115;
			estsal = 110;
		}
	}
	else if (estent == 112)
	{
		if (estsal == 113)
		{
			estent = 112;
			estsal = 115;
		}
	}
	else if (estent == 113)
	{
		if (estsal == 114)
		{
			estent = 111;
			estsal = 114;
		}
		else if (estsal == 115)
		{
			estent = 111;
			estsal = 115;
		}
	}
	else if (estent == 117)
	{
		if (estsal == 118)
		{
			estent = 106;
			estsal = 114;
		}
		else if (estsal == 119)
		{
			estent = 106;
			estsal = 111;
		}
		else if (estsal == 110)
		{
			estent = 105;
			estsal = 112;
		}
		else if (estsal == 109)
		{
			estent = 117;
			estsal = 113;
		}

	}
	else if (estent == 115)
	{
		if (estsal == 113)
		{
			estent = 115;
			estsal = 108;
		}
	}
	else if (estent == 114)
	{
		if (estsal == 113)
		{
			estent = 114;
			estsal = 106;
		}
	}
	else if (estent == 119)
	{
		if (estsal == 110)
		{
			estent = 119;
			estsal = 104;
		}
		else if (estsal == 118)
		{
			estent = 119;
			estsal = 105;
		}
		else if (estsal == 109)
		{
			estent = 119;
			estsal = 103;
		}
		else if (estsal == 117)
		{
			estent = 119;
			estsal = 105;
		}
	}
	else if (estent == 118)
	{
		if (estsal == 110)
		{
			estent = 118;
			estsal = 115;
		}
		else if (estsal == 117)
		{
			estent = 118;
			estsal = 106;
		}
		else if (estsal == 109)
		{
			estent = 118;
			estsal = 102;
		}
		else if (estsal == 119)
		{
			estent = 118;
			estsal = 113;
		}

	}
	else if (estent == 109)
	{
		if (estsal == 118)
		{
			estent = 106;
			estsal = 115;
		}
		else if (estsal == 110)
		{
			estent = 102;
			estsal = 109;
		}
		else if (estsal == 117)
		{
			estent = 109;
			estsal = 103;
		}
		else if (estsal == 119)
		{
			estent = 109;
			estsal = 114;
		}
		else if (estsal == 108)
		{
			estent = 109;
			estsal = 102;
		}

	}
	else if (estent == 108)
	{
		if (estsal == 109)
		{
			estent = 108;
			estsal = 115;
		}
		else if (estsal == 110)
		{
			estent = 108;
			estsal = 112;
		}
	}
	else if (estent == 102)
	{
		if (estsal == 103)
		{
			estent = 108;
			estsal = 102;
		}
		else if (estsal == 104)
		{
			estent = 102;
			estsal = 109;
		}
		else if (estsal == 105)
		{
			estent = 102;
			estsal = 111;
		}
	}
	else if (estent == 103)
	{
		if (estsal == 102)
		{
			estent = 103;
			estsal = 111;
		}
		else if (estsal == 104)
		{
			estent = 103;
			estsal = 118;
		}
		else if (estsal == 105)
		{
			estent = 103;
			estsal = 119;
		}

	}
	else if (estent == 104)
	{
		if (estsal == 102)
		{
			estent = 115;
			estsal = 108;
		}
		else if (estsal == 103)
		{
			estent = 113;
			estsal = 106;
		}

	}
	else if (estent == 105)
	{
		if (estsal == 102)
		{
			estent = 113;
			estsal = 108;
		}
		else if (estsal == 103)
		{
			estent = 112;
			estsal = 106;
		}
		else if (estsal == 104)
		{
			estent = 110;
			estsal = 103;
		}
		else if (estsal == 106)
		{
			estent = 105;
			estsal = 112;
		}
	}
	else if (estent == 106)
	{
		if (estsal == 105)
		{
			estent = 108;
			estsal = 103;
		}
	}



}