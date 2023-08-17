#ifndef GESTORFIRMWARE_H
#define GESTORFIRMWARE_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "gestor/util/Trazador.h"
#include "gestor/util/ConstTraza.h"
#include "../thread/Thread.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   GESTORFIRMWARE.H
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

class GestorFirmware
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
    Trazador mTrazador;
    Cadena   mVersion;
    Cadena   mPuerto;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      // ===============   CICLO DE VIDA    ==================
      GestorFirmware();
      virtual ~GestorFirmware();
      // =================   OPERACIONES   ===================
      void tarea();
      void inicializar();
      virtual bool version();
      virtual bool download( const Cadena& lFichero);
      // =================   ACCESO       ====================
      // =================   CONSULTA      ===================
      Cadena getVersion();
      Cadena getPuerto();
  protected:
      // =================   ACCESO       ====================
      void setVersion(const Cadena& lVersion);
      void setPuerto(const Cadena& lPuerto);
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

