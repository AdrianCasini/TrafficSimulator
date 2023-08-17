#include "Traffic.h"
#include "Vehiculo.h"

//------------------------------------------------------------------------------
Vehiculo::Vehiculo()
{

	reset();

}
//------------------------------------------------------------------------------
Vehiculo::~Vehiculo()
{

}

//------------------------------------------------------------------------------
void Vehiculo::reset()
{
	mPosicion_x = 10000.0;
	mPosicion_y = 10000.0;
	mPosicion_z = 10000.0;

	mPosicion_lon = 0.0;
	mPosicion_lat = 0.0;

	mIndex = 0;
	mNodeIndex = 0;
	mCategory[0] = '\0';

	mSpeedAvrg    = 0;
	mSpeedNow     = 0;
	mSpeedHwTypeFactor = 1.0;
	mSpeedPolygonInsideFactor=false;
	mSpeedBrakeFactor=false;
	mSpeedBrakeObstacle = 0.0;

	mLaneDefault = 0;
	mLaneNow = 0;
	mLaneNew = 0;
	mLaneType = 0;
	mLaneCounter = 0;
	mLaneChangeCounter = 0;
	mLaneDeltaChange = 0.0;

	mSize = SIM_VEHICLE_SIZE;
	mSizeCatg = 0;
	mMomentoEntrada[0]  = '\0';
	mMomentoSalida[0]   = '\0';
	mEstacionEntrada[0] = '\0';
	mEstacionSalida[0]  = '\0';

	mAlive = false;

	mTimeIni = 0.0;
	mTimeFin = 0.0;
	mTimeLast = 0.0;
	mMaxNodesTrip = 0;
	mTripIndex = 0;

	mOrdenNode = 0;
	mNd2nd = 0.0;
	mIndexGral = 0;
	mLaneType = 0;
	mOrdenNodeRev = 0;

	mQuad_size = 0.0;
	mCounterOverlap = 0;
	mCounterLaneReturn = 0;
	mOverlapAvrg=0;
	mLaneReturnAvrg = 0;

	mState = 0;

	mPtrNodeInfoNow = NULL;
	//mTripName_frmt_hubautop[0] = '\0';
	mTripName_frmt_eurotoll[0] = '\0';

	mLPN[0] = '\0';
	mRegID = 1;
	mFileTail = false;
	mContGPS  = 0;
	mGPSId    = 0;
	mDistance = 0.0;
	mGPSInaccuracy = 0.0;
	mGPSPointTimer = 0.0;

	mInsideToolArea = false;
	mGetIntoTollArea = false;

	mNode_passed = 0;
	mSendDetectorInfo = false;
	mSendDetectorName[0] = '\0';

	mVhFollowing = -1;
	mVhBehind = -1;
	mpNO = NULL;

}

//------------------------------------------------------------------------------
double Vehiculo::get_quad_size(Cadena cad)
{
	double size = 1.4;


	if ((cad == "M2") || (cad == "M3"))
	{
		size = 1.3;
	}
	else if ((cad == "T2") || (cad == "T3"))
	{
		size = 1.4;
	}
	else if ((cad == "F2") || (cad == "F3"))
	{
		size = 1.7;
	}
	else if ((cad == "C2") || (cad == "C3"))
	{
		size = 2.1;
	}
	else if ((cad == "C4") || (cad == "C5"))
	{
		size = 3.0;
	}
	else
	{
		size = 1.5;
	}

	return size * SIM_DIST_PW;
}
