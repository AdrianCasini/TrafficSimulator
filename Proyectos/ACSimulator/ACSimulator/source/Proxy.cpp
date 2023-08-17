#include "Proxy.h"

//------------------------------------------------------------------------------
Proxy::Proxy()
{

}

//------------------------------------------------------------------------------
Proxy::~Proxy()
{
	if (mMasterTrip.is_open())
	{
		mMasterTrip.flush();
		mMasterTrip.close();
	}

	mMasterPerf.close();
}

//------------------------------------------------------------------------------
void Proxy::init(Cadena cadgral)
{
	//mpFILEfrmt_hubautop = new(std::nothrow) ofstream[SIM_MAX_VEHICLES];if (mpFILEfrmt_hubautop == NULL) { cout << "Can`t get enough memory at the moment" << endl;  exit(1); }
	mpFILEfrmt_eurotoll = new(std::nothrow) ofstream[SIM_MAX_VEHICLES];if (mpFILEfrmt_eurotoll == NULL) { cout << "Can`t get enough memory at the moment" << endl;  exit(1); }

	mOutputFile     = false;
	mOutputCloud    = false;
	mbSendposition  = false;
	mGPSPointsCount = 0;
	mGPSPointsSent  = 0;
	mTime_init      = glfwGetTime();

	mcadgral = cadgral;

	mSendSimInfo = false;

	mSendInfo.sim_detector = 0;
	mSendInfo.sim_gps = 0;
	mSendInfo.sattol_gps_eurotoll = 0;
	mSendInfo.sattol_gps_eurotoll_perf = 0;
	mSendInfo.master_trip = 0;
	mSendInfo.active = false;

	mRecordSimID = 1;
	mSimRegXFileIndex = 0;
	mSimFileOpened = false;
	Cadena cadremove;
	cadremove = mcadgral + SIM_DIR_TEST_PX_SIM_"ACTIVE_SENDING";
	std::remove(cadremove.getCadena());

	Cadena cadtrip_name;
	Cadena cadtime = Simulator::get_timestamp_format_YYMMDDhhmmss();
	cadtrip_name.formCadena(SIM_DIR_TEST_PX_EU_PERF_"trip_%s.txt", cadtime.getCadena());
	cadtrip_name = mcadgral + cadtrip_name;

	mMasterPerf.open(cadtrip_name.getCadena(), std::ios_base::app);
	if (!mMasterPerf.is_open()) { perror("SIM_DIR_TRIPS_FE_P_:error while opening file"); }
}

//------------------------------------------------------------------------------
void Proxy::send_info(Vehiculo* pVH)
{
	Cadena cadbuf_master, cad_csv, cadbuf_eurotoll, cadbuf_simulator;
	double time_diff, time_now, time_delta;
	bool sim_gps_time = false;
	bool sim_detector_time = false;
	bool sim_close_file_time = false;

	if (mSendSimInfo) 
	{

		time_now = glfwGetTime();
		time_diff = time_now - pVH->mTimeLast;
		time_delta = 1.5 * (rand() % 100 / 100.0);

		sim_gps_time = ((time_diff > pVH->mGPSPointTimer + time_delta) && mSendInfo.sim_gps);
		sim_detector_time = (pVH->mSendDetectorInfo && mSendInfo.sim_detector);
		sim_close_file_time = (((time_now - mTimeLast) > SIM_PROXY_SIM_RECORDS_TIME_ELAPSED) && (mSimRegXFileIndex > 0));

		if (sim_gps_time || sim_detector_time)
		{
			format_simulator_file(pVH, cadbuf_simulator, sim_gps_time, sim_detector_time);
			write_simulator_info(pVH, cadbuf_simulator);
			pVH->mTimeLast = glfwGetTime();
			mSimRegXFileIndex++;
		}

		//Close the current Simulator Format file and open a new one:
		// -- many records
		// -- too much time elapsed
		if ((mSimRegXFileIndex >= SIM_PROXY_SIM_RECORDS_X_FILE) || (sim_close_file_time))
		{
			mSimRegXFileIndex = 0;
			mTimeLast = time_now;
			sim_file_close();
			sim_file_open();
		}
	}

	//Store All trips GPS information in one single file
	//This MASTER file could be used to generate all needed files 
	//in the preferred way/protocol-structure and agregation, afterwards
	if (mSendInfo.master_trip)
	{
		pVH->mContGPS = 0;
		time_now = glfwGetTime();
		time_diff = time_now - pVH->mTimeLast;
		time_delta = 1.5 * (rand() % 100 / 100.0);
		//time_delta = 0.0;

		if ((time_diff > pVH->mGPSPointTimer + time_delta) || (mbSendposition == true))
		{
			mbSendposition = false;
			pVH->mTimeLast = glfwGetTime();

			format_alltrips(pVH, cad_csv);
			send_alltrips(pVH, &cad_csv);
		}
	}
}

