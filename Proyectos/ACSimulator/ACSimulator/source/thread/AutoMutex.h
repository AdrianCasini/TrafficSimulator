// =======================
//  FICHERO - AutoMutex.h
// =======================

#ifndef AUTOMUTEX_H
#define AUTOMUTEX_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO

// INCLUDES LOCALES
#include "../thread/Mutex.h"

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: AutoMutex.h
   *
   * DESCRIPCION: mutex de inicializacion automatica. Cuando se invoca al
   * constructor, se llama a inicializar(), y cuando se invoca al destructor se
   * llama a destruir(), todo ello automaticamente. Ideal para ser usado como
   * miembro estatico de una clase
   *
   * FECHA CREACION: 31-01-2007
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Marc Coll
   *
   * MODIFICACIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |                |                                          |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */

//JML Eliminar AutoMutex ya que Mutex ya se comporta como esta clase
class AutoMutex : public Mutex
{
   public:

      AutoMutex ();
      virtual ~AutoMutex ();
};

#endif // AUTOMUTEX_H
