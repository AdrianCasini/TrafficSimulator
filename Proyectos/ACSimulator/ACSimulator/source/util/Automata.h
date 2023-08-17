// =======================
//  FICHERO - AUTOMATA.H
// =======================

#ifndef AUTOMATA_H
#define AUTOMATA_H


#include "AutPorReferencia.h"

/**
 * <PRE>
 * Automata
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: Automata.H
 *
 * DESCRIPCION:
 *   Template generico para automatas.
 *
 * FECHA CREACION: 27-06-2005
 *
 * AUTOR DISENYO:
 *
 * PROGRAMADOR: Toni Bassas Serramia
 *
 * MODIFICATIONES:
 *
 *    |   FECHA    |   AUTOR   |  RAZON                                   |
 *    ---------------------------------------------------------------------
 *    | DD-MM-YYYY |           |                                          |
 *    ---------------------------------------------------------------------
 * </PRE>
*/

template <typename T>
class Automata : public AutPorReferencia<T>
{
public:

   
/**
 * Inicializa el automata.
 * @return Nada.
*/ 
   void inicializar(bool bCambioEstadoTrasSemantica = true);
   virtual void cargarTabla(SDWordType sdwTabla){};

   T  mthis;

};


template <typename T>
void Automata<T>::inicializar(bool bCambioEstadoTrasSemantica) {
   AutPorReferencia<T>::inicializar(bCambioEstadoTrasSemantica);
   AutPorReferencia<T>::out=&mthis;
}


#endif