//-----------------------------------------------------------------------------
void Proxy::send_master_trip_info(bool active)
{
	Cadena cadtrip_name;
	Cadena cadtime = Simulator::get_timestamp_format_YYMMDDhhmmss();

	cadtrip_name.formCadena(SIM_DIR_TEST_PX_MST_"trip_%s.txt", cadtime.getCadena());
	cadtrip_name = mcadgral + cadtrip_name;

	if (mMasterTrip.is_open() && (active == false))
	{
		mMasterTrip.flush();
		mMasterTrip.close();
	}
	else if ((!mMasterTrip.is_open()) && (active == true))
	{
		mMasterTrip.open(cadtrip_name.getCadena(), std::ios::app| std::ios::in | std::ios::out);
	}
}

//-----------------------------------------------------------------------------
void Proxy::send_simulator_info(bool bsend)
{
	ofstream  active_sending;
	if (bsend == true)
	{
		if (mSendSimInfo == true) sim_file_close();
		sim_file_open();

		mSendSimInfo = true;
		Cadena cadopen = mcadgral + SIM_DIR_TEST_PX_SIM_"ACTIVE_SENDING";
		active_sending.open(cadopen.getCadena(), std::ios_base::app);
		mTimeLast = glfwGetTime();
	}
	else
	{
		mSendSimInfo = false;
		sim_file_close();
		active_sending.close();
		Cadena cadremove = mcadgral + SIM_DIR_TEST_PX_SIM_"ACTIVE_SENDING";
		std::remove(cadremove.getCadena());
	}
}

//-----------------------------------------------------------------------------
void Proxy::sim_file_open()
{
	Cadena cad_header;
	std::time_t transmission_time;
	mCadSimFileName.formCadena(SIM_DIR_TEST_PX_SIM_"sim_%s_%d.txt", Simulator::get_timestamp_format_YYMMDDhhmmss().getCadena(), mRecordSimID);
	mCadSimFileName = mcadgral + mCadSimFileName;
	mSimFile.open(mCadSimFileName.getCadena(), std::ios_base::app);

	transmission_time = std::time(nullptr);

	cad_header.formCadena("{\n\t\"version\": \"1.0\",           \
				            \n\t\"transmissionTime\" : %lld,    \
				            \n\t\"records\" :\n\t[", 
		transmission_time
	);

	mSimFile << cad_header.getCadena() << endl;
	mSimFileOpened = true;

}

//-----------------------------------------------------------------------------
void Proxy::sim_file_close()
{
	Cadena file_renamed;
	Cadena cad_tail;
	cad_tail.formCadena("\t]\n}");
	mSimFile << cad_tail.getCadena() << endl;
	mSimFile.flush();
	mSimFile.close();
	file_renamed.formCadena("%s_ready.json", mCadSimFileName.getCadena());

	if (std::rename(mCadSimFileName.getCadena(), file_renamed.getCadena())) {}
	
}

//-----------------------------------------------------------------------------
void Proxy::write_simulator_info(Vehiculo* pVH, Cadena& cadbuf_simulator)
{
	mSimFile << cadbuf_simulator.getCadena() << endl;
}

