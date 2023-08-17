#ifndef _PROXY_H
#define _PROXY_H

#include "Simulator.h"
#include "Vehiculo.h"

class Proxy
{
	public:



		SOCKET mConnectSocket;

		bool   mOutputFile;
		bool   mOutputCloud;
		bool   mbSendposition;
		double mTime_init;
		int    mGPSPointsSent;
		int    mGPSPointsCount;

		ofstream* mpFILEfrmt_hubautop;
		ofstream* mpFILEfrmt_eurotoll;

		ofstream  mMasterTrip;
		ofstream  mMasterPerf;

		struct addrinfo* mPtrAdrrInfo;
		struct addrinfo* mPtrAdrrInfo_ORIG;

		struct s_send_info
		{
			bool sim_detector;
			bool sim_gps;
			bool sattol_gps_eurotoll;
			bool sattol_gps_eurotoll_perf;
			bool master_trip;
			bool active;
		};

		s_send_info  mSendInfo;
	
		//Simulator Interface Files
		ofstream  mSimFile;
		Cadena    mCadSimFileName;
		bool      mSendSimInfo;
		int       mRecordSimID;
		int       mSimRegXFileIndex;
		double    mTimeLast;
		bool      mSimFileOpened;

		Cadena mcadgral;

	public:

		Proxy();
		~Proxy();

		void init(Cadena cadgral);
		void send_info(Vehiculo* pVH);
		void initialize_file(Vehiculo* pVH);
		void finalize_file(Vehiculo* pVH);
		
		//-----------------------------------
		void send_pos_file_format_hubautop(Vehiculo* pVH, Cadena*, Cadena*);
		void send_pos_file_format_eurotoll(Vehiculo* pVH, Cadena* pcad);
		void send_pos_file_format_eurotoll_perf(Vehiculo* pVH, Cadena* pcad);
		void send_alltrips(Vehiculo* pVH, Cadena* pcad);

		void format_info_pos_format_eurotoll_perf(Vehiculo* pVH, Cadena& cadbuf);
		void format_info_pos_format_hubautop(Vehiculo* pVH, Cadena& cadbuf);
		void format_alltrips(Vehiculo* pVH, Cadena& cad_csv);

		void format_info_pos_format_eurotoll(Vehiculo* pVH, Cadena& cadbuf);
		void format_info_pos_eurotoll_head(Vehiculo* pVH, Cadena& cadbuf);
		void format_info_pos_eurotoll_tail(Vehiculo* pVH, Cadena& cadbuf);
		void format_info_pos_eurotoll_tailer(Vehiculo* pVH);


		void send_master_trip_info(bool active);


		void sim_file_open();
		void sim_file_close();
		void send_simulator_info(bool bsend);
		void format_simulator_file(Vehiculo* pVH, Cadena& cadbuf, bool sim_gps_time, bool sim_detector_time);
		void write_simulator_info(Vehiculo* pVH, Cadena& pcad);


		void send_pos_cloud(Vehiculo* pVH, Cadena* pcad);
		void socket2cloud_ini();



};

#endif
