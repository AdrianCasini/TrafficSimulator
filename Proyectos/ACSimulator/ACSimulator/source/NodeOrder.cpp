#include "NodeOrder.h"

//------------------------------------------------------------------------------
NodeOrder::~NodeOrder()
{

}

//------------------------------------------------------------------------------
NodeOrder::NodeOrder()
{
	m_nid=-1;

	for (int i = 0; i < 10; i++)
	{
		laneorder[i].m_vh_first = - 1;
		laneorder[i].m_cant = 0;
	}

}
