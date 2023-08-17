// =======================
//  FICHERO - DesconocidoExcepcion.CPP
// =======================

#include "DesconocidoExcepcion.h"


  DesconocidoExcepcion::DesconocidoExcepcion
        (const UWordType uwNumLinea,const char* uwNomFichero,
         const char* uwNomFuncion,const Cadena& mStr,
         const SDWordType sdwNumError,Excepcion* peExcepcion):
        Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)
  {

  }

  DesconocidoExcepcion::~DesconocidoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - DESCONOCIDOEXCEPCION.CPP
// ===============================

