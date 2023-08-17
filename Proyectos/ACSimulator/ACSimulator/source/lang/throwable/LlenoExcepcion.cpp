// =======================
//  FICHERO - LlenoExcepcion.CPP
// =======================

#include "LlenoExcepcion.h"


LlenoExcepcion::LlenoExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  LlenoExcepcion::~LlenoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - LLENOEXCEPCION.CPP
// ===============================

