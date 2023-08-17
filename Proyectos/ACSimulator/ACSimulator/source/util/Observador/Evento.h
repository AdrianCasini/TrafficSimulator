#ifndef EVENTO_H
#define EVENTO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "lang/Types.h"
#include "lang/throwable/RangoExcepcion.h"
#include <string.h>

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   EVENTO.H
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

class Evento
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      static const UWordType MAXIMOS_EVENTOS = 500;
      static const UWordType SIZE_DATOS = 128;
  ////////////////////   PRIVATE     ////////////////////
  private:
      struct mensajeTypeE
      {
       SDWordType msdwId;
       UWordType  muwLong;
       SByteType  msbDatos[SIZE_DATOS];
      };
      // Indica el offset actual de los objetos
      UWordType  muwOffset;
      struct mensajeTypeE mMensaje;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      // ===============   CICLO DE VIDA    ==================
      Evento();
      Evento( const SDWordType lId,const SDWordType lParametro);
      virtual ~Evento();
      // =================   OPERACIONES   ===================
      void inicioDatos(const SDWordType lId);
      void inicioLectura();
      void set(const Evento& lEvento);
      // =================   ACCESO       ====================
      void setId(const SDWordType lId);
      void setParametro(const SDWordType lParametro);
      void setDato(CPSByteType psbDatos,const UWordType luwLong);
      void addDato(CPSByteType psbDatos,const UWordType luwLong);
      void addDato(const Cadena& lDato);
      // =================   CONSULTA      ===================
      SDWordType getId() const;
      SDWordType getParametro() const;
      void copyDatos(PSByteType psbDatos,UWordType luwLong) const ;
      void getDatos(PSByteType psbDatos,UWordType luwLong);
      void getDatos(Cadena& lTexto) ;
      UWordType getLong() const;
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

