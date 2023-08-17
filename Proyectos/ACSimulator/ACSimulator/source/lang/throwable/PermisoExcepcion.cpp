// =======================
//  FICHERO - PERMISOEXCEPCION.CPP
// =======================

#include "PermisoExcepcion.h"


PermisoExcepcion::PermisoExcepcion
       (const UWordType uwNumLinea,const char* uwNomFichero,
        const char* uwNomFuncion,const Cadena& mStr,
        const SDWordType sdwNumError,Excepcion* peExcepcion):
       Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  PermisoExcepcion::~PermisoExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - PERMISOEXCEPCION.CPP
// ===============================

