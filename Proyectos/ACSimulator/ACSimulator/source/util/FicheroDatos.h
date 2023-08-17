// =======================
//  FICHERODATOS - FICHERODATOS.H
// =======================

#ifndef FICHERODATOS_H
#define FICHERODATOS_H

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
   * FICHERODATOS: FICHERODATOS.H
   *
   * DESCRIPCION: Clase optimizada para la lectura de ficheros de datos
   *
   *
   * FECHA CREACION: 18-11-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Marc Coll y Javier Tarradas
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
class FicheroDatos : public FicheroConfiguracion
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   private:
      static const UWordType MAX_LONG_LINEA = 1024;


   ////////////////////   PROTECTED      ////////////////////
   protected:

      FILE*        mFichero;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

      FicheroDatos();
      virtual  ~FicheroDatos();

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
      * Permite controlar el buffer interno del fichero
      *  @param buffer -> El buffer a usar (NULL para desactivarlo)
      *  @param sdwTamanyo -> El tamanyo del buffer (0 para desactivarlo)
      */
      void setBuffer (char* buffer, SDWordType sdwTamanyo);

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
      *  Escribe el contenido del buffer buf, desde la posicion actual
      *  del fichero. Escribe un total de udwCount bytes
      *  @param      buf Puntero a los datos
      *  @param      udwCount Longitud de los datos
      *  @return     El numero de bytes escritos
      *  @exception  Ninguno
      */
      SDWordType escribir(const void *buf,SDWordType udwCount);

     /**
      *  Escribe el contenido del buffer buf desde la posicion indicada
      *  Escribe un total de udwCount bytes
      *  @param      sdwOffset La posicion inicial
      *  @param      buf Puntero a los datos
      *  @param      udwCount Longitud de los datos
      *  @return     El numero de bytes escritos
      *  @exception  Ninguno
      */
      SDWordType escribir(SDWordType sdwOffset,const void *buf,SDWordType udwCount);

     /**
      *  Escribe el contenido de la cadena desde la posicion actual
      *  del fichero.
      *  @param      lValor Cadena de texto con los datos para escribir
      *  @return     El numero de bytes escritos
      *  @exception  Ninguno
      */
      SDWordType escribir(const Cadena& lValor);

      /** Fuerza el volcado del buffer en el disco */
      void sincronizar();

      /**
      * Lee del fichero una linea (hasta el caracter fin de linea)
      * @param linea Buffer donde se dejara el texto leido. Deberia ser un puntero
      * NULL o un buffer reservado con malloc. De lo contrario podria petar si
      * la linea no cabe en el buffer. Debera ser liberado con un free al terminar.
      * @param linSz Tamanyo del buffer. 0 si se pasa un puntero nulo. Si el
      * tamanyo del buffer es insuficiente, se reservara mas memoria y se
      * actualizara el valor del tamanyo.
      * @param bFinalLinea Si toma el valor true (valor por defecto),
      * el caracter de fin de linea se copia en la cadena
      * @return SDWordType El numero de bytes leidos, 0 si no se ha leido nada
      * @exception LLenoExcepcion Si la linea no cabe en el buffer
      */
      SDWordType leerLinea(PSByteType* linea, size_t* linSz, bool bFinalLinea=true);

     /**
      *  Borra toda la linea desde la posicion actual del cursor hasta el
      *  final de la linea ('\n') o el final de fichero. El borrado
      *  consiste con colocar el caracter ' ', en todos los datos de la
      *  linea
      *  @param      lFinalLinea true indica que tambien se borra el ('\n')  que
      *              marca el final de linea y false que no
      *  @return     UWordType Numero de caracteres borrados
      *  @exception  Ninguno
      */
      UWordType borrarLinea(bool lFinalLinea = true);

     /**
      *   Borra desde la posicion actual del cursor el caracter y desplaza toto el texto
      *  una posicion arriba, el fichero no se trunca y conserva el mismo tamano.
      *  La posicion del cursor se queda en la posicion actual
      *  @param      Ninguno
      *  @return     true En el caso de que se pueda borrar false en caso contrario
      *  @exception  Ninguno
      */
      bool borrarCaracter();

     /**
      *  Borra desde la posicion actual del fichero luwnUmCaracteres, todo el resto
      *  del texto se desplaza una posicion para arriba
      *  La posicion del cursor se queda en la posicion actual
      *  @param      luwNumCaracteres Numero de caracteres para insertar
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      SWordType borrarCaracteres(UWordType luwNumCaracteres);

     /**
      *  Inserta una linea desde la posicion actual del cursor, desplaza todo el texto
      *  n posiciones a la derecha dependiento del tamano del texto insertado
      *  @param      lTexto Cadena de texto para insertar
      *  @param      lFinalLinea le indicamos si se inserta el '\n' al final de la linea o no
      *  @return     -1 en caso de error o el tamano del texto insertado
      *  @exception  Ninguno
      */
      SDWordType insertarLinea(const Cadena& lTexto,bool lFinalLinea = true);

     /**
      *  Inserta luwNumCaracteres en el fichero desplazando el texto hacia la derecha
      *  @param      buf Puntero a los datos que se insertan
      *  @param      luwNumCaracteres Numero de caracteres insertados
      *  @return     -1 en caso de error o el tamano del texto insertado
      *  @exception  Ninguno
      */
      SWordType insertarCaracteres(const void *buf,SDWordType luwNumCaracteres);

     /**
      *  Lee del fichero una linea (hasta el caracter fin de linea),
      *  la linea no puede ocupar mas de MAX_LONG_LINEA
      *  @param      lLinea Cadena donde se dipositara la linea leida
      *  @param bFinalLinea Si toma el valor true (valor por defecto),
      *   el caracter de fin de linea se copia en la cadena
      *  @return True si se ha leido una linea; false en otro caso
      *  @exception  Ninguno
      */
      bool leerLinea(Cadena& lLinea,bool bFinalLinea = true);

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
      void irFin();

      /**
      * Mueve puntero de escritura/lectura del fichero al inicio del fichero.
      * @return Nada.
      * @throw Lanza las mismas excepciones que mover
      */
      void irInicio();

      /**
      * Obtiene el siguiente caracter del fichero.
      * @return Caracter del fichero o EOF.
      * @throw Lanza las mismas excpeciones que leer.
      */
      SWordType getCaracter();

      /**
      * Salta a la siguiente linea de texto del fichero.
      * @throw Lanza las mismas excepciones que leer.
      */
      void saltaLinea();

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

   private:

      Cadena         getModo (bool bCrear);
      SDWordType setPosicion (SDWordType sdwOffset,UDWordType udwPosicion);
      void               errorAbrirCrear (int err);

      PSByteType mpsbBuffer;
      size_t     mLongBuffer;
};

#endif // FICHERODATOS_H
