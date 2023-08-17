// =======================
//  FICHERO - CorrompidoExcepcion.CPP
// =======================

#include "CorrompidoExcepcion.h"


  CorrompidoExcepcion::CorrompidoExcepcion
        (const UWordType uwNumLinea,const char* uwNomFichero,
         const char* uwNomFuncion,const Cadena& mStr,
         const SDWordType sdwNumError,Excepcion* peExcepcion):
        Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)
  {

  }

  CorrompidoExcepcion::~CorrompidoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - CORROMPIDOEXCEPCION.CPP
// ===============================

