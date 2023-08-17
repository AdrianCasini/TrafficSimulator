#ifndef _DBMNGR_H
#define _DBMNGR_H

#include "Simulator.h"
#include "Control.h"
#include <iostream>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>


class DBMngr
{
	public:

		int mDBPort;
		Cadena mDBDir;
		Control* mpctrl;

	public:

		DBMngr();
		~DBMngr();

		void init(Control*);
		void start_db();
		void stop_db();
		void create_database();
		void init_db();
		void delete_db_dir();
		void create_tables();
		void droptable_db();



		void execute();
		void create_db();


};

#endif