//-----------------------------------------------------------------------------
void Proxy::format_simulator_file(Vehiculo* pVH, Cadena& cadbuf, bool sim_gps_time, bool sim_detector_time)
{

	Cadena detector_type, detector_id, cad_time_stamp, category, lpn;
	double lat, lon, angle;
	int lane;
	int speed;

	double meters_spread_x, meters_spread_y;
	double delta_gps_x, delta_gps_y;

	meters_spread_x = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	meters_spread_y = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	delta_gps_x = meters_spread_x * (double(rand() % 2000) / 2000.0) - (meters_spread_x / 2.0);
	delta_gps_y = meters_spread_y * (double(rand() % 2000) / 2000.0) - (meters_spread_y / 2.0);

	if (sim_detector_time)
	{

		detector_type = "DETECTOR";
		detector_id.formCadena("%s", pVH->mSendDetectorName);
		lat = 0.0;
		lon = 0.0;
		category = "";
		lpn = "";
		speed = 0;
		lane =  0;
		angle = 0.0;

	}
	else
	{
		category.formCadena("%s", pVH->mCategory);
		detector_type = "GPS";
		detector_id = "";
		pVH->mPosicion_lat = Simulator::get_delta_lat(pVH->mPtrNodeInfoNow->lat, (pVH->mPosicion_y - pVH->mPtrNodeInfoNow->Y) + delta_gps_y);
		pVH->mPosicion_lon = Simulator::get_delta_lon(pVH->mPtrNodeInfoNow->lon, pVH->mPtrNodeInfoNow->lat, pVH->mPosicion_lat, (pVH->mPosicion_x - pVH->mPtrNodeInfoNow->X) + delta_gps_x);
		lat = pVH->mPosicion_lat;
		lon = pVH->mPosicion_lon;
		lpn = pVH->mLPN;
		speed = int(pVH->mSpeedNow);
		angle = Simulator::rad2deg(pVH->mPtrNodeInfoNow->angle);
		lane  = int(pVH->mLaneNow);

	}
	cad_time_stamp = Simulator::get_timestamp_format_YYYYMMDD_T_hhmmss();

	cadbuf.formCadena("\t\t{\
		                \n\t\t\t\"record_id\": %d,\
                        \n\t\t\t\"vehicle_id\" : %d,\
                        \n\t\t\t\"time_stamp\" : \"%s\",\
                        \n\t\t\t\"detector_type\" : \"%s\",\
                        \n\t\t\t\"detector_id\" : \"%s\",\
                        \n\t\t\t\"category\" : \"%s\",\
		                \n\t\t\t\"latitude\" : %lf,\
		                \n\t\t\t\"longitude\" : %lf,\
		                \n\t\t\t\"heading\" : %.02lf,\
		                \n\t\t\t\"speed\" : %d,\
		                \n\t\t\t\"lane\" : %d,\
		                \n\t\t\t\"license_plate\" : \"%s\"\
		                \n\t\t}",
		mRecordSimID,
		pVH->mIndexGral,
		cad_time_stamp.getCadena(),
		detector_type.getCadena(),
		detector_id.getCadena(),
		category.getCadena(),
		lat,
		lon,
		angle,
		speed,
		lane,
		lpn.getCadena()

	);


	if (mSimFileOpened)
	{
		mSimFileOpened = false;
	}
	else
	{
		cadbuf.formCadena("\t\t,\n%s", cadbuf.getCadena());      
	}

	mRecordSimID++;
}

//-----------------------------------------------------------------------------
void Proxy::initialize_file(Vehiculo* pVH)
{
	Cadena cadbuf, cadfile;
	format_info_pos_eurotoll_head(pVH, cadbuf);
	mpFILEfrmt_eurotoll[pVH->mIndex].open(cadfile.formCadena("%s", pVH->mTripName_frmt_eurotoll).getCadena(), std::ios_base::app);
	if (!mpFILEfrmt_eurotoll[pVH->mIndex].is_open()) { perror("format_info_pos_eurotoll_head:error while opening file"); }
	mpFILEfrmt_eurotoll[pVH->mIndex] << cadbuf.getCadena() << endl;
	mpFILEfrmt_eurotoll[pVH->mIndex].close();
}

//-----------------------------------------------------------------------------
void Proxy::finalize_file(Vehiculo* pVH)
{
	Cadena cadbuf,cadfile;
	mpFILEfrmt_eurotoll[pVH->mIndex].open(cadfile.formCadena("%s",pVH->mTripName_frmt_eurotoll).getCadena(), std::ios_base::app);
	format_info_pos_eurotoll_tail(pVH, cadbuf);

	mpFILEfrmt_eurotoll[pVH->mIndex] << cadbuf.getCadena() << endl;
	mpFILEfrmt_eurotoll[pVH->mIndex].flush();
	mpFILEfrmt_eurotoll[pVH->mIndex].close();
}

