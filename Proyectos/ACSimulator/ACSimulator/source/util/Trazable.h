// =======================
//  FICHERO - TRAZABLE.H
// =======================

#ifndef TRAZABLE_H
#define TRAZABLE_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"
#include "../lang/Cadena.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: TRAZABLE.H
  *
  * DESCRIPCION: Clase trazable utilizada por todos los procesos para la gestion
  *  de las trazas. Es una clase virtual que sirve como interfaz para configurar
  *  y generar las trazas.
  *
  * FECHA CREACION: 20-06-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |             |                                              |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */
class Trazable
{
   // CLASES INTERNAS
   // =====================================================

   /** descripcion de la clase interna */

   // ATRIBUTOS
   // =====================================================


   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   OPERACIONES  ==================

     /**
      * Definicion del metodo virtual inicializar
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual void inicializar() = 0;

     /**
      * Definicion del metodo virtual destruir
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual void destruir() = 0;

     /**
      * Definicion del metodo virtual debug
      *  @param      swTipoTraza -> Tipo de traza
      *  @param      psbTraza -> Cadena de texto a trazar
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual void debug(SWordType swTipoTraza, CPSByteType psbTraza) = 0;

     /**
      * Definicion del metodo virtual basic
      *  @param      swTipoTraza -> Tipo de traza
      *  @param      psbTraza -> Cadena de texto a trazar
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual void basic(SWordType swTipoTraza, CPSByteType psbTraza) = 0;

      // =================   ACCESO       ====================

      virtual UWordType getId() = 0;
      virtual UWordType getLongMaxMensaje() = 0;
      virtual Cadena& getNombreFichero() = 0;
      virtual UWordType getNumFicheros() = 0;
      virtual UDWordType getLongFicheros() = 0;

      virtual void setProceso(const Cadena& lNombre,UWordType luwId) = 0;
      virtual void setLongMaxMensaje(UWordType uwLongMax) = 0;
      virtual void setNombreFichero(const Cadena& mFichAltern) = 0;
      virtual void setNumFicheros(UWordType uwNumFichAltern) = 0;
      virtual void setLongFicheros(UDWordType udwLongFichAltern) = 0;

};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //TRAZABLE_H

// ===========================
//  FIN DEL FICHERO - TRAZABLE.H
// ===========================
