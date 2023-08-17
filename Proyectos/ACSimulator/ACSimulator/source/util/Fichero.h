// =======================
//  FICHERO - FICHERO.H
// =======================

#ifndef FICHERO_H
#define FICHERO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include "stdio.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


// INCLUDES DEL PROJECTO
#include "../lang/Cadena.h"
#include "../lang/throwable/FormatoExcepcion.h"
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"
#include "../lang/throwable/SinRecursosExcepcion.h"
#include "../lang/throwable/NoAplicaExcepcion.h"
#include "../lang/throwable/ParametroExcepcion.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/LlenoExcepcion.h"
#include "../lang/throwable/PermisoExcepcion.h"
#include "../lang/throwable/InterrupcionExcepcion.h"
#include "../lang/throwable/OcupadoExcepcion.h"
#include "../lang/throwable/IOExcepcion.h"


// INCLUDES LOCALES

#include "FicheroConfiguracion.h"
#include "Directorio.h"

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: FICHERO.H
   *
   * DESCRIPCION: Clase para el manejo de ficheros.
   *
   *
   * FECHA CREACION: 14-07-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Oscar Lopez Calvo y Toni Bassas Serramia
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |            |                                         |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class Fichero : public FicheroConfiguracion
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   protected:

      SDWordType    fd;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

      Fichero();
      virtual  ~Fichero();

       // ===============   OPERACIONES    ==================
      /**
      * Abre un fichero.
      * @return Nada.
      * @throw OcupadoExcepcion
      * @throw NoAplicaExcepcion
      * @throw RangoExcepcion
      * @throw NoExisteExcepcion
      * @throw PermisoExcepcion
      * @throw FormatoExcepcion
      * @throw LlenoExcepcion
      * @throw MemoriaExcepcion
      * @throw SinRecursosExcepcion
      * @throw DesconocidoExcepcion
      */
      void abrir();

      /**
      * Crea un fichero.
      * @return Nada.
      * @throw OcupadoExcepcion
      * @throw NoAplicaExcepcion
      * @throw RangoExcepcion
      * @throw NoExisteExcepcion
      * @throw PermisoExcepcion
      * @throw FormatoExcepcion
      * @throw LlenoExcepcion
      * @throw MemoriaExcepcion
      * @throw SinRecursosExcepcion
      * @throw DesconocidoExcepcion
      */
      void crear();

      void crearFifo();

      /**
      * Cierra un fichero.
      * @return Nada.
      * @throw NoExisteExcepcion
      * @throw InterrupcionExcepcion
      * @throw IOExcepcion
      * @throw DesconocidoExcepcion
      */
      void cerrar();
      /**
      * Escribe en el fichero.
      * @param sdwOffset Posicion a escribir relativa al inicio del fichero.
      * @param buf Puntero al buffer de datos.
      * @param udwcount Longitud en bytes de los datos a escribir.
      * @return Numero de bytes que ha escrito.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw InterrupcionExcepcion
      * @throw IOExcepcion
      * @throw DesconocidoExcepcion
      */
      SDWordType escribir(SDWordType sdwOffset, const void *buf, UDWordType udwcount);
      SDWordType escribir(const void *buf, UDWordType udwcount);
      /**
      * Lee del fichero.
      * @param buf Puntero al buffer de datos a llenar con la lectura.
      * @param udwcount Longitud en bytes del Buffer de datos.
      * @return Numero de bytes leidos.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw InterrupcionExcepcion
      * @throw IOExcepcion
      * @throw DesconocidoExcepcion
      */
      SDWordType leer(void *buf, SDWordType udwcount);

      /**
      * Escribe en fichero la cadena mLinea y anade el caracter fin de linea
      * @param linea Texto a escribir
      * @return Numero de bytes que ha escrito
      * @exception Las del metodo escribir
      */
      SDWordType escribirLinea( const Cadena& linea );

   /**
   * Lee del fichero una linea ( hasta el caracter fin de linea )
   * @param Cadena donde se dejara el texto leido
   * @param bFinalLinea. Si toma el valor true (valor por defecto) el caracter
   * de fin de linea se copia en la cadena
   * @return True si se ha leido una linea; false en otro caso
   *
   */
   bool leerLinea( Cadena& linea, bool bFinalLinea = true );

   /**
   * Lee del fichero una linea ( hasta el caracter fin de linea ) o el size maximo.
   * @param buf - Buffer donde se dejara el texto leido.
   * @param bFinalLinea. Si toma el valor true (valor por defecto) el caracter
   * de fin de linea se copia en la cadena
   * @param size. Tamano maximo de lectura.
   * @return True si se ha leido una linea; false en otro caso
   */
   SDWordType leerLinea( SByteType *buf, SDWordType size, bool bFinalLinea = true );

