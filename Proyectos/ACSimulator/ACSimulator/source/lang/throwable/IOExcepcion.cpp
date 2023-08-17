// =======================
//  FICHERO - IOExcepcion.CPP
// =======================

#include "IOExcepcion.h"


    IOExcepcion::IOExcepcion
       (const UWordType uwNumLinea,const char* uwNomFichero,
        const char* uwNomFuncion,const Cadena& mStr,
        const SDWordType sdwNumError,Excepcion* peExcepcion):
       Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  IOExcepcion::~IOExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - IOEXCEPCION.CPP
// ===============================

