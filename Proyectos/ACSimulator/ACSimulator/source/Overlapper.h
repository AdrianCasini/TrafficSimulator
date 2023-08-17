#ifndef _OVERLAPPER_H
#define _OVERLAPPER_H

#include "Simulator.h"
#include "Tools.h"
#include "Menu.h"
#include "Test.h"
#include "PofI.h"
//#include "Model.h"


class Overlapper
{
	public:
		bool mDebug;
		
	public:

		Overlapper();
		~Overlapper();

		void init();

		bool overlap_new(Vehiculo* pVH, Vehiculo* pVH_any, double dist, int& index_next);
		bool overlap_lane_new(Vehiculo* pVH, Vehiculo* pVH_any, double dist, int& vh_inf, int& vh_sup);
	
};

#endif

