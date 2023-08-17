//
// C++ Interface: MonitorException
//
// Description:
//
//
// Author: Ruben abad prados <rabadpra@bcnadese0003>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MONITOREXCEPTON_H
#define MONITOREXCEPTON_H
#include "../Cadena.h"
#include "../throwable/Excepcion.h"

class MonitorException: public Excepcion
{
public:
   MonitorException(const UWordType uwNumLinea,const char* uwNomFichero,
                  const char* uwNomFuncion,const Cadena& mStr,
                  const SDWordType sdwNumError = -1,Excepcion* peExcepcion = NULL);
   virtual ~MonitorException();
};



#endif
