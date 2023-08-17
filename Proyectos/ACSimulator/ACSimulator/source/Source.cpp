#include "Source.h"

//------------------------------------------------------------------------------
Source::~Source()
{

}

//------------------------------------------------------------------------------
Source::Source()
{

}

//------------------------------------------------------------------------------
void Source::init(Cadena cadgral)
{

	mpSources = new Line[SIM_MAX_SOURCES];
	for (int i = 0; i < SIM_MAX_SOURCES; i++)
	{
		mpSources[i].pveh = new Simveh[SIM_MAX_VEH_X_SOURCE];
		mpSources[i].end = true;
		mpSources[i].index = 0;
		mpSources[i].time_following = 0;
		mpSources[i].nmaxveh = 0;
		mpSources[i].db_cycle = false;
	}
	mIndexSources = 0;

	mcadgral = cadgral;

}

//------------------------------------------------------------------------------
void Source::reset()
{
	mIndexSources = 0;
	for (int i = 0; i < SIM_MAX_SOURCES; i++)
	{
		mpSources[i].end = true;
		mpSources[i].index = 0;
		mpSources[i].time_following = 0;
		mpSources[i].nmaxveh = 0;
		mpSources[i].db_cycle = false;
	}
}

//------------------------------------------------------------------------------
bool Source::get_source( SimulLine* psimulline)
{
	Cadena cadfile, cad_time, cad_ptime;
	Simveh sv;

	mItems.clear();
	int count = 0;

	//Obtaining information for every vehicle
	cadfile.formCadena(SIM_DIR_TEST_SRC_"%s", psimulline->cad_source.getCadena());
	cadfile = mcadgral + cadfile;

	mpSources[mIndexSources].cant_type_seg = 0;

	if (!mItems.get_data_from_txt(cadfile))
	{
		cout << "Error: There is an error obtaining information from file:[" << cadfile.getCadena() << "]" << endl;
		return false;
	}

	if (mItems.mType == "VEHICLEINFO")
	{
		for (int i = 0; i < mItems.mMaxreg; i++)
		{

			mpSources[mIndexSources].cad_trip    = psimulline->cad_trip;
			mpSources[mIndexSources].cad_type    = psimulline->cad_type;
			mpSources[mIndexSources].cad_source  = psimulline->cad_source;
			mpSources[mIndexSources].iteration   = psimulline->iteration;
			mpSources[mIndexSources].cad_est_in  = psimulline->cad_est_in;
			mpSources[mIndexSources].cad_est_out = psimulline->cad_est_out;
			mpSources[mIndexSources].index       = 0;
			mpSources[mIndexSources].db_cycle    = true;
			mpSources[mIndexSources].end         = false;
			mpSources[mIndexSources].time_following = Simulator::get_time_now_ms();//from 1970

			if (mItems.mRegtable[i].reg["TYPE"].str == "INFO")
			{
				//INFO#120#T2#830202000200000#3000#2#10
				sv.speed           = double(mItems.mRegtable[i].reg["SPEED"].num_i);
				sv.cattabulacion   = mItems.mRegtable[i].reg["CATEGORY"].str;
				sv.gpsId           = mItems.mRegtable[i].reg["GPS_DEVICE_ID"].num_li;
				sv.delta_time_fll  = mItems.mRegtable[i].reg["TIME_FOLLOWING"].num_i;
				sv.gps_inaccuracy  = mItems.mRegtable[i].reg["GPS_INNACURACY"].num_i;
				sv.gps_point_timer = mItems.mRegtable[i].reg["GPS_POINT_TIMER"].num_i;

				mpSources[mIndexSources].pveh[count] = sv;
				mpSources[mIndexSources].pveh[count].cattabulacion = sv.cattabulacion;
				mpSources[mIndexSources].source_type = "FILE";
				count++;
				mpSources[mIndexSources].nmaxveh = count;

			}
			else if (mItems.mRegtable[i].reg["TYPE"].str == "DB")
			{
				//DB#102#103#20170707100000

				mpSources[mIndexSources].db_estent   = mItems.mRegtable[i].reg["EST_IN"].num_i;
				mpSources[mIndexSources].db_estsal   = mItems.mRegtable[i].reg["EST_OUT"].num_i;
				mpSources[mIndexSources].db_momento  = mItems.mRegtable[i].reg["DB_MOMENT"].num_i;
				mpSources[mIndexSources].source_type = "DB";
				mpSources[mIndexSources].nmaxveh     = 0;
				count++;

				break;
			}
			else if (mItems.mRegtable[i].reg["TYPE"].str == "RANDOM")
			{
				//FORMAT;INT;PCAT;STR;CAT;INT;SPEED;INT;DSPEED;INT;CNT_SEG_TIME;
				//INT;CNT_SEG_SPEED;INT;MIN_SPEED;
				//INT;SPEED_1;INT;PSPEED_1;INT;SPEED_2;INT;PSPEED_2;INT;SPEED_3;INT;PSPEED_3;INT;SPEED_4;INT;PSPEED_4;INT;SPEED_5;INT;PSPEED_5;
		
				//INT;TIME_1;INT;PTIME_1;INT;TIME_2;INT;PTIME_2;INT;TIME_3;INT;PTIME_3;INT;TIME_4;INT;PTIME_4;INT;TIME_5;INT;PTIME_5;
				//INT;TIME_6;INT;PTIME_6;INT;TIME_7;INT;PTIME_7;INT;TIME_8;INT;PTIME_8;INT;TIME_9;INT;PTIME_9;INT;TIME_10;INT;PTIME_10;
				//RANDOM; 85; T2; 105; 20; 5; 1000; 15; 2000; 15; 3000; 15; 6000; 40; 10000; 15;;;;;;;;;;;
				//RANDOM; 15; C2; 80; 20; 5; 1000; 15; 2000; 15; 3000; 15; 6000; 40; 10000; 15;;;;;;;;;;;


				// |
				// |   Probabilistic Curve Segmentation
				// |
				// |     ----------------------------
				// |
				// |                        
				// |-----                            --------
				// |                                         -----
				// |                                                ------
				// |----------------------------------------------------------------------------------------------->
				// 0%                                                     100%

				mpSources[mIndexSources].source_type      = "RANDOM";
				mpSources[mIndexSources].cant_type_seg++;
				mpSources[mIndexSources].nmaxveh                  = mItems.mRegtable[i].reg["MAX_VEH"].num_i;
				mpSources[mIndexSources].rnd_vh[i].category_prctg = mItems.mRegtable[i].reg["PCAT"].num_i;
				mpSources[mIndexSources].rnd_vh[i].category       = mItems.mRegtable[i].reg["CAT"].str;
				mpSources[mIndexSources].rnd_vh[i].cant_time_seg  = mItems.mRegtable[i].reg["CNT_SEG_TIME"].num_i;
				mpSources[mIndexSources].rnd_vh[i].cant_speed_seg = mItems.mRegtable[i].reg["CNT_SEG_SPEED"].num_i;
				mpSources[mIndexSources].rnd_vh[i].min_speed      = mItems.mRegtable[i].reg["MIN_SPEED"].num_i;


				for (int j = 0; j < mpSources[mIndexSources].rnd_vh[i].cant_time_seg; j++)
				{
					cad_time.formCadena("TIME_%d", j+1);
					cad_ptime.formCadena("PTIME_%d", j+1);
					mpSources[mIndexSources].rnd_vh[i].time_seg[j]        = mItems.mRegtable[i].reg[cad_time.getCadena()].num_i;
					mpSources[mIndexSources].rnd_vh[i].time_seg_prctg[j]  = mItems.mRegtable[i].reg[cad_ptime.getCadena()].num_i;

				}


				for (int j = 0; j < mpSources[mIndexSources].rnd_vh[i].cant_speed_seg; j++)
				{
					cad_time.formCadena("SPEED_%d", j + 1);
					cad_ptime.formCadena("PSPEED_%d", j + 1);
					mpSources[mIndexSources].rnd_vh[i].speed_seg[j] = mItems.mRegtable[i].reg[cad_time.getCadena()].num_i;
					mpSources[mIndexSources].rnd_vh[i].speed_seg_prctg[j] = mItems.mRegtable[i].reg[cad_ptime.getCadena()].num_i;

				}


				count++;

			}
		}
	}

	if (count > 0) return true;
	return false;

}
