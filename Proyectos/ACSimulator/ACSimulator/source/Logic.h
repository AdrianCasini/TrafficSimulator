#ifndef _LOGIC_H
#define _LOGIC_H

#include "Simulator.h"
#include "Vehiculo.h"

class Traffic;



class Logic
{
	public:
		bool     mDebug;
		Traffic* mpTR;

		typedef void (Logic::*do_move) (Vehiculo*, int& );

		do_move mpFunc_domove;

		
	public:

		Logic();
		~Logic();

		void init(Traffic* ptr);
		void do_alg_overpass(Vehiculo* pvh, int&);
		void do_alg_simple(Vehiculo* pvh, int&);
		bool move_overpass(Vehiculo* pVH, double dist, double dt_n2n, double dt, bool change_lane, int vh_inf, int vh_sup);
		bool move_simple(Vehiculo* pVH);
		bool move_outsider(NodoInfo*& ptr, int maxnodestrip, double speed, int& ordennode, double& nd2nd, double& posx, double& posy);

		void set_alg( int ealg);
		void log(Vehiculo* pVH);
		Cadena get_text_state(int state);

		

};

#endif

