#ifndef _SOURCE_H
#define _SOURCE_H

#include "Simulator.h"
#include "FileInfo.h"

class Source
{
	public:


		//EJ:  SOURCE;MASTER_C32/aucat_s06_s16.txt;TRIP;s06_s16.txt;s06;s16;2000
		struct SimulLine
		{
			Cadena cad_source;
			Cadena cad_type;
			Cadena cad_trip;
			Cadena cad_est_in;
			Cadena cad_est_out;
			int    iteration;
		};

		struct random_line
		{
			//delta time segmentation
			int cant_time_seg;
			int time_seg[10];
			int time_seg_prctg[10];

			//speed segmentation
			int cant_speed_seg;
			int min_speed;
			int speed_seg[5];
			int speed_seg_prctg[5];
		
			//category segmentation
			int category_prctg;
			Cadena  category;

		};


		struct Line
		{
			Simveh* pveh;
			int     nmaxveh;
			int     index;
			bool	end;
			int     iteration;
			bool    db_cycle;
			std::time_t  time_following;

			Cadena  source_type;
			int     db_momento;
			int     db_estent;
			int     db_estsal;
		
			Cadena  cad_trip;
			Cadena  cad_type;
			Cadena  cad_source;

			Cadena  cad_est_in;
			Cadena  cad_est_out;

			//random -- stuff
			int cant_type_seg;
			random_line  rnd_vh[50];


		};

		FileInfo mItems;
		Line*   mpSources;
		int     mIndexSources;
		Cadena  mcadgral;

	public:

		Source();
		~Source();

		void init(Cadena cadgral);
		void reset();
		bool get_source(SimulLine* psimulline);

};

#endif

