#include "Test.h"
#include "Traffic.h"

//------------------------------------------------------------------------------
Test::Test()
{
	
}

//------------------------------------------------------------------------------
Test::~Test()
{
	
}

//------------------------------------------------------------------------------
void Test::init(TestHandle* phandle, Cadena cadgral)
{
	Cadena cadfile;
	mpTH = phandle;
	mcadgral = cadgral;
	mSrc.init(mcadgral);
	

	cadfile.formCadena(SIM_DIR_ETC_CONFIG_"AP7_toll_distance.txt");
	cadfile = mcadgral + cadfile;
	get_AP7DB_distance(cadfile);
	m_delta_time_random = 1.0;
}

//------------------------------------------------------------------------------
void Test::reset()
{
	DWORD dwWaitResult;
	dwWaitResult = WaitForSingleObject(mpTH->source_mutex, INFINITE);
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		for (int i = 0; i < SIM_MAX_VEHICLES; i++)
		{
			mpTH->pVH[i].reset();
		}

		//Algorithm stuff
		mpTH->map_norder.clear();
		for (int i = 0; i < mpTH->norder_index; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				mpTH->pNO[i].m_nid = 0;
				mpTH->pNO[i].laneorder[j].m_vh_first = -1;
				mpTH->pNO[i].laneorder[j].m_cant = 0;
			}
		}
		mpTH->norder_index = 0;

		mSrc.reset();
		ReleaseMutex(mpTH->source_mutex);
	}
	else
	{
		cout << "Test::Unexpected behaviour of mutex" << endl;
	}

	m_delta_time_random = 1.0;
}

//------------------------------------------------------------------------------
void Test::get_simulation_info(Cadena cadfile)
{
	Source::SimulLine sl;
	mItems.clear();

	if (WaitForSingleObject(mpTH->source_mutex, INFINITE) == WAIT_OBJECT_0)
	{
		if (mItems.get_data_from_txt(cadfile))
		{
			cout << "--------------------------------------------------------------------------------------------" << endl;
			cout << "SIMULATION File:[" << cadfile.getCadena() << endl; 
			cout << "--------------------------------------------------------------------------------------------" << endl;

			if (mItems.mType == "SOURCETRIP")
			{
				mVehicleSource = mItems.mType;

				for (int i = 0; i < mItems.mMaxreg; i++)
				{
					sl.cad_source  = mItems.mRegtable[i].reg["SOURCE_FILE"].str;
					sl.cad_type    = mItems.mRegtable[i].reg["TYPE"].str;
					sl.cad_trip    = mItems.mRegtable[i].reg["TRIP_FILE"].str;
					sl.cad_est_in  = mItems.mRegtable[i].reg["EST_IN"].str;
					sl.cad_est_out = mItems.mRegtable[i].reg["EST_OUT"].str;
					sl.iteration   = mItems.mRegtable[i].reg["ITERATION"].num_i;

					if (mSrc.mIndexSources < SIM_MAX_SOURCES)
					{
						if (mSrc.get_source(&sl))
						{
							mSrc.mIndexSources++;
							cout << "Source Index:[" << mSrc.mIndexSources << "]  Source file: [" << sl.cad_trip.getCadena() << "]" << endl;
						}
						else
						{
							cout << "Error in vehicle information line" << endl;
						}
					}
					else
					{
						cout << "---------------------------------------------------------------" << endl;
						cout << "We have reached the limit of different sources:[" << mSrc.mIndexSources << "]" << endl;
						cout << "---------------------------------------------------------------" << endl;
					}

				}
			}
			cout << "--------------------------------------------------------------------------------------------" << endl;
		}
	}

	ReleaseMutex(mpTH->source_mutex);
}

//------------------------------------------------------------------------------
void Test::get_new_vehicle_info()
{
	std::time_t minimun_time_following, time_now;
	int delta_sleep = 0;
	bool moreveh = true;
	minimun_time_following = Simulator::get_time_now_ms() * 2;

	while (moreveh)
	{
		if  (WaitForSingleObject(mpTH->source_mutex, INFINITE) == WAIT_OBJECT_0)
		{
			//For every source / vehicle
			for (int i = 0; i < mSrc.mIndexSources; i++)
			{
				if (mSrc.mpSources[i].end == false)
				{
					if (mSrc.mpSources[i].source_type == "FILE")
					{
						setup_vh_from_file(i);
					}
					else if (mSrc.mpSources[i].source_type == "DB")
					{
						setup_vh_from_db(i);
					}
					else if (mSrc.mpSources[i].source_type == "RANDOM")
					{
						setup_vh_from_random(i);
					}

					//get the lower time for the following insertion of a vehicle
					if (minimun_time_following > mSrc.mpSources[i].time_following)
					{
						minimun_time_following = mSrc.mpSources[i].time_following;
					}
				}
			}

			//Sleep while wait for the following vehicle
			time_now = Simulator::get_time_now_ms();
			delta_sleep = (minimun_time_following - time_now) ;
			if (delta_sleep <=0 )   delta_sleep = 10;
			if (delta_sleep > 5000) delta_sleep = 5000;

			moreveh = true;
			for (int i = 0; i < mSrc.mIndexSources; i++)
			{
				moreveh = (moreveh && mSrc.mpSources[i].end);
			}
			moreveh = !moreveh;

			minimun_time_following = Simulator::get_time_now_ms() * 2;
			ReleaseMutex(mpTH->source_mutex);
			if (moreveh) Sleep(delta_sleep);

		} 
		else
		{
			ReleaseMutex(mpTH->source_mutex);
		}

		while (mpTH->state != Traffic::SIM_TR_STATE_PLAY)
		{
			Sleep(500);
		}
	}//while
}


