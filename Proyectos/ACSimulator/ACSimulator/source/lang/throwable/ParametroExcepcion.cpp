// =======================
//  FICHERO - PARAMETROEXCEPCION.CPP
// =======================

#include "ParametroExcepcion.h"


ParametroExcepcion::ParametroExcepcion
       (const UWordType uwNumLinea,const char* uwNomFichero,
        const char* uwNomFuncion,const Cadena& mStr,
        const SDWordType sdwNumError,Excepcion* peExcepcion):
       Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  ParametroExcepcion::~ParametroExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - PARAMETROEXCEPCION.CPP
// ===============================

