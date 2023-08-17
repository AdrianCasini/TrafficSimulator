#ifndef OBJOBSERVADOR_H
#define OBJOBSERVADOR_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "lang/Types.h"

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

class ObjObservador
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PUBLIC     ////////////////////
  public:

  enum tiposTypeE
  {
    GENERICO = 0,
    PROPIO,
  };
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
   // ===============   CICLO DE VIDA    ==================
   // =================   OPERACIONES   ===================
   // =================   ACCESO       ====================
   // =================   CONSULTA      ===================
   // Retorna el identificador del objeto
   virtual SDWordType getId() const = 0;
   // Retorna el tipo de objeto
   virtual enum tiposTypeE getTipo() const = 0;

   // Retorna el identificador del proceso observador
   virtual SDWordType getPid() const = 0;
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

