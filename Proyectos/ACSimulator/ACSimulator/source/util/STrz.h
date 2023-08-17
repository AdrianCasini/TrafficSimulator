
// =======================
//  FICHERO - STrz.H
// =======================

#ifndef STrz_H
#define STrz_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO

#ifdef _VIALINUX_SYSLOG

#include "SysTrz.h"

#else

#include "../lang/Types.h"
#include "../thread/AutoMutex.h"
#include "../lang/Cadena.h"
#include "../util/Trazable.h"
#include "../util/FechaHora.h"
#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/IOExcepcion.h"
#include "../thread/MutexLocker.h"
#include "../util/ConstTraza.h"

#define TRACE( texto ) \
   STrz::trace( ConstTraza::T_TRAZA_ERROR, "%s", texto )

#define CHECKPOINT() \
   STrz::trace( ConstTraza::T_TRAZA_ERROR, "%s:%d", __PRETTY_FUNCTION__, __LINE__)

#define NULLPOINTER() \
   STrz::error( ConstTraza::T_TRAZA_ERROR, "***ERROR: Puntero NULL en %s:%d", __PRETTY_FUNCTION__, __LINE__ );

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: STrz.H
  *
  * DESCRIPCION: Clase estatica para su uso sin necesisdad de tener un enlaze directo
  * con la clase que implementa la traza.
  *
  * FECHA CREACION: 11-07-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
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

class Cadena;

class STrz
{
   // CLASES INTERNAS
   // =====================================================

   /** descripcion de la clase interna */

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE       ////////////////////
   private:
      static Trazable*      pmTrazable;
      static AutoMutex      mMutex;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   OPERACIONES  ==================
      STrz();
      ~STrz();

     /**
      * Definicion del metodo virtual debug
      *  @param      eTipoTraza -> Tipo de traza
      *  @param      psbParametros -> Cadena de texto sin o con formato, en el caso
      *              de contener parametros se tienen que definir acontinuacion
      *              igual que el printf
      *  @param      ...   -> Lista de parametros
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      static void debug( SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );

      static void debug( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg, SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );

      /// Genera trazas basic de color rojo
      static void error( SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );

      /// Genera trazas basic de color amarillo
      static void warning( SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );

      /// Genera trazas basic de color cian
      static void info( SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );

      /// Genera trazas basic de color magenta
      static void trace( SWordType swTipoTraza,
           CPSByteType psbParametros, ... );

      /// Genera trazas basic de color blanco sobre fondo rojo
      static void excepcion( SWordType swTipoTraza, Excepcion& e );

      static void debug( SWordType swTipoTraza, const Cadena& str );
      static void debug( SWordType swTipoTraza, Excepcion& e );

     /**
      * Definicion del metodo virtual basic
      *  @param      eTipoTraza -> Tipo de traza
      *  @param      psbParametros -> Cadena de texto sin o con formato, en el caso
      *              de contener parametros se tienen que definir acontinuacion
      *              igual que el printf
      *  @param      ...   -> Lista de parametros
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      static void basic( SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );

      static void basic( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg, SWordType swTipoTraza,
    		  CPSByteType psbParametros, ... );


      static void basic( SWordType swTipoTraza, const Cadena& str );
      static void basic( SWordType swTipoTraza, Excepcion& e );


     /**
      *  Asigna un ejecutor de la traza.
      *  @param      pTrazable -> Puntero al objeto  que implenta la traza.
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      static void  setTrazable(Trazable *pTrazable);

      // =================   ACCESO       ====================

      static SWordType getProceso();
      static UWordType getLongMaxMensaje();
      static Cadena& getNombreFichero();
      static UWordType getNumFicheros();
      static UDWordType getLongFicheros();

      static void setProceso(const Cadena& lNombre,SWordType swCodProc);
      static void setLongMaxMensaje(UWordType uwLongMax);
      static void setNombreFichero(const Cadena& mFichAltern);
      static void setNumFicheros(UWordType uwNumFichAltern);
      static void setLongFicheros(UDWordType udwLongFichAltern);

   ////////////////////   PRIVATE       ////////////////////
   private:

      static const UDWordType BUFFER_SIZE = 2048;

      /**
      *  Retorna el ejecutor asignado a la traza.
      *  @param      Ninguno
      *  @return     Trazable -> Puntero al ejecutor
      *  @exception  Genera una excepcion si el puntero es igual a NULL
      */
      static Trazable*   getTrazable();

