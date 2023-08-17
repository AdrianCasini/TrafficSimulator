#ifndef SEMAFORO_H
#define SEMAFORO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <pthread.h>

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"
#include "../thread/Mutex.h"
#include "../thread/MutexLocker.h"
#include "../thread/VarsCondicion.h"
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
  * FICHERO:   SEMAFORO.H
  *
  * DESCRIPCION: Semaforos formados por mutex y variables de condicion, para el
  * uso dentro de un mismo proceso.
  *
  * FECHA CREACION: 15-06-2005
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

class Semaforo: public VarsCondicion
{
  // ATRIBUTOS
  // =====================================================

  ////////////////////   PUBLIC       ////////////////////
  public:
    static const UWordType SEMAFORO_BINARIO = 0;
    static const UWordType SEMAFORO_INCREMENTAL = 1;

  ////////////////////   PRIVATE      ////////////////////
  private:
    UWordType     uwContador;
    UWordType     uwTipoSem;
    Mutex         mMutex;

  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:

    // ===============   CICLO DE VIDA    ==================

   /**
    * Constructor.
    *  @param uwVal -> Valor inicial del semaforo, 0 por defecto
    *  @return Ninguno
    *  @exception  Ninguno
    */
    Semaforo(UDWordType uwVal = 0);

   /**
    *  Destructor, realiza la destruccion del mutex y la variable de condicion.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception Ninguno
    */
    virtual ~Semaforo();

    // ===============   OPERACIONES    ==================

   /**
    *  Inicializa los parametros de la variable de condicion y el mutex.
    *  @param  Ninguno
    *  @return Ninguno
    *  @exception  ParametroExcepcion "No se puede crear la variable de condicion."
    */
    void inicializar();

   /**
    *  Desbloquea el mutex y espera la senalizacion de la variable de condicion.
    *  Si el semaforo es binario y previamente se realiza un signal no se realiza
    *  el wait.
    *  Si es incremental y el valor de semaforo es > 0 decrementa en 1 y solo se
    *  espera si el valor resultante es 0.
    *  @param  bForzar En semaforos binarios, el wait se realiza aunque no se
    *          haya realizado previamentre un signal
    *  @return Ninguno
    *  @exception  Ninguno
    */
    void wait( bool bForzar = false );

   /**
    *  Senala la variable de condicion. Si el semaforo es binario activa la senal
    *  para que se ponga en signal, si el semaforo es incremental incrementa la variable
    *  asociada.
    *  @param  Ninguno
    *  @return Ninguno
    * @exception RangoExcepcion (__AQUI__,"El semaforo tiene el maximo numero de signals");
    */
    void signal();

   /**
    *  Destruye la variable de condicion y el mutex.
    *   @param  Ninguno
    *   @return Ninguno
    *   @exception  OcupadoExcepcion(__AQUI__," EBUSY  some threads are currently waiting on cond.");
    */
    void destruir();

  /**
   * @param uwTipo Asigna el tipo de semafor de los definidos en Semaforo por
   *  defecto es un semaforo SEMAFORO_BINARIO.
   */
   void setTipoSemaforo(UWordType uwTipo);

};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif //SEMAFORO_H






