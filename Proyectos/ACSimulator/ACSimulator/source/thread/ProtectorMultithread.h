// =======================
//  FICHERO - ProtectorMultithread.h
// =======================

#ifndef PROTECTORMULTITHREAD_H
#define PROTECTORMULTITHREAD_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA
#include <stdlib.h>

// INCLUDES DEL PROJECTO
#include "MultithreadSeguro.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: ProtectorMultithread.h
   *
   * DESCRIPCION:
   * Utilidad que permite bloquear el acceso a un objeto proporcioando un
   * mecanismo de acceso seguro en entornos concurrentes. El objeto a proteger
   * debe heredar de la clase MultithreadSeguro o Lockable.
   *
   * USO:
   * Debe declararse un objeto de esta clase al inicio del metodo que vaya a
   * manipular el objeto a proteger. El constructor, que requiere un objeto
   * ConcurreciaSegura como parametro, invoca el metodo lock del mutex heredado,
   * evitando que otros hilos puedan accedar a el. Al finalizar el metodo,
   * el destructor invoca el metodo unlock() del mutex del objeto Lockable o
   * MultithreadSeguro, liberandolo para que otros hilos puedan acceder a el.
   * Este mecanismo funciona incluso en el caso de que se produzcan excepciones
   * no atrapadas dentro del metodo
   *
   * FECHA CREACION: 18-10-2005
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

typedef class ProtectorMultithread Locker;

class ProtectorMultithread
{
   // CLASES INTERNAS
   // =====================================================

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      static const SWordType GLOBAL = -1;

   ////////////////////   PROTECTED    ////////////////////
   ////////////////////   PRIVATE      ////////////////////
   private:
      const MultithreadSeguro* mpMultithreadSeguro;
      SWordType                mswId;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // =================   CICLO DE VIDA    ================
      ProtectorMultithread( const MultithreadSeguro* pMultithreadSeguro,
                            SWordType swId = GLOBAL );
      ~ProtectorMultithread();

      // =================   OPERADORES    ===================
      // =================   OPERACIONES   ===================
      // =================   ACCESO       ====================
      // =================   CONSULTA      ===================
   ////////////////////   PROTECTED    ////////////////////
   ////////////////////   PRIVATE      ////////////////////

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================
};
#endif // PROTECTORMULTITHREAD_H

// ===========================
//  FIN DEL FICHERO - ProtectorMultithread.h
// ===========================
