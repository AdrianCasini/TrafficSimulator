#include "Logic.h"
#include "Traffic.h"

//------------------------------------------------------------------------------
Logic::Logic()
{

}

//------------------------------------------------------------------------------
Logic::~Logic()
{

}

//------------------------------------------------------------------------------
void Logic::init(Traffic* ptr)
{
	mpTR = ptr;
	mpFunc_domove = &Logic::do_alg_simple;
}

//------------------------------------------------------------------------------
void Logic::set_alg(int ealg)
{
	if (ealg == Traffic::SIM_TR_MOVE_SIMPLE)
	{
		mpFunc_domove = &Logic::do_alg_simple;
	}
	else // Traffic::SIM_TR_MOVE_ALG
	{
		mpFunc_domove = &Logic::do_alg_overpass;
	}
}

//------------------------------------------------------------------------------
void Logic::do_alg_simple(Vehiculo* pVH, int& index)
{
	move_simple(pVH);
}


//------------------------------------------------------------------------------
Cadena Logic::get_text_state(int state)
{
	switch (state)
	{
	case Traffic::SIM_TR_ALG_NOT_ALIVE:
		return "SIM_TR_ALG_NOT_ALIVE";
		break;
	case Traffic::SIM_TR_ALG_NORMAL:
		return "SIM_TR_ALG_NORMAL";
		break;
	case Traffic::SIM_TR_ALG_CHANGING_NODE:
		return "SIM_TR_ALG_CHANGING_NODE";
		break;
	case Traffic::SIM_TR_ALG_OVERLAPING:
		return "SIM_TR_ALG_OVERLAPING";
		break;
	case Traffic::SIM_TR_ALG_STATE_NEW_LANE:
		return "SIM_TR_ALG_STATE_NEW_LANE";
		break;
	case Traffic::SIM_TR_ALG_EXIT:
		return "SIM_TR_ALG_EXIT";
		break;
	case Traffic::SIM_TR_ALG_CHANGING_LANE_PASS:
		return "SIM_TR_ALG_CHANGING_LANE_PASS";
		break;
	case Traffic::SIM_TR_ALG_CH_LANE_MANDATORY:
		return "SIM_TR_ALG_CH_LANE_MANDATORY";
		break;
	case Traffic::SIM_TR_ALG_CH_LANE:
		return "SIM_TR_ALG_CH_LANE";
		break;
	case Traffic::SIM_TR_ALG_LANE_RETURN:
		return "SIM_TR_ALG_LANE_RETURN";
		break;
	case Traffic::SIM_TR_ALG_CHANGING_LANE_DISTURBING:
		return "SIM_TR_ALG_CHANGING_LANE_DISTURBING";
		break;
	default: 
		break;

	}

	return "";
}
//------------------------------------------------------------------------------
void Logic::log(Vehiculo* pVH)
{
	Cadena cadstate;
	cadstate = get_text_state(pVH->mState);
	if (mDebug)
	{
		cout << "index=[" << pVH->mIndex << "] speed:[" << pVH->mSpeedNow << "]  lane:[" << pVH->mLaneNow << "]  lanenew:[" << pVH->mLaneNew << "] overlap:[" << pVH->mCounterOverlap << "] lanereturn:[" << pVH->mCounterLaneReturn << "]" <<  cadstate.getCadena()  << endl;
	}
}

