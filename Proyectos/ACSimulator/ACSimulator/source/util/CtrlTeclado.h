// =======================
//  FICHERO - RunFuncion.H
// =======================

#ifndef RUNFUNCION_H
#define RUNFUNCION_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../lang/Cadena.h"
#include "DLList.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: CTRLTECLADO.H
   *
   * DESCRIPCION: Asignacion de teclas sobre funciones
   *
   * FECHA CREACION: 10/11/2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Oscar Lopez Calvo
   *
   * MODIFICATIONES:
   *
   *       FECHA       AUTOR     RAZON
   *    ---------------------------------------------------------------------
   *     DD-MM-YYYY
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */

template <typename T>
class CtrlTeclado
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   private:
      static const UWordType  MAXIMA_LISTA = 1024;
      typedef struct MapaFunciones
      {
        UWordType   muwId;
        void (T::*pFuncion)();
      };
      DLList<MapaFunciones,MAXIMA_LISTA> mLista;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // =================   OPERACIONES   ===================
//      void asignaTecla(const Cadena& lTextoTecla,const UWordType luwCodigo);
//      void asignaFuncion(const Cadena& lTextoFuncion,void (T::*pFuncion)());
      void addFuncion(const UWordType lmuwId,void (T::*lpFuncion)());
      void runTecla(const UWordType lmuwId);
};

template <typename T>
void CtrlTeclado<T>::addFuncion(const UWordType lmuwId,void (T::*lpFuncion)())
{
  MapaFunciones   lFuncion;

  if(NULL == mLista.get(muwId) )
  {
     lFuncion.muwId = lmuwId;
     lFuncion.pFuncion = lpFuncion;
     mLista.add(lFuncion);
  }
  else
  {
     throw  ParametroExcepcion(__AQUI__,"Error el id ya existe");
  }
}


template <typename T>
void CtrlTeclado<T>::runTecla(const UWordType lmuwId)
{
   MapaFunciones   *lpFuncion;

   lpFuncion = mLista.get(muwId);
   if(NULL != lpFuncion )
   {
      lpFuncion.pFuncion();
   }
   else
   {
      NoExisteExcepcion(__AQUI__,"No existe el elemento");
   }
}

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //CTRLTECLADO_H
