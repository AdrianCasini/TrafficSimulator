#ifndef _DBTRAFFICFILL_H
#define _DBTRAFFICFILL_H

#include "Simulator.h"
#include "Control.h"
#include <regex>

class DBTrafficFill
{
	public:

		Control* mpctrl;
		BaseDatos mdb;
		long long int mprocessed;
		long long int mprocessed_old;



	public:

		DBTrafficFill();
		~DBTrafficFill();

		void init(Control*);
		void execute();
		int get_timestamp_num(Cadena& timestamp);


};

#endif