//--------------------------------------------------------------------------------------------------------------------------------
//     GPS POSITION FILE EUROTOLL 
// Note: This format was requested for using in Reply Satellite Tolling Test
//--------------------------------------------------------------------------------------------------------------------------------

//------------ HEAD ------------------------------------------------------------
//------------------------------------------------------------------------------
void Proxy::format_info_pos_eurotoll_head(Vehiculo* pVH, Cadena& cadbuf)
{
	/*
	{
		"version": "1.0",
			"clientReference" : "NHTL71O",
			"psn" : "07770002131812560000055647",
			"vehicleReference" : "3004900012",
			"lpn" : "DJ-353-GP",
			"lpCountry" : "F",
			"transmissionEventId" : 1,
			"transmissionTime" : 1604059973495,
			"records" : [{

	*/
	time_t curr_time;
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);
	std::time_t result = std::time(nullptr);
	Cadena cadLPN;

	cadbuf.formCadena("{\n\t\"version\": \"1.0\",\n\t\"clientReference\" : \"SIMULATOR%d\",\n\t\"psn\" : \"%lld\",\n\t\"vehicleReference\" : \"%d\",\n\t\"lpn\" : \"%s\",\n\t\"lpCountry\" : \"E\",\n\t\"transmissionEventId\" : \"%d\",\n\t\"transmissionTime\" : \"%lld\",\n\t\"records\" :\n\t[\n",
		pVH->mIndexGral,
		pVH->mGPSId,
		pVH->mIndexGral,
		cadLPN.formCadena("%s",pVH->mLPN).getCadena(),
		pVH->mIndexGral,
		result
		//tm_local->tm_mday, tm_local->tm_mon + 1, (tm_local->tm_year + 1900) % 100,
		//tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec
	);

}

//------------ CORE ------------------------------------------------------------
//------------------------------------------------------------------------------
void Proxy::format_info_pos_format_eurotoll(Vehiculo* pVH, Cadena& cadbuf)
{
	time_t curr_time;
	double time_now;

	double meters_spread_x, meters_spread_y;
	double delta_gps_x, delta_gps_y;

	meters_spread_x = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	meters_spread_y = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	delta_gps_x = meters_spread_x * (double(rand() % 2000) / 2000.0) - (meters_spread_x / 2.0);
	delta_gps_y = meters_spread_y * (double(rand() % 2000) / 2000.0) - (meters_spread_y / 2.0);

	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	pVH->mPosicion_lat = Simulator::get_delta_lat(pVH->mPtrNodeInfoNow->lat, (pVH->mPosicion_y - pVH->mPtrNodeInfoNow->Y) + delta_gps_y);
	pVH->mPosicion_lon = Simulator::get_delta_lon(pVH->mPtrNodeInfoNow->lon, pVH->mPtrNodeInfoNow->lat, pVH->mPosicion_lat, (pVH->mPosicion_x - pVH->mPtrNodeInfoNow->X) + delta_gps_x);

	std::time_t result = std::time(nullptr);

	time_now = glfwGetTime();

	cadbuf.formCadena("\t\t{\n\t\t\t\"recordId\": \"%d\",\n\t\t\t\"recordTime\" : \"%lld\",\n\t\t\t\"recordingEventId\" : \"%d\",\n\t\t\t\"latitude\" : \"%lf\",\n\t\t\t\"longitude\" : \"%lf\",\n\t\t\t\"stopDuration\" : \"0\",\n\t\t\t\"drivingDuration\" : \"%ld\",\n\t\t\t\"drivingDistance\" : \"%d\",\n\t\t\t\"hdop\" : \"1\",\n\t\t\t\"heading\" : \"%d\",\n\t\t\t\"speed\" : \"%d\",\n\t\t\t\"gpsOdometer\" : \"0\"\n\t\t}, ",
		pVH->mRegID,
		//tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec, tm_local->tm_mday, tm_local->tm_mon + 1, (tm_local->tm_year + 1900) % 100,
		result,
		pVH->mIndexGral,
		pVH->mPosicion_lat,
		pVH->mPosicion_lon,
		int(time_now - pVH->mTimeIni),
		int(pVH->mDistance * 1000.0),
		int(Simulator::rad2deg(pVH->mPtrNodeInfoNow->angle)),
		int(pVH->mSpeedNow)
	);

	pVH->mRegID++;
}