     /**
      * Cuando la traza no se puede enviar a ningun gestor se extrae directamente
      * a pantalla.
      *  @param      str   Texto completo de la traza.
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      static void salirPantalla(SWordType swTipoTraza, CPSByteType str);


      static void doDebug( SWordType swTipoTraza, CPSByteType str );
      static void doBasic( SWordType swTipoTraza, CPSByteType str );

      static void va_debug( SWordType swTipoTraza, CPSByteType psbParametros, va_list ap );
      static void va_debug( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg, SWordType swTipoTraza, CPSByteType psbParametros, va_list ap );
      static void va_basic( SWordType swTipoTraza, CPSByteType psbParametros, va_list ap );
      static void va_basic( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg, SWordType swTipoTraza, CPSByteType psbParametros, va_list ap );



      static SByteType sbBuffer[];
};

#endif

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

// ================================================================
// SISTEMA DE MACROS PARA FACILITAR EL USO DE LAS TRAZAS
// ================================================================

/**

   El sistema de uso de trazas que se presenta a continuacion tiene como
   objetivo facilitar el uso de la clase 'STrz' que se ha definido. Ademas,
   las trazas generadas incluyen siempre el nombre de la clase y el nombre
   del metodo en el que se genera la traza.

   Para usarlo basta con seguir las instrucciones que siguen:

      1. Se debe activar el sistema de macros para trazas definiendo la clase
         y el tipo de trazas. Esto consiste en definir las macros __CLASS__ y
         __TRAZA__ **ANTES** de hacer el '#include "STrz.h".

      2. La macro __CLASS__ debe ser una constante string (entre comillas)

      3. La macro __TRAZA__ debe ser un elemento de ConstTraza::TrazasID


   Por ejemplo, para trazar la clase ConfiguradorCortinas usada por el gestor
   de cortinas, se deben activar las macros con:

      #define  __CLASS__  "ConfiguradorCortinas"
      #define  __TRAZA__  T_TRAZA_CORTINA
      #include "STrz.h"


   Las macros disponibles para generar trazas con formato constante son:

      trz_basic( "mensaje" );
      trz_debug( "mensaje" );
      trz_error( "mensaje" );


   y para generar trazas con formato variable:

      trz_basic_fmt( "%s = %d", "nombre", valor );
      trz_debug_fmt( "%s = %d", "nombre", valor );
      trz_error_fmt( "%s = %d", "nombre", valor );


   En este ultimo caso se debe pasar un string de formato y un numero
   variable de argumentos (al estilo 'printf').  Para las excepciones, existe
   la macro:

      trz_excepcion( e );


   cuyo unico parametro es un objeto Excepcion.

   Las macros trz_error y trz_error_fmt vierten las trazas generadas sobre
   ConstTraza::T_TRAZA_ERROR. El resto lo hacen sobre el tipo de traza
   especificado en la macro __TRAZA__

   Existe ademas una pequena ayuda para trazas valores booleanos. Se trata de
   la macro

      trz_bool( b )

   en la que 'b' debe ser una expresion evaluable con el operador '?:'. Lo
   que genera esta macro es un 'char' que representa al valor booleano
   despues de evaluar la expresion dada. Si la expresion es cierta genera
   'T', y si es falsa 'F'. Estos valores de verdad pueden cambiarse
   redefiniendo las macros __TRUE__ y __FALSE__ con los valores deseados,
   pero deben ser de tipo 'char'. Por ejemplo:

      #define __TRUE__  'S'
      #define __FLASE__ 'N'
      #include "STrz.h"

      trz_basic( "Sera verdad que 0 > 9 ? Resp.=%c", trz_bool( 0 > 9 ));

**/

#if defined( __CLASS__ )  and  defined( __TRAZA__ )

#define __STrz_TRAZA__ ConstTraza::__TRAZA__
#define __STrz_ERROR__ ConstTraza::T_TRAZA_ERROR

#define trz_basic( x )                                      \
                                                            \
   STrz::basic( /* traza   */   __STrz_TRAZA__,             \
                /* formato */   "%s",                       \
                /* args    */   x )


#define trz_debug( x )                                     \
                                                           \
   STrz::debug( /* traza   */   __STrz_TRAZA__,            \
                /* formato */   __CLASS__ "::%s: %s",      \
                /* args    */   __FUNCTION__, x )


#define trz_error( x )                                      \
                                                            \
   STrz::basic( /* traza   */   __STrz_ERROR__,             \
                /* formato */   __CLASS__ "::%s: %s",       \
                /* args    */   __FUNCTION__, x )


#define trz_excepcion( e )                               \
                                                         \
   STrz::basic( /* traza   */   __STrz_TRAZA__,          \
                /* args    */   e )


#define trz_basic_fmt( fmt, ... )                              \
                                                               \
   STrz::basic( /* traza   */   __STrz_TRAZA__,                \
                /* formato */   fmt,                           \
                /* args    */   __VA_ARGS__ )


#define trz_debug_fmt( fmt, ... )                              \
                                                               \
   STrz::debug( /* traza   */   __STrz_TRAZA__,                \
                /* formato */   __CLASS__ "::%s: " fmt,        \
                /* args    */   __FUNCTION__, __VA_ARGS__ )


#define trz_error_fmt( fmt, ... )                             \
                                                              \
   STrz::basic( /* traza   */   __STrz_ERROR__,               \
                /* formato */   __CLASS__  "::%s: " fmt,      \
                /* args    */   __FUNCTION__, __VA_ARGS__ )


#if defined(__TRUE__)
#define __STrz__TRUE__ __TRUE__
#else
#define __STrz__TRUE__ 'T'
#endif

#if defined(__FALSE__)
#define __STrz__FALSE__ __FALSE__
#else
#define __STrz__FALSE__ 'F'
#endif

#define trz_bool( x ) (( x ) ? __STrz__TRUE__ : __STrz__FALSE__ )


#else // not( defined( __CLASS__ )  and  defined( __TRAZA__ ))

#define trz_basic( x )
#define trz_debug( x )
#define trz_error( x )
#define trz_excepcion( e )
#define trz_basic_fmt( fmt, ... )
#define trz_debug_fmt( fmt, ... )
#define trz_error_fmt( fmt, ... )
#define trz_bool( x )

#if defined( __CLASS__ )
#warning
#warning Macro '__CLASS__' encontrada,
#warning falta la definicion de la macro __TRAZA__
#warning
#warning Sistema de macros para trazas ** DESACTIVADO **
#warning
#endif

#if defined( __TRAZA__ )
#warning
#warning Macro '__TRAZA__' encontrada,
#warning falta la definicion de la macro __CLASS__
#warning
#warning Sistema de macros para trazas ** DESACTIVADO **
#warning
#endif

#endif // defined( __CLASS__ )  and  defined( __TRAZA__ )


#endif //STrz_H

// ===========================
//  FIN DEL FICHERO - STrz.H
// ===========================
