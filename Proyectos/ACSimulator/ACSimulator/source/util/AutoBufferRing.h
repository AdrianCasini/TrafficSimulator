
#ifndef _AUTO_BUFFER_RING_H_
#define _AUTO_BUFFER_RING_H_

/**
 <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: BufferRing.h
 *
 * $Id: AutoBufferRing.h,v 1.1 2006/03/03 11:05:10 frocher Exp $
 *
 * DESCRIPCION:
 *   Buffer de tamano variable para la gestion de datos conjunta entre un
 *   productor y un consumidor. Gestiona de forma automatica el tamano de las
 *   reservas de memoria realizadas.
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

#include "BufferRing.h"

class AutoBufferRing : public BufferRing
{
      /*
       *   Esta clase anade a BufferRing la gestion automatica de las
       *   reservas de memoria realizadas. Esto es util para escenarios
       *   productor-consumidor en los que resulta dificil o incomoda la
       *   comunicacion del tamano de las reservas de memoria realizadas. Es
       *   decir, usar BufferRing implica que el productor debe hacer, por
       *   ejemplo, 'alloc(50)', mientras que el consumidor debe hacer
       *   'free(50)'. Usando la clase AutoBufferRing el consumidor solo debe
       *   usar 'free()'.
       *
       *   AutoBufferRing usa de forma transparente al productor y al
       *   consumidor 2 bytes de cada reserva de memoria para almacenar el
       *   tamano de esta. Esto significa que la mayor reserva de memoria
       *   posible equivale al maximo entero sin signo de dos bytes, o sea
       *   65536 bytes (64 Kb).
       */

   public:

      explicit  AutoBufferRing( int size );
      virtual  ~AutoBufferRing( void );

      /**
       *   Reserva \t size bytes de memoria. Si no hay espacio suficiente
       *   devuelve cero.
       */
      char *alloc( int size );

      /**
       *   Libera el espacio de memoria reservado mas antiguo.
       */
      void free( void );

      /**
       *   Libera el espacio de memoria reservado mas antiguo. El parametro
       *   en BufferRing ya no se usa en AutoBufferRing.
       */
      inline void free( int ) { free(); }


   private:

      static const int N = 2;
};

#endif // _AUTO_BUFFER_RING_H_
