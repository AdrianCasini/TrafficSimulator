#ifndef _CONTROL_H
#define _CONTROL_H

#include "Simulator.h"



class Control
{
	public:

		bool mtext;
		bool mtext_droptable;

		bool mdb_m;
		bool mdb_start;
		bool mdb_stop;
		bool mdb_create;
		bool mdb_drop;

		bool mbdb_dir;
		bool mbdb_tables;
		bool mbdb_name;
		bool mbdb_port;
		bool mbfilemap;
		bool mbdrop_table;
		bool mbdrop_tablename;
		bool mbfseek;

		bool mdb_map_fill;

		
		bool mdb_tr_fill;
		bool mbtr_source;
		Cadena mtr_source;

		int mdb_port;

		Cadena mfilemap;

		Cadena mdirexec;
		Cadena mdb_dirinit;
		Cadena mdb_dir;
		Cadena mdb_name;
		Cadena mdb_tables;
		Cadena mdb_droptable;
		long long int mfseek;




	public:

		Control();
		~Control();

		void process_args(int argc, const char* argv[]);
		void error(const char* argv[]);
		void showhelp(const char* argv[]);


};

#endif