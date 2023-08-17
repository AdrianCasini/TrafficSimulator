/*
 * EstacionTypes.cpp
 *
 *  Created on: 29/06/2012
 *      Author: jmartinez
 */

#include "Types.h"
#include <string.h>



//-----------------------------------------------------------------------------
static const char* ESTADO_PORTICO[]=
{
   "AR",
   "CE",
   "FS",
   "AB",
   "BL"
};

//-----------------------------------------------------------------------------
const char* toStr( EstadoPorticoTypeE estado)
{
   if ( estado < ESTADO_ARRANQUE || estado > ESTADO_BLOQUEADO )
      return LITERAL_INVALIDO;
   else
      return ESTADO_PORTICO [ estado ];
};

