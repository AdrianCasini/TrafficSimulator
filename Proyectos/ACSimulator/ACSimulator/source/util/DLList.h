// =======================
//  FICHERO - DLLIST.H
// =======================

#ifndef DLLIST_H
   #define DLLIST_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include <iostream>

#include "../lang/Types.h"
#include "../thread/Mutex.h"
#include "../lang/throwable/LlenoExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/** < Handle de un Item de Lista. */
typedef void * HITEM;


/**
 <PRE>
 *
 * @param F classe base de la lista doble enlazada
 * @param MAX_ITEMS maximo numero de items de la lista doble enlazada
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: DLLIST.H
 *
 * DESCRIPCION:
 *   Template generico para la creacion de Listas Doble Enlazadas
 *   de distintos objetos y tamanos.
 *
 * FECHA CREACION: 29-06-2005
 *
 * AUTOR DISENYO:
 *
 * PROGRAMADOR: Toni Bassas Serramia
 *
 * MODIFICATIONES:
 *
 *    |   FECHA    |   AUTOR   |  RAZON                                   |
 *    ---------------------------------------------------------------------
 *    | DD-MM-YYYY |           |                                          |
 *    ---------------------------------------------------------------------
 * </PRE>
*/



template < typename F, long MAX_ITEMS >
class DLList
{
   // ATRIBUTOS
   // =====================================================

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
public:
   // =================   OPERACIONES   ===================


   DLList(void);  /** < Constructor. */
   ~DLList(void); /** < Destructor. */
   void clear(void); /** < Borrar Lista Doble Enlazada. */
   /**
       Anyadir objeto a la lista.
       IMPORTANTE: Debido a las restricciones de NO CREACION dinamica
       de memoria se anyade una COPIA del objeto a la lista.
       @param object Objeto de la clase F a anyadir.
       @param sdTag Identificador dado por el usuario.
       @return Handle del objeto o en caso de error NULL.
   */
   HITEM add(F object, SDWordType sdTag);
   /**
       Obtiene el handle de un objeto de la lista.
       @param sdTag Identificador del objeto.
       @see getObject
       @return Handle del objeto o en caso de error NULL.
   */
   HITEM get(SDWordType sdTag);
   /**
       Obtiene el handle del siguiente objeto de la lista.
       @param item Identificador del objeto anterior. NULL para
    obtener el primero.
       @see getObject
       @return Handle del objeto o en caso de error NULL.
   */
   HITEM getNext(HITEM item);
   /**
       Elimina un objeto de la lista.
       IMPORTANTE: Debido a las restricciones de NO CREACION dinamica
       de memoria hay una COPIA del objeto en la lista que en caso de no
       ser una struct quizas necesite ser previamente destruido.
       @param item Handle del objeto.
       @see getObject
       @return Si se ha borrado con exito.
   */
   bool remove(HITEM item);
   /**
       Obtiene el numero de objetos de la lista.
       @return El numero de objetos de la lista.
   */
   int getNumItems(void) const;
   /**
       Obtiene el objeto a partir de su handle.
       @param item Handle del objeto.
       @see get
       @return Puntero al objeto. NULL en caso de error
   */
   F * getObject(HITEM item);
   /**
       Obtiene el objeto a partir de su identificador.
       @param sdTag Identificador del objeto.
       @see get
       @return Puntero al objeto. NULL en caso de error
   */
   F * getObject(SDWordType sdTag);

   void inicializar(void);
   void lock(void);
   void unlock(void);
   bool tryLock(void);

private:

   typedef struct
   {
      bool bUsed;
      SDWordType sdTag;
      F object;            /**Objeto de la lista. IMPORTANTE: no puede ser un puntero debido a las restricciones de creacion de memoria*/
      void * prev;
      void * next;
   }
   _dlitem;

   SDWordType sdNitems;
   SDWordType sdCounter;
   _dlitem * first;
   _dlitem * last;
   _dlitem listitem[MAX_ITEMS];

   Mutex mutex;

   bool  bInit;

   void checkInit(void);
};



template < typename F, long MAX_ITEMS >
int DLList < F, MAX_ITEMS >::getNumItems(void) const
{
   return (sdNitems);
}

template < typename F, long MAX_ITEMS >
HITEM DLList < F, MAX_ITEMS >::add(F object, SDWordType sdTag)
{
   _dlitem * newitem = NULL;
   SDWordType sdI;

   checkInit();
   try
   {
      mutex.lock();
      if (sdNitems < MAX_ITEMS)
      {
         if (sdNitems == 0)
         {
            newitem = & listitem[0];
            first = newitem;
            last = newitem;
         }
         else
         {
            for (sdI = 0; sdI < MAX_ITEMS; sdI++)
            {
               if (!listitem[sdI].bUsed)
               {
                  newitem = & listitem[sdI];
                  break;
               }
            }
         }
         if (newitem != NULL)
         {
            newitem->sdTag = sdTag;
            newitem->bUsed = true;
            newitem->object=object;
            newitem->prev = last;
            newitem->next = NULL;

            if (last != newitem)
            {
               last->next = newitem;
            }
            last = newitem;

            sdNitems++;
            mutex.unlock();
         }
      }
      else
      {
         //unlock();
         throw LlenoExcepcion(__AQUI__,"NO se puede agregar datos, esta lleno");
      }
   }
   catch (Excepcion &e)
   {
      mutex.unlock();
      throw;
   }
   return newitem;
}

