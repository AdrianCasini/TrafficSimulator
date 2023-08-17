#ifndef MARCHA_H
#define MARCHA_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "CtrlGrupos.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   MARCHA.H
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

class Marcha  : public CtrlGrupos
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

