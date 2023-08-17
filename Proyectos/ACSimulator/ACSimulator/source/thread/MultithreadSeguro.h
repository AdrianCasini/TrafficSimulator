// =======================
//  FICHERO - MultithreadSeguro.h
// =======================

#ifndef MULTITHREADSEGURO_H
#define MULTITHREADSEGURO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../lang/Types.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: MultithreadSeguro.h
   *
   * DESCRIPCION:
   * Interfaz que aporta un mecanismo de proteccion en escenarios concurrentes
   * ( multithreads ). Los objetos de las clases que heredan de
   * MultithreadSeguro pueden se utilizados por un objeto ProtectorMultithread
   * para gestionar de forma transparentelos los bloqueos necesarios en accesos
   * concurrentes
   *
   *
   * FECHA CREACION: 19-10-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Jorge Martinez
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |            |                                              |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */

typedef class MultithreadSeguro Lockable;

class MultithreadSeguro
{
   // CLASES INTERNAS
   // =====================================================

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   ////////////////////   PROTECTED    ////////////////////
   ////////////////////   PRIVATE      ////////////////////

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // =================   CICLO DE VIDA    ================
      // =================   OPERADORES    ===================
      // =================   OPERACIONES   ===================
      virtual void lock( SWordType swId ) const = 0;
      virtual void unlock( SWordType swId ) const = 0;

      // =================   ACCESO       ====================
      // =================   CONSULTA      ===================
   ////////////////////   PROTECTED    ////////////////////
      // =================   CICLO DE VIDA    ================
      // =================   OPERADORES    ===================
      // =================   OPERACIONES   ===================
      // =================   ACCESO       ====================
      // =================   CONSULTA      ===================

   ////////////////////   PRIVATE      ////////////////////
      // =================   CICLO DE VIDA    ================
      // =================   OPERADORES    ===================
      // =================   OPERACIONES   ===================
      // =================   ACCESO       ====================
      // =================   CONSULTA      ===================


// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================
};
#endif // MULTITHREADSEGURO_H

// ===========================
//  FIN DEL FICHERO - MultithreadSeguro.h
// ===========================
