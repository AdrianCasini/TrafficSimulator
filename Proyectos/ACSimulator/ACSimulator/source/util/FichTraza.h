// =======================
//  FICHERO - FICHTRAZA.H
// =======================

#ifndef FICHTRAZA_H
#define FICHTRAZA_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <libgen.h>

// INCLUDES DEL PROJECTO

#include "../lang/Cadena.h"
#include "FichConfig.h"
#include "FicheroDatos.h"
#include "Directorio.h"
#include "FechaHora.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: FICHTRAZA.H
   *
   * DESCRIPCION: Se encarga de la grabacion de ficheros de log en disco. Se
   *  encarga de guardar un fichero de nombre mNombreFichero, con una longitud
   *  maxima de udwLongMaxFichero.
   *  Cuando el fichero supera el tamano udwLongMaxFichero, se modifica el nombre
   *  y se le coloca un 1 al final del nombre mNombreFichero1.
   *  Cuando el numero de ficheros supera el uwNumMaxFicheros, se borra el ultimo
   *  fichero de todos y se incrementa en uno todos los ficheros anteriores, en
   *  resumen.
   *
   *            mNombreFichero      -> Fichero actual de grabacion de log's
   *            mNombreFichero1     -> Fichero mas NUEVO de grabacion de log's
   *            .
   *            .
   *            .
   *            mNombreFicheroN     -> Fichero mas VIEJO de grabacion de log's.
   *  Todas las trazas se formatean segun la especificacion, utilizando los
   *  separadores sbSeparadorCampos y con una longitud maxima de uwLongMaxMensaje.
   *
   * FECHA CREACION: 17-05-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Oscar Lopez Calvo
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |                |                                                                  |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class FichTraza : public FicheroDatos
{
   // ATRIBUTOS
   // =====================================================

   ////////////////////   PRIVATE      ////////////////////
   private:

      // Declaracion de constantes, para el formateo de la traza
      static const UWordType  MAXIMA_LONGITUD_IDPROC      = 5;
      static const UWordType  MAXIMA_LONGITUD_TIPOTRAZA   = 5;

      Cadena      mNombreFichero;     //mNombreFichero.XXXXX
      Cadena      mExtension;         //XXXXXXX.mExtension
      Cadena      mNombreCompleto;    // mNombreFichero.mExtension
      SByteType   sbSeparadorCampos;
      UDWordType  udwLongMaxFichero;
      UWordType   uwNumMaxFicheros;
      UWordType   uwLongMaxMensaje;
      UDWordType  mudwTamFichero;

      //Variables para la funcion registra()
      Cadena      mTexto;
      Cadena      mProcRecortado;
      Cadena      mTipoTrRecortado;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC      ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

     /**
      * Constructor se inicializan a un valor conocido todas las variables de la
      *  clase excepto el nombre del fichero.
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      FichTraza();

     /**
      * Destructor de la clase
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual  ~FichTraza();


   ////////////////////   PRIVATE      ////////////////////
   private:

      // ===============   OPERADORES    ==================

     /**
      *  Retorna el numero de ficheros que tienen actualmente el sistema, sobre la
      *  base asignada. Es decir, el numero de ficheros de trazas que hay en disco
      *  pertenecientes al nombre asignado al objeto. Sin tener en cuenta el primer
      *  fichero de todos que no lleva sumada ninguna numeracion de esta manera:
      *
      *   <Contenido directorio>  (Numero total de ficheros = 2)
      *     NombreFicheroBase
      *     NombreFicheroBase1
      *     NombreFicheroBase2
      *
      * @param      mNombre -> Cadena de texto con el nombre base.
      * @return     UWordType -> Numero de ficheros que hay en disco.
      * @exception  Las generadas por el metodo Fichero::cerrar de la clase Fichero.
      */
      UWordType dameNumeroFicheros(Cadena& mNombre);

     /**
      * Asegura que el numero de ficheros de disco no supere el maximo permitido,
      * en el caso de superarlo, borra el fichero mas viejo (el ultimo fichero) y
      * realiza un movimiento de todos los ficheros (el segundo pasa a ser el tercero,
      * el tercero el cuarto...) . Tambien borra los ficheros que puedan estar
      * por encima del maximo permitido, manteniendo siempre el numero de ficheros
      * programado.
      *
      * @param       Ninguno
      * @return      Ninguno
      * @exception   Las generadas por los metodos Fichero::getTamano, Fichero::borrar y
      *              Fichero::renombrar de la clase Fichero.
      *
      */
      void organizaFicheros();

   ////////////////////   PUBLIC      ////////////////////
   public:

      // ===============   ACCESO    ==================

     /**
      *  Devuelve una cadena de texto con el nombre del fichero
      *  @param      Ninguno
      *  @return     Devuelve el objeto cadena con el contenido de texto del nombre del
      *              fichero.
      *  @exception  Ninguno
      */
      Cadena& getNombreFichero();

     /**
      *  Asigna un nombre de fichero, este nombre de fichero servira de base
      *  para la creacion de los n ficheros de la traza. Siguiente un formato
      *  especifico el sistema crea los nombres de los ficheros a partir de la
      *  base y sumandole un indice. Para facilitar el calculo separamos el nombre
      *  de la extension.
      *
      *  @param      mNombreFichero -> Cadena de texto con el nombre del fichero
      *  @return     Ninguno
      *  @exception  Ninguno
      *
      */
      void setNombreFichero(const Cadena& mNombreFichero);

     /**
      * Devuelve el separador de los campos, este separador identifica los diferentes
      * campos del mensaje de traza segun especificacion.
      * @param      Ninguno
      * @return     SByteType con el caracter separador
      * @exception  Ninguno
      */
      SByteType getSeparadorCampos();

     /**
      * Asigna el separador de campos utilizado en el metodo registra. Este separador
      * se coloca entre cada uno de los campos utilizado en el metodo registra,
      * el tamano que ocupa tambien se tiene en cuenta para recortarlo en caso de que
      * supere el maximo permitido.
      * @param      sbSeparadorCampos -> Caracter que separa los campos utilzados en registra.
      * @return     Ninguno
      * @Exception  Ninguno
      */
      void setSeparadorCampos(const SByteType sbSeparadorCampos);

     /**
      *  Retorna la longitud maxima del fichero en bytes.
      *  @param      Ninguno
      *  @return     UWordType -> Con la longiud maxima del fichero
      *  @exception  Ninguno
      */
      UDWordType getLongMaxFichero();

     /**
      *  Asigna la longitud maxima del fichero. La longitud maxima del fichero tendria
      *  que ser divisible entre la longitud maxima de la linea, de esta manera
      *  todos los ficheros ocuparian el mismo tamano. En caso de que no sea asi, si
      *  durante la escritura de un registro se supera la longitud maxima del fichero
      *  este no se recorta quedando el tamano incrementado como maximo en (longitud
      *  maxima de la linea).
      *
      *  @param       uwLongMaxFichero -> Longitud maxima del fichero en bytes.
      *  @return      Ninguno
      *  @exception   Ninguno
      */
      void setLongMaxFichero(const UDWordType udwLongMaxFichero);

     /**
      *  Retorna el numero maximo de ficheros.
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      UWordType getNumMaxFicheros();

     /**
      *  Asigna el numero maximo de ficheros. Este numero maximo se respeta desde el inicio
      *  es decir, en el metodo registra borra todos los ficheros que esten por encima
      *  del maximo. De esta manera si se reduce el numero de ficheros maximos, a
      *  la primera llamada del metodo registra se borran los archivos sobrantes.
      *
      *  En el caso de que el numero de ficheros sea igual a 0, no se realiza ninguna
      *  acccion ni de escritura ni de borrado de las trazas existentes en disco, se
      *  considera que la grabacion de trazas esta desactivada.
      *  @param      uwNumMaxFicheros -> Numero maximo de ficheros
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void setNumMaxFicheros(UWordType uwNumMaxFicheros);

     /**
      *  Retorna la longitud maxima del mensaje de traza.
      *  @param      Ninguno
      *  @return     UWordType -> Con la longitud maxima del mensaje
      *  @exception  Ninguno
      */
      UWordType getLongMaxMensaje();

     /**
      *  Asigna la longitud maxima del mensaje. Si un mensaje es superior se
      *  recorta.
      *  @param      uwLongMaxMensaje -> Longitud maxima del mensaje en bytes.
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void setLongMaxMensaje(UWordType uwLongMaxMensaje);

     /**
      *   Registra una traza en el fichero de traza asignado, selecciona el ultimo
      *   fichero libre y realiza la operacion de borrar el archivo mas viejo para
      *   no superar el maximo de ficheros permitidos.
      *   La cadena de texto se formatea de la siguiente manera:
      *
      *     Fecha - Hora / Sep / Proceso - Tarea /Sep /Tipo Traza / Mensaje
      *  @param    mProc   -> Cadena de texto que especifica el proceo o tarea
      *  @param    mTipoTr -> Cadena de texto con el tipo de traza
      *  @param    mMsj    -> Mensaje de usuario de la traza.
      *  @param    lMomento -> Momento de creacion de la traza.
      *  @return   Ninguno
      *  @exception Ninguno
      */
      void registra(const Cadena& mProc, const Cadena& mTipoTr, const Cadena& mMsj,
         const FechaHora& lMomento);
};

#endif // FICHTRAZA_H
