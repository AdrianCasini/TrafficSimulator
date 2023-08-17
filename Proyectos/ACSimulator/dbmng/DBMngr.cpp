#include "DBMngr.h"


//------------------------------------------------------------------------------
DBMngr::DBMngr()
{
	mpctrl = NULL;
}

//------------------------------------------------------------------------------
DBMngr::~DBMngr()
{

}

//------------------------------------------------------------------------------
void DBMngr::init(Control* pctrl)
{
	mpctrl = pctrl;
	mDBPort = mpctrl->mdb_port;
	mDBDir = "DataBase";
}

//------------------------------------------------------------------------------
void DBMngr::create_db()
{
	stop_db();
	delete_db_dir();
	init_db();
	start_db();
	create_database();
	create_tables();
}

//------------------------------------------------------------------------------
void DBMngr::delete_db_dir()
{
	Cadena dir2delete;
	dir2delete.formCadena("%s/%s/%s", mpctrl->mdb_dirinit.getCadena(), mDBDir.getCadena(), mpctrl->mdb_name.getCadena());
	cout << endl << "Deleting DataBase Directory:[" << dir2delete.getCadena() << "]" << endl;
	cout << "-----------------------------------------------------------" << endl;
	std::filesystem::remove_all(dir2delete.getCadena());
}

//------------------------------------------------------------------------------
void DBMngr::init_db()
{
	Cadena execute;
	cout << endl << "Initializing DB" << endl;
	cout << "-----------------------------------------------------------" << endl;
	execute.formCadena("%s%s%s -A trust -U %s -D %s/%s/%s", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_INIT, SIM_DB_USER, mpctrl->mdb_dirinit.getCadena(), mDBDir.getCadena(), mpctrl->mdb_name.getCadena());
	cout << execute.getCadena() << endl;
	system(execute.getCadena());
}


//------------------------------------------------------------------------------
void DBMngr::start_db()
{
	Cadena cadexe;

	cout << endl << "Iniciando servicio de Base de Datos:[" << mpctrl->mdb_name.getCadena() << "]" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	cadexe.formCadena("%s%s%s -U %s -D %s/%s/%s start ", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PGCTL, SIM_DB_USER, mpctrl->mdb_dirinit.getCadena(), mDBDir.getCadena(), mpctrl->mdb_name.getCadena());
	cout << cadexe.getCadena() << endl;
	system(cadexe.getCadena());

}

//------------------------------------------------------------------------------
void DBMngr::create_database()
{
	Cadena execute;

	cout << endl << "Creating data base:[" << mpctrl->mdb_name.getCadena() << " ]" <<  endl;
	cout << "-----------------------------------------------------------" << endl;
	execute.formCadena("%s%s%s -U %s -e %s", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_CRT, SIM_DB_USER, mpctrl->mdb_name.getCadena());
	cout << execute.getCadena() << endl;
	system(execute.getCadena());

}

//------------------------------------------------------------------------------
void DBMngr::create_tables()
{
	Cadena execute;

	cout << endl << "Creating Tables base:[" << mpctrl->mdb_tables.getCadena() << " ]" << endl;
	cout << "-----------------------------------------------------------" << endl;
	execute.formCadena("%s%s%s -U %s -d %s -f %s", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PSQL, SIM_DB_USER, mpctrl->mdb_name.getCadena(), mpctrl->mdb_tables.getCadena());
	cout << execute.getCadena() << endl;
	system(execute.getCadena());

}


/*
//------------------------------------------------------------------------------
void DBMngr::stop_db()
{
	Cadena cadexe;

	cout << endl << "Deteniendo servicio de Base de Datos" << endl;
	cout << "-----------------------------------" << endl;

	cadexe.formCadena("%s%s%s -U %s -D %s%s stop ", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PGCTL, SIM_DB_USER, mpctrl->mdb_dirinit.getCadena(), SIM_DIR_DB_NAME);
	cout << cadexe.getCadena() << endl;
	system(cadexe.getCadena());

}*/

//------------------------------------------------------------------------------
void DBMngr::stop_db()
{
	Cadena cadexe;

	cout << endl << "Deteniendo servicio de Base de Datos:[" << mpctrl->mdb_name.getCadena() << "]" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	cadexe.formCadena("%s%s%s -U %s -D %s/%s/%s stop ", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PGCTL, SIM_DB_USER, mpctrl->mdb_dirinit.getCadena(), mDBDir.getCadena(), mpctrl->mdb_name.getCadena());
	cout << cadexe.getCadena() << endl;
	system(cadexe.getCadena());

}

//------------------------------------------------------------------------------
void DBMngr::droptable_db()
{
	Cadena cadexe;

	cout << endl << "Droping Table :[" << mpctrl->mdb_droptable.getCadena() << "]" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	cadexe.formCadena("%s%s%s -U %s -d %s -c \"DROP TABLE %s\"", mpctrl->mdirexec.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PSQL, SIM_DB_USER,  mpctrl->mdb_name.getCadena(), mpctrl->mdb_droptable.getCadena() );
	cout << cadexe.getCadena() << endl;
	system(cadexe.getCadena());

}

//------------------------------------------------------------------------------
void DBMngr::execute()
{
	if (mpctrl->mdb_start)
	{
		start_db();
	}
	else if (mpctrl->mdb_stop)
	{
		stop_db();
	}
	else if (mpctrl->mdb_create)
	{
		create_db();
	}
	else if (mpctrl->mbdrop_table)
	{
		droptable_db();
	}
}
