#ifndef OBJOBSERVABLE_H
#define OBJOBSERVABLE_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO

#include "util/Observador/ObjObservador.h"
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
  * FICHERO:   OBJOBSERVABLE.H
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

class ObjObservable
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
      virtual void add( const ObjObservador &lObservador, const SDWordType lEvento) = 0;
      virtual void sub( const ObjObservador &lObservador, const SDWordType lEvento) = 0;
      virtual void sub( const ObjObservador &lObservador) = 0;
      virtual void informar(const SDWordType lEvento,const SDWordType lParametro = 0) = 0;
      virtual void informar(const Evento& lEvento) = 0;
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

