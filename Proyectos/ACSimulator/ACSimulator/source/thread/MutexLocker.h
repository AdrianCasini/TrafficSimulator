// =======================
//  FICHERO - MUTEXLOCKER.H
// =======================

#ifndef MUTEXLOCKER_H
#define MUTEXLOCKER_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO

#include "Mutex.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: MutexLocker.H
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: 29-12-2005
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

class MutexLocker
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE       ////////////////////
   private:
      Mutex*      mpMutex;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

     /**
      * Constructor de la clase. Asigna todos los valores por defecto del objeto
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      MutexLocker(Mutex& lMutex);

     /**
      * Constructor de la clase version \t const. Asigna todos los valores
      * por defecto del objeto
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      MutexLocker(const Mutex& lMutex);
      MutexLocker(Mutex* lpMutex);

     /**
      * Destructor de la clase. Asigna todos los valores por defecto del objeto
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual  ~MutexLocker();

};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //MUTEXLOOKER_H

