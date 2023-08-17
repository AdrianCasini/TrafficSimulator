// =======================
//  FICHERO - CONSTRAZA.H
// =======================

#ifndef CONSTRAZA_H
#define CONSTRAZA_H

#define TRAZA_COMS     ConstTraza::T_TRAZA_COMS
#define TRAZA_GAMENS   ConstTraza::T_TRAZA_GAMENS
#define TRAZA_PERIODOS ConstTraza::T_TRAZA_PERIODOS
#define TRAZA_BD       ConstTraza::T_TRAZA_BD
#define TRAZA_ERROR    ConstTraza::T_TRAZA_ERROR

#include "../lang/Types.h"
#include "../lang/Cadena.h"



/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: CONSTRAZA.H
  *
  * DESCRIPCION: Clase donde se definen todos los nombres de las trazas y constantes
  *  relacionadas.
  *
  * FECHA CREACION: 11-07-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
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
class ConstTraza
{
   // CLASES INTERNAS
   // =====================================================

   /** descripcion de la clase interna */

   // ATRIBUTOS
   // =====================================================

   ////////////////////   PUBLIC       ////////////////////
   public:
   //--------- TIPOS IDENTIFICADORES DE TRAZA---------------

      enum TrazasID
      {
         T_TRAZA_TELECARGA,          //  0 Telecargas en general
         T_TRAZA_TELE_DATA,          //  1 Telecargas de Datos
         T_TRAZA_TELE_SOFT,          //  2 Telecargas de Software
         T_TRAZA_INTERFAZ,           //  3 Con el interfaz de usuario
         T_TRAZA_ALARMA,             //  4 Alarma
         T_TRAZA_COMS,               //  5 Comunicaciones con los diferentes hosts
         T_TRAZA_ESTADO,             //  6 Estado de los procesos
         T_TRAZA_CONFIGURACION,      //  7 Configuracion
         T_TRAZA_CTRLPROC,           //  8 Control de procesos
         T_TRAZA_CTRLSO,             //  9 Control de sistema operativo
         T_TRAZA_PERIODOS,           // 10 Mensajes fuera y dentro de periodo
         T_TRAZA_SOCKETS,            // 11 Sockets
         T_TRAZA_MSJSISTEMA,         // 12 Mensajes de sistema
         T_TRAZA_VAR,                // 13 Variables entorno
         T_TRAZA_ERROR,              // 14 Errores generales
         T_TRAZA_EVENTO,             // 15 Eventos de via
         T_TRAZA_NUCLEO,             // 16 Nucleo
         T_TRAZA_TRANSP,             // 17 Transporte
         T_TRAZA_BD,                 // 18 Base de datos
         T_TRAZA_INFO,               // 19 Info
         T_TRAZA_GAMENS,             // 20 Mensajes GAMENS
         T_TRAZA_CARGADOR,           // 21 Cargador
         T_TRAZA_SIU,                // 22 SIU
         T_TRAZA_TREFC,              // 23 Gestor TrEFC
         T_TRAZA_LIC,                // 24 Traza LIC
         T_TRAZA_ARH,                // 25 Trazas del Proxy con ARH
         T_TRAZA_SICK,               // 26 Trazas del Laser SICK
         T_TRAZA_E_S,                // 27 Trazas del Gestor IO
         T_TRAZA_MNT                 // 28 Trazas del Monitor de Sistema
      };

      // Tipo de configuracion de las trazas
      enum NivelTrazaTypeE
      {
         TRAZAR_NO             =   0,
         TRAZAR_BASIC          =   1,
         TRAZAR_DEBUG          =   2
      };

      // Longitud maxima de la traza que permitimos trazar
      static const SWordType MAXIMA_LONGITUD_TRAZA = 150;

   // METODOS
   // =====================================================

};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //CONSTRAZA_H

// ===========================
//  FIN DEL FICHERO - CONSTRAZA.H
// ===========================
