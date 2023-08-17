#ifndef _LABELS_H
#define _LABELS_H

#include "Simulator.h"
#include "FileInfo.h"

class Labels
{
	public:

		FileInfo mpData;
		Cadena mcadgral;

	public:

		Labels();
		~Labels();
		void init(Cadena cadgral);

};

#endif

