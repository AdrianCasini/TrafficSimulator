//================================================================
// ACSimulator: Test.h
//================================================================
//  FILE: Test.h
//  DESCRIPTION :
//  CREATION DATE : 01 - 06 - 2020
//  DESIGN AUTHOR : Adrian Casini
//  DEVELOPER : Adrian Casini
//  COMPANY : Adrian Casini
//================================================================

#ifndef _TEST_H
#define _TEST_H

#include "Simulator.h"
#include "Source.h"
#include "Vehiculo.h"
#include "Trip.h"
#include "Toller.h"
#include "Tools.h"
#include "Source.h"
#include "Menu.h"
#include "Proxy.h"
#include "FileInfo.h"
#include "NodeOrder.h"


struct TripUse
{
	char used;
	int index;
};


struct TestHandle
{
	Vehiculo*  pVH;
	BaseDatos* pDB;
	TripInfo*  pTripI;
	Trip*      pTrip;
	Proxy*     pPxy;
	NodeOrder* pNO;
	HANDLE     source_mutex;

	int        trip_index;
	map<Cadena, TripUse> map_trip_name;
	
	int        norder_index;
	map<Cadena, int> map_norder;

	int vh_alive;
	int vh_alive_real_time;
	int guard;

	int state;
	int type;
	int render;

	int index_veh;
	int index_gral;
	Cadena dir_init;

	long long int gps_counter;

	int pw_speed;
	double delta_lane;

	int    db_time;
	double last_time;
	double current_time;
	double delta_time;

};



class Test
{
	public:

		Cadena      mVehicleSource;
		Cadena      mDBName;
		Cadena      mMomento;
		int         mMaxVehicles;
		int         mRepeat;

		Source      mSrc;
		TestHandle* mpTH;

		FileInfo    mItems;
		FileInfo    mpData;
		FileInfo    mpTest;

		map<int, int> m_map_distance;

		FILE*       mSaveTest;
		double m_delta_time_random;

		Cadena mcadgral;

	public:

		Test();
		~Test();

		void init(TestHandle* phandle, Cadena cadgral);
		void reset();
		void reset_veh();
		void get_simulation_info(Cadena cafile);
		bool setup_vh_from_file(int i);
		bool setup_vh_from_db(int i);
		bool setup_vh_from_random(int i);

		int get_random_delta_timefollowing(Source::Line* psource, int index);
		Cadena get_random_category(Source::Line* psource, int& index_type_cat);
		int get_random_speed(Source::Line* psource, int index);


		void get_new_vehicle_info();
		bool set_new_vehicle_info(Simveh* ptrSimVH);
		bool set_new_vehicle_info_from_bin(Vehiculo* pvh);

		int  get_new_index(int index);
		bool get_trip_index(Cadena cadtrip_name, Cadena cad_type, int& index_veh, int& index_master);

		void setget_following_start(Vehiculo* pVH, int n_vh_order);
		void setget_following(Vehiculo* pVH, int n_vh_order, int n_vh_order_old);
		void setget_following_lane(Vehiculo* pVH, bool new_node, int vh_inf, int vh_sup);
		void set_nodeorder(NodoInfo* ptr);
		
		//--- gral functions -----
		void get_query(Cadena& cad, int mnt, int estent, int estsal);
		bool is_on_time(double time_ini);
		void get_AP7DB_distance(Cadena cadfile);
		void save_test(Cadena cadfilename);
		void set_test(Cadena cadfilename);

		void log_veh_in(Vehiculo* pvh);
		void log_veh_out(Vehiculo* pvh);

};

#endif