template < typename F, long MAX_ITEMS >
HITEM DLList < F, MAX_ITEMS >::get(SDWordType sdTag)
{
   _dlitem * item;

   checkInit();
   try
   {
      mutex.lock();
      item = first;
      while ((item != NULL) && (item->bUsed))
      {
         if (item->sdTag == sdTag)
         {
            mutex.unlock();
            return (item);
         }
         item = (_dlitem *) item->next;
      }
      mutex.unlock();
      return(NULL);
   }
   catch (Excepcion &e)
   {
      mutex.unlock();
      throw;
   }

}

template < typename F, long MAX_ITEMS >
HITEM DLList < F, MAX_ITEMS >::getNext(HITEM lastitem)
{
   _dlitem * current;

   checkInit();
   try
   {
      mutex.lock();
      if (lastitem == NULL)
      {
         if (first != NULL)
         {
            mutex.unlock();
            return (first);
         }
         else
         {
            mutex.unlock();
            return (NULL);
         }
      }
      current = (_dlitem *) lastitem;
      current = (_dlitem *) current->next;
      if ((current != NULL) && (current->bUsed))
      {
         mutex.unlock();
         return (current);
      }
      mutex.unlock();
      return NULL;
   }
   catch(Excepcion &e)
   {
      mutex.unlock();
      throw;
   }
}

template < typename F, long MAX_ITEMS >
F * DLList < F, MAX_ITEMS >::getObject(HITEM item)
{
   checkInit();
   if (item == NULL)
   {
      return (NULL);
   }
   _dlitem * current = (_dlitem *) item;

   return (& current->object);
}

template < typename F, long MAX_ITEMS >
F * DLList < F, MAX_ITEMS >::getObject(SDWordType sdTag)
{
   checkInit();
   return(getObject(get(sdTag)));
}

template < typename F, long MAX_ITEMS >
bool DLList < F, MAX_ITEMS >::remove(HITEM current)
{
   _dlitem * item = (_dlitem *) current;
   _dlitem * next;
   _dlitem * prev;

   checkInit();
   try
   {
      mutex.lock();
      if ((item != NULL) && (item->bUsed))
      {
         item->bUsed = false;
         if (sdNitems == 1)
         {
            first = NULL;
            last = NULL;
         }
         else
         {
            next = (_dlitem *) item->next;
            prev = (_dlitem *) item->prev;
            if (first == item)
            {
               first = next;
            }
            else
            {
               prev->next = item->next;
            }
            if (last == item)
            {
               last = prev;
            }
            else
            {
               next->prev = item->prev;
            }
         }
         sdNitems--;
         mutex.unlock();
         return true;
      }
      else
      {
         mutex.unlock();
      }
   }
   catch(Excepcion &e)
   {
      mutex.unlock();
      throw;
   }
   return false;
}

template < typename F, long MAX_ITEMS >
DLList < F, MAX_ITEMS >::DLList(void)
{
   bInit=false;
}

template < typename F, long MAX_ITEMS >
DLList < F, MAX_ITEMS >::~DLList(void)
{

}

template < typename F, long MAX_ITEMS >
void DLList < F, MAX_ITEMS >::checkInit(void) {
  if (!bInit) {
    inicializar();
  }
}

template < typename F, long MAX_ITEMS >
void DLList < F, MAX_ITEMS >::inicializar(void) {
  if (!bInit) {
    bInit=true;
    clear();
  }
}

template < typename F, long MAX_ITEMS >
void DLList < F, MAX_ITEMS >::lock(void) {
  checkInit();
  mutex.lock();
}

template < typename F, long MAX_ITEMS >
void DLList < F, MAX_ITEMS >::unlock(void) {
  checkInit();
  mutex.unlock();
}

template < typename F, long MAX_ITEMS >
bool DLList < F, MAX_ITEMS >::tryLock(void) {
  checkInit();
  return mutex.tryLock();
}

template < typename F, long MAX_ITEMS >
void DLList < F, MAX_ITEMS >::clear(void)
{
   SDWordType i;

   checkInit();
   try
   {
      mutex.lock();
      sdNitems = 0;
      first = NULL;
      last = NULL;
      for(i=0;i<MAX_ITEMS;i++) {
         listitem[i].bUsed=false;
         listitem[i].next = NULL;
         listitem[i].prev = NULL;
         listitem[i].sdTag = 0;
      }
      mutex.unlock();
   }
   catch(Excepcion &e)
   {
      mutex.unlock();
      throw;
   }
}


#endif //DLLIST_H

// ===========================
//  FIN DEL FICHERO - DLLIST.H
// ===========================



