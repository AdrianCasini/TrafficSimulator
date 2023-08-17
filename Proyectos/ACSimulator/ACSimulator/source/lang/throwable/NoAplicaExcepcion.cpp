// =======================
//  FICHERO - NoAplicaExcepcion.CPP
// =======================

#include "NoAplicaExcepcion.h"


NoAplicaExcepcion::NoAplicaExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  NoAplicaExcepcion::~NoAplicaExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - NOAPLICAEXCEPCION.CPP
// ===============================