//------------------------------------------------------------------------------
Cadena Test::get_random_category(Source::Line* psource, int& index_type_cat)
{
	int index_cat, prctg = 0;;
	Cadena cad_tabulacion;

	index_cat = rand() % 100;

	for (int i = 0; i < psource->cant_type_seg; i++)
	{
		prctg = prctg + psource->rnd_vh[i].category_prctg;
		if (index_cat < prctg)
		{
			index_type_cat = i;
			cad_tabulacion = psource->rnd_vh[i].category;
			break;
		}
	}
	
	return cad_tabulacion;
}

//------------------------------------------------------------------------------
int Test::get_random_delta_timefollowing(Source::Line* psource, int index)
{
	int delta_time_fll, index_time, delta_min = 0, prctg = 0;
	index_time = rand() % 100;

	delta_time_fll = 0;

	for (int i = 0; i < psource->rnd_vh[index].cant_time_seg; i++)
	{
		prctg = prctg + psource->rnd_vh[index].time_seg_prctg[i];

		if (index_time < prctg)
		{
			delta_time_fll = delta_min + rand() % (psource->rnd_vh[index].time_seg[i] - delta_min);
			break;
		}

		delta_min = psource->rnd_vh[index].time_seg[i];
	}

	return int (delta_time_fll * m_delta_time_random);

}
//------------------------------------------------------------------------------
int Test::get_random_speed(Source::Line* psource, int index)
{
	int speed, index_speed;
	int delta_min = 0, prctg = 0;
	
	index_speed = rand() % 100;
	delta_min = psource->rnd_vh[index].min_speed;

	speed = 0;

	for (int i = 0; i < psource->rnd_vh[index].cant_speed_seg; i++)
	{
		prctg = prctg + psource->rnd_vh[index].speed_seg_prctg[i];

		if (index_speed < prctg)
		{
			speed = delta_min + rand() % (psource->rnd_vh[index].speed_seg[i] - delta_min);
			break;
		}

		delta_min = psource->rnd_vh[index].speed_seg[i];
	}

	return speed;

}
//------------------------------------------------------------------------------
bool Test::setup_vh_from_random(int i)
{
	double speed_pw;
	double size;
	int delta_time_fll, nreg, lane_type, index_cat, ivh = 0;
	int speed;
	Color color;
	Cadena cad_category;

	if ((is_on_time(mSrc.mpSources[i].time_following)))
	{
		ivh = 0;

		cad_category = get_random_category(&mSrc.mpSources[i], index_cat);
		speed = get_random_speed(&mSrc.mpSources[i], index_cat);
		delta_time_fll = get_random_delta_timefollowing(&mSrc.mpSources[i], index_cat);

		mSrc.mpSources[i].time_following = mSrc.mpSources[i].time_following + delta_time_fll;

		mSrc.mpSources[i].pveh[ivh].sentido = ' ';
		mSrc.mpSources[i].pveh[ivh].cattabulacion = cad_category;
		mSrc.mpSources[i].pveh[ivh].speed         = speed;
		Simulator::get_category_stuff(cad_category, size, speed_pw, lane_type, color);
		mSrc.mpSources[i].pveh[ivh].speed_pw = speed_pw;
		mSrc.mpSources[i].pveh[ivh].color = color;
		mSrc.mpSources[i].pveh[ivh].size = size;
		mSrc.mpSources[i].pveh[ivh].cadwayfile = mSrc.mpSources[i].cad_trip;
		mSrc.mpSources[i].pveh[ivh].cad_type = mSrc.mpSources[i].cad_type;
		mSrc.mpSources[i].pveh[ivh].cad_est_in = mSrc.mpSources[i].cad_est_in;
		mSrc.mpSources[i].pveh[ivh].cad_est_out = mSrc.mpSources[i].cad_est_out;
		mSrc.mpSources[i].pveh[ivh].gps_inaccuracy = 0.001;
		mSrc.mpSources[i].pveh[ivh].gps_point_timer = 8.0;

		if (set_new_vehicle_info(&(mSrc.mpSources[i].pveh[ivh])) != true)
		{
			MessageBox(NULL, mSrc.mpSources[i].pveh[ivh].cadwayfile.getCadena(), "There was a problem with some vehicle info!", MB_OK);
		}

		mSrc.mpSources[i].index++;
		//new cycle??
		if (mSrc.mpSources[i].index == mSrc.mpSources[i].nmaxveh)
		{
			mSrc.mpSources[i].iteration--;
			mSrc.mpSources[i].end = true;
			if (mSrc.mpSources[i].iteration > 0)
			{
				mSrc.mpSources[i].end = false;
				mSrc.mpSources[i].index = 0;
			}
		}
	}
	return true;

}

