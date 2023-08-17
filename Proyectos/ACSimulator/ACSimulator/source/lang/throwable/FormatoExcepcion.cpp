// =======================
//  FICHERO - FormatoExcepcion.CPP
// =======================

#include "FormatoExcepcion.h"


  FormatoExcepcion::FormatoExcepcion
        (const UWordType uwNumLinea,const char* uwNomFichero,
         const char* uwNomFuncion,const Cadena& mStr,
         const SDWordType sdwNumError,Excepcion* peExcepcion):
        Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)
  {

  }

  FormatoExcepcion::~FormatoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - FORMATOEXCEPCION.CPP
// ===============================

