// =======================
//  FICHERO - PROCESOHIJO.H
// =======================

#ifndef PROCESOHIJO_H
#define PROCESOHIJO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <signal.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>

// INCLUDES DEL PROJECTO

#include "lang/Cadena.h"
#include "lang/Types.h"
#include "util/proceso/EjecutorCtrlProceso.h"
#include "lang/throwable/MemoriaExcepcion.h"
#include "lang/throwable/OcupadoExcepcion.h"
#include "lang/throwable/PermisoExcepcion.h"
#include "lang/throwable/NoAplicaExcepcion.h"
#include "lang/throwable/IOExcepcion.h"
#include "lang/throwable/CorrompidoExcepcion.h"
#include "util/Timer.h"
#include "util/STrz.h"
#include "thread/Mutex.h"
#include "thread/MutexLocker.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: PROCESOHIJO.H
  *
  * DESCRIPCION: Objeto para el control de los procesos del gestor de procesos,
  *  se encarga de guardar toda la informacion relacionada con cada uno de los
  *  procesos y el envio de ordenes por cola IPC, senales y su creacion.
  *
  * FECHA CREACION: 19-05-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |             |                                              |
  *    ---------------------------------------------------------------------
*
  * </PRE>
 */
class ProcesoHijo
{
   // ATRIBUTOS
   // =====================================================

   ////////////////////   PUBLIC       ////////////////////
   public:
      enum TiposProcesosTypeE
      {
        PROC_EXPLOTACION,
        PROC_TEST,
        PROC_SIEMPRE
      };
      enum EstadoProcesoTypeE
      {
      // El proceso esta arrancado y en funcionamiento normal
          EST_ARRANCADO = 0,
      // El proceso esta lanzado y estamos a la espera de recibir su ACK
          EST_ARRANCANDO = 1,
      // Se envia la orden de parada al proceso y esperamos su salida por
      // el wait
          EST_PARANDO = 2,
      // El proceso esta parado
          EST_PARADO = 3,
      // Se lanza un proceso que fallo su ultima ejecucion
          EST_RESUCITANDO = 4,
      // Se bloquea el proceso que despues de los intentos programados
      // no se lanza de nuevo.
          EST_BLOQUEADO = 5
      };
      // Cuando se lanza la aplicacion no se le pueden enviar mas de 5 argumentos
      static const UWordType    MAXIMOS_ARGUMENTOS = 5;

   ////////////////////   PRIVATE      ////////////////////
   private:

      UWordType         uwGrupoProcesos;
      SDWordType        sdwPID;
      EstadoProcesoTypeE  uwEstadoProc;
      UWordType         uwOrdenParada;
      SByteType         sbTipoProceso;
      Cadena            mParametros;
      Cadena            mNombreProceso;
      SByteType         sbRecepcionACK;
      time_t            horaMuerte;
      time_t            horaArranque;
      SDWordType        sdwNumArranques;
      UWordType         uwIdSemaforo;
      EjecutorCtrlProceso*   mpEjecutorCtrlProceso;
      UDWordType        udwTiempo;
      UDWordType        udwTiempoEsperaParada;
      TiposProcesosTypeE mTipoProcTypeE;
      bool              mbInterrogado;
      Mutex             mMutex;
      bool              mbBackground;
      bool              mbProcVia;
      bool              mbFinaliza;
      bool              mbPermitirRearranque;
      SDWordType        msdwIdProc;

    // METODOS
    // =====================================================
    ////////////////////   PUBLIC       ////////////////////
    public:

      // ===============   CICLO DE VIDA    ==================

