#ifndef OBJOBSERVADORGENERICO_H
#define OBJOBSERVADORGENERICO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "lang/Types.h"
#include "util/Observador/EjecutorEvento.h"
#include "thread/Thread.h"
#include "ipc/Mensaje/MsjEvento.h"
#include "ipc/ColaIPC.h"
#include "util/Observador/ObjObservador.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   OBJOBSERVADOR.H
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

class ObjObservadorGenerico : public Thread, public ObjObservador
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
  static const UWordType MAXIMO_BUFFER = (8 * 1024);
  MsjEvento mMsjEvento;
  ColaIPC   mColaIPC;
  SDWordType msdwId;
  SDWordType msdwPid;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      // =================   OPERACIONES   ===================
    void inicializar(const SDWordType lsdwClave);
    void finalizar();
    void iniciar();
    void tarea();
    // =================   ACCESO       ====================
    void setEjecutor(EjecutorEvento* lEjecutor);
    // =================   CONSULTA      ===================
    SDWordType getId() const;
    enum ObjObservador::tiposTypeE getTipo() const;

    //PID del proceso observador
    void setPid(SDWordType lsdwPid);
    SDWordType getPid() const;
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

