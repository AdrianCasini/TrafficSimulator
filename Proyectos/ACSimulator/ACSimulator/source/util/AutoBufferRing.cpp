
// $Id: AutoBufferRing.cpp,v 1.2 2006/03/03 11:04:52 frocher Exp $

#include "AutoBufferRing.h"

#include <cassert>
#include <stdint.h>

AutoBufferRing::AutoBufferRing( int size )
   :
   BufferRing( size )
{}

AutoBufferRing::~AutoBufferRing( void )
{}

char *
AutoBufferRing::alloc( int size )
{
   assert( 0 < size and size <= 65536 );

   char *ptr = BufferRing::alloc( size + N );
   if( ptr )
   {
      // Se guarda el tamano reservado en los N primeros bytes.
      //
      // El uso de N = 2  =>  uint16_t
      //
      uint16_t *s = ( uint16_t * )( ptr );
      *s = uint16_t( size );

      // Se devuelve el puntero al area libre de la zona de memoria.
      //
      ptr += N;
   }

   return( ptr );
}

void
AutoBufferRing::free( void )
{
   if( freeMem() < size())
   {
      // Se recupera el tamano de la reserva mas antigua, usando sus N
      // primeros bytes.
      //
      // El uso de N = 2  =>  uint16_t
      //
      uint16_t *s = ( uint16_t * )( buffer() + begin());
      int size = int( *s );

      // Se libera la zona de memoria
      //
      BufferRing::free( size + N );
   }
}
