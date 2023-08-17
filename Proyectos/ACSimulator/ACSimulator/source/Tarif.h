#ifndef _TARIF_H
#define _TARIF_H

#include "Simulator.h"
#include "Toller.h"
#include "Trace.h"


class Tarif
{
	public:

		Toller* mTLL;
		Trace* mTC;		
		double*   mbuffer_trip;

		bool mTarif;
		Cadena mText_km;
		Cadena mText_eu;

	public:

		Tarif();
		~Tarif();


		void init(Toller* tll, Trace* tc);
		void calculate();
		void clear();

};

#endif

