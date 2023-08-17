//---------------------------------------------------------------------------
// VarEntrono
//---------------------------------------------------------------------------

#if !defined(VarEntorno_H)
   #define VarEntorno_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include "lang/Cadena.h"

#include "STrz.h"
#include "ConstTraza.h"

#include "lang/throwable/NoExisteExcepcion.h"
#include "lang/throwable/NoAplicaExcepcion.h"

// INCLUDES DEL PROJECTO

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: VarEntorno.H
  *
  * DESCRIPCION:  Acceso a los variables de entorno del sistema.
  *
  * FECHA CREACION: 16-05-2006
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oriol  Herrera Mari
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |        |                   |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class VarEntorno : public Cadena
{
// CLASES INTERNAS
// =====================================================

// ATRIBUTOS
// =====================================================

   ////////////////////   PUBLIC       ////////////////////
  public:

      enum TipoVariable
      {
        VACIO=-1,
        NO=0,
        ENTORNO,
        REEMPLAZAR,
        DEFECTO,
        ENTORNO_REEMPLAZAR,
        DEFECTO_ENTORNO,
        DEFECTO_REEMPLAZAR,
        DEFECTO_ENTORNO_REEMPLAZAR
      };

      static const SByteType VARIABLE_ENTORNO[][ 30 ];

  ////////////////////   PROTECTED    ////////////////////
  protected:


  ////////////////////   PRIVATE      ////////////////////
  private:




// METODOS
// =====================================================

  ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

      /**
      *  Constructor de la clase.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Ninguno
      */
      VarEntorno();

     /**
      *  Destructor de la clase.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Ninguno
      */
      virtual ~VarEntorno();

   // ===============   OPERACIONES    ==================


   ////////////////////   PRIVATE      ////////////////////
   private:

   ////////////////////   PUBLIC      ////////////////////
   public:

      SDWordType contieneVarEntorno(Cadena cadReemplazar="");

      SDWordType resuelveVarEntorno(Cadena cadDefecto="");
      SDWordType resuelveVarEntorno(Cadena cadReemplazar, Cadena cadReemplazo);
      SDWordType resuelveVarEntorno(Cadena cadDefecto, Cadena cadReemplazar, Cadena cadReemplazo);

      VarEntorno& operator=(Cadena cadCadena);

};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif // VarEntorno_H

//---------------------------------------------------------------------------
// VarEntorno_H
//---------------------------------------------------------------------------
