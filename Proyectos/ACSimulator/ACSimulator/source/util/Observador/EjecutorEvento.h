#ifndef EJECUTOREVENTO_H
#define EJECUTOREVENTO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/Observador/Evento.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   EJECUTOREVENTO.H
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

class EjecutorEvento
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
      virtual void evento ( const Evento& lEvento ) = 0;
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