//------------ TAIL ------------------------------------------------------------
//------------------------------------------------------------------------------
void Proxy::format_info_pos_eurotoll_tailer(Vehiculo* pVH)
{
	Cadena cadbuf,cadfile;
	int i = SIM_MAX_VEHICLES;

	while (i--)
	{
		if ((pVH[i].mAlive == true) && (pVH[i].mFileTail == false) && (mOutputFile == true))
		{
			mpFILEfrmt_eurotoll[pVH[i].mIndex].open(cadfile.formCadena("%s",pVH[i].mTripName_frmt_eurotoll).getCadena(), std::ios_base::app);
			format_info_pos_eurotoll_tail(&pVH[i], cadbuf);
			mpFILEfrmt_eurotoll[pVH[i].mIndex] << cadbuf.getCadena() << endl;
			mpFILEfrmt_eurotoll[pVH[i].mIndex].flush();
			mpFILEfrmt_eurotoll[pVH[i].mIndex].close();

		}
	}
}

//------------------------------------------------------------------------------
void Proxy::format_info_pos_eurotoll_tail(Vehiculo* pVH, Cadena& cadbuf)
{
	cadbuf.formCadena("\t]\n}");
	pVH->mFileTail = true;
}

//--------------------------------------------------------------------------------------------------------------------------------
//     GPS POSITION FILE EUROTOLL PERF
// Note: This format was requested for using in Reply Satellite Tolling PERFORMANCE Test
//--------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Proxy::format_info_pos_format_eurotoll_perf(Vehiculo* pVH, Cadena& cadbuf)
{

	time_t curr_time;
	double time_now;

	double meters_spread_x, meters_spread_y;
	double delta_gps_x, delta_gps_y;

	meters_spread_x = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	meters_spread_y = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	delta_gps_x = meters_spread_x * (double(rand() % 2000) / 2000.0) - (meters_spread_x / 2.0);
	delta_gps_y = meters_spread_y * (double(rand() % 2000) / 2000.0) - (meters_spread_y / 2.0);

	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	pVH->mPosicion_lat = Simulator::get_delta_lat(pVH->mPtrNodeInfoNow->lat, (pVH->mPosicion_y - pVH->mPtrNodeInfoNow->Y) + delta_gps_y);
	pVH->mPosicion_lon = Simulator::get_delta_lon(pVH->mPtrNodeInfoNow->lon, pVH->mPtrNodeInfoNow->lat, pVH->mPosicion_lat, (pVH->mPosicion_x - pVH->mPtrNodeInfoNow->X) + delta_gps_x);

	std::time_t time_stamp = std::time(nullptr);

	time_now = glfwGetTime();

	cadbuf.formCadena("veh%d#%lld#%ld#%ld#%lf#%lf#%d#%d#%d#%d%d",
		pVH->mIndexGral,
		time_stamp,
		long int(time_now - mTime_init),
		long int(time_now - pVH->mTimeIni),
		pVH->mPosicion_lat,
		pVH->mPosicion_lon,
		int(pVH->mDistance * 1000.0),
		int(Simulator::rad2deg(pVH->mPtrNodeInfoNow->angle)),
		int(pVH->mSpeedNow),
		pVH->mEstacionEntrada,
		pVH->mEstacionSalida
	);
}

//------------------------------------------------------------------------------
void Proxy::send_pos_file_format_eurotoll_perf(Vehiculo* pVH, Cadena* pcad)
{
	mMasterPerf << pcad->getCadena() << endl;
}

