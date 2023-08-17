//---------------------------------------------------------------------------
// VarEntorno.cpp
//---------------------------------------------------------------------------

#include "VarEntorno.h"  // clase implementada

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

/*
typedef enum TipoVariable
{
  VACIO=-1,
  NO=0,
  ENTORNO,
  REEMPLAZAR,
  DEFECTO,
  ENTORNO_REEMPLAZAR,
  DEFECTO_ENTORNO,
  DEFECTO_REEMPLAZAR,
  DEFECTO_ENTORNO_REEMPLAZAR
};
*/

const SByteType VarEntorno::VARIABLE_ENTORNO[][30] = {
   "VACIO","NINGUN", "ENTORNO", "REEMPLAZAR", "DEFECTO",
   "ENTORNO REEMPLAZAR", "DEFECTO_ENTORNO", "DEFECTO REEMPLAZAR", "DEFECTO_ENTORNO_REEMPLAZAR"
                                                      };




// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

  ////////////////////   PUBLIC       ////////////////////
// ===============   CICLO DE VIDA    ==================

//------------------------------------------------------------------------------
VarEntorno::VarEntorno()
{
}

//------------------------------------------------------------------------------
VarEntorno::~VarEntorno()
{
}

//----------------------------------------------------------------------------
VarEntorno& VarEntorno::operator=(Cadena cadCadena)
{
   (getString()) = cadCadena.getString();

   return(*this);
}


//----------------------------------------------------------------------------
SDWordType VarEntorno::contieneVarEntorno(Cadena cadReemplazar)
{
   SDWordType sdwResultado;
   bool       booTrazar2;

   booTrazar2=false;

   // No hay variable de entorno ni cadena a reemplazar.
   sdwResultado = VarEntorno::NO;

   if (this->contieneVariableEntorno())
   {
      // Hay variable de entrono a reemplazar.
      sdwResultado = VarEntorno::ENTORNO;
   }

   if ( cadReemplazar!="" )
   {
      if ( 0 < this->primerIndiceDe( cadReemplazar ) )
      {
         if (VarEntorno::ENTORNO == sdwResultado)
         {
            // Hay variable de entorno y cadena a reemplazar
            sdwResultado = VarEntorno::ENTORNO_REEMPLAZAR;
         }
         else
         {
            // Hay solo cadena que reempalazar.
            sdwResultado = VarEntorno::REEMPLAZAR;
         }
      }
   }

   return( sdwResultado );
}

//----------------------------------------------------------------------------
SDWordType VarEntorno::resuelveVarEntorno(Cadena cadDefecto, Cadena cadReemplazar, Cadena cadReemplazo)
{
   SDWordType     sdwResultado;
   SDWordType     sdwAux;
   bool           booTrazar2;

   booTrazar2=true;

   // No hay variable de entrono ni cadena a reemplazar.
   sdwResultado = VarEntorno::VACIO;

   sdwAux = contieneVarEntorno(cadReemplazar);
   while (
          (
            ( VarEntorno::ENTORNO == sdwAux )
            || ( VarEntorno::REEMPLAZAR == sdwAux )
            || ( VarEntorno::ENTORNO_REEMPLAZAR == sdwAux)
          )
          && (sdwResultado != VarEntorno::NO)
         )
   {
      if (
            ( VarEntorno::ENTORNO == sdwAux)
            || ( VarEntorno::ENTORNO_REEMPLAZAR == sdwAux)
         )
      {
         try
         {
            resuelveVariableEntorno();
            if ( VarEntorno::NO == sdwResultado )
            {
               sdwResultado = VarEntorno::ENTORNO;
            }
            else if ( VarEntorno::REEMPLAZAR == sdwResultado )
            {
               sdwResultado = VarEntorno::ENTORNO_REEMPLAZAR;
            }
            else if ( VarEntorno::DEFECTO == sdwResultado )
            {
               sdwResultado = VarEntorno::DEFECTO_ENTORNO;
            }
            else if ( VarEntorno::DEFECTO_REEMPLAZAR == sdwResultado )
            {
               sdwResultado = VarEntorno::DEFECTO_ENTORNO_REEMPLAZAR;
            }
         }
         catch( NoAplicaExcepcion& e )
         {
            if (cadDefecto =="")
            {
               sdwResultado = VarEntorno::NO;
               throw;
            }
            else
            {
               Cadena::operator=(cadDefecto);
               if ( VarEntorno::NO == sdwResultado )
               {
                  sdwResultado = VarEntorno::DEFECTO;
               }
               else if ( VarEntorno::REEMPLAZAR == sdwResultado )
               {
                  sdwResultado = VarEntorno::DEFECTO_REEMPLAZAR;
               }
               else if ( VarEntorno::ENTORNO == sdwResultado )
               {
                  sdwResultado = VarEntorno::DEFECTO_ENTORNO;
               }
               else if ( VarEntorno::ENTORNO_REEMPLAZAR == sdwResultado )
               {
                  sdwResultado = VarEntorno::DEFECTO_ENTORNO_REEMPLAZAR;
               }
            }
         }
      }

      if (
            ( VarEntorno::REEMPLAZAR == sdwAux)
            || ( VarEntorno::ENTORNO_REEMPLAZAR == sdwAux)
         )
      {
         ReemplazarCadena( cadReemplazar , cadReemplazo );
         if ( VarEntorno::NO == sdwResultado )
         {
            sdwResultado = VarEntorno::REEMPLAZAR;
         }
         else if ( VarEntorno::ENTORNO == sdwResultado )
         {
            sdwResultado = VarEntorno::ENTORNO_REEMPLAZAR;
         }
         else if ( VarEntorno::DEFECTO == sdwResultado )
         {
            sdwResultado = VarEntorno::DEFECTO_REEMPLAZAR;
         }
         else if ( VarEntorno::DEFECTO_ENTORNO == sdwResultado )
         {
            sdwResultado = VarEntorno::DEFECTO_ENTORNO_REEMPLAZAR;
         }
      }
      if (sdwResultado != VarEntorno::NO)
      {
         sdwAux=contieneVarEntorno(cadReemplazar);
      }
   }

   return(sdwResultado);
}

//----------------------------------------------------------------------------
SDWordType VarEntorno::resuelveVarEntorno(Cadena cadReemplazar, Cadena cadReemplazo)
{
   return( resuelveVarEntorno( "", cadReemplazar, cadReemplazo ) );
}

//----------------------------------------------------------------------------
SDWordType VarEntorno::resuelveVarEntorno(Cadena cadDefecto)
{
   return( resuelveVarEntorno( cadDefecto, "", "" ) );
}


// ===============================================
//  FIN DEL FICHERO - VarEntorno.cpp
// ===============================================
