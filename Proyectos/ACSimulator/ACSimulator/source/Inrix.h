#ifndef _INRIX_H
#define _INRIX_H

#include "Simulator.h"
#include "Controls.h"
#include "Render.h"
#include "Menu.h"
#include "SimCurl.h"

#define SIM_MAX_SEGMENTS 500
#define SIM_MAX_SEGMEN_COORD 500

struct SSegCoords
{
	double lon;
	double lat;
};

struct SSegInfo
{
	long long int segid;
	long long int prevsegid;
	long long int nextsegid;
	SSegCoords coords_original[SIM_MAX_SEGMEN_COORD];
	SSegCoords coords_mapped[SIM_MAX_SEGMEN_COORD];

	Cadena time;
	double speed;
	double average;
	double reference;
	double travel_time_minutes;
	int speed_bucket;
	int c_value;
	int score;

};



class Inrix
{
	public:

		enum eInrixSpeed
		{
			SIM_INRIX_SPEED_NONE,
			SIM_INRIX_SPEED, 
			SIM_INRIX_SPEED_BUCKET, 
			SIM_INRIX_SPEED_AVERAGE, 
			SIM_INRIX_SPEED_REFERENCE
		};

		eInrixSpeed m_speed_type;
		QuadMap* m_quad_map;
		BaseDatos mDB;
		BaseDatos mDB_ext;
		SimCurl   mCurl;
		SimCurl::SCurl_Buffer msCurlBuff;
		SJsonHandler msJSONhandler;
		HANDLE mMutex;

		Controls* mCtrl;
		Menu* mpMN;
		Render mR;
		SSegInfo* mseginfo;
		map<long long int,Color> mseg_color;
		int m_maxseg;
		int m_grp_segment_gps_pts;
		int m_grp_segment_limits;
		int m_grp_segment_line;

		bool m_active;

		int m_color_count;
		bool m_refresh_color;

		Cadena mcadgral;

	public:

		Inrix();
		~Inrix();

		void init(Controls* ctrl, Menu* mn, QuadMap* quad_map, Cadena cadgral);
		void get_segments();
		Color get_color(double speed, double average, double reference, int speed_bucket);
		void parse_segment(int index, SJsonHandler& msJSONhandler);
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void refresh_thread();
		void set_reset_speed(enum eInrixSpeed e_speed);
		void set_reset_speed_bucket(enum eInrixSpeed e_speed);
		bool is_active();
		void refresh();

		Color get_x_color();

};
#endif
