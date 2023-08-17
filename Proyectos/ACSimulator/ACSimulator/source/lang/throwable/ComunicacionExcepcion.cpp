
// =======================
//  FICHERO - ComunicacionExcepcion.cpp
// =======================

#include "ComunicacionExcepcion.h"


ComunicacionExcepcion::ComunicacionExcepcion
       (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
       Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

ComunicacionExcepcion::~ComunicacionExcepcion()
  {

  }

// ===============================
//  FIN DEL FICHERO - ComunicacionExcepcion.cpp
// ===============================
