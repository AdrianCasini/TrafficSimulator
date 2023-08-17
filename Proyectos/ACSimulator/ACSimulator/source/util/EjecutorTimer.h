
#ifndef EJECUTORTIMER_H
#define EJECUTORTIMER_H


#include "../gestor/lang/Types.h"


/**
* <PRE>
*
* FICHERO: EJECUTORTIMER.H
*
* DESCRIPCION:
*   Interfaz (clase abstracta) que permite ejecutar periodicamente un determinado
*   metodo mediante un objeto Timer
*
* FECHA CREACION: 18-05-05
*
* AUTOR DISENYO:
*
* PROGRAMADOR: Jorge Martinez
*
* MODIFICATIONES:
*
*    |   FECHA    |   AUTOR   |  RAZON                                   |
*    ---------------------------------------------------------------------
*    | DD-MM-YYYY |               |                                              |
*    ---------------------------------------------------------------------
*
* </PRE>*/
class EjecutorTimer
{
   // ATRIBUTOS
   // =====================================================

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // =================   OPERACIONES   ===================
     /**
      * <PRE>
      *   Metodo que sera ejecutado cuando un Timer programado se dispare.
      *   Este metodo, al ser virtual puro, debera ser implemetado por todas las
      *   clases que hereden de EjecutorTimer
      *
      * PARAMETROS:
      *       idTimer -> Significado de parmetros y precondiciones
      *
      * VALORES RETORNO:
      *
      * EXCEPCIONES:
      *
      * NOTAS:
      *
      * </PRE>
      */
      virtual void tareaTimer(UDWordType idTimer) = 0;
};

#endif //EJECUTORTIMER_H

// ===========================
//  FIN DEL FICHERO - EJECUTORTIMER.H
// ===========================
