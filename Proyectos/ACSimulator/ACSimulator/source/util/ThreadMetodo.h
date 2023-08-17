// =======================
//  FICHERO - THREADMETODO.H
// =======================

#ifndef THREADMETODO_H
#define THREADMETODO_H

#include <iostream>

#include "lang/Types.h"
#include "thread/Thread.h"

/**
 * <PRE>
 * ThreadMetodo
 * @param T Classe del metodo.
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: ThreadMetodo.H
 *
 * DESCRIPCION:
 *   Template generico para crear thread con metodos de objetos de cualquier clase.
 *
 * FECHA CREACION: 27-06-2005
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

template <typename T>
class ThreadMetodo : public Thread {
public:

   typedef void (T:: *tarea_metodo)(void);                  /**< Metodo del objeto tipo T usado como thread. */
   typedef void (T:: *tarea_metodo_param)(void *param);     /**< Metodo con parametro del objeto tipo T usado como thread. */
   
   ThreadMetodo(void);                             /**< Constructor. */
   virtual ~ThreadMetodo(void);                   /**< Destructor. */
/**
 * Especifica el objeto y metodo a ejecutar como thread.
 * @param t Objeto de la clase T.
 * @param tarea metodo del objeto t a ejecutar como thread.
 * @return Nada.
*/
   void     setMetodo(T *t,tarea_metodo tarea);
/**
 * Especifica el objeto y metodo a ejecutar como thread.
 * @param t Objeto de la clase T.
 * @param tarea metodo del objeto t a ejecutar como thread.
 * @param param parametro del metodo a ejecutar.
 * @return Nada.
*/
   void     setMetodo(T *t,tarea_metodo_param tarea_param,void *param);
   
private:
   T                    *out;             /**< puntero al objeto. */
   tarea_metodo         metodo;           /**< metodo del objeto. */
   tarea_metodo_param   metodo_param;     /**< metodo del objeto con parametro. */
   void                 *parametro;       /**< parametro del metodo */
   
   void     tarea(void);            /**< Implementacion de Thread.*/
};



template <typename T>
ThreadMetodo<T>::ThreadMetodo(void) : Thread() {
   out=NULL;
   metodo=NULL;
   metodo_param=NULL;
}

template <typename T>
ThreadMetodo<T>::~ThreadMetodo(void) {
}

template <typename T>
void ThreadMetodo<T>::setMetodo(T *t,tarea_metodo tarea) {
   out=t;
   metodo=tarea;
   metodo_param=NULL;
}

template <typename T>
void ThreadMetodo<T>::setMetodo(T *t,tarea_metodo_param tarea_param,void *param) {
   out=t;
   metodo_param=tarea_param;
   parametro=param;
   metodo=NULL;
}


template <typename T>
void ThreadMetodo<T>::tarea(void) {
   if ((metodo!=NULL) && (out!=NULL)){
      (out->*metodo)();
   }
   if ((metodo_param!=NULL) && (out!=NULL)){
      (out->*metodo_param)(parametro);
   }
}


#endif
