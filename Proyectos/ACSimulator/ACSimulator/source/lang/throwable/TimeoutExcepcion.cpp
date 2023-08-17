// =======================
//  FICHERO - TimeoutExcepcion.CPP
// =======================

#include "TimeoutExcepcion.h"


TimeoutExcepcion::TimeoutExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  TimeoutExcepcion::~TimeoutExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - TIMEOUTEXCEPCION.CPP
// ===============================

