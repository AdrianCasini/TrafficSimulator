#ifndef VARSCONDICION_H
#define VARSCONDICION_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"
#include "../thread/Mutex.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/TimeoutExcepcion.h"
#include "../lang/throwable/InterrupcionExcepcion.h"
#include "../lang/throwable/NoAplicaExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/PermisoExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   VARSCONDICION.H
  *
  * DESCRIPCION: Tratamiento y encapsulamiento de las variables de condicion.
  * Esta clase se tiene que linkar con la opcion -lpthread.
  *
  * FECHA CREACION: 25-04-2005
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |           |                                        |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class VarsCondicion
{

  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE      ////////////////////
  private:
    // Marca el tiempo limite de espera en el caso de trabajar con el waitTime
    struct    timespec  Timeout;
    pthread_cond_t    idVarCon;

  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:

    // ===============   CICLO DE VIDA  ==================

   /**
    * Constructor.
    *
    * @param Ninguno
    * @return  Ninguno
    * @exception Ninguno
    */
    VarsCondicion();

   /**
    *  Destructor, realiza la destruccion del mutex.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception  Ninguno
    */
    virtual ~VarsCondicion();

   // ===============   OPERACIONES  ==================

   /**
    *  Inicializa los parametros de la variable de condicion , en el caso de
    *  que el parametro = NULL, lo creara con los parametros por defecto.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception ParametroExcepcion "No se puede crear la variable de condicion."
    */
    void inicializar();

   /**
    *  Desbloquea el mutex y espera la senalizacion de la variable de condicion
    *  @param  Mutex de referencia
    *  @return Ninguno
    *  @exception  Ninguno
    */
    void wait(Mutex& lMutex);

   /**
    *  Desbloquea el mutex y espera la senalizacion de la variable de condicion, o
    *  que salte el tiempo de timeout.
    *  Para establecer el tiempo de Timeout utilizar la funcion
    *  setTimeout(CUWordType  cuwTimemsg);
    *   @param  Mutex de referencia
    *   @return Ninguno
    *  @exception TimeoutExcepcion  "the condition variable was not signaled until the timeout specified by abstime"
    *  @exception InterrupcionExcepcion "pthread_cond_timedwait was interrupted by a signal"
    *  @exception ParametroExcepcion "cond, mutex, or abstime is invalid
    *            different mutexes for concurrent waits, mustx is not owned by calling thread");
    *   @exception DesconocidoExcepcion "Error desconocido en pthread_cond_timedwait"
    */
    void waitTime(Mutex& lMutex);
    void waitTime (MutexMock& lMutex );
   /**
    *  Senala la variable de condicion.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception  Ninguno
    */
    void signal();

   /**
    *  Marca el inicio y el final del tiempo de espera. El inicio es el momento
    *  actual que marca el reloj del sistema, y el final es el inicio mas el offset
    *  en milisegundos que le pasamos como parametro.
    *  @param cuwTimemsg -> Tiempo en milisegundos de esperamaxima.
    *   @return Ninguno
    *  @exception PermisoExcepcion "settimeofday is called by someone other than the superuser."
    *  @exception ParametroExcepcion "Timezone (or something else) is invalid."
    *  @exception MemoriaExcepcion "One of tv or tz pointed outside y our accessible address space."
    *  @exception DesconocidoExcepcion "Error desconocido en pthread_cond_timedwait"
    *
    */
    void setTimeout(CUWordType  cuwTimemsg);

   /**
    *  Destruye la variable de condicion.
    *   @param  Ninguno
    *   @return Ninguno
    *   @exception    OcupadoExcepcion(__AQUI__," EBUSY  some threads are currently waiting on cond.");
    */
    void destruir();

    // ===============   ACCESO  ==================

   /**
    * @return pthread_cond_t Con el identificador de la variable de condicion
    */
    pthread_cond_t*    getId();

};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================


// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif //VARSCONDICION_H





