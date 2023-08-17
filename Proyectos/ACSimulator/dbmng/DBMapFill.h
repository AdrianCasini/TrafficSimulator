#ifndef _DBMAPFILL_H
#define _DBMAPFILL_H

#include "Simulator.h"
#include "Control.h"
#include <regex>

class DBMapFill
{
	public:

		Control* mpctrl;
		BaseDatos mdb;
		long int mprocessed;
		long int mprocessed_old;



	public:

		DBMapFill();
		~DBMapFill();

		void init(Control*);
		void execute();
		int get_timestamp_num(Cadena& timestamp);


};

#endif