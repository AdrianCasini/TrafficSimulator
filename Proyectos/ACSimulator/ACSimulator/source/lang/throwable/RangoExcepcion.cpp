// =======================
//  FICHERO - RangoExcepcion.CPP
// =======================

#include "RangoExcepcion.h"


RangoExcepcion::RangoExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  RangoExcepcion::~RangoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - RANGOEXCEPCION.CPP
// ===============================

