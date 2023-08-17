#ifndef _TRIP_H
#define _TRIP_H

#include "Simulator.h"
#include "Vehiculo.h"

class Trip
{
	public:

		struct SPoint
		{
			double lat;
			double lon;

			double X;
			double Y;

			int lanes;

			long long int nid;
		};


		long long int* mArrayWays;
		Cadena*  mArrayWaysPoints;
		SPoint*  mArrayPoints;
		WayInfo  mWayInfo;
		NodoInfo mNodoInfo[5000];

		FILE*        mpFichero;
		BaseDatos*   mpDB;

		Cadena mcadgral;
		
	public:

		Trip();
		~Trip();

		void init(BaseDatos* mpdb, Cadena cadgral);

		bool get_trip(Cadena file, Cadena cadtype, TripInfo* trip_info);
		bool get_trip_way(Cadena file, TripInfo* trip_info);
		bool get_trip_route(Cadena file, TripInfo* trip_info);
		bool get_trip_point(Cadena file, TripInfo* trip_info, int count_nodes_ext);
		void get_way_info_from_nodes(TripInfo* trip_info);

		bool get_way(long long wid, WayInfo* mWayInfo);
		bool get_way_db(long long wid, WayInfo* way_info);
		void get_way_reverse(WayInfo*, int*);

};

#endif
