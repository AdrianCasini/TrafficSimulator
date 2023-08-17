#ifndef EVENTOSGRUPO_H
#define EVENTOSGRUPO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/proceso/Grupo.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   EVENTOSGRUPO.H
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

class EventosGrupo
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
      virtual void inicioArranque( Grupo* lpGrupo ) = 0;
      virtual void finalArranque ( Grupo* lpGrupo , bool lbProcVia ) = 0;
      virtual void error ( Grupo* lpGrupo, ProcesoHijo* lpProceso ) = 0;
      virtual void error ( ProcesoHijo* lpProceso ) = 0;
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

