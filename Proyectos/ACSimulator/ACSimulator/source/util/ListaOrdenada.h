#ifndef LISTAORDENADA_H
#define LISTAORDENADA_H

#include "util/ListaOrdenada.h"


/**
 <PRE>
 * *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: LISTAORDENADA.H
 *
 * DESCRIPCION:
 *   Template generico para la creacion de Listas ordenadas
 *
 * FECHA CREACION: 30-03-2006
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



template < typename F, typename K, long MAX_ITEMS >
class ListaOrdenada
{
   
public:

   ListaOrdenada(void);          /** < Constructor. */
   virtual ~ListaOrdenada(void); /** < Destructor. */
   void clear(void);             /** < Borrar Lista. */
   
   /**
   * Anyadir una copia del objeto a la lista.
   * @param object objeto a anyadir.
   * @param clave objeto que se usa como clave tanto para oredenar la lista.
   * @return Si se ha realizado la operacion con exito.
   */
   bool add(F object, K clave);
   /**
   * Eliminar un objeto a la lista.
   * @param clave clave del objeto.
   * @return Si se ha realizado la operacion con exito.
   */   
   bool remove(K clave);
   /**
   * Obtener un objeto a la lista.
   * @param clave clave del objeto.
   * @return Puntero del objeto de la lista.
   */
   F * get(K clave);
   /**
   * Obtener el numero de objetos de la lista.
   * @return Numero de objetos de la lista.
   */
   int getNumItems(void);

   /**
   * Obtener un objeto a la lista por el indice del orden.
   * @param index_item indice del objeto en la lista. [ 0 a getNumItems()-1]
   * @return Puntero del objeto de la lista.
   */
   F * getByIndex(SDWordType index_item);
   /**
   * Eliminar un objeto a la lista por el indice del orden.
   * @param index_item indice del objeto en la lista. [ 0 a getNumItems()-1]
   * @return Si se ha realizado la operacion con exito.
   */ 
   bool removeByIndex(SDWordType index_item);


protected:

   SDWordType getIndex(K clave);
   virtual bool findIndex(K clave,SDWordType &index);


   typedef struct
   {
      K        clave;
      F        object;      
   }
   _listitem;

   SDWordType  sdwTag;

   DLList< _listitem , MAX_ITEMS > dllist;

   HITEM       indice[MAX_ITEMS];

};



template < typename F, typename K, long MAX_ITEMS >
int ListaOrdenada < F, K, MAX_ITEMS >::getNumItems(void)
{
   return (dllist.getNumItems());
}

template < typename F, typename K, long MAX_ITEMS >
bool ListaOrdenada < F, K, MAX_ITEMS >::findIndex(K clave,SDWordType &index)
{

   dllist.lock();
   for(index=0;index<dllist.getNumItems();index++) {
      if (dllist.getObject(indice[index])->clave == clave){
         dllist.unlock();
         return true;
      } 
      if (dllist.getObject(indice[index])->clave > clave) {
         break;
      }
   }
   dllist.unlock();
   return false;
}

template < typename F, typename K, long MAX_ITEMS >
bool ListaOrdenada < F, K, MAX_ITEMS >::add(F object, K clave)
{
   _listitem listitem;
   SDWordType i;
   SDWordType j;
   bool exit;
   
   if (dllist.getNumItems()>=MAX_ITEMS) return false;
   
   dllist.lock();
   
   if (!findIndex(clave,i)) {
      for(j=dllist.getNumItems();j>i;j--) {
         indice[j]=indice[j-1];
      }
   }
   listitem.clave=clave;
   listitem.object=object;
   indice[i]=dllist.add(listitem,sdwTag++);

   exit=(indice[i]!=NULL);

   dllist.unlock();
   return exit;
}

template < typename F, typename K, long MAX_ITEMS >
SDWordType ListaOrdenada < F, K, MAX_ITEMS >::getIndex(K clave)
{
   SDWordType i;

   dllist.lock();

   if (findIndex(clave,i)) {
      dllist.unlock();
      return(i);
   }
   dllist.unlock();
   return (-1);
}

template < typename F, typename K, long MAX_ITEMS >
F * ListaOrdenada < F, K, MAX_ITEMS >::getByIndex(SDWordType index_item)
{
   if ((index_item<0) && (index_item>=dllist.getNumItems())) return NULL;
   return(&dllist.getObject(indice[index_item])->object);
}

template < typename F, typename K, long MAX_ITEMS >
F * ListaOrdenada < F, K, MAX_ITEMS >::get(K clave)
{
   return (getByIndex(getIndex(clave)));
}

template < typename F, typename K, long MAX_ITEMS >
bool ListaOrdenada < F, K, MAX_ITEMS >::removeByIndex(SDWordType index_item)
{
   SDWordType i;

   dllist.lock();
   if ((index_item>=0) && (index_item<dllist.getNumItems())) {
      if (dllist.remove(indice[index_item])) {
         for(i=index_item;i<dllist.getNumItems();i++) {
            indice[i]=indice[i+1];
         }
         dllist.unlock();
         return true;
      }
   }
   dllist.unlock();
   return false;
}

template < typename F, typename K, long MAX_ITEMS >
bool ListaOrdenada < F, K, MAX_ITEMS >::remove(K clave)
{
   return (removeByIndex(getIndex(clave)));
}

template < typename F, typename K, long MAX_ITEMS >
ListaOrdenada < F, K, MAX_ITEMS >::ListaOrdenada(void)
{
   sdwTag=0;
   clear();
}

template < typename F, typename K, long MAX_ITEMS >
ListaOrdenada < F, K, MAX_ITEMS >::~ListaOrdenada(void)
{
   
}

template < typename F, typename K, long MAX_ITEMS >
void ListaOrdenada < F, K, MAX_ITEMS >::clear(void)
{
   dllist.clear();
}


#endif



