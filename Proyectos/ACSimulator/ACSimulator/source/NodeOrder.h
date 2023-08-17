#ifndef _NODEORDER_H
#define _NODEORDER_H

#include "Simulator.h"

class NodeOrder
{
	public:

		struct lane_order
		{
			int m_vh_first;
			int m_cant;
		};
		

		long long int m_nid;

		lane_order laneorder[10];

		


	public:

		NodeOrder();
		~NodeOrder();



};

#endif