//------------------------------------------------------------------------------
void Logic::do_alg_overpass(Vehiculo* pVH, int& index)
{
	double dt;
	double dt_n2n, dist;
	int index_next;
	bool node_next=false;
	int vh_inf, vh_sup;

	switch (pVH->mState)
	{

		//----------------------------------------------------------------
		//Cambiar por montaje de logicas
		case Traffic::SIM_TR_ALG_NORMAL:  

			//log(pVH);
			// In this state the vehicle is 
			// running at first without any interruption

			 //Real distance in km (speed in km/h) 
			dt = pVH->mSpeedNow * mpTR->mTH.delta_time;
			dt_n2n = dt / SIM_SPEED_VH; //(FACTOR)
			dist = pVH->mNd2nd + dt_n2n;

			//Are we facing an overlapping?
			if (mpTR->mOV.overlap_new(pVH, mpTR->mTH.pVH, dist, index_next))
			{
				//The vehicle goes to OVERLAPING state
				//The vehicle reduce its speed
				pVH->mSpeedBrakeFactor = true;
				pVH->mSpeedBrakeObstacle = mpTR->mTH.pVH[index_next].mSpeedNow;
				pVH->mState = Traffic::SIM_TR_ALG_OVERLAPING;

			}
			else
			{
				//Here we are going to do the movement. 
				//This movement could be the last and exit
				if (move_overpass(pVH, dist, dt_n2n, dt, false, -1, -1))
				{
					//If we are moving but not in the default lane, we noted.
					//Note: Lane default was calculated previously by move function
					if (pVH->mLaneNow != pVH->mLaneDefault)
					{
						pVH->mState = Traffic::SIM_TR_ALG_LANE_RETURN;
					}
				}
				else
				{
					//Goodbye
					pVH->mState = Traffic::SIM_TR_ALG_EXIT;
				}
			}

			break;


			//----------------------------------------------------------------
		case Traffic::SIM_TR_ALG_LANE_RETURN:
			//log(pVH);

			pVH->mCounterLaneReturn--;
			// In this state the vehicle is 
			// running at first without any interruption trying to go back to the default lane

			dt = pVH->mSpeedNow * mpTR->mTH.delta_time;
			dt_n2n = dt / SIM_SPEED_VH;
			dist = pVH->mNd2nd + dt_n2n;

			//Are we facing an overlapping?
			if (mpTR->mOV.overlap_new(pVH, mpTR->mTH.pVH, dist, index_next))
			{
				//The vehicle goes to OVERLAPING state
				//The vehicle reduce its speed 
				pVH->mSpeedBrakeFactor = true;
				pVH->mSpeedBrakeObstacle = mpTR->mTH.pVH[index_next].mSpeedNow;
				pVH->mState = Traffic::SIM_TR_ALG_OVERLAPING;
			}
			else
			{
				pVH->mSpeedBrakeFactor = false;
				//Here we are going to do the movement. 
				//This movement could be the last and exit
				if (move_overpass(pVH, dist, dt_n2n, dt, false, -1, -1))
				{
					if (pVH->mCounterLaneReturn <= 0)
					{
						pVH->mLaneNew = Simulator::get_delta_lane_return(pVH->mLaneNow);
						pVH->mState = Traffic::SIM_TR_ALG_CHANGING_LANE_PASS;
					}
				}
				else
				{
					//Goodbye
					pVH->mState = Traffic::SIM_TR_ALG_EXIT;
				}
			}

			break;

			//----------------------------------------------------------------
		case Traffic::SIM_TR_ALG_OVERLAPING:
			//log(pVH);

			dt = pVH->mSpeedNow * mpTR->mTH.delta_time;
			dt_n2n = dt / SIM_SPEED_VH;
			dist = pVH->mNd2nd + dt_n2n;

			pVH->mCounterOverlap--;
			pVH->mCounterLaneReturn = pVH->mLaneReturnAvrg;

			if (mpTR->mOV.overlap_new(pVH, mpTR->mTH.pVH, dist, index_next))
			{
				pVH->mSpeedBrakeFactor = true;
				pVH->mSpeedBrakeObstacle = mpTR->mTH.pVH[index_next].mSpeedNow;
			}
			else
			{
				pVH->mSpeedBrakeFactor = false;
				if (!move_overpass(pVH, dist, dt_n2n, dt, false, -1, -1))
				{
					pVH->mState = Traffic::SIM_TR_ALG_EXIT;
				}
			}

			if ((pVH->mCounterOverlap <= 0) && (pVH->mPtrNodeInfoNow->lanes > 1))
			{
				pVH->mLaneNew = Simulator::get_new_lane(pVH->mLaneNow, pVH->mPtrNodeInfoNow->lanes);
				pVH->mState = Traffic::SIM_TR_ALG_CHANGING_LANE_PASS;
			}

			break;

			//----------------------------------------------------------------
		case Traffic::SIM_TR_ALG_CHANGING_LANE_PASS:
			//log(pVH);
			// If we are here is because 
			// we need to change the lane right now

			dt = pVH->mSpeedNow * mpTR->mTH.delta_time;
			dt_n2n = dt / SIM_SPEED_VH;
			dist = pVH->mNd2nd + dt_n2n;

			pVH->mCounterOverlap = SIM_TR_LANE_NEW;
			pVH->mCounterLaneReturn = pVH->mLaneReturnAvrg;

			if (mpTR->mOV.overlap_lane_new(pVH, mpTR->mTH.pVH, dist, vh_inf, vh_sup))
			{
				//Start again all the procees to pass a vehicle
				pVH->mSpeedBrakeFactor = false;
				pVH->mState = Traffic::SIM_TR_ALG_NORMAL;
			}
			else
			{
				pVH->mSpeedBrakeFactor = false;
				if (move_overpass(pVH, dist, dt_n2n, dt, true, vh_inf, vh_sup))
				{
					//Start again all the procces to pass a vehicle
					pVH->mSpeedBrakeFactor = false;
					pVH->mState = Traffic::SIM_TR_ALG_NORMAL;
				}
				else
				{
					pVH->mState = Traffic::SIM_TR_ALG_EXIT;
				}
			}

			break;

			//----------------------------------------------------------------
		case Traffic::SIM_TR_ALG_CH_LANE_MANDATORY:
			//log(pVH);
			// If we are here is because 
			// the new node has less lanes than the actual

			dt = pVH->mSpeedNow * mpTR->mTH.delta_time;
			dt_n2n = dt / SIM_SPEED_VH;
			dist = pVH->mNd2nd + dt_n2n;

			pVH->mLaneNew = pVH->mLaneDefault;

			if (mpTR->mOV.overlap_lane_new(pVH, mpTR->mTH.pVH, dist, vh_inf, vh_sup))
			{
				pVH->mSpeedBrakeFactor = false;
				pVH->mState = Traffic::SIM_TR_ALG_CH_LANE_MANDATORY;
			}
			else
			{
				pVH->mSpeedBrakeFactor = false;
				if (move_overpass(pVH, dist, dt_n2n, dt, true, vh_inf, vh_sup))
				{
					pVH->mSpeedBrakeFactor = false;
					pVH->mState = Traffic::SIM_TR_ALG_NORMAL;
					pVH->mCounterOverlap = SIM_TR_LANE_NEW;
					pVH->mCounterLaneReturn = pVH->mLaneReturnAvrg;
				}
				else
				{
					pVH->mState = Traffic::SIM_TR_ALG_EXIT;
				}
			}

			break;

			//----------------------------------------------------------------
		case Traffic::SIM_TR_ALG_EXIT:
			//log(pVH);
			mpTR->vehicle_out(pVH);
			break;

			//----------------------------------------------------------------
		case Traffic::SIM_TR_ALG_NOT_ALIVE:
			break;

			//----------------------------------------------------------------
		default:
			break;

	}
	mpTR->get_speed(pVH);
}

