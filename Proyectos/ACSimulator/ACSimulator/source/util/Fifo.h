// =======================
//  FICHERO - FIFO.H
// =======================

#ifndef FIFO_H
#define FIFO_H

#include <iostream>

#include "../lang/Types.h"
#include "../thread/VarsCondicion.h"
#include "../thread/Mutex.h"

/**
 * <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: FIFO.H
 *
 * DESCRIPCION:
 *   Template generico para la creacion de Fifos de distintas clases
 *   y tamanyos.
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

template <typename F,UDWordType MAX_ITEMS>
class Fifo {
public:

   static const SDWordType INFINITO = 0;
   static const SDWordType NOBLOQUEANTE = -1;

   Fifo(void);                         /**< Constructor. */
   ~Fifo(void);                        /**< Destructor. */
   void  clear(void);                  /**< Borrar fifo.*/
/**
 * Anyadir objeto a la fifo.
 * @param i Objeto de la clase F a anyadir.
 * @return Devuelve si se ha realizado con exito.
*/
   bool  add(const F &i);
/**
 * Obtener objeto de la fifo.
 * @param i Objeto de la clase F a obtener.
 * @param sdwTimeout Timeout en milisegindos de bloqueo en espera del objeto.
 * En case de ser igual a 0 el bloqueo es infinito. @see INFINITO
 * En caso de ser negativo no hay bloqueo. @see NOBLOQUEANTE
 * @param remove Eliminar de la fifo una vez obtenido el objeto.
 * @return Devuelve si se ha realizado con exito.
*/
   bool  get(F &i,SDWordType sdwTimeout=INFINITO,bool remove=true);

/**
 * Chequea si la fifo esta vacia.
 *
 * NOTA: Este metodo no esta totalmente protegido por razones de eficiencia
 *       y, de vez en cuando, puede devolver valores que no son totalmente
 *       ciertos. Pero dado que se trata de una simple comparacion entre
 *       enteros es muy dificil que se den tales casos.
 */
   bool  isEmpty(void);

   size_t size();
   size_t max_size();

   void  inicializar(void);
   void lock(void);
   void unlock(void);

private:
   SDWordType        in;               /**< Indice de entrada de la fifo. */
   SDWordType        out;              /**< Indice de salida de la fifo. */
   F                 item[MAX_ITEMS+1];/**< Array de objetos de la fifo.*/
   VarsCondicion     vcond;
   Mutex             mutex;

   bool  isFull(void);                 /**< @return Chequea si la fifo esta llena. */
   bool  bInit;

   void checkInit(void);
};


template <typename F,UDWordType MAX_ITEMS>
Fifo<F,MAX_ITEMS>::Fifo(void) {
  bInit=false;
}

template <typename F,UDWordType MAX_ITEMS>
Fifo<F,MAX_ITEMS>::~Fifo(void) {
}

template <typename F,UDWordType MAX_ITEMS>
void Fifo<F,MAX_ITEMS>::checkInit(void) {
  if (!bInit) {
    inicializar();
  }
}

template <typename F,UDWordType MAX_ITEMS>
void Fifo<F,MAX_ITEMS>::clear(void) {
   checkInit();

   mutex.lock();
   in=0;
   out=0;
   mutex.unlock();
}

template <typename F,UDWordType MAX_ITEMS>
bool Fifo<F,MAX_ITEMS>::isFull(void) {
   SDWordType temp;

   temp=in+1;
   temp%=(MAX_ITEMS+1);
   return (temp==out);
}


template <typename F,UDWordType MAX_ITEMS>
bool Fifo<F,MAX_ITEMS>::isEmpty(void) {
   checkInit();
   return(in==out);
}

template <typename F,UDWordType MAX_ITEMS>
size_t Fifo<F,MAX_ITEMS>::size(){
   int size = in-out;
   if (size<0)
      size+=MAX_ITEMS;
   return size;
};

template <typename F,UDWordType MAX_ITEMS>
size_t Fifo<F,MAX_ITEMS>::max_size(){
   return MAX_ITEMS;
};

template <typename F,UDWordType MAX_ITEMS>
bool Fifo<F,MAX_ITEMS>::add(const F &i) {
   checkInit();
   mutex.lock();
   try {
      if (!isFull()) {
         item[in]=i;
         in++;
         in%=(MAX_ITEMS+1);
         vcond.signal();
         mutex.unlock();
         return true;
      }
   }
   catch(Excepcion &e) {

   }
   mutex.unlock();
   return false;
}

template <typename F,UDWordType MAX_ITEMS>
bool  Fifo<F,MAX_ITEMS>::get(F &i,SDWordType sdwTimeout,bool remove) {
   checkInit();
   mutex.lock();
   while (isEmpty()) {
      try{
         if (sdwTimeout==INFINITO) {
            vcond.wait(mutex);
         }
         else if (sdwTimeout > 0) {
            vcond.setTimeout((CUWordType) sdwTimeout);
            vcond.waitTime(mutex);
         }
         else {
            mutex.unlock();
            return false;
         }
      }
      catch (Excepcion &e) {
         mutex.unlock();
         return false;
      }
   }
   i=item[out];
   if (remove) {
      out++;
      out%=(MAX_ITEMS+1);
   }
   mutex.unlock();
   return true;

}

template <typename F,UDWordType MAX_ITEMS>
void  Fifo<F,MAX_ITEMS>::inicializar(void) {
   if (!bInit) {
      vcond.inicializar();
      bInit=true;
      clear();
   }
}

template <typename F,UDWordType MAX_ITEMS>
void  Fifo<F,MAX_ITEMS>::lock(void) {
  checkInit();
  mutex.lock();
}

template <typename F,UDWordType MAX_ITEMS>
void  Fifo<F,MAX_ITEMS>::unlock(void) {
  checkInit();
  mutex.unlock();
}

#endif


