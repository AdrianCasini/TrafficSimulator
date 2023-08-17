// =======================
//  FICHERO - EjecutorCtrlProceso
// =======================

#ifndef EjecutorCtrlProceso_H
#define EjecutorCtrlProceso_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

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
  * FICHERO: EjecutorCtrlProceso.h
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: 08:33:50
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |      |                 |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
 */
class EjecutorCtrlProceso
{

   // CLASES INTERNAS
   // =====================================================

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   private:

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
     virtual void releerConfig() = 0;
     virtual void keepAlive() = 0;
     virtual void pararEjecucion() = 0;
     // ===============   CICLO DE VIDA    ==================
     // =================   OPERACIONES   ===================
   ////////////////////   PRIVATE       ////////////////////
   private:
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //EjecutorCtrlProceso.h

// ===========================
//  FIN DEL FICHERO - EjecutorCtrlProceso.h
// ===========================



