
#include "../ACSimulator/source/Simulator.h"
#include "../ACSimulator/source/FileInfo.h"
#include "BaseDatos.h"
#include "Texter.h"
#include "Control.h"
#include "DBMngr.h"
#include "DBMapFill.h"
#include "DBTrafficFill.h"

//========================================================================================================
int main(int argc, const char* argv[])
{
	Control ctrl;
	Texter tx;
	DBMapFill dbmapfill;
	DBTrafficFill dbtrfill;
	DBMngr dbm;
	
	ctrl.process_args(argc, argv);


	if (ctrl.mtext)
	{
		tx.init(&ctrl);
		tx.execute();
	}
	//To manage DB
	else if (ctrl.mdb_m)
	{
		dbm.init(&ctrl);
		dbm.execute();
	}
	else if (ctrl.mdb_map_fill)
	{
		dbmapfill.init(&ctrl);
		dbmapfill.execute();

	}
	else if (ctrl.mdb_tr_fill)
	{
		dbtrfill.init(&ctrl);
		dbtrfill.execute();

	}

	return 0;
}
//========================================================================================================

