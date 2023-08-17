#ifndef CADENAPATTERN_H
#define CADENAPATTERN_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA
#include <sys/types.h>
#include <regex.h>


// INCLUDES DEL PROJECTO
#include "../lang/Cadena.h"
#include "../lang/throwable/Excepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   CADENAPATTERN.H
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

class CadenaPattern
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
    bool mbExpresion;
    bool mbMay;
    bool mbNoSubstring;
    bool mbNewLine;
    regex_t mRegex_t;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      // ===============   CICLO DE VIDA    ==================
      CadenaPattern();
      virtual ~CadenaPattern();
      // =================   OPERACIONES   ===================
      void setPattern(const Cadena& lPattern);
      bool check(const Cadena& lTexto);
      // =================   ACCESO       ====================
      void setExpresionExtendida(bool lOpcion);
      void setDiffMayusculas(bool lOpcion);
      void setNoSubString(bool lOpcion);
      void setNewLine(bool lOpcion);
      // =================   CONSULTA      ===================
      static bool checkIP ( const Cadena& lCadena );
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

