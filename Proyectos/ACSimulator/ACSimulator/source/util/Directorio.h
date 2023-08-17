//---------------------------------------------------------------------------
// Directory.h
//---------------------------------------------------------------------------

#ifndef DIRECTORY_H
#define DIRECTORY_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>





// INCLUDES DEL PROJECTO

#include "../util/Fichero.h"
#include "../lang/Cadena.h"
#include "../lang/Types.h"

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

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: Directorio.h
  *
  * DESCRIPCION:  Clase para el acceso a todos los elementos de un directorio de Unix.
  *                      Con esta clase se puede eliminar o mover los elementos de un directorio.
  *                      Ademas se incluye la interficie basica de acceso a ficheros.
  *
  * FECHA CREACION: 25-10-2007
  *
  * AUTOR DISENYO: Javier Tarradas
  *
  * PROGRAMADOR: Javier Tarradas
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |        |                   |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class Directorio
{

// CLASES INTERNAS
// =====================================================

// ATRIBUTOS
// =====================================================

  ////////////////////   PUBLIC       ////////////////////
  public:

   /**
     * Enumeracion de tipos de entradas que podemos encontrar en un directorio.
     **/
    enum TipoEntradas
    {
      TODO=1,   ///< Cualquier tipo de entrada.
      FICHERO,   ///< Entrada del tipo fichero.
      DIRECTORIO   ///< Entrada del tipo directorio.
    };

  ////////////////////   PROTECTED    ////////////////////
  protected:

  ////////////////////   PRIVATE      ////////////////////
  private:

      struct dirent **namelist;  ///< Lista de texto utilizado para la scanear un directorio.
      SDWordType sdwNumFiles; ///< El numero de elementos dentro de la lista.
      SDWordType sdwIndexFile; ///< Lugar donde se encuentra.
      Cadena mcadRuta; ///< Path desde donde se leen todos los ficheros o directorios.

// METODOS
// =====================================================

  ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

    /**
      *  Constructor de la clase.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Ninguno
      */
      Directorio ( );
    /**
      *  Destructor de la clase.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Ninguno
      */
      ~Directorio ( );

      // ===============   OPERACIONES    ==================

    /**
      * Metodo para inicializar todos los variables de la clase.
      * @param   cadDir Cadena con la ruta del directorio.
      * @return  Ninguno
      */
      void inicializar ( const Cadena& cadDir );
    /**
      * Metodo para leer el fichero al cual estamos leyendo.
      * @param   Ninguno
      * @return  La ruta del directorio que estamos trabajando.
      */
      Cadena getRutaDirectorio ( );
    /**
      * Metodo para asignar el valor del directorio a leer.
      * @param   cadDir Cadena con la ruta del directorio.
      * @return  Ninguno
      */
      void setRutaDirectorio ( const Cadena& cadDir );
    /**
      * Proceso para leer todos los ficheros y directorios del path asignado.
      * @param   cadDir Cadena con la ruta del directorio.
      * @return  Retorna el numero de ficheros y directorios leidos.
      */
      SDWordType leer ( );
    /**
      * Metodo que retorna el siguiente elemento del directorio.
      * @param   eTipos Cadena con la ruta del directorio.
      * @return  Retorna el siguiente elemento.
      */
      Cadena siguiente ( enum TipoEntradas eTipos );
    /**
      * Metodo que valida la existencia de un fichero o un directorio.
      * @param   cadDir Cadena con la ruta del directorio.
      * @param   eTipos El tipo de elemento que buscamos.
      * @return  La existencia o no del elemento buscado. Puede dar error si
      *            se trata de un directorio pero se busca un fichero.
      */
      static bool existe ( Cadena cadDir, enum TipoEntradas eTipos=TODO );
    /**
      * Metodo para crear un directorio.
      * @param   cadDestino Cadena donde se quiere crear el directorio.
      * @return  Si ha existido algun problema a la hora de crear el directorio.
      */
      static bool crearDirectorio ( Cadena cadDestino );
    /**
      * Metodo para crear un fichero vacio.
      * @param   cadDestino Cadena donde se quiere crear el fichero.
      * @return  Si ha existido algun problema a la hora de crear el fichero.
      */
      bool crearFichero ( const Cadena& cadDestino );
    /**
      * Metodo para mover un fichero o un directorio.
      * @param   cadDestino Cadena donde se quiere mover el fichero o directorio.
      * @return  Si ha ido bien.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw InterrupcionExcepcion
      * @throw LlenoExcepcion
      * @throw DesconocidoExcepcion
      */
      bool mover ( Cadena& cadDestino );
    /**
      * Metodo para mover o renombrar un fichero.
      * @param   cadOrigen Cadena origen del fichero.
      * @param   cadDestino Cadena donde se quiere mover o renombrar el fichero.
      * @return  Si ha ido bien.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw InterrupcionExcepcion
      * @throw LlenoExcepcion
      * @throw DesconocidoExcepcion
      */
      static bool mover ( Cadena& cadOrigen, Cadena& cadDestino );
    /**
      * Metodo para copiar un fichero.
      * @param   cadOrigen Cadena origen del fichero.
      * @param   cadDestino Cadena donde se quiere copair el fichero.
      * @return  Si ha ido bien.
      * @throw NoExisteExcepcion
      * @throw ParametroExcepcion
      * @throw RangoExcepcion
      * @throw NoAplicaExcepcion
      * @throw InterrupcionExcepcion
      * @throw LlenoExcepcion
      * @throw DesconocidoExcepcion
      */
      static bool copiar ( Cadena& cadOrigen, Cadena& cadDestino );

    /**
      * Metodo eliminar un directorio o fichero de la lista leida.
      * @param   cadDir Cadena con la ruta del fichero o directorio.
      * @param   eTipos El tipo de elemento que queremos eliminar.
      * @return  Si se ha podido eliminar el fichero.
      */
      bool eliminar ( enum TipoEntradas eTipos );
    /**
      * Metodo para eliminar los ficheros de un directorio. No se eliminan los
      *   subdirectorios ni su contenido.
      * @param cadPath Path del directorio.
      * @return  Si se ha podido elimar los ficheros.
      */
      static bool eliminarContenidoDirectorio ( Cadena cadPath );
    /**
      * Metodo para eliminar un directorio o fichero.
      * @param cadPath Path del fichero o directorio.
      * @param   eTipos El tipo de elemento que queremos eliminar.
      * @return  Si se ha podido eliminar el fichero.
      */
      static bool eliminar ( Cadena cadPath, enum TipoEntradas eTipos );

    /**
      * Metodo para eliminar un fichero. Se recomienda utilizar la funcion eliminar.
      * @cadPath cadPath Path del fichero.
      * @return  Si se ha podido eliminar el fichero.
      */
      static bool eliminarFichero ( const Cadena& cadPath );
    /**
      * Metodo para eliminar un fichero. Se recomienda utilizar la funcion eliminar.
      * @param cadPath Path del directorio.
      * @return  Si se ha podido eliminar el fichero.
      */
      static bool eliminarDirectorio ( const Cadena& cadPath );

/******************************** VERSIONES SEGURAS ************************************/


/**
      * Metodo eliminar un directorio o fichero de la lista leida.
      * @param   cadDir Cadena con la ruta del fichero o directorio.
      * @param   eTipos El tipo de elemento que queremos eliminar.
      * @return  Si se ha podido eliminar el fichero.
      */
      bool eliminarSeguro ( enum TipoEntradas eTipos );
    /**
      * Metodo para eliminar los ficheros de un directorio. No se eliminan los
      *   subdirectorios ni su contenido.
      * @param cadPath Path del directorio.
      * @return  Si se ha podido elimar los ficheros.
      */
      static bool eliminarContenidoDirectorioSeguro ( Cadena cadPath );
    /**
      * Metodo para eliminar un directorio o fichero.
      * @param cadPath Path del fichero o directorio.
      * @param   eTipos El tipo de elemento que queremos eliminar.
      * @return  Si se ha podido eliminar el fichero.
      */
      static bool eliminarSeguro ( Cadena cadPath, enum TipoEntradas eTipos );

    /**
      * Metodo para eliminar un fichero. Se recomienda utilizar la funci�n eliminar.
      * @cadPath cadPath Path del fichero.
      * @return  Si se ha podido eliminar el fichero.
      */
      static bool eliminarFicheroSeguro ( const Cadena& cadPath );

     /**
      * Metodo para capturar el resultado de la funcion stat de un fichero.
      * @cadPath cadPath Path del fichero.
      * @param mstat Estado del fichero.
      * @return  Si se ha podido leer fichero.
      */
      static bool getStat ( const Cadena& cadPath, struct stat& mstat );

      // =================   CONSULTA      ===================

   ////////////////////   PRIVATE      ////////////////////
   private:

    /**
      * Metodo para liberar la memoria almacenada por la lectura del directorio.
      * @param   Ninguno
      * @return  Ninguno
      */
      void freeNameList ( void );
    /**
      * Metodo para buscar el siguiente fichero o directorio dependiendo del parametro.
      * @param   eTipos Tipo de fichero o directorio.
      * @return  El indice que indica la posicion dentro de la lista de ficheros
      *         o directorios. Retorna -1 si es el final de la lista.
      */
      SDWordType  findNext ( enum TipoEntradas eTipos );
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif // DIRECTORIO_H

//---------------------------------------------------------------------------
// Directorio.h
//---------------------------------------------------------------------------
