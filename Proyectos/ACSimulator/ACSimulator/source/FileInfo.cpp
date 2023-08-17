#include "FileInfo.h"

//------------------------------------------------------------------------------
FileInfo::FileInfo()
{
	clear();
}

//------------------------------------------------------------------------------
FileInfo::~FileInfo()
{

}

//------------------------------------------------------------------------------
void FileInfo::clear()
{
	mFmt.clear();
	mRegtable.clear();
	mMaxreg = 0;
}

//------------------------------------------------------------------------------
bool FileInfo::get_data_from_txt(Cadena cadfile)
{

	string line, substr;
	vector<Cadena> result;
	vector<Cadena>::const_iterator it;
	Cadena cad;
	int cont_fmtitem = 0, cont_reg = 0;
	bool btype   = false;
	bool bformat = false;
	bool debug   = false;

	ifstream f(cadfile.getCadena());
	if (!f.is_open())
	{
		if (debug) cout << "get_data_from_txt: Error opening file:[" << cadfile.getCadena() << "]" << endl;
		return false;
	}

	if (debug) cout << "File:" << cadfile.getCadena() << endl;

	while (getline(f, line))
	{
		if (debug) cout << "================================================================================" << endl;
		if ((line[0] != '#') && (strcmp(line.c_str(), "") != 0))
		{
			stringstream ss(line);
			result.clear();
			while (ss.good())
			{
				getline(ss, substr, ';');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();

				if (debug) cout << "cadena:[" << cad.getCadena() << "]" << endl;
				result.push_back(cad);
			}

			if (result[0] == "TYPE")
			{
				mType = result[1];
				if (debug) cout << "maptxt.type:" << mType.getCadena() << endl;
				btype = true;
			}
			else if ((result[0] == "FORMAT") && (btype))
			{
				for (int i = 1; i < result.size(); i++)
				{
					mFmt[cont_fmtitem].name = result[i + 1];
					mFmt[cont_fmtitem].fmt = result[i];
					i++;
					cont_fmtitem++;
				}
				if (debug) cout << "FORMAT  ITEMS:[" << cont_fmtitem << "]" << endl;
				bformat = true;
			}
			else if (bformat == true)
			{
				if (debug) cout << endl;
				if (debug) cout << "================================================================================" << endl;
				if (debug) cout << "reg: [" << cont_reg << "]" << endl;
				if (cont_fmtitem == result.size())
				{
					for (int i = 0; i < cont_fmtitem; i++)
					{


						if (mFmt[i].fmt == "STR")
						{
							mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].str = result[i].c_str();
							if (debug) cout << "STR " << mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].str.getCadena() << " " << endl;
						}
						else if (mFmt[i].fmt == "DD")
						{
							mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].num_d = double(atof(result[i].c_str()));
							if (debug) cout << "DD " << mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].num_d << "  " << endl;
						}
						else if (mFmt[i].fmt == "INT")
						{
							mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].num_i = atoi(result[i].c_str());
							if (debug) cout << "INT " << mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].num_i << "  " << endl;
						}
						else if (mFmt[i].fmt == "LI")
						{
							mRegtable[cont_reg].reg[mFmt[i].name.getCadena()].num_li = atoll(result[i].c_str());
						}
					}
				}
				else
				{
					cout << "get_data_from_txt:Error processing file: Number of format items diferent from number of register items" << endl;
					cout << cont_fmtitem << " <> " << result.size() << endl;
					return false;
				}

				cont_reg++;

			}
		}
	}
	mMaxreg = cont_reg;


	if (btype && bformat)
	{
		return true;
	}



	return false;
}


