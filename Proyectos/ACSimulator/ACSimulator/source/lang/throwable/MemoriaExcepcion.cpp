// =======================
//  FICHERO - MemoriaExcepcion.CPP
// =======================

#include "MemoriaExcepcion.h"


MemoriaExcepcion::MemoriaExcepcion
      (const UWordType uwNumLinea,const char* uwNomFichero,
       const char* uwNomFuncion,const Cadena& mStr,
       const SDWordType sdwNumError,Excepcion* peExcepcion):
      Excepcion (uwNumLinea,uwNomFichero,uwNomFuncion,mStr,sdwNumError,peExcepcion)

  {

  }

  MemoriaExcepcion::~MemoriaExcepcion()
  {

  }


// ===============================
//  FIN DEL FICHERO - MEMORIAEXCEPCION.CPP
// ===============================

