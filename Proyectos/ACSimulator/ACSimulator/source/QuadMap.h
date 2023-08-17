#ifndef _QUADMAP_H
#define _QUADMAP_H

#include "Simulator.h"


class QuadMap
{

	public:

		struct QuadRow
		{
			GPoint Qxy[SIM_MAP_QUADS];
		};


		QuadRow* mQuadMng;

		double mDelta_lat;
		double mDelta_lon;

	public:
		
		QuadMap();
		~QuadMap();

		void init();
		void get_ref(double x, double y, double& xref, double& lonref, double& yref, double& latref, double& lat, double& lon);
		void set_quad(double corner_NW_lat, double corner_NW_lon, double corner_SE_lat, double corner_SE_lon);
		

};

#endif

