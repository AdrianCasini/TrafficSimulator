// =======================
//  FICHERO - TRAZADOR.H
// =======================

#ifndef TRAZADOR_H
#define TRAZADOR_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../util/FichTraza.h"
#include "../thread/Mutex.h"
#include "Trazable.h"
#include "../util/NivelTraza.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: TRAZADOR.H
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: 18-11-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |               |                                                                      |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */
class Trazador : public Trazable, public FichTraza
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   private:
      // Mutex que evita la concurrencia a las funciones debug y basic
      Mutex       mMutex;
      SWordType   mswProceso;
      Cadena      mNombreProc;
      NivelTraza::NivelTrazaTypeE  mTypeENivel;
      FechaHora   mFechaHora;

   ////////////////////   PUBLIC       ////////////////////
   public:
   // ===============   CICLO DE VIDA    ==================

     /**
      *  Constructor de la clase, se inicializa el nivel de traza en debug
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      Trazador();


     /**
      *  Destructor de la clase
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual ~Trazador();

   // ===============   OPERACIONES    ==================

     /**
      *   Inicializa todos los elementos internos de la clase en concreto el
      *   mutex que controla el acceso a los metodos debug y basic.
      *
      *   @param      Ninguno
      *   @return     Ninguno
      *   @exception  Todas las generadas por Mutex::inicializar()
      *
      */
      void inicializar();

      /**
       *   Se invoca para la destruccion de todos los elementos de la clase.
       *
       *   @param      Ninguno
       *   @return     Ninguno
       *   @exception  Todas las generadas por Mutex::destruir()
       *
       */
       void destruir();

      /**
       *   Guarda las trazas de tipo debug, el nivel programado se consulta en
       *   las tablas de memoria compartida. Solo guarda la traza si el tipo de
       *   traza es igual a SI y el nivel del proceso esta en modo DEBUG.
       *   Estos parametros dependen de los ficheros de configuracion
       *   $FF_CFG/trazasproceso.cfg y $FF_CFG/tipotrazas.cfg.
       *
       *   @param   swTipoTraza -> Campo numerico con el tipo de traza de los posibles
       *            definidos en Trazable
       *   @param   psbTraza -> Texto de la traza
       *   @return  Ninguno
       *   @exception  Todas las lanzadas por FichTrazas::registra (...)
       *   @see
       *
       */
       virtual void debug( SWordType lswTipoTraza, CPSByteType psbTraza);

      /**
       *
       *  Guarda las trazas de tipo basic, el nivel programado se consulta en
       *  las tablas de memoria compartida. Solo guarda la traza si el tipo de
       *  traza es igual a SI y el nivel del proceso esta en modo DEBUG o BASIC.
       *  Estos parametros dependen de los ficheros de configuracion
       *  $FF_CFG/trazasproceso.cfg y $FF_CFG/tipotrazas.cfg.
       *
       *  @param   swTipoTraza -> Campo numerico con el tipo de traza de los posibles
       *           definidos en Trazable
       *  @param   psbTraza -> Texto de la traza
       *  @return  Ninguno
       *  @exception Todas las lanzadas por FichTraza::registra (...)
       *
       */
       virtual void basic( SWordType lswTipoTraza, CPSByteType psbTraza);

   // =================   ACCESO       ====================
   public:
      /**
       *  @param   swIdProceso Identificador numerico del proceso que genera las trazas definido en Proceso
       */
      virtual void setProceso(const Cadena& lNombre,UWordType luwId);

      /**
       *  @param   uwLongMax Longitud maxima del mensaje de traza en bytes. Este campo
       *           tan solo tiene efecto sobre el fichero de trazas del mismo proceso
       *           (no el que se envia al gestor de trazas)
       */
       void setLongMaxMensaje( UWordType luwLongMax );

      /**
       *  @param   mFichAltern Nombre del fichero sobre el que se realizan las trazas
       *           en principio es opcional ya que la clase asigna un nombre de fichero por defecto
       */
       void setNombreFichero( const Cadena & lFichAltern );

      /**
       *  @param   uwNumFichAltern Numero de ficheros trazables.
       */
       void setNumFicheros( UWordType luwNumFichAltern );

      /**
       *  @param   udwLongFichAltern Longitud maxima del fichero en bytes.
       */
       void setLongFicheros( UDWordType ludwLongFichAltern );

     /**
      *  @param   uwNivel Selecciona el nivel de debug del proceso definidos en Trazable
      */
      void setNivel( NivelTraza::NivelTrazaTypeE );
      // =================   CONSULTA      ===================
      UWordType getId();
      UWordType getLongMaxMensaje();
      Cadena & getNombreFichero();
      UWordType getNumFicheros();
      UDWordType getLongFicheros();
      Cadena& getNombreProc();
      NivelTraza::NivelTrazaTypeE getNivel();
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //TRAZADOR_H

// ===========================
//  FIN DEL FICHERO - TRAZADOR.H
// ===========================
