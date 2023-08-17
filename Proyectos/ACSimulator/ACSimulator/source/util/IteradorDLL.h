#ifndef ITERADORDLL_H
#define ITERADORDLL_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../util/DLList.h"
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
   * FICHERO:   ITERADORDLL.H
   *
   * DESCRIPCION: Iterador especifico para una lista DLL
   *
   * FECHA CREACION: 12/7/2006
   *
   * AUTOR DISENO:
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

template  < typename T , long MAX_ITEMS>
class IteradorDLL
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE     ////////////////////
   private:
      DLList <T,MAX_ITEMS> *mpLista;
      HITEM mpHitem;
   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================
      IteradorDLL();
      virtual ~IteradorDLL();
      // =================   OPERACIONES   ===================
      void set (DLList <T,MAX_ITEMS> *lpLista);
      bool extraer(SDWordType sdTag,T& lObjeto);
      bool get(SDWordType sdTag,T& lObjeto);
      bool borrar(T*);
      void borrar();
      //Metodo anyadido para mantener la compatibilidad de gestionremota
      //Metodo que hace lo mismo que en la via LINUX
      void borrarAndMoveNext();
      // =================   ACCESO       ====================
      T* getPrimero();
      T* getSiguiente();
      T* get();
      // =================   CONSULTA      ===================
};

// ===============   CICLO DE VIDA    ==================
template  < typename T , long MAX_ITEMS>
IteradorDLL<  T ,  MAX_ITEMS>::IteradorDLL()
{
   mpLista = NULL;
   mpHitem = NULL;
}
template  < typename T , long MAX_ITEMS>
IteradorDLL<  T ,  MAX_ITEMS>::~IteradorDLL()
{

}
// =================   OPERACIONES   ===================
template  < typename T , long MAX_ITEMS>
void IteradorDLL<  T ,  MAX_ITEMS>::set (DLList <T,MAX_ITEMS> *lpLista)
{
   mpLista = lpLista;
   mpHitem = NULL;
}

template  < typename T , long MAX_ITEMS>
void IteradorDLL<  T ,  MAX_ITEMS>::borrar()
{
  mpLista->remove(mpHitem);
}

template  < typename T , long MAX_ITEMS>
void IteradorDLL<  T ,  MAX_ITEMS>::borrarAndMoveNext()
{
  HITEM lpHitem;

  lpHitem = mpLista->getNext(mpHitem);
  mpLista->remove(mpHitem);
  mpHitem = lpHitem;
}

template  < typename T , long MAX_ITEMS>
bool IteradorDLL<  T ,  MAX_ITEMS>::borrar(T* lObjetoEntrada)
{
 HITEM lpHitem;
 T* object;

 lpHitem = mpLista->getNext(NULL);
 object = mpLista->getObject(lpHitem);
 while ( lpHitem != NULL )
 {
  if ( object == lObjetoEntrada )
  {
    mpLista->remove(lpHitem);
    return(true);
  }
  lpHitem = mpLista->getNext(NULL);
  object = mpLista->getObject(lpHitem);
 }
 return(false);
}

template  < typename T , long MAX_ITEMS>
bool IteradorDLL<  T ,  MAX_ITEMS>::get(SDWordType sdTag,T& lObjeto)
{
  T* lpObjeto;

  lpObjeto = mpLista->getObject(sdTag);
  if ( lpObjeto != NULL )
  {
      lObjeto = *lpObjeto;
      return(true);
  }
  return(false);
}

template  < typename T , long MAX_ITEMS>
bool IteradorDLL<  T ,  MAX_ITEMS>::extraer(SDWordType sdTag,T& lObjeto)
{
  T* lpObjeto;

  lpObjeto = mpLista->getObject(sdTag);
  if ( lpObjeto != NULL )
  {
      lObjeto = *lpObjeto;
      mpLista->remove(mpLista->get(sdTag));
      return(true);
  }
  return(false);
}
// =================   ACCESO       ====================
template  < typename T , long MAX_ITEMS>
T* IteradorDLL<  T ,  MAX_ITEMS>::getPrimero()
{
   if (NULL != mpLista )
   {
      mpHitem = mpLista->getNext(NULL);
      return (mpLista->getObject(mpHitem));
   }
   else
   {
      throw MemoriaExcepcion(__AQUI__,"No esta asignada la lista");
   }
}
template  < typename T , long MAX_ITEMS>
T* IteradorDLL<  T ,  MAX_ITEMS>::getSiguiente()
{
   if (NULL != mpLista )
   {
      mpHitem = mpLista->getNext(mpHitem);
      return (mpLista->getObject(mpHitem));
   }
   else
   {
      throw MemoriaExcepcion(__AQUI__,"No esta asignada la lista");
   }
}

template  < typename T , long MAX_ITEMS>
T* IteradorDLL<  T ,  MAX_ITEMS>::get()
{
   if (NULL != mpLista )
   {
      return (mpLista->getObject(mpHitem));
   }
   else
   {
      throw MemoriaExcepcion(__AQUI__,"No esta asignada la lista");
   }
}

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

