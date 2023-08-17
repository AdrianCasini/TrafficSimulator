
#include "SICK.h"


//------------------------------------------------------------------------------
SICK::SICK()
{
}
//------------------------------------------------------------------------------
SICK::~SICK()
{
}


//------------------------------------------------------------------------------
bool SICK::validarRegexp (CPSByteType formato, CPSByteType str)
{
   /*
   regex_t regexpFormato;
   SDWordType resp;

   resp = regcomp (&regexpFormato, formato, REG_EXTENDED);
   if (resp != 0) {printf("Error: EXP-REG: Formato:[%s]",formato);}
   else {resp = regexec (&regexpFormato, str, 0, NULL, 0);regfree (&regexpFormato);}
   return (resp == 0);
   */
	return true;
}

//----------------------------------------------------------------------------
Cadena SICK::getCadPuntos(Cadena cadTexto)
{
   Cadena cad;
   UDWordType udwSizeFalta;
   UDWordType i, udwMaxPt = 35;
   SByteType cPuntos[35];
   cad = "";

   udwSizeFalta = udwMaxPt - cadTexto.getLong() - 1;
   if (cadTexto.getLong() < udwMaxPt)
   {
      for (i = 0; i < udwSizeFalta; i++) {cPuntos[i] = '.';}
      cPuntos[i] = '\0';
      cad.formCadena("%s", cPuntos);
   }

   return cad;
}