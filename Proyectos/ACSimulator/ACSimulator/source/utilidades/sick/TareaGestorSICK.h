// =========================================
//  FICHERO - TAREAGESTORSICK.H
// =========================================

#ifndef TAREAGESTORSICK_H
#define TAREAGESTORSICK_H

#include "stdlib.h"

#include "thread/Thread.h"
#include "util/STrz.h"
#include "util/ConstTraza.h"
#include "util/Timer.h"
#include "util/EjecutorTimer.h"
#include "gestor/util/Traza.h"
#include "gestor/gestproc/Proceso.h"
#include "lang/throwable/NoAplicaExcepcion.h"
#include "lang/Types.h"

#include "GestorSICK.h"

//------------------------------------------------------------------------------
/**
 * <PRE>
  *
  * FICHERO: TAREAGESTORSICK.H
  *
  * DESCRIPCION:  Gestion del Laser SICK
  *
  * FECHA CREACION: 05/02/2015
  *
  * AUTOR DISENYO:  Adrian Casini
  *
  * PROGRAMADOR:    Adrian Casini
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
//------------------------------------------------------------------------------

class TareaGestorSICK : public Proceso
{

   private:

      GestorSICK  mSick;

   public:

      TareaGestorSICK();
      virtual ~TareaGestorSICK();

      void inicializarTarea();
      void parse(PUByteType lpsbDatos, UWordType luwLong, SDWordType sdwIdRemitente);
      void finalizarTarea();


};



#endif

