// =======================
//  FICHERO - SLLIST.H
// =======================

#ifndef SLLIST_H
   #define SLLIST_H

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

/** Handle de un item de lista */
typedef void* HITEM;


/**
 <PRE>
 *
 * @param F clase base de la lista enlazada
 * @param MAX_ITEMS maximo numero de items de la lista enlazada
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: SLLIST.H
 *
 * DESCRIPCION:
 *   Template generico para la creacion de Listas Enlazadas
 *   de distintos objetos y tamanyos. Basado en el template de DLList
 *
 * FECHA CREACION: 12-12-2005
 *
 * AUTOR DISENYO:
 *
 * PROGRAMADOR: Marc Coll
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
class SLList : public Mutex
{
   // ATRIBUTOS
   // =====================================================

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////

   public:

      // =================   OPERACIONES   ===================

      /** Constructor */
      SLList();

      /** Destructor */
      ~SLList();

      /** Borrar lista */
      void clear();

      /**
      Anyadir objeto a la lista
         @param object Objeto de la clase F a anyadir
         @param tag Identificador dado por el usuario
         @return Si se ha realizado con exito
      */
      void add(const F& object, SDWordType sdTag);

      /**
         Obtiene el handle de un objeto de la lista
         @param tag Identificador del objeto
         @see getObject
         @return Handle del objeto o en caso de error NULL
      */
      HITEM get(SDWordType sdTag);

      /**
      Obtiene el handle del siguiente objeto de la lista
         @param item Identificador del objeto anterior. NULL para
         obtener el primero
         @see getObject
         @return Handle del objeto o en caso de error NULL
      */
      HITEM getNext(HITEM item);

      /**
      Elimina un objeto de la lista
         @param item Handle del objeto
         @see getObject
         @return Si se ha borrado con exito
      */
      void remove(HITEM item);

      /**
      Obtiene el numero de objetos de la lista
         @return El numero de objetos de la lista
      */
      int getNumItems();

      /**
      Obtiene el objeto a partir de su handle
         @param item Handle del objeto
         @see get
         @return Puntero al objeto. NULL en caso de error
      */
      F* getObject(HITEM item);

   private:

      struct _litem
      {
         bool       bUsed;
         SDWordType sdTag;
         F          object;
         void*      next;
      };

      SDWordType  sdNitems;
      _litem*     first;
      _litem*     last;
      _litem      listitem[MAX_ITEMS];
};


//////////////////////////////////////////////////////////////////////////////

template < typename F, long MAX_ITEMS >
SLList < F, MAX_ITEMS >::SLList()
{
   inicializar ();
   clear ();
}

template < typename F, long MAX_ITEMS >
SLList < F, MAX_ITEMS >::~SLList()
{
   clear ();
   destruir ();
}

template < typename F, long MAX_ITEMS >
int SLList < F, MAX_ITEMS >::getNumItems()
{
   return sdNitems;
}

template < typename F, long MAX_ITEMS >
void SLList < F, MAX_ITEMS >::add(const F& object, SDWordType sdTag)
{
   _litem*    newitem = NULL;
   SDWordType sdI;

   try
   {
      lock ();
      if (sdNitems < MAX_ITEMS)
      {
         if (sdNitems == 0)
         {
            newitem = &listitem[0];
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
            newitem->next = NULL;

            if (last != newitem)
               last->next = newitem;
            last = newitem;

            sdNitems++;
            unlock ();
         }
      }
      else
      {
         throw LlenoExcepcion(__AQUI__,"NO se puede agregar datos, esta lleno");
      }
   }
   catch (Excepcion &e)
   {
      unlock ();
      throw;
   }
}

template < typename F, long MAX_ITEMS >
HITEM SLList < F, MAX_ITEMS >::get(SDWordType sdTag)
{
   _litem* item;

   try
   {
      lock ();
      item = first;
      while ((item != NULL) && (item->bUsed))
      {
         if (item->sdTag == sdTag)
         {
            unlock ();
            return item;
         }
         item = (_litem*)item->next;
      }
      unlock ();
      return NULL;
   }
   catch (Excepcion &e)
   {
      unlock ();
      throw;
   }
}

template < typename F, long MAX_ITEMS >
HITEM SLList < F, MAX_ITEMS >::getNext(HITEM lastitem)
{
   _litem* current;

   try
   {
      lock ();
      if (lastitem == NULL)
      {
         if (first != NULL)
         {
            unlock ();
            return first;
         }
         else
         {
            unlock ();
            return NULL;
         }
      }
      current = (_litem*)lastitem;
      current = (_litem*)current->next;
      if ((current != NULL) && (current->bUsed))
      {
         unlock ();
         return current;
      }
      unlock ();
      return NULL;
   }
   catch(Excepcion &e)
   {
      unlock ();
      throw;
   }
}

template < typename F, long MAX_ITEMS >
F* SLList < F, MAX_ITEMS >::getObject(HITEM item)
{
   if (item == NULL)
      return NULL;
   _litem* current = (_litem*)item;

   return &current->object;
}

template < typename F, long MAX_ITEMS >
void SLList < F, MAX_ITEMS >::remove(HITEM current)
{
   _litem* item = (_litem*)current;
   _litem* next;
   _litem* prev;

   try
   {
      lock ();
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
            HITEM it;

            it = getNext (NULL);
            prev = (_litem*)it;
            while (it != NULL && it != current)
            {
               prev = (_litem*)it;
               it = getNext (it);
            }
            next = (_litem*)item->next;

            if (first == item)
               first = next;
            else
               prev->next = item->next;

            if (last == item)
               last = prev;
         }
         sdNitems--;
         unlock ();
      }
      else
      {
         unlock ();
      }
   }
   catch(Excepcion &e)
   {
      unlock ();
      throw;
   }
}

template < typename F, long MAX_ITEMS >
void SLList < F, MAX_ITEMS >::clear()
{
   SDWordType i;

   try
   {
      lock ();
      sdNitems = 0;
      first = NULL;
      last = NULL;
      for(i=0;i<MAX_ITEMS;i++) {
         listitem[i].bUsed=false;
      }
      unlock ();
   }
   catch(Excepcion &e)
   {
      unlock ();
      throw;
   }
}


#endif //SLLIST_H

// ===========================
//  FIN DEL FICHERO - SLLIST.H
// ===========================