//------------------------------------------------------------------------------
bool Logic::move_overpass(Vehiculo* pVH, double dist, double dt_n2n, double dt, bool change_lane, int vh_inf, int vh_sup)
{
	double pl, pw = 1.0;
	bool new_node = false;

	pVH->mNd2nd = dist;

	//---Need to change node?---
	if (dist > pVH->mPtrNodeInfoNow->dist)
	{
		pVH->mNd2nd = dist - pVH->mPtrNodeInfoNow->dist;
		pw = pVH->mNd2nd / dt_n2n;

		new_node = true;
		if (!change_lane) mpTR->mpTST->setget_following(pVH, (pVH->mPtrNodeInfoNow + 1)->node_vh_order, (pVH->mPtrNodeInfoNow)->node_vh_order);
		//if (mDebug) cout << "new node:[" << pVH->mIndex << "]" << endl;

		pVH->mPtrNodeInfoNow++;
		pVH->mNodeIndex++;
		mpTR->get_speed(pVH);

		pVH->mLaneDefault = 1;// Simulator::get_lane_default(pVH->mSpeedNow, pVH->mPtrNodeInfoNow->lanes, pVH->mLaneType);
		if (pVH->mLaneNow > pVH->mPtrNodeInfoNow->lanes) pVH->mState = Traffic::SIM_TR_ALG_CH_LANE_MANDATORY;

		// ---- Is at the end of the trip ?----
		// ---- I finish my trip one node before--
		if (pVH->mOrdenNodeRev--)
		{
			//---Adjust x and y for the last node---
			pl = mpTR->mTH.delta_lane * Simulator::get_delta_lane(pVH->mLaneNow, pVH->mPtrNodeInfoNow->lanes);
			pVH->mPosicion_x = pVH->mPtrNodeInfoNow->X - pl * pVH->mPtrNodeInfoNow->delta_lane_x;
			pVH->mPosicion_y = pVH->mPtrNodeInfoNow->Y - pl * pVH->mPtrNodeInfoNow->delta_lane_y;

		}
		else //---Vehicle OUT---
		{
			return false;
		}
	}

	
	//if (pVH->mLaneChangeCounter <= 0) {pVH->mLaneChangeCounter = 0;	pVH->mLaneDeltaChange = 0.0;}

	//---Do the Movement---
	pVH->mLaneDeltaChange = 0.0;
	if (change_lane)
	{
		mpTR->mpTST->setget_following_lane(pVH, new_node,vh_inf, vh_sup);
		//pVH->mLaneDeltaChange = mpTR->mTH.delta_lane * (double) (Simulator::get_delta_lane_ch(pVH->mLaneNow, pVH->mLaneNew)) / (double)(SIM_TR_LANE_CH_DELTA_CNT);
		pVH->mLaneDeltaChange = mpTR->mTH.delta_lane * (double) (Simulator::get_delta_lane_ch(pVH->mLaneNow, pVH->mLaneNew));
		//pVH->mLaneChangeCounter = SIM_TR_LANE_CH_DELTA_CNT;
		pVH->mLaneNow = pVH->mLaneNew;
	}
	
	pVH->mPosicion_x = pVH->mPosicion_x + pVH->mPtrNodeInfoNow->deltamv_x * dt * pw - pVH->mLaneDeltaChange * pVH->mPtrNodeInfoNow->delta_lane_x;
	pVH->mPosicion_y = pVH->mPosicion_y + pVH->mPtrNodeInfoNow->deltamv_y * dt * pw - pVH->mLaneDeltaChange * pVH->mPtrNodeInfoNow->delta_lane_y;
	//pVH->mLaneChangeCounter--;

	return true;
}

