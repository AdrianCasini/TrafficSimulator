#ifndef ARRANQUE_H
#define ARRANQUE_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "CtrlGrupos.h"
#include "util/ConstTraza.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   ARRANQUE.H
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

class Arranque : public CtrlGrupos
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    // ===============   CICLO DE VIDA    ==================
    // =================   OPERACIONES   ===================
    void parado( SDWordType lpid, SDWordType idExit);
    void arrancado ( SDWordType lpid );
    void arranca(CtrlGrupos::ModoGrupoTypeE lModo);
    Grupo* siguiente ();
  private:
    ProcesoHijo* primerProceso ();
    ProcesoHijo* siguienteProceso ();
  protected:
    virtual bool checkProc(ProcesoHijo* lpProc);
    virtual void checkFinArranqueGrupo(ProcesoHijo* lpProc);
    // =================   ACCESO       ====================
    // =================   CONSULTA      ===================
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

