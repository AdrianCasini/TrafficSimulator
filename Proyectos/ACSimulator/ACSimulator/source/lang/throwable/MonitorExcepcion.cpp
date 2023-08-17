//
// C++ Implementation: MonitorExcepcion
//
// Description:
//
//
// Author: Ruben abad prados <rabadpra@bcnadese0003>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MonitorExcepcion.h"


  MonitorException::MonitorException
        (const UWordType uwNumLinea,const char* uwNomFichero,
         const char* uwNomFuncion,const Cadena& mStr,
         const SDWordType sdwNumError,Excepcion* peExcepcion):
        Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)
  {

  }

  MonitorException::~MonitorException()
  {

  }
