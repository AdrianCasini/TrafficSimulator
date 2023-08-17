#ifndef _TEXTER_H
#define _TEXTER_H

#include "Simulator.h"
#include "FileInfo.h"
#include "Control.h"


class Texter
{
	public:

		Control* mpctrl;
		BaseDatos mdb;
		BaseDatos mdb_r;
		BaseDatos mdb_r2;
		FileInfo  mfileinfo;


	public:

		Texter();
		~Texter();

		void init(Control*);
		void execute();


};

#endif