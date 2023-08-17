#ifndef MSJEVENTO_H
#define MSJEVENTO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/Observador/EjecutorEvento.h"
#include "util/Observador/Evento.h"
#include "lang/throwable/NoAplicaExcepcion.h"
#include "lang/throwable/NoExisteExcepcion.h"
#include "lang/throwable/DesconocidoExcepcion.h"
#include "lang/throwable/FormatoExcepcion.h"
// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   MSJEVENTO.H
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

class MsjEvento
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
     static const UWordType MAXIMOS_DATOS = 512;

     struct eventoTypeS
     {
       SDWordType lsdwTipo;
       UByteType   lubLong;
     };

     SByteType msbBuffer[MAXIMOS_DATOS];

     EjecutorEvento  *mpEjecutorEvento;
     struct eventoTypeS* mpDatos;
     Evento mEvento;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      // ===============   CICLO DE VIDA    ==================
      MsjEvento();
      virtual ~MsjEvento();
      // =================   OPERACIONES   ===================
      void parse(CPSByteType lpsbDatos,SDWordType lsdwLong);
      void setEvento(const Evento& lEvento);
      // =================   ACCESO       ====================
      void setEjecutor(EjecutorEvento *lpEjecutor);
      // =================   CONSULTA      ===================
      UWordType getLongitud();
      CPSByteType getDatos();
  private:
      PSByteType getInicioDatos(PSByteType lpsbDatos);
      CPSByteType getInicioDatos(CPSByteType lpsbDatos);
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

