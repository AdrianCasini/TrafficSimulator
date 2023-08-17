#include "Inrix.h"


//------------------------------------------------------------------------------
Inrix::Inrix()
{
	
}

//------------------------------------------------------------------------------
Inrix::~Inrix()
{
	mDB.desconectar();
	mDB_ext.desconectar();
}

//------------------------------------------------------------------------------
void Inrix::init(Controls* ctrl, Menu* mn, QuadMap* quad_map, Cadena cadgral)
{
	mCtrl = ctrl;
	mpMN = mn;
	ColorF color_pts, color_limits;
	m_quad_map = quad_map;
	m_active = false;
	m_maxseg = 0;
	mcadgral = cadgral;

	msJSONhandler.array_item = new Cadena[1000];
	msJSONhandler.count_item = 0;
	msCurlBuff.m_buff = new char[10000];
	msCurlBuff.m_indx = 0;


	mseginfo = new SSegInfo[SIM_MAX_SEGMENTS];
	mseg_color.clear();

	if (mDB.conectar(SIM_DB_HOST, SIM_DB_PORT, SIM_DB_NAME, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) {};
    if (mDB_ext.conectar(SIM_DB_HOST, SIM_DB_PORT, SIM_DB_NAME, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) {};

	mR.init(ctrl, quad_map);

	color_limits.r = 0.22f;
	color_limits.g = 0.88f;
	color_limits.b = 0.88f;
	color_limits.f = 0.50f;

	color_pts.r = 0.88f;
	color_pts.g = 0.88f;
	color_pts.b = 0.99f;
	color_pts.f = 0.50f;

	m_grp_segment_gps_pts = mR.add_group_point("segments_gps_pts", color_pts, 1, 0.05);
	m_grp_segment_limits  = mR.add_group_point("segments_limits", color_limits, 0, 1.9);
	m_grp_segment_line    = mR.add_group_line("segments_lines", 0, 0.9);

	get_segments();

	m_refresh_color = false;
	mMutex = CreateMutex(NULL, FALSE, NULL);
	m_speed_type = SIM_INRIX_SPEED_NONE;
	
}

//------------------------------------------------------------------------------
void Inrix::get_segments()
{
	Cadena sql_query;
	int num_reg;

	sql_query.formCadena("SELECT section, ver, XDSegID, PreviousXD, NextXDSegI FROM Segments");
	mDB.ejecutarConsulta(sql_query);
	m_maxseg = mDB.getNumRegistros();

	for (int i = 0; i < m_maxseg; i++)
	{
		mseginfo[i].segid     = long long(mDB.getDouble("XDSegID", i));
		if (mseginfo[i].segid != 447327497)
		{
			mseginfo[i].prevsegid = long long(mDB.getDouble("PreviousXD", i));
			mseginfo[i].nextsegid = long long(mDB.getDouble("NextXDSegI", i));

			sql_query.formCadena("SELECT lon, lat, nid from Segnodes WHERE XDSegID = %lld", mseginfo[i].segid);
			mDB_ext.ejecutarConsulta(sql_query);
			num_reg = mDB_ext.getNumRegistros();

			for (int j = 0; j < num_reg; j++)
			{
				if (j < SIM_MAX_SEGMEN_COORD)
				{
					mseginfo[i].coords_original[j].lon = mDB_ext.getDouble("lon", j);
					mseginfo[i].coords_original[j].lat = mDB_ext.getDouble("lat", j);

					mR.add_gps_point(m_grp_segment_gps_pts, mseginfo[i].coords_original[j].lon, mseginfo[i].coords_original[j].lat);
					mR.add_gps_point_line(m_grp_segment_line, mseginfo[i].coords_original[j].lon, mseginfo[i].coords_original[j].lat, mseginfo[i].segid);

					if ((j == 0) || (j == (num_reg - 1)))
					{
						mR.add_gps_point(m_grp_segment_limits, mseginfo[i].coords_original[j].lon, mseginfo[i].coords_original[j].lat);
					}
				}
				else
				{
					cout << "Error: the number of segment coords are greater than: " << SIM_MAX_SEGMEN_COORD << " SegID: [" << mseginfo[i].segid << "]" << endl;
				}
			}
		}
	}

	mR.finish_line(m_grp_segment_line);

}

//------------------------------------------------------------------------------
void Inrix::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	if (WaitForSingleObject(mMutex, INFINITE) == WAIT_OBJECT_0)
	{
		if (m_refresh_color == true)
		{
			m_refresh_color = false;
			mR.set_color(m_grp_segment_line, mseg_color);
		}
	}
	ReleaseMutex(mMutex);

	m_color_count++;
	mR.render(viewmatrix, viewprojectionmatrix);
}

//------------------------------------------------------------------------------
void Inrix::refresh()
{
	mR.refresh();
}

//------------------------------------------------------------------------------
void Inrix::set_reset_speed_bucket(enum eInrixSpeed e_speed)
{
	if (m_active)
	{
		m_active = false;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED_BUCKET].flag = false;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED].flag = false;
		m_speed_type = SIM_INRIX_SPEED_NONE;
	}
	else
	{
		m_active = true;
		m_speed_type = e_speed;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED_BUCKET].flag = true;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED].flag = false;
	}
}

