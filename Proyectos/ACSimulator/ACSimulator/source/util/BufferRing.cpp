
// $Id: BufferRing.cpp,v 1.2 2006/03/03 11:03:50 frocher Exp $

#include "BufferRing.h"

#include <cassert>
#include <cstring>

BufferRing::BufferRing( int size )
   :
   _size   ( size ),
   _buffer ( new char[_size] ),
   _begin  ( 0 ),
   _end    ( 0 ),
   _free   ( _size ),
   _ring   ( _size )
{
   memset( _buffer, 0, _size );
   _mtx.inicializar();
}

BufferRing::~BufferRing( void )
{
   delete[]( _buffer );
}

int
BufferRing::freeMem( void ) const
{
   int f;

   _mtx.lock();
   f = _free;
   _mtx.unlock();

   return( f );
}

char *
BufferRing::alloc( int size )
{
   assert( 0 < size and size <= _size );

   char *ptr = 0;

   _mtx.lock();
   {
      if( size <= _free ) // Hay espacio en el buffer
      {
         ptr = ( _begin < _end ? _alloc( size ) : _colla( size ));
      }
   }
   _mtx.unlock();

   return( ptr );
}

char *
BufferRing::_alloc( int size ) // _begin < _end
{
   char *ptr = 0;

   // Se intenta reservar espacio en el intervalo [_end,_size]
   //
   if( size <= _size - _end )
   {
      ptr    = _buffer + _end;
      _end  += size;
      _free -= size;
   }
   else
   {
      // No hay espacio suficiente. Se intenta ahora reservar espacio en el
      // intervalo [0,_begin]
      //
      if( size <= _begin )
      {
         // Hay espacio en el inicio del buffer. Se marca el espacio libre
         // que hay en [_end,_size] para liberarlo mas tarde. La marca se
         // hace con '_ring', que es el toque circular del asunto ...
         //
         // A partir de ahora, y hasta que esta reserva de memoria no sea
         // liberada, se cumple que:
         //
         //    _end < _begin
         //
         // por lo que las siguientes reservas de memoria seran realizadas
         // por el metodo '_colla' ('alloc' al reves).
         //
         _ring  = _end;
         ptr    = _buffer;
         _end   = size;
         _free -= size;
      }
   }

   return( ptr );
}

char *
BufferRing::_colla( int size ) // _end <= _begin
{
   char *ptr = 0;

   if( _begin == _end )
   {
      if( _free == _size ) // Buffer totalmente libre
      {
         ptr = _alloc( size );
      }
      else
      {
         // _begin y _end son iguales porque el buffer esta completamente
         // reservado y _end "ha dado la vuelta al buffer" (ring) hasta
         // encontrarse de nuevo con _begin. En este caso no hay mas espacio
         // disponible, aunque es posible que _free != _size. Esto es debido
         // a que una reserva no ha cabido completamente hasta el fin del
         // buffer y se ha empezado la nueva reserva desde el inicio del
         // buffer, en el que habia espacio libre.
         //
      }
   }
   else
   {
      // Se intenta reservar espacio en [_end,_begin]
      //
      if( size <= _begin - _end )
      {
         ptr    = _buffer + _end;
         _end  += size;
         _free -= size;
      }
   }

   return( ptr );
}

void
BufferRing::reset( void )
{
   _mtx.lock();
   {
      _begin = 0;
      _end   = 0;
      _free  = _size;
      _ring  = _size;
   }
   _mtx.unlock();
}

void
BufferRing::free( int size )
{
   assert( 0 < size );
   assert(( char * )( _begin + size ) <= ( _buffer + _size ));

   _mtx.lock();
   {
      memset(( void * )( _buffer + _begin ), 0, size );

      _free  += size;
      _begin += size;

      if( _free == _size )
      {
         // EL buffer ha quedado completamente libre. Reiniciamos la
         // estructura para aprovechar mejor las siguientes reservas de
         // memoria.
         //
         reset();
      }
      else if( _begin == _ring )
      {
         // Se ha llegado al final de la memoria ocupada. El inicio y el fin
         // vuelven a su sitio original.
         //
         _begin = 0;
         _ring  = _size;
      }
   }
   _mtx.unlock();
}