     /**
      * Constructor
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      ProcesoHijo();

     /**
      * Destructor
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual  ~ProcesoHijo();

      // ===============   OPERACIONES    ==================


     /**
      *  Lanza el proceso actual. Realiza una creacion de un nuevo proceso y
      *  ejecuta la aplicacion esperando que esta finalize, se anota el pid del
      *  nuevo proceso creado.
      *  Atencion a las excepciones lanzadas ya que las puede lanzar el padre
      *  o el hijo con lo cual tenemos que controlar el pid que estamos ejecutando
      *  en el caso de ser el hijo realizamo un exit.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception PermisoExcepcion(__AQUI__,mNombreProceso + " The file or a script interpreter is"\
      *     " not  a  regular file. Execute  permission  is  denied  for  the "\
      *     " file or a script or ELF interpreter. The file system is mounted noexec.",errno);
      *  @exception PermisoExcepcion(__AQUI__,mNombreProceso + " The file system is mounted nosuid, "\
      *      " the user is  not the superuser, and the file has an SUID or SGID "\
      *      " bit set. The process is being traced, the user  is  not  the "\
      *      " superuser and the file has an SUID or SGID bit set.",errno);
      *  @exception FormatoExcepcion(__AQUI__,mNombreProceso + " The argument list is too big.",errno);
      *  @exception FormatoExcepcion(__AQUI__,\
      *           mNombreProceso +  " An executable is not in a recognised format, is for "\
      *             " the  wrong  architecture,  or has some other format "\
      *             " error that means it cannot be executed.",errno);
      *  @exception MemoriaExcepcion(__AQUI__,mNombreProceso + " filename points  outside  your "\
      *      "  accessible  address space.",errno);
      *  @exception RangoExcepcion(__AQUI__,mNombreProceso + " filename is too long.",errno);
      *  @exception NoExisteExcepcion(__AQUI__,\
      *     mNombreProceso +  " The  file  filename  or a script or ELF interpreter "\
      *      " does not exist, or a shared library needed for file "\
      *      " or interpreter cannot be found.",errno);
      *  @exception MemoriaExcepcion(__AQUI__,mNombreProceso + " Insufficient kernel memory was available.",errno);
      *  @exception NoExisteExcepcion(__AQUI__,\
      *     mNombreProceso + " A  component  of  the  path prefix of filename or a "\
      *     " script or ELF interpreter is not a directory.",errno);
      *  @exception NoAplicaExcepcion(__AQUI__,mNombreProceso + " Too many symbolic links were encountered "\
      *      " in resolving  filename or the name of a script or ELF interpreter.",errno);
      *  @exception OcupadoExcepcion(__AQUI__,\
      *       mNombreProceso + " Executable was open for writing by one or more processes.",errno);
      *  @exception IOExcepcion(__AQUI__,mNombreProceso + " An I/O error occurred.",errno);
      *  @exception RangoExcepcion(__AQUI__,mNombreProceso + " The  limit on the total number of files "\
      *      "pen on the system has been reached.The process has the maximum "\
      *       " number of files open.",errno);
      *  @exception NoAplicaExcepcion(__AQUI__,\
      *       mNombreProceso +  " An ELF executable had more than one PT_INTERP segment  (i.e., "\
      *        " tried  to  name  more than one interpreter).",errno);
      *  @exception NoAplicaExcepcion(__AQUI__,"An ELF interpreter was a directory.",errno);
      *  @exception CorrompidoExcepcion(__AQUI__,mNombreProceso + " An ELF interpreter was not in a recognised format.",errno);
      *  @exception DesconocidoExcepcion(__AQUI__,mNombreProceso + " Error en execv",errno);
      *  @exception OcupadoExcepcion(__AQUI__,"fork  cannot allocate sufficient" \
      *      " memory to copy the parent's page tables and allocate a task  "
      *      " structure for the child.",errno);
      *  @exception MemoriaExcepcion(__AQUI__,"fork failed to allocate the necessary" \
      *       " kernel structures because memory is tight.",errno);
      *  @exception DesconocidoExcepcion(__AQUI__,"Error en execv",errno);
      */
      void lanzarEjec();

