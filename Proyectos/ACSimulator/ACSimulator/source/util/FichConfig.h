#ifndef FICHCONFIG_H
#define FICHCONFIG_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <iostream>

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"
#include "../lang/Cadena.h"
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/ParametroExcepcion.h"
#include "../util/Directorio.h"
#include "../util/FicheroDatos.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   FICHCONFIG.H
  *
  * DESCRIPCION: Tratamiento de los ficheros de configuracion (.cfg) segun la
  *  guia de estilos de codificacion.
  *
  * FECHA CREACION: 20-04-2005
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |                |                                                             |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class FichConfig : protected FicheroDatos
{

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   protected:
       static const SByteType  DEF_SEPARADOR_VALOR    = ';';
       static const SByteType  DEF_CARACTER_RESERVADO = '\\';

      // Contiene el nombre del fichero sobre el que se esta trabajando
      Cadena      mNombreFichero;
      SByteType   sbSeparador;
      SByteType   sbReservado;
      UDWordType  udwUltimaPosicion;
      UDWordType  udwUltimaLinea;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC      ////////////////////
   public:

      // =================   CICLO DE VIDA      ===================

     /**
      * Constructor
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      FichConfig();

     /**
      * Destructor
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      virtual ~FichConfig();

      // =================   OPERACIONES      ===================

     /**
      *  Cuenta el numero de lienas de un fichero .cfg
      *  @param      Ninguno
      *  @return     CUDWordType -> El numero total de lineas
      *  @exception  Ninguno
      */
      CUDWordType getNumTotalLineas();

      // =================   OPERACIONES      ===================

     /**
      * Lee el valor de un numero de parametro iCampo,de la linea con el Identificador
      * IdParam dejando el resultado sobre Param
      * @param   mIdParam -> Nombre del identificador.
      * @param   sdwCampo -> Numero del campo Valor que se quiere leer, el primero es el 1
      *          en el caso de que el parametro sea incorrecto se retorna un false.
      * @param   mParam -> Parametro de salida donde se coloca el Param
      * @param   mDefecto  -> En el caso de no encontrar el parametro asigna mDefecto
      *         como si fuera el parametro leido
      * @return  true -> Elemento encontrado
      * @return  false -> Elemento NO encontrado
      * @exception  Todas las generadas por Fichero::abrir Fichero::cerrar
      * @exception  Todas las generadas por Fichero:setNombre Fichero::setPosicion Fichero::getCaracter
      */
      bool getParam(const Cadena& mIdParam,const SDWordType sdwCampo,Cadena& mParam,const Cadena& mDefecto);

     /**
      * Lee el valor de un numero de parametro iCampo,del n- linea numLinea
      * dejando el resultado sobre Param
      * @param   uwNumLinea -> Numero de linea donde esta el parametro, empezando por la 1,
      *          en el caso de que el parametro sea incorrecto se genera la excepcion
      *          ParametroExcepcion
      * @param   sdwCampo -> Numero del campo Valor que se quiere leer, el primero es el 1
      *          en el caso de que el parametro sea incorrecto se retorna un false.
      * @param   mParam -> Parametro de salida donde se coloca el Param
      * @param   mDefecto  -> En el caso de no encontrar el parametro asigna mDefecto
      *         como si fuera el parametro leido
      * @return  true -> Elemento encontrado
      * @return  false -> Elemento NO encontrado
      * @exception ParametroExcepcion  "Parametros incorrectos No existe la linea."
      * @exception ParametroExcepcion  "Parametros incorrectos no se puede leer la linea N-0"
      * @exception  Todas las generadas por Fichero::abrir Fichero::cerrar
      * @exception  Todas las generadas por Fichero:setNombre Fichero::setPosicion Fichero::getCaracter
      */
      bool getParam(const UWordType uwNumLinea,const SDWordType sdwCampo, Cadena& mParam,const Cadena& mDefecto);

     /**
      *  Devueve la cadena de texto sobre el identificador ident, del numero de linea
      *  numLinea.
      *
      *  @param   uwNumLinea -> Numero de linea donde esta el parametro, empezando por la 1,
      *           en el caso de que el parametro sea incorrecto se genera la excepcion
      *           ParametroExcepcion
      *  @param   mIdent -> Parametro de salida con el identificador de texto de la linea
      *           correspondiente.
      *  @return     true -> Elemento encontrado
      *  @return     false -> Elemento NO encontrado
      *  @exception  ParametroExcepcion "Parametros incorrectos."
      *  @exception  Todas las generadas por Fichero::abrir Fichero::cerrar
      *  @exception  Todas las generadas por Fichero:setNombre Fichero::setPosicion Fichero::getCaracter
      */
      bool getId(const UWordType uwNumLinea, Cadena& mIdent) ;

      bool setParam(const Cadena& mIdParam,const SDWordType sdwCampo,const Cadena& mValor);

      // =================   ACCESO      ===================

     /**
      *  Escribe el nombre del fichero .cfg
      *  @param      nombreFichero -> Nombre completo con el path incluido del fichero de
      *              configuracion.
      *  @return     Ninguno
      *  @exception  Todas las del metodo TratarMensajeError
      */
      void setFicheroCfg(const Cadena& nombreFichero);

     /**
      *  Asigna un caracter como separador entre campos del fichero
      *  @param      sbSeparador -> Caracter identificador
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void  setCaracterSeparador(SByteType sbSeparador);

     /**
      *  Asigna un caracter como reservado, el caracter que hay acontinuacion es ignorado
      *  @param      sbReservado -> Caracter reservado
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void  setCaracterReservado(SByteType sbReservado);

     /**
      *  Retorna el valor del caracter separador
      *  @param      Ninguno
      *  @return     SByteType con el valor del caracter separador
      *  @exception  Ninguno
      */
      SByteType  getCaracterSeparador();

     /**
      * Retorna el caracter reservado
      *  @param      Ninguno
      *  @return     SByteType con el caracter reservado
      *  @exception  Ninguno
      */
      SByteType  getCaracterReservado( );

    ////////////////////   PRIVATE      ////////////////////
    protected:

      // =================   OPERACIONES      ===================

      /**
      *  Averigua si el parametro de entrada es aceptado en el campo ID del fichero
      *  cfg
      *  @param      sbCaracter -> Caracter de entrada
      *  @return     true -> en el caso de encontrar el dato
      *  @return     false -> no se pude encontrar el dato
      *  @exception  Ninguno
      */
      bool caracterValidoId(const SByteType sbCaracter);

     /**
      * Averigua si el parametro de entrada es aceptado en el campo VALOR del
      * fichero cfg.
      *
      *  @param      sbCaracter -> Caracter de entrada
      *  @return     true -> en el caso de encontrar el dato
      *  @return     false -> no se pude encontrar el dato
      *  @exception  Ninguno
      */
      bool caracterValidoParam(const SByteType sbCaracter);

     /**
      *  Lee el parametro numero udwCampo de la linea con el identificador mId y
      *  lo coloca sobre la cadena mStr.
      *  @param      mId   -> Identificador del parametro qe se busca
      *  @param      udwCampo -> Numero de parametro del mId especificado
      *  @param      mStr  -> Objeto cadena donde se deposita el resultado
      *  @return     true -> en el caso de encontrar el dato
      *  @return     false -> no se pude encontrar el dato
      *  @exception  Ninguno
      */
      bool dameParametro( const Cadena& mId,const UDWordType udwCampo, Cadena& mStr);

     /**
      *  De la linea actual del fichero devuelve el numero de parametro udwCampo y
      *  lo coloca en la Cadena mStr.
      *  @param  udwCampo -> Numero del campo Valor que se quiere leer, el primero es el 1
      *          en el caso de que el parametro sea incorrecto se retorna un false.
      *  @param      mStr -> Cadena de salida con el valor del campo
      *  @return     true -> en el caso de encontrar el dato
      *  @return     false -> no se pude encontrar el dato
      *  @exception  Ninguno
      */
      bool dameParametro( const UDWordType udwCampo, Cadena & mStr );

     /**
      * Devuelve el identificador que se encuentra en la linea actual del fichero
      *  @param      mStr -> Objeto Cadena donde se guarda el identificador
      *  @return     true -> en el caso de encontrar el dato
      *  @return     false -> no se pude encontrar el dato
      *  @exception  Ninguno
      */
      bool dameIdentificador( Cadena& mStr);

     /**
      *  Obtienes la ultima posicion del fichero leido , de esta manera cuando
      *  se accede a los metodos getParam(N-linea...) y getId(N-linea...), se
      *  incrementa la velocidad cuando se accede consecutivamente a los numeros
      *  de linea.
      *  @param      Ninguno
      *  @return     UDWordType Con la posicon del cursor
      *  @exception  Ninguno
      */
      UDWordType getUltPos();

      /**
       *  Obtienes la ultima posicion del fichero leido , de esta manera cuando
       *  se accede a los metodos getParam(N-linea...) y getId(N-linea...), se
       *  incrementa la velocidad cuando se accede consecutivamente a los numeros
       *  de linea.
       *  @param      Ninguno
       *  @return     UDWordType Con la posicon del cursor
       *  @exception  Ninguno
       */
      UDWordType getUltLinea();

     /**
      *  Gurada el numero de lineas que se han saltado juntamente con la
      *  posicion del cusor
      *  @param      udwUltLinea Ultima linea saltada
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void setUltLinea(UDWordType udwUltLinea);

     /**
      *  Guarda el la posicion del cursor desde la ultima operacion realizada.
      *  @param      udwUltPos Ultima posicion del cursor
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void setUltPos(UDWordType udwUltiPos);

     /**
      *  Salta a la linea que recibe como parametro, consulta la ultima linea consultada
      *  y si es menor de la que se quiere ir, coloca el cursor en la posicion adecuada
      *  y tan solo salta las lineas necesarias, incrementando la velocidad de acceso
      *  @param      udwNumLinea Numero de linea de la 0 a la getNumLIneas
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void IrLinea(UDWordType udwNumLinea);

      bool lineaCorrecta(const Cadena& lValorLinea);
      void escribirParametro(Cadena& lValorLinea,SWordType sdwCampo,const Cadena &mValor);
      Cadena getIdLinea(const Cadena& lValorLinea);
      bool esSeparadorParametro(const SByteType sbCaracter);
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //FICHCONFIG_H

