
// $Id: FichConfigGrupo.cpp,v 1.2 2006/03/16 08:49:11 frocher Exp $

#include "FichConfigGrupo.h"

FichConfigGrupo::FichConfigGrupo( void )
   :
   _linea   ( 0 ),
   _primero ( false )
{}


FichConfigGrupo::~FichConfigGrupo( void )
{}

bool
FichConfigGrupo::getPrimero( Cadena &parametro, Cadena &valor )
{
   _linea   = 1;
   _primero = true;

   return( leer( parametro, valor ));
}

bool
FichConfigGrupo::getSiguiente( Cadena &parametro, Cadena &valor )
{
   if( not( _primero ))
      return( getPrimero( parametro, valor ));
   else
      return( leer( parametro, valor ));
}

bool
FichConfigGrupo::leer( Cadena &parametro, Cadena &valor )
{
   Cadena ident;
   bool   hay_ident = false;

   setNombre( mNombreFichero );
   setEscritura( false );
   setLectura( true );
   abrir();
   setPosicion( 0 );

   IrLinea( _linea );
   while( not( hay_ident ) and not( esFinalFichero()))
   {
      if( dameIdentificador( ident ))
      {
         hay_ident = true;
         parametro = ident;
         valor.setEmpty();
         leerLinea( valor, false );
         valor.trim();
      }
      IrLinea( ++_linea );
   }

   cerrar();
   return( hay_ident );
}
