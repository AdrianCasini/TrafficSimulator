#ifndef MUTEX_H
#define MUTEX_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <pthread.h>

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/ParametroExcepcion.h"
#include "../lang/throwable/OcupadoExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   MUTEX.H
  *
  * DESCRIPCION: Tratamiento y encapsulamiento de los mutex de POSIX.
  * Esta clase se tiene que linkar con la opcion -lpthread.
  *
  * FECHA CREACION: 22-04-2005
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

class Mutex
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // Tipos de mutex
      static const UDWordType TIPO_MUTEX_ERROR_CHECK = PTHREAD_MUTEX_ERRORCHECK;
      static const UDWordType TIPO_MUTEX_NORMAL = PTHREAD_MUTEX_NORMAL;
      static const UDWordType TIPO_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE;
      static const UDWordType TIPO_MUTEX_DEFAULT = PTHREAD_MUTEX_DEFAULT;

   ////////////////////   PROTECTED    ////////////////////
   protected:
      // Estructura de identificion y control del mutex
      pthread_mutex_t        mhMutex;
      pthread_mutexattr_t    pattr;
      // Tipo de mutex de acceso por el metodo set. El mutex solo puede
      // funcionar con uno de los tipos.
      UDWordType             udwTipo;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================

     /**
      *  Constructor
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      Mutex();

      /**
       *  TIPO_MUTEX_ERROR_CHECK
       *
       *  Seleccionamos el funcionamiento del mutex. En este tipo de funcionamiento
       *  no se comprueba los interbloqueos y puden producirse abrazos mortales.
       *  De todos los metodos de funcionamiento tan solo podemos trabajar con uno
       *  de ellos.
       *
       *  TIPO_MUTEX_NORMAL
       *
       *  Seleccionamos el funcionamiento del mutex. En este tipo de funcionamiento
       *  se comprueban los intentos de bloqueo y desbloqueo desde diferentes hilos
       *  para evitar los abrazos mortales.
       *  De todos los metodos de funcionamiento tan solo podemos trabajar con uno
       *  de ellos.
       *
       *  TIPO_MUTEX_RECURSIVE
       *
       *  Seleccionamos el funcionamiento del mutex. En este tipo de funcionamiento
       *  se pueden realizar varias operaciones de bloqueo y desbloqueo desde diferentes
       *  hilos sin que se produzca abrazo mortal.
       *  De todos los metodos de funcionamiento tan solo podemos trabajar con uno
       *  de ellos.
       *
       *  TIPO_MUTEX_DEFAULT
       *
       *  En el funcionamiento del mutex no comprueba ningun tipo de error.
       *  De todos los metodos de funcionamiento tan solo podemos trabajar con uno
       *  de ellos.
       *
       *  @param      uwTipo -> Tipo de informacion
       *  @return     Ninguno
       *  @exception  Ninguno
       */
      Mutex( UWordType uwTipo );

     /**
      *  Destructor
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual ~Mutex();

      // ===============   OPERACIONES    ==================

   public:

     /**
      *  Mira si el mutex esta bloqueado, pero sin quedarse parado.
      *  @param  Ninguno
      *  @return  true -> En el caso que  este bloqueado
      *  @return falso -> En el caso que NO este bloqueado
      *  @exception ParametroExcepcion "the mutex has not been properly initialized"
      *  @exception  DesconocidoExcepcion "Error desconocido en trylock mutex."
      */
      bool tryLock();

     /**
      *  Bloquea el mutex, en el caso de que ya este bloqueado lo anade a la cola de
      *  hilos que estan esperando que se desbloquee el mutex.
      *  @param  Ninguno
      *  @return Ninguno
      *  @exception  ParametroExcepcion "the mutex has not been properly initialized."
      *  @exception  OcupadoExcepcion "the mutex is already locked by the calling thread (``
      *              error checking'' mutexes only)."
      *  @exception  DesconocidoExcepcion "Error desconocido en lock mutex."
      */
      void lock() const;

     /**
      * Desbloquea el mutex.
      * @param Ninguno
      * @return   Ninguno
      * @exception ParametroExcepcion "the mutex has not been properly initialized"
      * @exception OcupadoExcepcion "the calling thread does not own the mutex (``error
      *            checking'' mutexes only)"
      * @exception DesconocidoExcepcion "Error desconocido en unlock mutex."
      */
      void unlock() const;

      // ===============   ACCESO    ==================

     /**
      *  Devuelve el tipo de configuracion que tiene el mutex.
      *  @param      Ninguno
      *  @return     UDWordType -> Con el valor numerico del tipo de mutex
      *  @exception  Ninguno
      */
      UDWordType getTipoMutex()  ;

      // ===============   ACCESO  ==================

     /**
      * @return pthread_cond_t Con el identificador de la variable de condicion
      */
    pthread_mutex_t*    getId();

     /**
      *  Inicializa el mutex  esto son los posibles
      *  valores. PTHREAD_MUTEX_NORMAL, PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_
      *  RECURSIVE, PTHREAD_MUTEX_DEFAULT. Solo se pude seleccionar un modo de
      *  funcionamiento a traves del metodo set apropiado.
      *  @param Ninguno
      *  @return Ninguno
      *  @exception ParametroExcepcion "kind is neither PTHREAD_MUTEX_FAST_NP nor
      *             PTHREAD_MUTEX_RECURSIVE_NP nor PTHREAD_MUTEX_ERRORCHECK_NP"
      *  @exception DesconocidoExcepcion "Error desconocido en settype mutex"
      */
      void inicializar( );

     /**
      *  Destruye el mutex asociado, en el caso de que el mutex este bloqueado
      *  devuelve un error.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Excepcion ->"No se puede destruir un mutex que esta bloqueado."
      */
      void destruir();

};

class MutexMock
{
   public:
      // Tipos de mutex
      static const UDWordType TIPO_MUTEX_ERROR_CHECK = PTHREAD_MUTEX_ERRORCHECK;
      static const UDWordType TIPO_MUTEX_NORMAL = PTHREAD_MUTEX_NORMAL;
      static const UDWordType TIPO_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE;
      static const UDWordType TIPO_MUTEX_DEFAULT = PTHREAD_MUTEX_DEFAULT;

   protected:
      pthread_mutex_t        mhMutex;
      pthread_mutexattr_t    pattr;
      UDWordType             udwTipo;

   public:
      MutexMock();
      MutexMock (const MutexMock &p);

      virtual ~MutexMock();

      bool tryLock();
      void lock() const;
      void unlock() const;
      void inicializar( );
      void destruir();
      void setTipoMutex(UWordType uwTipo);
      UDWordType getTipoMutex()  ;
      pthread_mutex_t*    getId();
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif //MUTEX_H





