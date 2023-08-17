// =======================
//  FICHERO - FichVarEntorno.cpp
// =======================

#include "FichVarEntorno.h"  // clase implementada
#include "STrz.h"

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////

// ===============   CICLO DE VIDA    ==================
FichVarEntorno::FichVarEntorno()
{
   this->mbCrear = false;
}

FichVarEntorno::~FichVarEntorno()
{

}

// =================   OPERACIONES   ===================
void FichVarEntorno::setNombre(const Cadena& lNombre,bool lbCrear)
{
   FicheroDatos::setNombre(lNombre);
   mbCrear  = lbCrear;
}

// =================   ACCESO       ====================
void FichVarEntorno::setValor(const Cadena& lNombreVarEntorno,const Cadena& lValor)
{
   SDWordType lsdwPosicion;
   Cadena lTemp;

   setLectura(true);
   setEscritura(true);
   setAppend(false);

   if( (true == mbCrear) && (false == Directorio::existe(getNombre(), Directorio::FICHERO) ) )
    crear();
   // Se abre el fichero
   abrir ();
   lsdwPosicion = getPosicionValor (lNombreVarEntorno );
   if( -1 != lsdwPosicion)
   {
      // Modificamos su valor
      lTemp = lValor;
      lTemp.insertaCaracter(1,'"');
      lTemp.insertaCaracter('"');
      setPosicion(lsdwPosicion);
      borrarLinea(true);
      setPosicion(lsdwPosicion);
      insertarLinea(lValor,true);
   }
   else
   {
      // Creamos nosotros el parametro
      irFin();
      escribir((Cadena)"export " + lNombreVarEntorno + "=" + '"' + lValor + '"'+ "\n");
   }
   cerrar();
}

// =================   CONSULTA      ===================
Cadena FichVarEntorno::getValor(const Cadena& lNombreVarEntorno)
{
   SDWordType lsdwPosicion;
   SDWordType lsdwPosicionFinal;
   Cadena lResultado;

   setLectura(true);
   setEscritura(false);
   setAppend(false);
   try
   {
      abrir();
      lsdwPosicion = getPosicionValor(lNombreVarEntorno);
      if(-1 != lsdwPosicion)
      {
         setPosicion(lsdwPosicion);
         leerLinea(lResultado);
         // Eliminamos las comillas y los retornos de carro
         lsdwPosicion = lResultado.primerIndiceDe('"');
         if( -1 == lsdwPosicion)
           lsdwPosicion = 0;
         lsdwPosicionFinal = lResultado.ultimoIndiceDe('"');
         if( -1 == lsdwPosicionFinal)
          lsdwPosicionFinal = lResultado.getLong();
         if (  (lsdwPosicionFinal - lsdwPosicion) > 1 )
         {
          lResultado = lResultado.subCadena(lsdwPosicion + 1,lsdwPosicionFinal - 1);
          lResultado.trim();
         }
      }
      cerrar();
   }
   catch(Excepcion& e)
   {
      STrz::basic( ConstTraza::T_TRAZA_VAR, e );
   }
   return(lResultado);
}

////////////////////   PRIVATE       ////////////////////
SDWordType FichVarEntorno::getPosicionValor(const Cadena& lNombreVarEntorno)
{
   Cadena      lLinea;
   bool        lbContinua;
   SDWordType  lsdwPosVar;
   UDWordType  luwPosLinea;

   irInicio();
   // Primera linea
   luwPosLinea = 0;
   lbContinua = leerLinea(lLinea,false);
   while(true == lbContinua)
   {
     lsdwPosVar = lLinea.primerIndiceDe(lNombreVarEntorno);
     if(-1 != lsdwPosVar)
     {
        lsdwPosVar = lLinea.primerIndiceDe('=',lsdwPosVar);
        // Variable de entorno encontrada
        return(luwPosLinea + lsdwPosVar );
     }
     // Siguiente lineas
     luwPosLinea = getPosicion();
     lbContinua = leerLinea(lLinea,false);
   }
   // Variable de entorno no encontrada
   return(-1);
}



// =======================
//  FIN DEL FICHERO - FichVarEntorno.cpp
// =======================

