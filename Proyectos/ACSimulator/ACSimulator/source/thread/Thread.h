#ifndef THREAD_H
#define THREAD_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <pthread.h>
#ifdef _PROFILE
#include <sys/time.h>
#endif

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/NoAplicaExcepcion.h"
#include "../lang/throwable/ParametroExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"
#include "../lang/throwable/OcupadoExcepcion.h"
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/ParametroExcepcion.h"
#include "../lang/throwable/LlenoExcepcion.h"
#include "AutoMutex.h"


// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   THREAD.H
  *
  * DESCRIPCION: Tratamiento y encapsulamiento de los thread de POSIX.
  * Esta clase se tiene que linkar con la opcion -lpthread.
  *
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
  *    | DD-MM-YYYY |      |               |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class Thread {

  // ATRIBUTOS
  // =====================================================

  ////////////////////   PRIVATE      ////////////////////
private:

    enum EstadoTypeE
    {
      PARADO,
      SOLICITUD_CANCELACION,
      ARRANCADO
    };

    pthread_attr_t mAtributos;
    pthread_t     midHilo;           // Identificador del hilo
    EstadoTypeE   mEstado;

    // Mutex para acceder al atributo mFinalizar
    AutoMutex mMutexFinalizar;

    // Flag que indica que se ha solicitado finalizar la tarea del hilo
    bool mFinalizar;

  // METODOS
  // =====================================================
  ////////////////////   PUBLIC      ////////////////////
  public:

    // ===============   CICLO DE VIDA  ==================

   /**
    * Constructor.
    *
    * @param     Ninguno.
    * @return    Ninguno.
    * @exception Ninguno.
    */
    Thread();

   /**
    * Destructor.
    *
    * @param     Ninguno
    * @return    Ninguno
    * @exception Ninguno
    */
    virtual ~Thread();

    // ===============   OPERACIONES  ==================

   /**
    *  Tarea que se ejecuta en el Thread, es virtual y se obligatoriamente
    *  implementa en la clase que hereda. Podemos realizar dos tipos de procesos:
    *  1- Un proceso que realiza alguna/s acciones y termina por si mismo.
    *  2- Un conjunto de acciones infinitas que necesita la accion de un tercero
    *     para su finalizacion. En este caso desde un lugar distinto al hilo,
    *     podemos solicitar su cancelacion a traves de su Id. En este momento nuestro
    *     hilo finalizara en cuanto pase por el metodo puntoDeCancelacion u otros
    *     puntos de cancelacion que soporta el sistema (sleep, wait...)
    *
    *    Obcionalmente la clase puede heredar el metodo virtual tareaSalida que
    *    se ejecutara para finalizar el hilo.
    *
    * @param      Ninguno
    * @return     Ninguno
    * @exception  Ninguno
    */
    virtual void tarea() = 0;

   /**
    *  Tarea que se ejecuta posteriormente a la cancelacion del hilo, es virtual
    *  no pura, por lo que es obcional que las clases lo hereden.
    *  que hereda.
    *  Se ejecuta en el mismo hilo del thread
    *
    * @param      Ninguno
    * @return     Ninguno
    * @exception  Ninguno
    */
    virtual void tareaSalida();

   /**
    *  Inicializa y crea el thread.
    *
    *  @param *parg -> Puntero a los argumentos para el thread.
    *  @return Ninguno
    *  @exception LlenoExcepcion "No se puede crear el hilo, memoria insuficiente o maximo numero de hilos creados."
    *
    *  El hilo generado puede generar estas dos excepciones que son tratadas por
    *  el mismo hilo, generando la suspension de la ejecucion del mismo, con loq
    *  que no pueden ser tratadas por la funcion que invoca al metodo iniciar().
    *  Y se describen en este apartado por su directa relacion al metoro iniciar()
    *
    *  @exception ParametroExcepcion   "No se puede realizar setcancelstate parametro incorrecto."
    *  @exception ParametroExcepcion   "No se puede realizar el setcanceltype parametro incorrecto."
    *  @exception DesconocidoExcepcion "Error desconocido en pthread_setcanceltype."
    *  @exception DesconocidoExcepcion "Error desconocido en pthread_setcancelstate."
    */
    void iniciar();

   /**
    *  Para la ejecucion del thread y espera hasta la terminacion del hilo mTh
    *
    * @param   mTh -> Referencia al hilo que se espera la finalizacion.
    * @return  Ninguno
    * @exception ParametroExcepcion "Error no se puede esperar la finalizacion del hilo, parametro incorrecto."
    * @exception OcupadoExcepcion "Error no se puede esperar la finalizacion del hilo, el hilo referenciado no esta activo."
    * @exception NoExisteExcepcion "Error no se puede esperar la finalizacion del hilo, el hilo referenciado no existe."
    * @exception DesconocidoExcepcion "Error desconocido en pthread_join."
    */
    void join(Thread& mTh);

   /**
    *  Cancela la ejecucion del hilo que le invoca.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception NoExisteExcepcion "No se puede cancelar el hilo, no se encuentra el hilo identificado."
    *  @exception DesconocidoExcepcion "Error desconocido en pthread_cancel."
    */
    void destruir(bool lbDetach = false);

    void detach();

   /**
    *  Mira si hay alguna orden de cancelacion sobre el hilo. En caso afirmativo
    *  cancela la ejecucion del hilo y ejecuta la tareadesalida.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception  Ninguno
    */
    static void puntoDeCancelacion() ;

   /**
    *  Espera el final de ejecucion del hilo actual. No se tiene que llamar
    *  desde el metodo tarea ni tareaSalida ya que provocaria un abrazo mortal.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception NoAplicaExcepcion "No se puede leer los atributos del hilo"
    *  @exception ParametroExcepcion "No se puede configurar el hilo, parametro incorrecto."
    *  @exception ParametroExcepcion "Error no se puede esperar la finalizacion del hilo, parametro incorrecto."
    *  @exception OcupadoExcepcion "Error no se puede esperar la finalizacion del hilo, el hilo referenciado no esta activo."
    *  @exception NoExisteExcepcion "Error no se puede esperar la finalizacion del hilo, el hilo referenciado no existe."
    *  @exception DesconocidoExcepcion "Error desconocido en pthread_attr_setdetachstate."
    *  @exception DesconocidoExcepcion "Error desconocido en pthread_join."
    */
    void esperarFinalHilo() ;

   /**
    * Compara si la ejecucion del hilo corresponde al objeto.
    * @param Ninguno
    * @return  true -> en el caso de que sea el mismo hilo
    * @return  false -> en el caso de que sean dos hilos diferentes.
    * @exception Ninguno
    */
    bool comparar() const;

   /**
    *  Devuelve el identificador de hilo.
    *  @param  Ninguno
    *  @return pthread_t con el identificador del hilo.
    *  @exception  Ninguno
    */
    pthread_t getId() const;

   /**
    *  Anula toda la cancelacion diferida, el codigo que se ejecute despues
    *  de la llamada no se puede cancelar
    *  @param      Ninguno
    *  @return     Ninguno
    *  @exception  Ninguno
    */
    void anularCancelacion() const;

   /**
    *  Permite la cancelacion del hilo apartir de la llamada del metodo.
    *  @param      Ninguno
    *  @return     Ninguno
    *  @exception  Ninguno
    */
    void permitirCancelacion() const;

  /**
   *  Marca el hilo como parado, el metodo es publico porque se invoca
   *  desde metodos estaticos, pero es de uso interno de la clase.
   *  NO INVOCAR desde fuera de la clase.
   *  @param      Ninguno
   *  @return     Ninguno
   *  @exception  Ninguno
   */
   void setParado() ;

   /**
   *  Comprueba si se ha finalizado la tarea del Thread
   *  @param      Ninguno
   *  @return     Si/No
   *  @exception  Ninguno
   */
   bool isFinalizado(void) ;

   void requestFinalizar();
   bool queryFinalizar();
   void finalizar();

  ////////////////////   PRIVATE      ////////////////////
  private:

#ifdef _PROFILE
    struct itimerval mItimerVal;
#endif

   /**
    *  Sirve de puente para poder declarar un metodo de una clase como, una
    *  funcion, para utilizarlo como parametro en pthread_create
    *  Configura el hilo para su correcta ejecucion y cancelacion, es el codigo
    *  que se ejecuta antes del hilo.
    *   @param  pThis -> Puntero this al objeto.
    *   @return Ninguno
    *   @exception ParametroExcepcion "No se puede realizar setcancelstate parametro incorrecto."
    *   @exception ParametroExcepcion "No se puede realizar el setcanceltype parametro incorrecto."
    *   @exception DesconocidoExcepcion "Error desconocido en pthread_setcancelstate."
    *   @exception DesconocidoExcepcion "Error desconocido en pthread_setcanceltype."
    */
    static void *puenteEntrada(void *pThis);

   /**
    *  Sirve de puente para poder declarar un metodo de una clase como, una
    *  funcion, para utilizarlo como parametro en pthread_create
    *  Es el codigo que se ejecuta al realizar una peticion de cancelacion del
    *  hilo.
    *   @param  pThis -> Puntero this al objeto.
    *   @return Ninguno
    *   @exception  Ninguno
    */
   static void puenteSalida(void* pThis);
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif //THREAD_H










