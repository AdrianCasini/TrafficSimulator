#include "Control.h"


//------------------------------------------------------------------------------
Control::Control()
{
	mtext           = false;
	mtext_droptable = false;
	mdb_m           = false;
	mdb_start       = false;
	mdb_stop        = false;
	mdb_map_fill    = false;
	mdb_create      = false;


	mdb_tr_fill = false;
    mbtr_source = false;


	mfseek = 0;

	mbdb_dir = false;
	mbdb_tables = false;
	mbdb_name = false;
	mbdb_port = false;
	mbfilemap = false;
	mbfseek = false;
	mdb_port = 5432;

}

//------------------------------------------------------------------------------
Control::~Control()
{

}

//------------------------------------------------------------------------------
void Control::process_args(int argc, const char* argv[])
{
	size_t found;
	string str = argv[0];
	found = str.find_first_of("/\\");
	mdirexec = (str.substr(0, found)).c_str();
	bool bfilldb = false;
	bool bfilltr = false;

	if (argc >= 2)
	{
		int i = 1;
		//for (int i = 1; i < argc; i++)
		//{
			if (strcmp(argv[i], "-text") == 0)
			{
				mtext = true;
				if (strcmp(argv[i + 1], "d") == 0)       {mtext_droptable = true;}
				else if (strcmp(argv[i + 1], "nd") == 0) {mtext_droptable = false;}
				else                                     {error(argv);}
			}
			//------------------------------------------------------------------------
			// To fill the Database with transit info
			//------------------------------------------------------------------------
			else if (strcmp(argv[i], "-filltr") == 0)
			{
				for (int j = i + 1; j < argc; j++)
				{
					bfilltr = true;
					if ((strcmp(argv[j], "-f") == 0) && (bfilltr))
					{
						mtr_source = argv[j + 1];
						mbtr_source = true;
					}
					else if ((strcmp(argv[j], "-n") == 0) && (bfilltr))
					{
						mdb_name = argv[j + 1];
						mbdb_name = true;
					}
				}

				if ((bfilltr) && (mbtr_source) && (mbdb_name))
				{
					mdb_tr_fill = true;

				}
				else
				{
					{showhelp(argv); }
				}
			}
			//------------------------------------------------------------------------
			// To fill the Database with Map info
			//------------------------------------------------------------------------
			else if (strcmp(argv[i], "-filldb") == 0)
			{
				bfilldb = true;
				for (int j = i + 1; j < argc; j++)
				{
					if ((strcmp(argv[j], "-n") == 0) && (bfilldb))
					{
						mdb_name = argv[j + 1];
						mbdb_name = true;
						//cout << "-n" << endl;
					}
					else if ((strcmp(argv[j], "-f") == 0) && (bfilldb))
					{
						mfilemap = argv[j + 1];
						mbfilemap = true;
						//cout << "-n" << endl;
					}
					else if ((strcmp(argv[j], "-s") == 0) && (bfilldb))
					{
						mfseek = atoll(argv[j + 1]);
						mbfseek = true;
						//cout << "-n" << endl;
					}

				}

				if ((bfilldb) && (mbdb_name) && (mbfilemap))
				{
					mdb_map_fill = true;
					cout << "DB Name:" << mdb_name.getCadena() << endl;
					cout << "DB File Map:" << mfilemap.getCadena() << endl;
				}
				else
				{
					error(argv);
				}
			}
			else if (strcmp(argv[i], "-help") == 0) 		 {showhelp(argv);}
			//------------------------------------------------------------------------------------------------------
			//                       DB MANAGER
			//------------------------------------------------------------------------------------------------------
			else if (strcmp(argv[i], "-mngdb") == 0)
			{
				for (int j = i+1; j < argc; j++)
				{
					cout << "argv[j]" << argv[j] << endl;
					if (strcmp(argv[j], "-start") == 0)
					{
						mdb_start = true;
					}
					else if (strcmp(argv[j], "-stop") == 0)
					{
						mdb_stop = true;
					}
					else if (strcmp(argv[j], "-droptable") == 0)
					{
						cout << "mbdrop_table"  << endl;
						mbdrop_table = true;
					}
					else if (strcmp(argv[j], "-create") == 0)
					{
						mdb_create = true;
					}
					else if ((strcmp(argv[j], "-n") == 0) && ((mdb_create) || (mdb_start) || (mdb_stop) || (mdb_drop)))
					{
						mdb_name = argv[j+1];
						mbdb_name = true;
					}
					else if ((strcmp(argv[j], "-t") == 0) && (mdb_create))
					{
						mdb_tables = argv[j + 1];
						mbdb_tables = true;
					}
					else if ((strcmp(argv[j], "-d") == 0) && ((mdb_create) || (mdb_start) || (mdb_stop)))
					{
						mdb_dirinit = argv[j + 1];
						mbdb_dir = true;
					}		
					else if ((strcmp(argv[j], "-tb") == 0) && (mdb_drop))
					{
						mdb_droptable = argv[j + 1];
						mbdrop_tablename = true;
					}
				}

				if ((mdb_create) && (mbdb_dir) && (mbdb_tables) && (mbdb_name))
				{
					mdb_m = true;
					cout << "DB Name:" << mdb_name.getCadena() << endl;
					cout << "DB Tables:" << mdb_tables.getCadena() << endl;
					cout << "DB Dir:" << mdb_dirinit.getCadena() << endl;
				}
				else if ((mdb_start) && (mbdb_dir) && (mbdb_name))
				{
					mdb_m = true;
					cout << "DB Name:" << mdb_name.getCadena() << endl;
					cout << "DB Dir:" << mdb_dirinit.getCadena() << endl;
				}
				else if ((mdb_stop) && (mbdb_dir) && (mbdb_name))
				{
					mdb_m = true;
					cout << "DB Name:" << mdb_name.getCadena() << endl;
					cout << "DB Dir:" << mdb_dirinit.getCadena() << endl;

				}
				else if ((mbdrop_table) && (mbdrop_tablename) && (mbdb_name))
				{
					mdb_m = true;
					cout << "DB Name:" << mdb_name.getCadena() << endl;
					cout << "DB Dir:" << mdb_droptable.getCadena() << endl;

				}
				else
				{
					error(argv);
				}
				
			}
		//}
	}
	else
	{
		error(argv);
	}
}

//------------------------------------------------------------------------------
void Control::showhelp(const char* argv[])
{
	Cadena help;

	help.formCadena("\nExecute %s with the following arguments:\
                    \n\t-help   This help\
                    \n\t-text   Texter:\
		            \n\t\tnd: do not drop the table\
		            \n\t\td: drop the table\
                    \n\t-filldb   Fill Map DB\
                    \n\t-mngdb Create a Data Base\
                    \n\t\t-start -d [Data Base Dir ej e:] -n traffic_simulator\
                    \n\t\t-stop -d [Data Base Dir ej e:] -n traffic_simulator\
                    \n\t\t-create Create a Data Base\
                    \n\t\t\t-n: Database name   (mandatory)\
                    \n\t\t\t-d: Data Base Directory   (mandatory)\
                    \n\t\t\t-t: Data Base Tables  (mandatory)\
					\n\n\t-filltr: To fill DB with traffic info\
					\n\t\t -f: Complete file path where the info is (mandatory)\
                    \n\t\t -n: Database name   (mandatory)"
		, argv[0]);


	cout << help.getCadena() << endl;

}


//------------------------------------------------------------------------------
void Control::error(const char* argv[])
{
	cout << "Error en argumentos:" << endl;
	cout << "For a list of ll posibilities execute : " << argv[0] << " -h" << endl;
	cout << endl;
	showhelp(argv);
}