//------------------------------------------------------------------------------
void Proxy::send_pos_file_format_eurotoll(Vehiculo* pVH, Cadena* pcad)
{
	Cadena cadfile;
	if (!mpFILEfrmt_eurotoll[pVH->mIndex].is_open())
	{
		mpFILEfrmt_eurotoll[pVH->mIndex].open(cadfile.formCadena("%s",pVH->mTripName_frmt_eurotoll).getCadena(), std::ios_base::app);
		if (!mpFILEfrmt_eurotoll[pVH->mIndex].is_open()) { perror("send_pos_file_format_eurotoll: error while opening file"); }
		mpFILEfrmt_eurotoll[pVH->mIndex] << pcad->getCadena() << endl;

		mpFILEfrmt_eurotoll[pVH->mIndex].flush();
		mpFILEfrmt_eurotoll[pVH->mIndex].close();
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
//     GPS POSITION FILE FORMAT ALL TRIPS
// Note: This format is store all trip GPS information in a simple way
//--------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Proxy::format_alltrips(Vehiculo* pVH, Cadena& cad_csv)
{

	double meters_spread_x, meters_spread_y;
	double delta_gps_x, delta_gps_y;

	meters_spread_x = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	meters_spread_y = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	delta_gps_x = meters_spread_x * (double(rand() % 2000) / 2000.0) - (meters_spread_x / 2.0);
	delta_gps_y = meters_spread_y * (double(rand() % 2000) / 2000.0) - (meters_spread_y / 2.0);

	pVH->mPosicion_lat = Simulator::get_delta_lat(pVH->mPtrNodeInfoNow->lat, (pVH->mPosicion_y - pVH->mPtrNodeInfoNow->Y) + delta_gps_y);
	pVH->mPosicion_lon = Simulator::get_delta_lon(pVH->mPtrNodeInfoNow->lon, pVH->mPtrNodeInfoNow->lat, pVH->mPosicion_lat, (pVH->mPosicion_x - pVH->mPtrNodeInfoNow->X) + delta_gps_x);

	cad_csv.formCadena("%f,%f", pVH->mPosicion_lat, pVH->mPosicion_lon);

}

//------------------------------------------------------------------------------
void Proxy::send_alltrips(Vehiculo* pVH, Cadena* pcad_csv)
{
	mMasterTrip << pcad_csv->getCadena() << endl;
}

//--------------------------------------------------------------------------------------------------------------------------------
//     GPS POSITION FILE FORMAT HUB AUTOPISTAS
// Note: This format was requested for using in Hub Autopistas interface
//--------------------------------------------------------------------------------------------------------------------------------
void Proxy::format_info_pos_format_hubautop(Vehiculo* pVH, Cadena& cadbuf)
{

	/*
	*HQ, 1400046168, V1, 103025, A, 2234.3066, N, 11351.6829, E, 000.0, 000, 010221, FFFFFFFF#
	* indica el inicio del mensaje. # indica el final.Esto se utiliza para separar multiples tramas en una misma recepcción.
	HQ es un valor fijo.Siempre a ester valor.
	1400046168 es un identificador del dongle, pon el valor que quieras
	V1 es un mensaje de posición con coordenadas.Entiendo que todos serán de este  tipo
	103025 es la hora en formato HHMMSS en UTC
	A indica que las coordenadas GPS son válidas.En tu caso siempre será A.Si quieres enviar tramas sin posición GPS para generar gaps puedes enviar V en lugar de A y entonces no se consideran los valores GPS, es decir, el dongle está vivo pero no tiene recepción GPS
	2234.3066 latitud en formato DDFF.FFFF.DD son los grados de 0 a 90 y FF.FFFF los minutos en decimal.
	N norte o sur de la latitud.Si la latitud es negativa es N en caso contrario es S.Por ejemplo para convertir 22, 571777 a DDM se coje la parte entera para los DD y la parte decimal se multiplica por 60 y se cojen los 4 primeros decimals. 0, 571777 * 60 = 34.30662 = > 2234.3066 y N porque es positivo.
	11351.6829 longitud en formato DDDFF.FFFF.DDD son los grados de 0 a 180 y FF.FFFF los minutos en decimal.
	E este o oeste de la longitud.Si es negativa W en caso contratio E.Para convertir 113, 861382 a DDM tenemos 113 parte entera y 0, 861382 * 60 = 51, 68292 = > 11351.6829 y E porque es positiva.
	000.0 velocidad en millas por hora.Si no tienes velocidad puedes poner 000.0 o directamente dejar el campo vacio.Si te va mejor en kmh me lo comentas y no hago la conversión.
	000 angulo de dirección. 0 es el Norte, 90 es Este, 180 Sur y 270 es Oeste.Si no tienes este dato déjalo a 0 o vacio.
	010221 fecha en formato DDMMYY en UTC
	FFFFFFFF estado del vehículo.Son 4 bytes para indicar posibles alarmas del vehículo.Si no tenemos simplemente ponlo todo a 1 y ya implementaré para que no considere ninguna alarma.
	*/

	time_t curr_time;
	char lat_orientation;
	char lon_orientation;
	double lat_fractpart, lat_intpart;
	double lon_fractpart, lon_intpart;
	double meters_spread_x, meters_spread_y;
	double delta_gps_x, delta_gps_y;

	meters_spread_x = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	meters_spread_y = pVH->mGPSInaccuracy * 2.0 / 1000.0;
	delta_gps_x = meters_spread_x * (double(rand() % 2000) / 2000.0) - (meters_spread_x / 2.0);
	delta_gps_y = meters_spread_y * (double(rand() % 2000) / 2000.0) - (meters_spread_y / 2.0);

	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);

	pVH->mPosicion_lat = Simulator::get_delta_lat(pVH->mPtrNodeInfoNow->lat, (pVH->mPosicion_y - pVH->mPtrNodeInfoNow->Y) + delta_gps_y);
	pVH->mPosicion_lon = Simulator::get_delta_lon(pVH->mPtrNodeInfoNow->lon, pVH->mPtrNodeInfoNow->lat, pVH->mPosicion_lat, (pVH->mPosicion_x - pVH->mPtrNodeInfoNow->X) + delta_gps_x);

	lat_fractpart = modf(pVH->mPosicion_lat, &lat_intpart);
	lat_fractpart = lat_fractpart * 60.0;
	lon_fractpart = modf(pVH->mPosicion_lon, &lon_intpart);
	lon_fractpart = lon_fractpart * 60.0;

	if (lat_intpart > 0) { lat_orientation = 'N'; }
	else { lat_orientation = 'S'; }
	if (lon_intpart > 0) { lon_orientation = 'E'; }
	else { lon_orientation = 'W'; }

	cadbuf.formCadena("*HQ,%lld,V1,%02d%02d%02d,A,%02d%07.4f,%c,%03d%07.4f,%c,%05.1f,000,%02d%02d%02d,FFFFFFFF#",
		pVH->mGPSId,
		tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec,
		int(lat_intpart), lat_fractpart,
		lat_orientation,
		int(lon_intpart), lon_fractpart,
		lon_orientation,
		pVH->mSpeedNow * SIM_KM2MILLAS,
		tm_local->tm_mday, tm_local->tm_mon + 1, (tm_local->tm_year + 1900) % 100);

}

//------------------------------------------------------------------------------
void Proxy::send_pos_file_format_hubautop(Vehiculo* pVH, Cadena* pcad_hubautop, Cadena* pcad_csv)
{
	mMasterTrip << pcad_csv->getCadena() << endl;
}

//--------------------------------------------------------------------------------------------------------------------------------
//     THIS INTERFACE IS FOR SENDING GPS INFORMATION TO HUB AUTOPISTAS USING SOCKET
// 
//--------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Proxy::send_pos_cloud(Vehiculo* pVH, Cadena* pcad)
{
	int iResult;

	if (mConnectSocket != INVALID_SOCKET)
	{
		// Send an initial buffer
		iResult = send(mConnectSocket, pcad->getCadena(), pcad->getLong(), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(mConnectSocket);
			WSACleanup();
			mConnectSocket = INVALID_SOCKET;
			return;
		}

		mGPSPointsSent++;
		mGPSPointsCount++;

		if (mGPSPointsCount >= 100)
		{
			mGPSPointsCount = 0;
			cout << "GPS points sent:[" << mGPSPointsSent << "]" << endl;
		}
	}
	else
	{
		socket2cloud_ini();
	}

}

//------------------------------------------------------------------------------
void Proxy::socket2cloud_ini()
{
	int iResult;
	WSADATA wsaData;
	struct addrinfo hints;

	cout << "Initializing socket to Cloud" << endl;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(SIM_PROXY_CLOUD_HOST, SIM_PROXY_CLOUD_PORT, &hints, &mPtrAdrrInfo_ORIG);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	mConnectSocket = INVALID_SOCKET;

	// Attempt to connect to an address until one succeeds
	for (mPtrAdrrInfo = mPtrAdrrInfo_ORIG; mPtrAdrrInfo != NULL; mPtrAdrrInfo = mPtrAdrrInfo->ai_next)
	{
		// Create a SOCKET for connecting to server
		mConnectSocket = socket(mPtrAdrrInfo->ai_family, mPtrAdrrInfo->ai_socktype, mPtrAdrrInfo->ai_protocol);
		if (mConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect(mConnectSocket, mPtrAdrrInfo->ai_addr, (int)mPtrAdrrInfo->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(mConnectSocket);
			mConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
}