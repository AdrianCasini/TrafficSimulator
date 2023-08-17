#ifndef CFGGESTORPORCESOS_H
#define CFGGESTORPORCESOS_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/FichConfig.h"
#include "util/FichTraza.h"
#include "util/STrz.h"
#include "util/Trazador.h"
#include "util/NivelTraza.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   CFGGESTORPORCESOS.H
  *
  * DESCRIPCION:
  *
  * FECHA CREACION:
  *
  * AUTOR DISE\uFFFDO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *       FECHA       AUTOR     RAZON
  *    ---------------------------------------------------------------------
  *     DD-MM-YYYY
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class CfgGestorProcesos : public FichConfig
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
    SDWordType          sdwTInicioProcesos;
    SDWordType          sdwTEInicioProcesos;
    SDWordType          sdwTReInicioProcesos;
    SDWordType          sdwTMaxKill;
    SDWordType          sdwTCfgWatchDog;
    SDWordType          sdwEntreKeepAlive;
    SDWordType          sdwAckKeepAlive;
    SDWordType          sdwIntentosArranque;
    bool                mbWatchDog;
    Cadena              mPathProcesos;
    Cadena              mFichProcesos;
    NivelTraza::NivelTrazaTypeE mNivel;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    // ===============   CICLO DE VIDA    ==================
     CfgGestorProcesos();
     virtual ~CfgGestorProcesos();
    // =================   OPERACIONES   ===================
   /**
    *  Lee todos los datos de los procesos incluidos en el fichero .cfg.
    *  @param   Ninguno
    *  @return  Ninguno
    *  @exception  Todas las generadas por mFichConfig.getParam:
    *  @exception ParametroExcepcion(__AQUI__,"El formato del tipo de proceso no es correcto")
    *  @exception ParametroExcepcion(__AQUI__,"Error al leer Tipo de Proceso (No existe el parametro).")
    *  @exception ParametroExcepcion(__AQUI__,"El formato del identificador de cola no es correcto.")
    *  @exception ParametroExcepcion(__AQUI__,"Error al leer el identificador de cola")
    *  @exception ParametroExcepcion(__AQUI__,"El formato del id del semaforo no es correcta.")
    *  @exception ParametroExcepcion(__AQUI__,"Error al leer el identificador de cola.")
    *  @exception ParametroExcepcion(__AQUI__,"El formato de la orden de parada no es correcta.")
    *  @exception ParametroExcepcion(__AQUI__,"Error al leer la orden de parada no existe el parametro.")
    *  @exception ParametroExcepcion(__AQUI__,"Error al leer el nombre del proceso.")
    */
    void leer(Trazador* lTraza);

    // =================   ACCESO       ====================
    // =================   CONSULTA      ===================
    SDWordType  getTEInicioProcesos ();
    SDWordType  getTInicioProcesos ();
    SDWordType  getTReInicioProcesos();
    SDWordType  getTMaxKill ();
    SDWordType  getTCfgWatchDog ();
    SDWordType  getEntreKeepAlive ();
    SDWordType  getAckKeepAlive ();
    SDWordType  getIntentosArranque();
    bool getWatchDog();
    Cadena&      getPathProcesos();
    Cadena&      getFichProcesos();
    bool getDebug();
    NivelTraza::NivelTrazaTypeE getNivel();
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

