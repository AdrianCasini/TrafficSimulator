// =======================
//  FICHERO - NoExisteExcepcion.CPP
// =======================

#include "NoExisteExcepcion.h"


NoExisteExcepcion::NoExisteExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  NoExisteExcepcion::~NoExisteExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - NOEXISTEEXCEPCION.CPP
// ===============================

