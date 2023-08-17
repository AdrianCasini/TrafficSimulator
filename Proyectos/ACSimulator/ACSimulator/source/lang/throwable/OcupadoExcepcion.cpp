// =======================
//  FICHERO - OCUPADOEXCEPCION.CPP
// =======================

#include "OcupadoExcepcion.h"


OcupadoExcepcion::OcupadoExcepcion
       (const UWordType uwNumLinea,const char* uwNomFichero,
        const char* uwNomFuncion,const Cadena& mStr,
        const SDWordType sdwNumError,Excepcion* peExcepcion):
       Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  OcupadoExcepcion::~OcupadoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - OCUPADOEXCEPCION.CPP
// ===============================

