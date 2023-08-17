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
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"

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
class RunFuncion
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   private:
      typedef SWordType ParametroFuncion;
      typedef void (T::*FuncionTypeS)(ParametroFuncion lParam);

      struct MapaFunciones
      {
        UWordType          muwId;
        ParametroFuncion   lParam;
        FuncionTypeS       pFuncion;
      };
      static const UWordType  MAXIMA_LISTA = 1024;
      DLList<MapaFunciones,MAXIMA_LISTA> mLista;
      T              *mObjeto;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // =================   OPERACIONES   ===================
      RunFuncion();
      void addFuncion(const UWordType lmuwId,FuncionTypeS lpFuncion,ParametroFuncion lParam);
      void ejecutar(const UWordType lmuwId);
      void ejecutar(const UWordType lmuwId,ParametroFuncion lParam);
      void setObjeto(T *pObjeto);
      bool existeFuncion(const UWordType lmuwId);
      void delFuncion(const UWordType lmuwId);
      T *getObjeto();
};

template <typename T>
RunFuncion<T>::RunFuncion()
{
   mObjeto = NULL;
}

template <typename T>
void RunFuncion<T>::setObjeto(T *pObjeto)
{
   mObjeto = pObjeto;
}

template <typename T>
T *RunFuncion<T>::getObjeto()
{
   if( NULL != mObjeto )
      return(mObjeto);
    else
      throw MemoriaExcepcion (__AQUI__,"El puntero del objeto es NULL");
}

template <typename T>
void RunFuncion<T>::addFuncion(const UWordType lmuwId,FuncionTypeS lpFuncion,ParametroFuncion lParam)
{
  MapaFunciones   lFuncion;

  if(NULL == mLista.get(lmuwId) )
  {
     lFuncion.muwId = lmuwId;
     lFuncion.lParam = lParam;
     lFuncion.pFuncion = lpFuncion;
     mLista.add(lFuncion,lmuwId);
  }
  else
  {
     throw  ParametroExcepcion(__AQUI__,"Error el id ya existe");
  }
}


template <typename T>
void RunFuncion<T>::ejecutar(const UWordType lmuwId)
{
   MapaFunciones   *lpFuncion;

   lpFuncion = (MapaFunciones   *) mLista.getObject(mLista.get(lmuwId));
   if(NULL != lpFuncion )
   {
      (getObjeto()->*lpFuncion->pFuncion) (lpFuncion->lParam);
   }
   else
   {
      throw NoExisteExcepcion(__AQUI__,"No existe el elemento");
   }
}

template <typename T>
void RunFuncion<T>::ejecutar(const UWordType lmuwId,ParametroFuncion lParam)
{
   MapaFunciones   *lpFuncion;

   lpFuncion = (MapaFunciones   *) mLista.getObject(mLista.get(lmuwId));
   if(NULL != lpFuncion )
   {
      (getObjeto()->*lpFuncion->pFuncion) (lParam);
   }
   else
   {
      throw NoExisteExcepcion(__AQUI__,"No existe el elemento");
   }
}

template <typename T>
bool RunFuncion<T>::existeFuncion(const UWordType lmuwId)
{
  if(NULL == mLista.get(lmuwId) )
   return(false);
  else
   return(true);
}

template <typename T>
void RunFuncion<T>::delFuncion(const UWordType lmuwId)
{
   HITEM lItem;

   lItem = mLista.get(lmuwId);
   if(NULL != lItem)
   {
      mLista.remove(lItem);
   }
   else
   {
      throw NoExisteExcepcion(__AQUI__,"No existe el elemento");
   }
}



// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //RUNFUNCION_H