     /**
      *   Envia una senal al proceso.
      *  @param sdwSignal -> Numero de senal a enviar.
      *  @return  Ninguno
      *  @exception NoAplicaExcepcion(__AQUI__,"An invalid signal was specified.",errno);
      *  @exception NoExisteExcepcion(__AQUI__,\
      *  " The pid or process group does not exist.  Note that "\
      *     " an existing process might be a  zombie,  a  process "\
      *     " which  already  committed  termination, but has not "\
      *     "yet been wait()ed for.",errno);
      *  @exception PermisoExcepcion(__AQUI__,\
      *     " The process does not have permission  to  send  the "\
      *     " signal  to  any  of the receiving processes.  For a "\
      *     " process to have permission to send a signal to pro- "\
      *     " cess  pid  it  must either have root privileges, or "\
      *     " the real or effective user ID of the  sending  pro- "\
      *     " cess  must  equal  the real or saved set-user-ID of "\
      *     " the receiving process.  In the case of  SIGCONT  it "\
      *     " suffices  when  the sending and receiving processes "\
      *     " belong to the same session.",errno);
      *  @exception DesconocidoExcepcion(__AQUI__,"Error desconocido en la funcion kill",errno);
      *  @exception ParametroExcepcion(__AQUI__,"El pId del proceso no es correcto.");
      */
      void enviarSignal(SDWordType sdwSignal);

     /**
      *    Asigna la hora actual como ultima hora de ejecucion del proceso, esta
      *    marca de tiempo sirve tan solo como referencia.
      *    @param   Ninguno
      *    @return  Ninguno
      *    @exception MemoriaExcepcion(__AQUI__,"t points outside your accessible address space.");
      *    @exception DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
      */
      void setHoraMuerte();

     /**
      * Asigna la hora actual como hora inicial de ejecucion del proceso, esta
      * marca de tiempo sirve como referencia para la espera en la resucitacion
      * del proceso en caso necesario.
      * @param      Ninguno
      * @return     Ninguno
      * @exceptin   MemoriaExcepcion(__AQUI__,"t points outside your accessible address space.");
      * @exceptin   DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
      */
      void setHoraInicio();

     /**
      *   Mira si toca resucitar un proceso, respecto a la setHoraArranque
      *   @param UWordType -> La diferencia de tiempo en milisegundos desde la hora
      *          de su lanzamiento y la actual.
      *   @return true -> en el caso de que HoraMuerte + uwOffset  > HoraActual
      *   @return false -> en el caso contrario.
      *   @exception MemoriaExcepcion(__AQUI__,"t points outside your accessible address space.");
      *   @exception DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
      */
      bool esHoraResucitar(UWordType uwOffset);

     /**
      * Coloca en los vectores de atencion de las siguientes senales:
      * SIGHUP SIGUSR1 SIGUSR2 SIGINT SIGQUIT
      * un tratamiento nulo.
      * @param   Ninguno
      * @return  Ninguno
      * @exception NoAplicaExcepcion (__AQUI__,"Error en el signal del SIGHUP")
      * @exception NoAplicaExcepcion (__AQUI__,"Error en el signal del SIGUSR1")
      * @exception NoAplicaExcepcion (__AQUI__,"Error en el signal del SIGUSR2")
      * @exception NoAplicaExcepcion (__AQUI__,"Error en el signal del SIGINT")
      * @exception NoAplicaExcepcion (__AQUI__,"Error en el signal del SIGQUIT")
      */
      void ignorarSenales();

     /**
      *  Para el proceso si no esta ya parado
      *  @param      Ninguno
      *  @return     true en el caso de que se hayan realizado acciones para parar
      *  el proceso, false en caso contrario
      *  @exception  Ninguno
      */
      bool parar();

      void enviarKill(SDWordType sdwSignal);

      void generarRun();

      // =================   CONSULTA      ===================

     /**
      * @param uwGrupoProcesos Asigna el grupo de ejecucion del proceso.
      */
      void setGrupoProcesos(UWordType uwGrupoProcesos);

     /**
      * @param uwEstadoProc Asigna el estado del proceso hijo de los posibles
      *  definidos en ProcesoHijo , el metodo es publico ya que puede ser que
      *  tengamos modificaciones de estado por parte de otros objetos.
      */
      void setEstadoProc(EstadoProcesoTypeE uwEstadoProc);

