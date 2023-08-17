//---------------------------------------------------------------------------
// ControlTiempos
//---------------------------------------------------------------------------

#if !defined(ControlTiempos_H)
#define ControlTiempos_H


// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA
#include <sys/time.h>

#include "string.h"
#include "FechaHora.h"
#include "lang/Cadena.h"
#include "lang/Types.h"



/**
  * <PRE>
  *
  *
  * FICHERO:   CONTROLTIEMPOS.H
  *
  * DESCRIPCION: Tratamiento de los enumerados con texto.
  *    o textos (literales) como enumerados.
  *
  * FECHA CREACION: 15-07-2005
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Josep Oriol  Herrera Mari
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |                |                                                             |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class ControlTiempos
{

// ATRIBUTOS
// =====================================================
////////////////////   PRIVATE      ////////////////////
  private:

   static const SDWordType MAX_TIEMPOS=25;

   Cadena      cadTitulo;
   struct timeval   tmTiempoOn;
   struct timeval   tmTiempoOff;

   bool        booSuma;
   double      dSumaTotal;
   bool        booMax;
   SDWordType  msdwMaxTiempos;
   struct sTiempo
   {
      Cadena           cadTitulo;
      struct timeval   tmTiempoOn;
      struct timeval   tmTiempoOff;
      bool             isOn;
   };
   struct sTiempo    aTiempo[MAX_TIEMPOS];

// METODOS
// =====================================================
////////////////////   PUBLIC      ////////////////////
  public:

// =================   CICLO DE VIDA      ===================

    /**
     * Destructor
     *  @param      Ninguno
     *  @return     Ninguno
     *  @exception  Ninguno
    */
    virtual ~ControlTiempos();

    /**
     * Constructor
     *  @param      Ninguno
     *  @return     Ninguno
     *  @exception  Ninguno
    */
    ControlTiempos(SDWordType sdwMaxTiempos=1, bool booSum=true);

// =================   ACCESO      ===================

     void set(SDWordType mTiempo, Cadena cadCad="");
     void On(SDWordType mTiempo=0);
     void Off(SDWordType mTiempo=0);
     bool isOn(SDWordType mTiempo=0);
     void reset(SDWordType mTiempo=0);

     double getOn(SDWordType mTiempo=0);
     double getOff(SDWordType mTiempo=0);
     double get(SDWordType mTiempo=0);
     void   getTiempo(SDWordType mTiempo=0);
     void   getTiempos();

};


// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif // ControlTiempos_H

//---------------------------------------------------------------------------
// ControlTiempos
//---------------------------------------------------------------------------