/**
 * Borrar el fichero.
 * @return Nada.
 * @throw PermisoExcepcion
 * @throw NoAplicaExcepcion
 * @throw OcupadoExcepcion
 * @throw MemoriaExcepcion
 * @throw RangoExcepcion
 * @throw NoExisteExcepcion
 * @throw IOExcepcion
 * @throw DesconocidoExcepcion
*/
      void borrar();

      /**
      * Fuerza el volcado del buffer en el disco
      */
       void sincronizar();

      /**
      * Comprueba si es el final del fichero.
      * @return Si es final de fichero.
      * @throw Lanza las mismas excpeciones que mover y leer.
      */
      bool esFinalFichero();

     /**
      * Informa si el fichero esta abierto o no
      * @return true si el fichero esta abierto; false en otro caso
      */
      bool isAbierto() const;
      /**
      * Incrementa o decrementa el puntero de escritura/lectura del fichero.
      * @param sdwOffset Posicion a mover el puntero relativa a la posicion actual ej: -1 o +3 .
      * @return Nada.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw DesconocidoExcepcion
      */
      void mover(SDWordType sdwOffset);

      /**
      * Obtiene la posicion del puntero de escritura/lectura del fichero.
      * @return Posicion del puntero de escritura/lectura.
      * @throw Lanza las mismas excpeciones que mover.
      */
      SDWordType getPosicion ();
      /**
      * Obtiene el siguiente caracter del fichero.
      * @return Caracter del fichero o EOF.
      * @throw Lanza las mismas excpeciones que leer.
      */
      SWordType getCaracter();

      /**
      * Mueve puntero de escritura/lectura del fichero.
      * @param sdwOffset Posicion a mover el puntero relativa al inicio del fichero.
      * @return Nada.
      * @throw Lanza las mismas excepciones que mover.
      */
      void setPosicion (SDWordType sdwOffset);
      /**
      * Mueve puntero de escritura/lectura del fichero al final del fichero.
      * @return Nada.
      * @throw Lanza las mismas excepciones que mover
      */
      void irFin(void);

      /**
      * Mueve puntero de escritura/lectura del fichero al inicio del fichero.
      * @return Nada.
      * @throw Lanza las mismas excepciones que mover
      */
      void irInicio(void);
      /**
      * Escribe un texto en el fichero.
      * @param mtext Cadena a escribir.
      * @param sdwLongitud longitud de texto a escribir. -1 significa toda la cadena.
      * @return Numero de bytes que ha escrito.
      * @throw Lanza las mismas excepciones que escribir(const void *buf, UDWordType udwcount).
      */
      SDWordType escribir(const Cadena& mTexto, SDWordType sdwLongitud=-1);
      /**
      * Obtiene el tamanyo del fichero.
      * @return Tamanyo del fichero.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw PermisoExcepcion
      * @throw MemoriaExcepcion
      * @throw DesconocidoExcepcion
      */
      UDWordType getTamano();
      /**
      * Salta a la siguiente linea de texto del fichero.
      * @throw Lanza las mismas excepciones que leer.
      */
      void saltaLinea();

      SDWordType    getFileDescriptor(void) {return(fd);};

   private:

      SDWordType    getFlags();
      void          abrir(const bool bCrear);
      SDWordType    setPosicion (SDWordType sdwOffset,UDWordType udwPosicion);
};

#endif // FICHERO_H