//------------------------------------------------------------------------------
bool Test::setup_vh_from_file(int i)
{
	double speed_pw;
	double size;
	int    nreg, lane_type, ivh = 0;
	Color  color;

	if ((is_on_time(mSrc.mpSources[i].time_following)))
	{
		ivh = mSrc.mpSources[i].index;
		mSrc.mpSources[i].time_following = mSrc.mpSources[i].time_following + mSrc.mpSources[i].pveh[ivh].delta_time_fll;

		Simulator::get_category_stuff(mSrc.mpSources[i].pveh[ivh].cattabulacion, size, speed_pw, lane_type, color);
		mSrc.mpSources[i].pveh[ivh].sentido = ' ';
		mSrc.mpSources[i].pveh[ivh].speed_pw = speed_pw;
		mSrc.mpSources[i].pveh[ivh].color = color;
		mSrc.mpSources[i].pveh[ivh].size = size;
		mSrc.mpSources[i].pveh[ivh].cadwayfile = mSrc.mpSources[i].cad_trip;
		mSrc.mpSources[i].pveh[ivh].cad_type = mSrc.mpSources[i].cad_type;
		mSrc.mpSources[i].pveh[ivh].cad_est_in = mSrc.mpSources[i].cad_est_in;
		mSrc.mpSources[i].pveh[ivh].cad_est_out = mSrc.mpSources[i].cad_est_out;

		if (set_new_vehicle_info(&(mSrc.mpSources[i].pveh[ivh])) != true)
		{
			MessageBox(NULL, mSrc.mpSources[i].pveh[ivh].cadwayfile.getCadena(), "There was a problem with some vehicle info!", MB_OK);
		}

		mSrc.mpSources[i].index++;
		//new cycle??
		if (mSrc.mpSources[i].index == mSrc.mpSources[i].nmaxveh)
		{
			mSrc.mpSources[i].iteration--;
			mSrc.mpSources[i].end = true;
			if (mSrc.mpSources[i].iteration > 0)
			{
				mSrc.mpSources[i].end = false;
				mSrc.mpSources[i].index = 0;
			}
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool Test::setup_vh_from_db(int i)
{
	Cadena cadMEnt, cadMSal, cattabulacion;
	Cadena query;
	double speed_pw;
	double size;
	double delta_db_time = 0;
	int    nreg, lane_type, mntEnt;
	Color  color;

	int ivh = 0;
	//-------------------------------------------------------
	// Every minute we have to query the data base
	// and fill the structure with the vehicles retrieved
	//-------------------------------------------------------

	//DB_CYCLE is the 1 minute period for querying the database
	if (mSrc.mpSources[i].db_cycle == true)
	{

		Simulator::get_new_moment(&mSrc.mpSources[i].db_momento);
		get_query(query, mSrc.mpSources[i].db_momento, mSrc.mpSources[i].db_estent, mSrc.mpSources[i].db_estsal);

		mpTH->pDB->ejecutarConsulta(query);
		nreg = mpTH->pDB->getNumRegistros();

		mSrc.mpSources[i].iteration--;
		mSrc.mpSources[i].nmaxveh = nreg;
		mSrc.mpSources[i].db_cycle = false;

		//we spread the vehicles of the 1 minute's query along 1 minute
		if (nreg != 0) {delta_db_time = int(60000.0 / double(nreg));} else {delta_db_time = 60000.0;}

		for (int ivh = 0; ivh < nreg; ivh++)
		{
			mntEnt        = mpTH->pDB->getInt("momentoentrada", ivh);
			cadMEnt       = cadMEnt.formCadena("%d", mntEnt);
			cadMSal       = mpTH->pDB->getString("momento", ivh);
			cattabulacion = mpTH->pDB->getString("tabulacioncob", ivh);

			//Simulator::tranform_stations(estent, estsal);

			Simulator::get_category_stuff(cattabulacion, size, speed_pw, lane_type, color);
			mSrc.mpSources[i].pveh[ivh].cattabulacion = cattabulacion;
			mSrc.mpSources[i].pveh[ivh].sentido = ' ';
			mSrc.mpSources[i].pveh[ivh].speed_pw = speed_pw;
			mSrc.mpSources[i].pveh[ivh].color = color;
			mSrc.mpSources[i].pveh[ivh].size = size;
			mSrc.mpSources[i].pveh[ivh].cadwayfile = mSrc.mpSources[i].cad_trip;
			mSrc.mpSources[i].pveh[ivh].cad_type = mSrc.mpSources[i].cad_type;
			mSrc.mpSources[i].pveh[ivh].speed = Simulator::get_speed(m_map_distance[mSrc.mpSources[i].db_estsal], m_map_distance[mSrc.mpSources[i].db_estent], cadMSal, cadMEnt);
			mSrc.mpSources[i].pveh[ivh].delta_time_fll = delta_db_time;
			mSrc.mpSources[i].pveh[ivh].cad_est_in = mSrc.mpSources[i].cad_est_in;
			mSrc.mpSources[i].pveh[ivh].cad_est_out = mSrc.mpSources[i].cad_est_out;

			mSrc.mpSources[i].pveh[ivh].gps_point_timer = 10.0;
			mSrc.mpSources[i].pveh[ivh].gps_inaccuracy = 0.0;

		}
	}

	if (is_on_time(mSrc.mpSources[i].time_following))
	{
		ivh = mSrc.mpSources[i].index;
		mSrc.mpSources[i].time_following = mSrc.mpSources[i].time_following + mSrc.mpSources[i].pveh[ivh].delta_time_fll;
	
		if (mSrc.mpSources[i].nmaxveh > 0)
		{
			if (set_new_vehicle_info(&(mSrc.mpSources[i].pveh[mSrc.mpSources[i].index])) != true)
			{
				MessageBox(NULL, mSrc.mpSources[i].pveh[mSrc.mpSources[i].index].cadwayfile.getCadena(), "There was a problem setting the vehicle!", MB_OK);
			}
			mSrc.mpSources[i].index++;
		}

		//new cycle??
		if (mSrc.mpSources[i].index == mSrc.mpSources[i].nmaxveh)
		{
			mSrc.mpSources[i].end = true;
			mSrc.mpSources[i].db_cycle = true;
			mSrc.mpSources[i].index = 0;

			if (mSrc.mpSources[i].iteration > 0)
			{
				mSrc.mpSources[i].end = false;
			}
		}
	}

	return true;
}


//------------------------------------------------------------------------------
bool Test::is_on_time(double time_following)
{
	double time_now = Simulator::get_time_now_ms();
	if (time_now >= time_following)  { return true; }
	return false;
}

//====================================================================================================
//============================  SETUP VEHICLE INFORMATION ============================================
//====================================================================================================
//----------------------------------------------------------------------------------------------------
bool Test::set_new_vehicle_info(Simveh* ptrSimVH)
{
	Vehiculo  vhTemp;
	Cadena cadaux, cadentsal;
	double deltaPwLane;
	double deltax, deltay;
	NodoInfo* ptrnode;

	vhTemp.reset();

	//--- General Info-------------------------
	strcpy(vhTemp.mEstacionEntrada,ptrSimVH->cad_est_in.getCadena());
	strcpy(vhTemp.mEstacionSalida, ptrSimVH->cad_est_out.getCadena());
	strcpy(vhTemp.mCategory,       ptrSimVH->cattabulacion.getCadena());
	vhTemp.mSizeCatg               = ptrSimVH->size;
	vhTemp.mColor                  = ptrSimVH->color;

	cadentsal.formCadena("%s=>%s", vhTemp.mEstacionEntrada, vhTemp.mEstacionSalida);

	vhTemp.mTimeIni         = glfwGetTime();
	vhTemp.mTimeLast        = vhTemp.mTimeIni;

	vhTemp.mNd2nd = 0;
	vhTemp.mIndexGral = mpTH->index_gral;

	mpTH->index_veh = get_new_index(mpTH->index_veh);
	vhTemp.mIndex = mpTH->index_veh;
	strcpy(vhTemp.mCadFile,ptrSimVH->cadwayfile.getCadena());
	strcpy(vhTemp.mCadType,ptrSimVH->cad_type.getCadena());

	if (get_trip_index(ptrSimVH->cadwayfile, ptrSimVH->cad_type, vhTemp.mTripIndex, mpTH->trip_index) != true) { return false; }
	vhTemp.mMaxNodesTrip = mpTH->pTripI[vhTemp.mTripIndex].max_nodos;
	vhTemp.mPtrNodeInfoNow = &(mpTH->pTripI[vhTemp.mTripIndex].ntrip[0]);
	vhTemp.mOrdenNodeRev = vhTemp.mMaxNodesTrip - 2;

	//---Speed stuff-------------------------
	vhTemp.mSpeedHwTypeFactor = Simulator::get_speed_factor_from_hwtype(mpTH->pTripI[vhTemp.mTripIndex].ntrip[0].hwtype);
	vhTemp.mSpeedAvrg = ptrSimVH->speed * ptrSimVH->speed_pw;
	vhTemp.mSpeedNow = vhTemp.mSpeedAvrg * vhTemp.mSpeedHwTypeFactor;

	//---Lane stuff-------------------------
	vhTemp.mLaneDefault = Simulator::get_lane_default(vhTemp.mSpeedAvrg, vhTemp.mPtrNodeInfoNow->lanes, ptrSimVH->lane_type);
	vhTemp.mLaneNow     = vhTemp.mLaneDefault;
	vhTemp.mLaneNew     = vhTemp.mLaneDefault;
	vhTemp.mLaneType    = ptrSimVH->lane_type;

	vhTemp.mOverlapAvrg    = Simulator::get_overlap_avrg(vhTemp.mSpeedAvrg, vhTemp.mCategory);
	vhTemp.mLaneReturnAvrg = Simulator::get_lanereturn_avrg(vhTemp.mSpeedAvrg, vhTemp.mCategory);
	vhTemp.mLaneCounter     = 0;
	vhTemp.mCounterOverlap  = vhTemp.mOverlapAvrg;

	deltaPwLane = mpTH->delta_lane * Simulator::get_delta_lane(vhTemp.mLaneNow, vhTemp.mPtrNodeInfoNow->lanes);
	deltax = deltaPwLane * vhTemp.mPtrNodeInfoNow->delta_lane_x;
	deltay = deltaPwLane * vhTemp.mPtrNodeInfoNow->delta_lane_y;
	vhTemp.mPosicion_x = vhTemp.mPtrNodeInfoNow->X - deltax;
	vhTemp.mPosicion_y = vhTemp.mPtrNodeInfoNow->Y - deltay;
	vhTemp.mPosicion_lat = Simulator::get_delta_lat(vhTemp.mPtrNodeInfoNow->lat, vhTemp.mPosicion_y - vhTemp.mPtrNodeInfoNow->Y);
	vhTemp.mPosicion_lon = Simulator::get_delta_lon(vhTemp.mPtrNodeInfoNow->lon, vhTemp.mPtrNodeInfoNow->lat, vhTemp.mPosicion_lat, vhTemp.mPosicion_x - vhTemp.mPtrNodeInfoNow->X);

	//----Position reference -----
	vhTemp.mPosicion_z = 0.00013f;
	vhTemp.mPosicion_x_old = vhTemp.mPosicion_x;
	vhTemp.mPosicion_y_old = vhTemp.mPosicion_y;
	vhTemp.mQuad_size = vhTemp.get_quad_size(vhTemp.mCategory);

	//---- GPS - Simulation -------
	vhTemp.mGPSId         = ptrSimVH->gpsId;
	vhTemp.mGPSInaccuracy = ptrSimVH->gps_inaccuracy;
	vhTemp.mGPSPointTimer = ptrSimVH->gps_point_timer;
	cadaux.formCadena("%04d %s", mpTH->index_gral, "SIM");
	strcpy(vhTemp.mLPN, cadaux.getCadena());

	//---Output stuff-------------------------
	cadaux.formCadena(SIM_DIR_TEST_PX_EU_"trip_%s_%04d_%s.txt", cadentsal.getCadena(), vhTemp.mIndexGral, Simulator::get_timestamp_format_YYMMDDhhmmss().getCadena());
	cadaux = mpTH->dir_init + cadaux;
	strcpy(vhTemp.mTripName_frmt_eurotoll, cadaux.getCadena());

	if (mpTH->pPxy->mOutputFile == true) { mpTH->pPxy->initialize_file(&vhTemp); }

	//Alg overpassing stuff
	ptrnode = vhTemp.mPtrNodeInfoNow;
	for (int i = 0; i < vhTemp.mMaxNodesTrip; i++)	{set_nodeorder(ptrnode);ptrnode++;}
	setget_following_start(&vhTemp, vhTemp.mPtrNodeInfoNow->node_vh_order);
	vhTemp.mpNO = mpTH->pNO;

	//--- Ready!!!!! ------------
	mpTH->pVH[mpTH->index_veh] = vhTemp;
	mpTH->pVH[mpTH->index_veh].mState = Traffic::SIM_TR_ALG_NORMAL;
	mpTH->pVH[mpTH->index_veh].mAlive = true;
	mpTH->vh_alive_real_time++;

	log_veh_in(&vhTemp);

	mpTH->index_veh++;
	mpTH->index_gral++;

	if (mpTH->index_veh >= SIM_MAX_VEHICLES) { mpTH->index_veh = 0; }

	return true;

}
//------------------------------------------------------------------------------
void Test::log_veh_in(Vehiculo* pvh)
{
	Cadena cadEntSal;
	cadEntSal.formCadena("%s=>%s", pvh->mEstacionEntrada, pvh->mEstacionSalida);

	cout << "IN  ==> ["       << pvh->mIndex;
	cout << "] Gral Index: [" << pvh->mIndexGral;
	cout << "] Speed:["       << pvh->mSpeedAvrg;
	cout << "] Ctg:["         << pvh->mCategory;
	cout << "] Trip:["        << cadEntSal.getCadena();
	cout << "]" << endl;
}

//------------------------------------------------------------------------------
void Test::log_veh_out(Vehiculo* pvh)
{
	Cadena cadEntSal;
	cadEntSal.formCadena("%s=>%s", pvh->mEstacionEntrada, pvh->mEstacionSalida);
	
	cout << "OUT <== ["  << pvh->mIndexGral;
	cout << "]  Speed:[" << pvh->mSpeedNow;
	cout << "]  Rest: [" << mpTH->vh_alive_real_time;
	cout << "]  Trip: [" << cadEntSal.getCadena();
	cout << "]" << endl;
}

//------------------------------------------------------------------------------
bool Test::set_new_vehicle_info_from_bin(Vehiculo* pvh)
{
	Cadena cadfile,cadtype, cad_nod1_nod2;
	NodoInfo* ptrnode_ini;

	pvh->mAlive = false;
	cadfile.formCadena("%s", pvh->mCadFile);
	cadtype.formCadena("%s", pvh->mCadType);

	mpTH->pVH[pvh->mIndex].reset();
	mpTH->pVH[pvh->mIndex] = (*pvh);
	if (get_trip_index(cadfile, cadtype, mpTH->pVH[pvh->mIndex].mTripIndex, mpTH->trip_index) != true) { return false; }
	mpTH->pVH[pvh->mIndex].mPtrNodeInfoNow = &(mpTH->pTripI[mpTH->pVH[pvh->mIndex].mTripIndex].ntrip[pvh->mNodeIndex]);
	if (mpTH->pPxy->mOutputFile == true) { mpTH->pPxy->initialize_file(pvh); }

	//Alg overpassing stuff
	ptrnode_ini = &(mpTH->pTripI[mpTH->pVH[pvh->mIndex].mTripIndex].ntrip[0]);
	cad_nod1_nod2.formCadena("%lld%lld", ptrnode_ini->nid, (ptrnode_ini+1)->nid);

	for (int i = 0; i < mpTH->pVH[pvh->mIndex].mMaxNodesTrip; i++) { ptrnode_ini->node_vh_order = mpTH->map_norder[cad_nod1_nod2]; ptrnode_ini++;}
	mpTH->pVH[pvh->mIndex].mpNO = mpTH->pNO;

	mpTH->pVH[pvh->mIndex].mAlive = true;
	mpTH->vh_alive_real_time++;
	mpTH->index_veh++;
	mpTH->index_gral++;
	if (mpTH->index_veh >= SIM_MAX_VEHICLES) { mpTH->index_veh = 0; }

	log_veh_in(pvh);

	return true;
}

//------------------------------------------------------------------------------
void Test::set_nodeorder(NodoInfo* ptr)
{
	map<Cadena, int>::iterator it;
	Cadena cadnod_1_nod_2;
	cadnod_1_nod_2.formCadena("%lld%lld", ptr->nid, (ptr+1)->nid);

	it = mpTH->map_norder.find(cadnod_1_nod_2);
	//I'm the first in this node/lane
	if (it == mpTH->map_norder.end())
	{
		//I store all the nodes to find them easily later
		mpTH->map_norder[cadnod_1_nod_2] = mpTH->norder_index;
		
		for (int j = 0; j < 10; j++)
		{
			mpTH->pNO[mpTH->norder_index].m_nid = ptr->nid;
			mpTH->pNO[mpTH->norder_index].laneorder[j].m_vh_first = -1;
			mpTH->pNO[mpTH->norder_index].laneorder[j].m_cant = 0;
		}

		mpTH->norder_index++;
		if (mpTH->norder_index > SIM_MAX_NODE_ORDER) cout << "Error: We reache the maximun number of nodes ofr ordering purposes!!" << endl;
	}

	ptr->node_vh_order = mpTH->map_norder[cadnod_1_nod_2];
}

//------------------------------------------------------------------------------
void Test::setget_following_start(Vehiculo* pVH, int n_vh_order)
{
	//I'm getting the following even if it is -1
	pVH->mVhFollowing = mpTH->pNO[n_vh_order].laneorder[pVH->mLaneNow].m_vh_first;
	//Nobody behind me
	pVH->mVhBehind = -1;
	//Change the behind index of the following vehicle
	if (pVH->mVhFollowing != -1) mpTH->pVH[pVH->mVhFollowing].mVhBehind = pVH->mIndex;

	//I'm the new one in this node/lane
	mpTH->pNO[n_vh_order].laneorder[pVH->mLaneNow].m_vh_first = pVH->mIndex;
	mpTH->pNO[n_vh_order].laneorder[pVH->mLaneNow].m_cant++;

}

//------------------------------------------------------------------------------
void Test::setget_following(Vehiculo* pVH, int n_vh_order, int n_vh_order_old)
{

	//I'm getting the following even if it is -1
	pVH->mVhFollowing = mpTH->pNO[n_vh_order].laneorder[pVH->mLaneNow].m_vh_first;

	//I'm the new one in this node/lane
	mpTH->pNO[n_vh_order].laneorder[pVH->mLaneNow].m_vh_first = pVH->mIndex;
	mpTH->pNO[n_vh_order].laneorder[pVH->mLaneNow].m_cant++;

	if (pVH->mVhFollowing != -1) mpTH->pVH[pVH->mVhFollowing].mVhBehind = pVH->mIndex;
	//Reset any link behind if exist
	if (pVH->mVhBehind != -1)    mpTH->pVH[pVH->mVhBehind].mVhFollowing = -1;

	//I'm changing the node
	pVH->mVhBehind = -1;


	//I leave the old node
	mpTH->pNO[n_vh_order_old].laneorder[pVH->mLaneNow].m_cant--;
	if (mpTH->pNO[n_vh_order_old].laneorder[pVH->mLaneNow].m_cant == 0)
	{
		mpTH->pNO[n_vh_order_old].laneorder[pVH->mLaneNow].m_vh_first = -1;
	}
}

//------------------------------------------------------------------------------
void Test::setget_following_lane(Vehiculo* pVH, bool new_node, int vh_inf, int vh_sup)
{

	//----------------------------------------------------
	//     NEW NODE - NEW LANE
	// ---------------------------------------------------
	//I'm the new one in this node & lane
	if (new_node)
	{
		//-------------------------OLD STUFF----------------------------
		//I leave the old lane - old node
		mpTH->pNO[(pVH->mPtrNodeInfoNow - 1)->node_vh_order].laneorder[pVH->mLaneNow].m_cant--;
		if (mpTH->pNO[(pVH->mPtrNodeInfoNow - 1)->node_vh_order].laneorder[pVH->mLaneNow].m_cant == 0)
		{
			mpTH->pNO[(pVH->mPtrNodeInfoNow - 1)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first = -1;
		}
		//I was the first
		else if (mpTH->pNO[(pVH->mPtrNodeInfoNow - 1)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first == pVH->mIndex)
		{
			mpTH->pNO[(pVH->mPtrNodeInfoNow - 1)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first = pVH->mVhFollowing;
		}

		//reattach same lane links
		if (pVH->mVhBehind != -1)	{mpTH->pVH[pVH->mVhBehind].mVhFollowing = pVH->mVhFollowing;}
		if (pVH->mVhFollowing != -1){mpTH->pVH[pVH->mVhFollowing].mVhBehind = pVH->mVhBehind;}

		//-------------------------NEW STUFF----------------------------

		//I get into the new node
		pVH->mVhFollowing = mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first;
		//I'm changing the node
		pVH->mVhBehind = -1;
		mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first = pVH->mIndex;
		mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant++;

		//reattach new lane - new node links
		if (pVH->mVhFollowing != -1) mpTH->pVH[pVH->mVhFollowing].mVhBehind = pVH->mIndex;
		//if (pVH->mVhBehind != -1)  There is no behind entreing to a new node
		
	}
	//----------------------------------------------------
	//     SAME NODE  -- NEW LANE
	// ---------------------------------------------------
	else
	{
		//-------------------------OLD STUFF----------------------------
		
		//I leave the old lane - same node
		mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_cant--;
		if (mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_cant == 0)
		{
			mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first = -1;
		}
		//I was the first
		else if (mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first == pVH->mIndex)
		{
			mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first = pVH->mVhFollowing;
		}

		//reattach same lane links
		if (pVH->mVhBehind != -1) 		{mpTH->pVH[pVH->mVhBehind].mVhFollowing = pVH->mVhFollowing;}
		if (pVH->mVhFollowing != -1)	{mpTH->pVH[pVH->mVhFollowing].mVhBehind = pVH->mVhBehind;}

		//-------------------------NEW STUFF----------------------------
		pVH->mVhFollowing = vh_sup;
		pVH->mVhBehind    = vh_inf;


		//I get into the new lane - same node -- I'm the first??
		if (mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant == 0)
		{
			mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first = pVH->mIndex;
		}
		//I'm not the first, but behind me there is no other vehicle
		else
		{
			if (pVH->mVhBehind == -1) mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first = pVH->mIndex;
		}
		mpTH->pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant++;


		if (pVH->mVhBehind != -1) 	 {mpTH->pVH[pVH->mVhBehind].mVhFollowing = pVH->mIndex;}
		if (pVH->mVhFollowing != -1) {mpTH->pVH[pVH->mVhFollowing].mVhBehind = pVH->mIndex;}
	}
}

//------------------------------------------------------------------------------
void Test::reset_veh()
{
	for (int i = 0; i < SIM_MAX_VEHICLES; i++)
	{
		mpTH->pVH[i].mState = Traffic::SIM_TR_ALG_NOT_ALIVE;
		mpTH->pVH[i].mPosicion_y = 10000;
		mpTH->pVH[i].mPosicion_x = 10000;
	}
}

//------------------------------------------------------------------------------
int Test::get_new_index(int index)
{
	//--Check if already used and skip
	while (mpTH->pVH[index].mState != Traffic::SIM_TR_ALG_NOT_ALIVE)
	{
		index++;
		if (index >= SIM_MAX_VEHICLES) { index = 0; }
		Sleep(5);
	}

	return index;
}

//------------------------------------------------------------------------------
bool Test::get_trip_index(Cadena cadtrip_name, Cadena cad_type, int& index_trip_veh, int& index_trip_master)
{
	if (index_trip_master < SIM_MAX_TRIPS)
	{
		if (mpTH->map_trip_name[cadtrip_name].used != 'Y')
		{
			if (!mpTH->pTrip->get_trip(cadtrip_name, cad_type, &mpTH->pTripI[index_trip_master]))
			{
				cout << "Error: This way (file) is not known at the moment!: [" << cadtrip_name.getCadena() << "]" << endl;
				return false;
			}
			else
			{
				index_trip_veh = index_trip_master;
				mpTH->map_trip_name[cadtrip_name].used = 'Y';
				mpTH->map_trip_name[cadtrip_name].index = index_trip_master;
				index_trip_master++;

				cout << "New Trip processed:[" << cadtrip_name.getCadena() << "]" << endl;
			}
		}
		else
		{
			index_trip_veh = mpTH->map_trip_name[cadtrip_name].index;
		}
	}
	else
	{
		cout << "Error: The ammount of trips has reached the maximun!: [" << SIM_MAX_TRIPS << "]" << endl;
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
void Test::get_query(Cadena& cad, int mnt, int estent, int estsal)
{
	cad.formCadena("SELECT momento, momentoentrada, numeroestacion, estacionentrada, tabulacioncob FROM Transitos WHERE momentoentrada = %d AND estacionentrada = %d AND numeroestacion = %d ", mnt, estent, estsal);
}

//------------------------------------------------------------------------------
void Test::get_AP7DB_distance(Cadena cadfile)
{
	mpData.clear();

	if (!mpData.get_data_from_txt(cadfile))
	{
		cout << "Error: There is an error obtaining information from file:[" << cadfile.getCadena() << "]" << endl;
		return;
	}

	for (int i = 0; i < mpData.mMaxreg; i++)
	{
		m_map_distance[mpData.mRegtable[i].reg["STATION"].num_i] = mpData.mRegtable[i].reg["DISTANCE"].num_i;
	}
}

//------------------------------------------------------------------------------
void Test::save_test(Cadena cadfilename)
{
	uint total_size = 0;

	mSaveTest = fopen(cadfilename.getCadena(), "wb+");
	if (mSaveTest == NULL) { std::cout << "Error Fichero: " << cadfilename.getCadena() << endl; return; }

	fseek(mSaveTest, total_size, SEEK_SET);
	fwrite(&mpTH->norder_index, sizeof(int), 1, mSaveTest);
	total_size = sizeof(int) + total_size;
	
	for (int i = 0; i < mpTH->norder_index; i++)
	{
		fseek(mSaveTest, total_size, SEEK_SET);
		fwrite(&mpTH->pNO[i], sizeof(NodeOrder), 1, mSaveTest);
		total_size = sizeof(NodeOrder) + total_size;
	}

	for (int i = 0; i < SIM_MAX_VEHICLES; i++)
	{
		if (mpTH->pVH[i].mAlive)
		{
			fseek(mSaveTest, total_size, SEEK_SET);
			fwrite(&mpTH->pVH[i], sizeof(Vehiculo), 1, mSaveTest);
			total_size = sizeof(Vehiculo) + total_size;
		}
	}
	
	fflush(mSaveTest);
	fclose(mSaveTest);

}

//------------------------------------------------------------------------------
void Test::set_test(Cadena cadfile)
{

	Vehiculo vh;
	int total_size = 0;
	Cadena cadnod_1_nod_2;
	
	mSaveTest = fopen(cadfile.getCadena(), "rb");
	if (mSaveTest == NULL) 	{std::cout << "Error Fichero: " << cadfile.getCadena() <<  endl;return;	}
	else
	{
		fread(&mpTH->norder_index, sizeof(int), 1, mSaveTest);
		total_size = total_size + sizeof(int);
		fseek(mSaveTest, total_size, SEEK_SET);

		for (int i = 0; i < mpTH->norder_index; i++)
		{
			fread(&mpTH->pNO[i], sizeof(NodeOrder), 1, mSaveTest);
			total_size = total_size + sizeof(NodeOrder);
			fseek(mSaveTest, total_size, SEEK_SET);
			cadnod_1_nod_2.formCadena("%lld%lld", mpTH->pNO[i].m_nid, mpTH->pNO[i + 1].m_nid);

			mpTH->map_norder[cadnod_1_nod_2] = i;
		}

		while (fread(&vh, sizeof(Vehiculo), 1, mSaveTest))
		{
			total_size = total_size + sizeof(Vehiculo);
			fseek(mSaveTest, total_size, SEEK_SET);
			set_new_vehicle_info_from_bin(&vh);
		}	
	}
	fclose(mSaveTest);
	
}