     /**
      * @param uwOrdenParada Asigna un orden numerico para la parada de procesos
      * los procesos con el mismo numero de orden pararan segun su posicion dentro
      * del array.
      */
      void setOrdenParada(UWordType uwOrdenParada);

     /**
      * @param sbTipoProceso Asigna un tipo de proceso segun su configuracion.
      * Los tipos son A, M e I.
      */
      void setTipoProceso(SByteType sbTipoProceso);

     /**
      * @param mParametros Texto con todos los parametros que recibe el ejecutable
      * cuando se lanza.
      */
      void setParametros(const Cadena& mParametros);

     /**
      * @param mNombreProceso Asigna el nombre y el path donde se encuentra el proceso
      */
      void setNombreProceso(const Cadena& mNombreProceso);

     /**
      * @param sbRecepcionACK Asigna un numero de recepcion de ack's, sirve para
      * contabilizar el numero de acks que recibe el proceso
      */
      void setRecepcionACK(SByteType sbRecepcionACK);

     /**
      * @param sdwPID  Cuando se crea el nuevo proceso, le asignamos su PID.
      */
      void setPID(SDWordType sdwPID);

     /**
      * @param sdwNumArranques Se le asigna el numero de veces que es arrancado
      * el proceso
      */
      void setNumArranques(SDWordType sdwNumArranques);

      /**
      * @param Se le asigna el semaforo que tiene el proceso
      */
      void setIdSemaforo(UWordType uwIdSemaforo);

     /**
      * @param udwTiempo Tiempo actual para tenerlo en cuenta en la resucitacion
      */
      void setTimeoutResucitacion(UDWordType udwTiempo);

     /**
      * @param udwTiempo Tiempo en milisegundos entre el tiempo marcado + udwTiempo
      * suficiente para que se considere hora de resucitacion
      * @return true en el caso de que si este en resucitacion
      */
      bool hayTimeoutResucitacion(UDWordType udwTiempo);

     /**
      * @param udwTiempoEsperaParada Tiempo que se espera antes de enviar la orden
      *  de parada al proceso.
      */
      void setTiempoEsperaParada(UDWordType udwTiempoEsperaParada);

      /**
       * @param lTipoProcTypeE Tipo definido del proceso
       *
       */
      void setTipoProc(TiposProcesosTypeE lTipoProcTypeE);

   /**
    *  Cuando se realiza la interrogacion de un proceso, se levanta este flag
    *  @param   lbInterrogado indica el envio de un keepalive
    */
    void setInterrogado(bool lbInterrogado);

    void setBackground(bool lEstado);

    void setProcVia(bool lbProcVia);

    void setFinaliza(bool lbFinaliza);

      // =================   ACCESO       ====================

    UWordType   getGrupoProcesos();
    SDWordType  getPID();
    EstadoProcesoTypeE   getEstadoProc();
    UWordType   getOrdenParada();
    SByteType   getTipoProceso();
    Cadena&     getParametros();
    Cadena&     getNombreProceso();
    SByteType   getRecepcionACK();
    UWordType   getIdSemaforo();
    SDWordType  getNumArranques();
    EjecutorCtrlProceso* getServicio();
    UDWordType  getTiempoEsperaParada();
    ProcesoHijo::TiposProcesosTypeE getTipoProc();
    Cadena  getNombreTipoProc();
    bool  getInterrogado();
    bool  getBackground();
    bool  getMaster();
    bool  getProcVia();
    bool  getActivo();
    bool  getFinaliza();
    bool getPermitirReArranque();
    void setPermitirRearranque(bool lbAccion);
    void set(EjecutorCtrlProceso *lpEjecutorCtrlProceso);
    static CPSByteType getTexto(ProcesoHijo::TiposProcesosTypeE lTipo);
    static CPSByteType getLiteral(ProcesoHijo::EstadoProcesoTypeE lEstado);
    void setIdProc (SDWordType lsdwIdProc);
    SDWordType getIdProc(){return msdwIdProc;}


};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================


// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //PROCESOHIJO_H

// ===========================
//  FIN DEL FICHERO - PROCESOHIJO.H
// ===========================
