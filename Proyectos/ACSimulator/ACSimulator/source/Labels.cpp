#include "Labels.h"

//------------------------------------------------------------------------------
Labels::Labels()
{
}

//------------------------------------------------------------------------------
Labels::~Labels()
{
}

//------------------------------------------------------------------------------
void Labels::init(Cadena cadgral)
{
	Cadena cadfile;
	mcadgral = cadgral;
	
	cadfile.formCadena(SIM_DIR_TEST_LABELS_"aucat.txt");
	cadfile = mcadgral + cadfile;

	mpData.clear();

	if (!mpData.get_data_from_txt(cadfile))
	{
		cout << "Error: There is an error obtaining information from file:[" << cadfile.getCadena() << "]" << endl;
		return;
	}	


}


