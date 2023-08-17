
#ifndef _BUFFER_RING_H_
#define _BUFFER_RING_H_

/**
 <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: BufferRing.h
 *
 * $Id: BufferRing.h,v 1.3 2006/03/03 11:03:26 frocher Exp $
 *
 * DESCRIPCION:
 *   Buffer de tamano variable para la gestion de datos conjunta entre un
 *   productor y un consumidor.
 *
 * FECHA CREACION: 1-3-2006
 *
 * AUTOR DISENYO:
 *
 * PROGRAMADOR:
 *
 * MODIFICATIONES:
 *
 *    |   FECHA    |   AUTOR   |  RAZON                                   |
 *    ---------------------------------------------------------------------
 *    | DD-MM-YYYY |           |                                          |
 *    ---------------------------------------------------------------------
 * </PRE>
 */

#include "../thread/Mutex.h"

class BufferRing
{
      /*
       *   Esta clase implementa un buffer circular. La memoria del buffer se
       *   reserva con la llamada alloc y se libera con free, VRS. Es
       *   importante resaltar que el funcionamiento de este buffer es muy
       *   similar al de una cola en lo que a reserva de memoria se
       *   refiere. Cada alloc debe tener su correspondiente free, y los
       *   free's deben ser llamados en el mismo orden en que fueron llamados
       *   los alloc's. Es decir, a la secuencia:
       *
       *      alloc( 20 ) ... alloc( 50 ) ... alloc( 9 )
       *
       *   debe ser corresponder la secuencia:
       *
       *      free( 20 ) ... free( 50 ) ... free( 9 )
       *
       *   En resumen, NO SE PUEDE LIBERAR CUALQUIER ZONA DE MEMORIA, sino
       *   que se debe liberar en primer lugar la zona de memoria que fue
       *   reservada en primer lugar.
       *
       *   Este BufferRing esta pensado para escenarios productor-consumidor
       *   en el que se garantiza que los datos generados por el productor no
       *   son sobreescritos. Si el consumidor no es capaz de seguir el ritmo
       *   del productor, llegara un momento en el que todo el BufferRing
       *   estara ocupado y 'alloc' retornara cero. El productor debera
       *   detectar este importunio y tratarlo a su libre albedrio, pudiendo
       *   lanzar una excepcion, escribir una traza en rojo o hacer un reset
       *   del BufferRing y seguir escribiendo.
       *
       *   Existe una clase derivada de BufferRing, llamada AutoBufferRing,
       *   que gestiona de forma automatica el tamano de las reservas de
       *   memoria realizadas, de forma tal que el metodo free() libera
       *   automaticamente el espacio de memoria que toca. Vea el fichero
       *   AutoBufferRing.h para mas informacion.
       */

   public:

      explicit  BufferRing( int size );
      virtual  ~BufferRing( void );

      /**
       *   Reserva \t size bytes de espacio de memoria. Si no hay espacio
       *   suficiente devuelve cero.
       */
      virtual char *alloc( int size );

      /**
       *   Libera el primer espacio de memoria ocupado
       */
      virtual void free( int size );

      /**
       *   Inicializa por completo la estructura de datos.
       */
      void reset( void );

      /**
       *   Consulta el tamano del buffer.
       */
      inline int size( void ) const { return( _size ); }

      /**
       *   Consulta el tamano de memoria disponible.
       */
      int freeMem( void ) const;


   protected:

      char *_alloc( int size );
      char *_colla( int size );

      inline char *buffer ( void ) const { return( _buffer ); }
      inline int   begin  ( void ) const { return( _begin  ); }


   private:

      int   _size;    // Tamano del buffer
      char *_buffer;  // El buffer en si
      int   _begin;   // Inicio de la zona ocupada
      int   _end;     // Fin de la zona ocupada
      int   _free;    // Bytes libres
      int   _ring;    // Cola del buffer sin ocupar que debe ser liberada

      Mutex _mtx;
};

#endif // _BUFFER_RING_H_
