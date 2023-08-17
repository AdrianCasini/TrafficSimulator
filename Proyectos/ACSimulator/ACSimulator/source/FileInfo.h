#ifndef _FILEINFO_H
#define _FILEINFO_H

#include "Simulator.h"

class FileInfo
{
	public:


		struct SItem
		{
			Cadena         str;
			double         num_d;
			long long int  num_li;
			int            num_i;
		};

		struct SFmt
		{
			Cadena  name;
			Cadena  fmt;
		};

	
		struct SReg
		{
			map<Cadena, SItem> reg;
		};

		Cadena   mType;
		map<int, SFmt> mFmt;
		map<int, SReg> mRegtable;
		int      mMaxreg;
		

	public:

		FileInfo();
		~FileInfo();
		bool get_data_from_txt(Cadena cadfile);
		void clear();

};

#endif

