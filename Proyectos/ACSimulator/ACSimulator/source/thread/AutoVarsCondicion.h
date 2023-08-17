// ===============================
//  FICHERO - AutoVarsCondicion.h
// ===============================

#ifndef AUTOVARSCONDICION_H
#define AUTOVARSCONDICION_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO

// INCLUDES LOCALES
#include "../thread/VarsCondicion.h"

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: AutoVarsCondicion.h
   *
   * DESCRIPCION: Variable de condicion de inicializacion automatica. Cuando
   * se invoca al constructor, se llama a inicializar(), y cuando se invoca
   * al destructor se llama a destruir(), todo ello automaticamente. Ideal
   * para ser usado como miembro estatico de una clase
   *
   * FECHA CREACION: 27-03-2008
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Francesc Rocher
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

class AutoVarsCondicion : public VarsCondicion
{
   public:

      AutoVarsCondicion ();
      virtual ~AutoVarsCondicion ();
};

#endif // AUTOVARSCONDICION_H