//------------------------------------------------------------------------------
void Inrix::set_reset_speed(enum eInrixSpeed e_speed)
{
	if (m_active)
	{
		m_active = false;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED].flag = false;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED_BUCKET].flag = false;
		m_speed_type = SIM_INRIX_SPEED_NONE;
	}
	else
	{
		m_active = true;
		m_speed_type = e_speed;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED].flag = true;
		mpMN->mButtons[Menu::SIM_BTT_INRIX_SPEED_BUCKET].flag = false;
	}
}


//------------------------------------------------------------------------------
bool Inrix::is_active()
{
	return m_active;
}

//------------------------------------------------------------------------------
void Inrix::refresh_thread()
{
	Cadena sql_query;
	Cadena cad_url_path, cad_url, cad_header;
	Cadena cad_datetime;
	int min = 60 * 1000;//miliseconds

	cad_header = "x-api-key: ydrOZOsKwb1YnnMsDXOSdayAvhteS1oH81Tj7tW8";
	cad_url_path = "https://traffic-api.autopistashub.com/v1/segments/historic-speed?";

	while (true)
	{
		if (m_active)
		{
			cad_datetime = Simulator::get_timestamp_GMT_format_YYYYMMDD_X_hhmmss();

			for (int i = 0; i < m_maxseg; i++)
			{
				cout << "---------------------------------------------------------------------" << endl;
				cad_url.formCadena("%sids=%lld&dateTime=%s", cad_url_path.getCadena(), mseginfo[i].segid, cad_datetime.getCadena());
				cout << "GET INFO URL:[" << cad_url.getCadena() << "]" << endl;
				if (mCurl.curl_get(cad_url, cad_header, msCurlBuff) == true)
				{
					mCurl.json_parser(msCurlBuff.m_buff, msJSONhandler);
					parse_segment(i, msJSONhandler);
					msJSONhandler.count_item = 0;
					msCurlBuff.m_indx = 0;

					cout << "|-----speed:[" << mseginfo[i].speed << "]" << endl;
					cout << "|-----speed average:[" << mseginfo[i].average << "]" << endl;
					cout << "|-----speed reference (FreeFlow):[" << mseginfo[i].reference << "]" << endl;
					cout << "|-----speed_bucket:[" << mseginfo[i].speed_bucket << "]" << endl;
				}
			}

			if (WaitForSingleObject(mMutex, INFINITE) == WAIT_OBJECT_0)
			{
				for (int i = 0; i < m_maxseg; i++)
				{
					mseg_color[mseginfo[i].segid] = get_x_color();//get_color(mseginfo[i].speed, mseginfo[i].average, mseginfo[i].reference, mseginfo[i].speed_bucket);
				}
				m_refresh_color = true;
			}
			ReleaseMutex(mMutex);
			Sleep(2 * min);
		}
		else {Sleep(3000);}
	}
}


//------------------------------------------------------------------------------
Color Inrix::get_x_color()
{
	Color color;

	color.f = 50;
	color.r = rand() * 255; 
	color.g = rand() * 255; 
	color.b = rand() * 255;
	
	return color;
}