//------------------------------------------------------------------------------
bool Logic::move_simple(Vehiculo* pVH)
{
	double dt;
	double dt_n2n;
	double pl, pw = 1.0;


	dt = pVH->mSpeedNow * mpTR->mTH.delta_time;
	dt_n2n = dt / SIM_SPEED_VH;
	pVH->mNd2nd = pVH->mNd2nd + dt_n2n;

	//---Need to change node?---
	if (pVH->mNd2nd > pVH->mPtrNodeInfoNow->dist)
	{
		pVH->mNd2nd = pVH->mNd2nd - pVH->mPtrNodeInfoNow->dist;
		pw = pVH->mNd2nd / dt_n2n;

		pVH->mOrdenNode++;
		pVH->mPtrNodeInfoNow++;
		//pVH->mNodeInfoIndex++;

		mpTR->get_speed(pVH);
		pVH->mLaneDefault = Simulator::get_lane_default(pVH->mSpeedNow, pVH->mPtrNodeInfoNow->lanes, pVH->mLaneType);

		// ---- Is still in the trip ?----
		if (pVH->mOrdenNode < pVH->mMaxNodesTrip)
		{
			//---Adjust x and y for the last node---
			pl = mpTR->mTH.delta_lane * Simulator::get_delta_lane(pVH->mLaneNow, pVH->mPtrNodeInfoNow->lanes);
			pVH->mPosicion_x = pVH->mPtrNodeInfoNow->X - pl * pVH->mPtrNodeInfoNow->delta_lane_x;
			pVH->mPosicion_y = pVH->mPtrNodeInfoNow->Y - pl * pVH->mPtrNodeInfoNow->delta_lane_y;
		}
		else //---Vehicle OUT---
		{
			mpTR->vehicle_out(pVH);
			return true;
		}
	}

	//---Do the Movement---
	pVH->mPosicion_x = pVH->mPosicion_x + pVH->mPtrNodeInfoNow->deltamv_x * dt * pw;
	pVH->mPosicion_y = pVH->mPosicion_y + pVH->mPtrNodeInfoNow->deltamv_y * dt * pw;

	return true;

}

//------------------------------------------------------------------------------
bool Logic::move_outsider(NodoInfo*& ptr, int maxnodestrip, double speed, int& ordennode, double& nd2nd, double& posx, double& posy)
{
	double dt;
	double dt_n2n;
	double pl, pw = 1.0;

	dt = speed * 0.2;
	dt_n2n = dt / SIM_SPEED_VH;
	nd2nd = nd2nd + dt_n2n;

	//---Need to change node?---
	if (nd2nd > ptr->dist)
	{
		nd2nd = nd2nd - ptr->dist;
		pw = nd2nd / dt_n2n;
		ordennode++;
		ptr++;

		// ---- Is still in the trip ?----
		if (ordennode < maxnodestrip)
		{
			//---Adjust x and y for the last node---
			pl = mpTR->mTH.delta_lane * 1;
			posx = ptr->X - pl * ptr->delta_lane_x;
			posy = ptr->Y - pl * ptr->delta_lane_y;
		}
		else { return true; }
	}

	//---Do the Movement---
	posx = posx + ptr->deltamv_x * dt * pw;
	posy = posy + ptr->deltamv_y * dt * pw;

	return true;

}