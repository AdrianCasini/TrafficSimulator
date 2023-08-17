#include "Overlapper.h"
#include "Traffic.h"

//------------------------------------------------------------------------------
Overlapper::Overlapper()
{

}

//------------------------------------------------------------------------------
Overlapper::~Overlapper()
{

}

//------------------------------------------------------------------------------
void Overlapper::init()
{
	mDebug = false;
}

//------------------------------------------------------------------------------
bool Overlapper::overlap_new(Vehiculo* pVH, Vehiculo* pVH_any, double dist, int& index_next)
{
	int next_vh;
	bool overlap = false;
	double delta;

	//I dont have any vehicle in front of me in the same lane
	if (pVH->mVhFollowing == -1)
	{
		//about to cross the node
		delta = dist + pVH->mQuad_size - pVH->mPtrNodeInfoNow->dist;
		if (delta > 0)
		{
			next_vh = pVH->mpNO[(pVH->mPtrNodeInfoNow + 1)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first;
			//there is a vehicle in that node/lane
			if (next_vh != -1)
			{
				if ((pVH_any[next_vh].mNd2nd - delta) < 0) 
				{
					index_next = next_vh;
					overlap = true;
					//if (mDebug) cout << "Index=[" << pVH->mIndex << "] index_next:[" << index_next << "] overlap_new 1  pVH->NODO: [" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
				}
			}
		}
	}
	//There is a vehicle in front of me in the same lane/node
	else
	{
		if ((pVH_any[pVH->mVhFollowing].mNd2nd - dist) < (pVH->mQuad_size)) 
		{
			index_next = pVH->mVhFollowing;
			//if (mDebug) cout << "Index=[" << pVH->mIndex << "] index_next:[" << index_next << "] overlap_new 2 " << endl;
			overlap =  true;
		}
	}

	//if (mDebug) cout << "overlap_new: Index=[" << pVH->mIndex << "] pVH->mVhFollowing:[" << pVH->mVhFollowing << "] pVH->mVhBehind:[" << pVH->mVhBehind << "]  pVH->NODO:[" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
	
	return overlap;
}

//------------------------------------------------------------------------------
bool Overlapper::overlap_lane_new(Vehiculo* pVH, Vehiculo* pVH_any, double dist, int& vh_inf, int& vh_sup)
{
	int next_vh;
	bool overlap;
	double delta;

	overlap = false;
	vh_inf = -1;
	vh_sup = -1;

	//Is there any vehicle in the new lane?
	//if (mDebug)
	//{
	//	cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
	//	cout << "Index=[" << pVH->mIndex << "]  mLaneNew [" << pVH->mLaneNew << "] node_vh_order[" << (pVH->mPtrNodeInfoNow)->node_vh_order << "]" << endl;
	//	cout << "cant lane=[" << pVH->mpNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant << "]" << endl;
	//	cout << "vh first=[" << pVH->mpNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first << "]" << endl;
	//}


	if (pVH->mpNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant > 0)
	{
		if (mDebug) cout << "Index=[" << pVH->mIndex << "]  mLaneNew [" << pVH->mLaneNew << " overlap_lane_new 1 cant:[" << pVH->mpNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant << "] " << endl;
		//from the first to the end
		next_vh = pVH->mpNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first;
		if (next_vh != -1)
		{

			for (int i = 0; i < pVH->mpNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNew].m_cant; i++)
			{
				//if (mDebug) cout << "next_vh=[" << next_vh << "]" << endl;
				//I get one behind me
				if ((dist - pVH->mQuad_size) > pVH_any[next_vh].mNd2nd)
				{
					vh_inf = next_vh;
					//if (mDebug) cout << "Index=[" << pVH->mIndex << "] vh_inf:[" << vh_inf << "] overlap_lane_new 1  pVH->NODO:[" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
				}
				else
				{
					//I get the one in front of me
					if ((dist + pVH->mQuad_size) <= pVH_any[next_vh].mNd2nd)
					{
						vh_sup = next_vh;
						//if (mDebug) cout << "Index=[" << pVH->mIndex << "] vh_sup:[" << vh_sup << "] overlap_lane_new 2  pVH->NODO:[" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
						break;
					}
					else
					{
						//If I'm in the middle of some vehicle area (inf or sup), I'm overlapping
						//so there is no importance about the inf or the sup information
						//if (mDebug) cout << "Index=[" << pVH->mIndex << "] Overlap 3 " << endl;
						//if (mDebug) cout << "overlap_lane_new: Index=[" << pVH->mIndex << "] pVH->mVhFollowing:[" << pVH->mVhFollowing << "] pVH->mVhBehind:[" << pVH->mVhBehind << "]  pVH->NODO:[" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
						return true;
					}
				}

				next_vh = pVH_any[next_vh].mVhFollowing;
				if (next_vh == -1) break;
			}
		}

	}
	
	//There is no vehicle in front of me in the same node (new lane)
	if(vh_sup == -1 )
	{
		//if (mDebug) cout << "Index=[" << pVH->mIndex << "]  vh_sup=-1" << endl;

		//about to cross the node
		delta = (dist + pVH->mQuad_size) - pVH->mPtrNodeInfoNow->dist;
		if (delta > 0)
		{
			//if (mDebug) cout << "delta > 0" << endl;
			next_vh = pVH->mpNO[(pVH->mPtrNodeInfoNow + 1)->node_vh_order].laneorder[pVH->mLaneNew].m_vh_first;
			//is there a vehicle in that node/lane?
			if (next_vh != -1)
			{
				//if (mDebug) cout << "next_vh != -1" << endl;
				if ((pVH_any[next_vh].mNd2nd - delta) < 0)
				{
					//if (mDebug) cout << "overlap_lane_new Index=[" << pVH->mIndex << "] Overlap 4  pVH->NODO:[" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
					overlap = true;
				}
			}
		}
	}

	//if (mDebug)
	//{
	//	cout << "overlap_lane_new: Index=[" << pVH->mIndex << "] pVH->mVhFollowing:[" << pVH->mVhFollowing << "] pVH->mVhBehind:[" << pVH->mVhBehind << "]  pVH->NODO:[" << pVH->mPtrNodeInfoNow->nid << "] " << endl;
	//	cout << "-----------------------------------------------------------------------------------------------------------------------------------" << endl;
	//}
	return overlap;
}

