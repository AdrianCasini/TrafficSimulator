// =======================
//  FICHERO - InterrupcionExcepcion.CPP
// =======================

#include "InterrupcionExcepcion.h"


    InterrupcionExcepcion::InterrupcionExcepcion
       (const UWordType uwNumLinea,const char* uwNomFichero,
        const char* uwNomFuncion,const Cadena& mStr,
        const SDWordType sdwNumError,Excepcion* peExcepcion):
       Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  InterrupcionExcepcion::~InterrupcionExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - INTERRUPCIONEXCEPCION.CPP
// ===============================

