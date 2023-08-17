#ifndef _Vehiculo_H
#define _Vehiculo_H

#include "Simulator.h"
#include "NodeOrder.h"


class Vehiculo
{
	public:


		bool     mAlive;
		int      mState;

		NodeOrder* mpNO;
		int mVhFollowing;
		int mVhBehind;
		double mQuad_size;

		NodoInfo* mPtrNodeInfoNow;
		int mTripIndex;
		int mMaxNodesTrip;
		int mOrdenNode;
		int mOrdenNodeRev;

		double mNd2nd;
		int mIndexGral;
		int mIndex;
		int mNodeIndex;
		
		double mPosicion_x;
		double mPosicion_y;
		double mPosicion_z;

		double mPosicion_lon;
		double mPosicion_lat;

		double mPosicion_x_old;
		double mPosicion_y_old;

		char   mCategory[100];
		Color  mColor;
		double mSize;
		double mSizeCatg;

		char   mCadFile[500];
		char   mCadType[50];

		double mSpeedAvrg;
		double mSpeedNow;
		double mSpeedHwTypeFactor;
		bool   mSpeedPolygonInsideFactor;
		bool   mSpeedBrakeFactor;
		double mSpeedBrakeObstacle;


		int  mLaneDefault;
		int  mLaneNow;
		int  mLaneNew;
		int  mLaneType;
		int  mLaneCounter;
		int  mLaneChangeCounter;
		double mLaneDeltaChange;

		int  mCounterOverlap;
		int  mOverlapAvrg;
		int  mCounterLaneReturn;
		int  mLaneReturnAvrg;

		char mMomentoSalida[100];
		char mMomentoEntrada[100];
		char mEstacionSalida[100];
		char mEstacionEntrada[100];

		double mTimeIni;
		double mTimeFin;
		double mTimeLast;

		char mTripName_frmt_dum[500];
		char mTripName_frmt_eurotoll[500];

		char mLPN[100];
		int    mRegID;
		int    mContGPS;
		bool   mFileTail;
		long long int mGPSId;
		double mDistance;

		float mGPSInaccuracy;
		float mGPSPointTimer;

		bool mInsideToolArea;
		bool mGetIntoTollArea;

		long long int mNode_passed;
		bool mSendDetectorInfo;
		char mSendDetectorName[100];

	public:

		Vehiculo();
		~Vehiculo();
		void reset();
		double get_quad_size(Cadena cad);

};

#endif