//------------------------------------------------------------------------------
Color Inrix::get_color(double speed, double average, double reference, int speed_bucket)
{
	Color color;
	color.f = 240;

	if (m_speed_type == SIM_INRIX_SPEED_NONE)
	{
		color.f = 100;
		color.r = 250; color.g = 250; color.b = 250;
	}
	else if (m_speed_type == SIM_INRIX_SPEED_BUCKET)
	{
		switch (speed_bucket)
		{

			case 0:  //(speed_bucket >= 0) && (speed_bucket < 25)  //DARK RED
				color.r = 130; color.g = 20; color.b = 20;
				break;

			case 1: //(speed_bucket >= 26) && (speed_bucket < 45) //RED
				color.r = 255; color.g = 200; color.b = 200;
				break;

			case 2: //(speed_bucket >= 46) && (speed_bucket < 75) //YELLOW
				color.r = 255; color.g = 255; color.b = 0;
				break;

			case 3: //(speed_bucket >= 76) //GREEN
				color.r = 50; color.g = 255;	color.b = 50;
				break;

			default: //GRAY
				color.r = 200; color.g = 200; color.b = 200;
			}

	}
	else if (m_speed_type == SIM_INRIX_SPEED)
	{

		color.f = 200;
		if ((speed >= 0) && (speed < 50))  //DARK RED
		{
			color.r = 130; color.g = 10; color.b = 10;
		}
		else if ((speed >= 50) && (speed < 70)) //RED
		{
			color.r = 255; color.g = 40; color.b = 40;
		}
		else if ((speed >= 70) && (speed < 90)) //YELLOW
		{
			color.r = 255; color.g = 255; color.b = 0;
		}
		else if (speed >= 90) //GREEN
		{
			color.r = 50; color.g = 255;	color.b = 50;
		}
		else //GRAY
		{
			color.r = 200; color.g = 200; color.b = 200;
		}

	}

	return color;
}

//------------------------------------------------------------------------------
/*

[startobject]
	[copyright]
		[Autopistas HUB Traffic API]
	[source]
		[INRIX Inc.]
	[versionNumber]
		[v1]
	[createdDate]
		[Fri, 18 Nov 2022 11:33:39 GMT]
	[requestId]
		[8d92cf06-052e-4f76-87c6-7523aa5528c5]
	[result]
		[startobject]
			[unit]
				[KPH]
			[segmentspeeds]
			[startarray]
				[startobject]
					[time]
						[2022-11-18 11:28:00]
					[segments]
						[startarray]
							[startobject]
								[code]
									[202358274]
								[type]
									[XDS]
								[speed]
									[89]
								[average]
									[87]
								[reference]
									[89]
								[travelTimeMinutes]
									[0.502000]
								[speedBucket]
									[3]
								[c-value]
									[100]
								[score]
									[30]
							[endobject]
						[endarray]
				[endobject]
			[endarray]
	[endobject]
[endobject]

*/

void Inrix::parse_segment(int index, SJsonHandler& msJSONhandler)
{
	int start_obj=0;

	for (int x = 0; x < msJSONhandler.count_item; x++)
	{
		if (msJSONhandler.array_item[x] == "startobject")
		{
			start_obj++;
		}
		else if ((start_obj == 4) && (msJSONhandler.array_item[x] == "speed"))
		{
			mseginfo[index].speed = msJSONhandler.array_item[x + 1].toFloat();
		}
		else if ((start_obj == 4) && (msJSONhandler.array_item[x] == "average"))
		{
			mseginfo[index].average = msJSONhandler.array_item[x + 1].toFloat();
		}
		else if ((start_obj == 4) && (msJSONhandler.array_item[x] == "reference"))
		{
			mseginfo[index].reference = msJSONhandler.array_item[x + 1].toFloat();
		}
		else if ((start_obj == 4) && (msJSONhandler.array_item[x] == "speedBucket"))
		{
			mseginfo[index].speed_bucket = msJSONhandler.array_item[x + 1].toInt();
		}
		else if (msJSONhandler.array_item[x] == "endobject")
		{
			start_obj--;
		}
	}
}
