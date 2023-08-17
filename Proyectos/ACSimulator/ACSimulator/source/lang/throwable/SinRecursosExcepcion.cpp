// =======================
//  FICHERO - SinRecursosExcepcion.CPP
// =======================

#include "SinRecursosExcepcion.h"


SinRecursosExcepcion::SinRecursosExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  SinRecursosExcepcion::~SinRecursosExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - SINRECURSOSEXCEPCION.CPP
// ===============================

