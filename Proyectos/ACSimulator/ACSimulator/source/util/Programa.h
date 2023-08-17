#ifndef PROGRAMA_H
#define PROGRAMA_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


// INCLUDES DEL PROJECTO
#include "../lang/Types.h"
#include "../lang/throwable/IOExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   PROGRAMA.H
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

class Programa
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
    bool mbBack;
    SWordType mPid;
    SDWordType mstatus;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
      // ===============   CICLO DE VIDA    ==================
      Programa();
      virtual ~Programa();
      // =================   OPERACIONES   ===================
      void exec(const Cadena& lPrograma,const Cadena& arg1,const Cadena& arg2);
      void exec(const Cadena& lPrograma,const Cadena& arg1);
      void stop();
      // =================   ACCESO       ====================
      void setBackground(bool lbBackground);
      // =================   CONSULTA      ===================
      bool getbackground();
      SDWordType getExit();
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

