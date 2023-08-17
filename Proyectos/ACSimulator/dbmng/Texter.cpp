#include "Texter.h"


//------------------------------------------------------------------------------
Texter::Texter()
{
	mpctrl = NULL;
}

//------------------------------------------------------------------------------
Texter::~Texter()
{
	mdb_r2.desconectar();
	mdb_r.desconectar();
	mdb.desconectar();
}

//------------------------------------------------------------------------------
void Texter::init(Control* mpctrl)
{
	Cadena dbname = "simulator_db";

	if (mdb.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) { return; }
	if (mdb_r.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) { return; }
	if (mdb_r2.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) { return; }

	mfileinfo.clear();
}

//------------------------------------------------------------------------------
void Texter::execute()
{
	Cadena cadtext_config, cad_table, cad_key, cad_value, sql_qry, key_tag, value;
	long long int nid;
	int regs, regs_r, timestmp, ver;
	double lon, lat, xlon, ylat;

	mfileinfo.clear();
	cadtext_config = SIM_DIR_ETC_CONFIG_"texter.txt";


	if (mfileinfo.get_data_from_txt(cadtext_config))
	{

		if (mfileinfo.mType == "MAP_TEXT")
		{

			for (int i = 0; i < mfileinfo.mMaxreg; i++)
			{
				cout << "--------------------------------------------------------" << endl;
				cad_key = mfileinfo.mRegtable[i].reg["file"].str;
				cad_value = mfileinfo.mRegtable[i].reg["name"].str;
				cad_table.formCadena("Text_nodes_%s_%s", cad_key.getCadena(), cad_value.getCadena());
				cout << cad_table.getCadena() << endl;

				//Delete previous tbale
				if (mpctrl->mtext_droptable)
				{
					sql_qry.formCadena("DROP TABLE %s", cad_table.getCadena());
					mdb.ejecutarConsulta(sql_qry);
				}


				//Create table
				sql_qry.formCadena("CREATE TABLE %s                 \
					                 (                              \
						              nid        BIGINT DEFAULT 0,  \
					                  ver        INTEGER DEFAULT 0, \
						              timestmp   INTEGER DEFAULT 0, \
						              name       VARCHAR(200),      \
						              xlon       DOUBLE PRECISION,  \
						              ylat       DOUBLE PRECISION,  \
										                            \
										                            \
						              PRIMARY KEY(nid, ver)         \
										                            \
					                 );                             \
					                                                \
									CREATE INDEX SIM_%s_indx ON %s(xlon, ylat);",
					cad_table.getCadena(),
					cad_table.getCadena(),
					cad_table.getCadena()
				);

				mdb.ejecutarConsulta(sql_qry);


				//-----------------------------------------------------------------------------------------
				// Selecting nodes from NODE_TAGS which tags are 'place' and theirs values are 'city'
				//-----------------------------------------------------------------------------------------
					//nid           BIGINT DEFAULT 0,
					//ver           INTEGER DEFAULT 0,
					//timestmp      INTEGER DEFAULT 0,
					//key_tag       VARCHAR(200),
					//value         VARCHAR(200),

				sql_qry.formCadena("SELECT nid, max(ver) as ver, timestmp FROM Node_tags WHERE key_tag = '%s' AND value = '%s' GROUP BY nid,timestmp;", cad_key.getCadena(), cad_value.getCadena());
				mdb.ejecutarConsulta(sql_qry);
				regs = mdb.getNumRegistros();
				cout << "Nr regs:[" << regs << "]" << endl;

				if (regs > 0)
				{
					for (int i = 0; i < regs; i++)
					{
						nid = long long(mdb.getDouble("nid", i));
						ver = int(mdb.getInt("ver", i));
						timestmp = int(mdb.getInt("timestmp", i));

						//-------------------------------------------------------------------------------------------------------------------------
						// Selecting tags of kind name from NODE_TAGS which nid have tags of kind are 'place' and theirs values are 'city'
						//-------------------------------------------------------------------------------------------------------------------------
						sql_qry.formCadena("SELECT value FROM Node_tags WHERE nid = %lld AND ver = %d and key_tag = 'name'", nid, ver);
						mdb_r.ejecutarConsulta(sql_qry);
						regs_r = mdb_r.getNumRegistros();

						if (regs_r > 0)
						{
							for (int j = 0; j < regs_r; j++)
							{
								value = mdb_r.getString("value", j);
								cout << "value: [" << value.getCadena() << "] " << endl;
							}
						}


						sql_qry.formCadena("SELECT nid, ver, timestmp, lon, lat FROM Node_pos WHERE nid = %lld AND ver = %d", nid, ver);
						mdb_r.ejecutarConsulta(sql_qry);
						regs_r = mdb_r.getNumRegistros();

						if (regs_r > 0)
						{
							for (int j = 0; j < regs_r; j++)
							{

								nid = long long(mdb_r.getDouble("nid", j));
								ver = int(mdb_r.getInt("ver", j));
								timestmp = int(mdb_r.getInt("timestmp", j));
								lon = (double)mdb_r.getDouble("lon", j);
								lat = (double)mdb_r.getDouble("lat", j);
								xlon = Simulator::getXfromLon(lon);
								ylat = Simulator::getYfromLat(lat);


								//-----------------------------------------------------
								// INSERT into TABLE
								//-----------------------------------------------------
								sql_qry.formCadena("INSERT INTO %s (nid,ver,timestmp, name,xlon,ylat) VALUES (%lld, %d, %d,'%s', %lf, %lf)", cad_table.getCadena(), nid, ver, timestmp, value.getCadena(), xlon, ylat);
								mdb_r2.ejecutarConsulta(sql_qry);


							}
						}
					}
				}
			}
		}
	}
}
