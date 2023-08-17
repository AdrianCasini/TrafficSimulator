#ifndef CADENA_STR_H
#define CADENA_STR_H

#define FORMAT       "\x1B[%d;%d;%dm" // text attributes, foreground color, background color
#define RESET_FORMAT "\x1B[0m"

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <string.h>
#include <string>
#include <stdlib.h>

#include <stdarg.h>
#include <exception>

// INCLUDES DEL PROJECTO

#include "../lang/Types.h"

// INCLUDES LOCALES

using namespace std;

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: Cadena.h
  *
  * DESCRIPCION: Manipulacion de Kadenas de texto. Basada en los strings de las
  *              STL.
  *
  * FECHA CREACION: 17-09-2007
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Ruben Pinadero Hernandez
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |      |               |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class Cadena
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
     // Maxima longitud del String lo mantenemos porque lo necesitan otras clases
     static const UWordType LON_MAX_STRING = 512;

  ////////////////////   PRIVATE      ////////////////////
  private:
     string szString;

  ////////////////////   PUBLIC       ////////////////////
  public:

     // ===============   CICLO DE VIDA    ==================


     Cadena ( void );                    //Constructor vacio
     Cadena ( CPSByteType psbStr );
     Cadena ( SByteType sbCaracter );
     Cadena ( CPSByteType psbStr, UWordType uwLen );
     Cadena ( SDWordType sdwNum );
     Cadena ( UDWordType udwNum );
     Cadena ( float sdwNum );
     Cadena ( const Cadena& str );

     virtual ~Cadena();

     //string &getString( void ){ return szString; }

     Cadena& operator  = ( const Cadena& Str );
     //Cadena& operator  = ( CPSByteType psbStr );
     bool      operator   == ( const Cadena& str ) const;
     bool      operator   != ( const Cadena& str ) const;
     Cadena    operator   + ( const Cadena& Str ) const;
     Cadena&   operator   += ( const Cadena& Str );
     bool      operator   > ( const Cadena& str ) const;
     bool      operator   < ( const Cadena& str ) const;
     SByteType operator   [] ( UDWordType udwCoI ) const;

     Cadena& formCadena( const SByteType* psbParametros, ... );
     void formCadenaArg( const SByteType* psbParametros, va_list va );

     bool isInteger() const;
     bool isEmpty( void ) const;
     void setEmpty( void );

     UDWordType getLong() const;
     CPSByteType getCadena() const;
     CPSByteType c_str() const;

     Cadena setCharArray(CPSByteType szCharArray, SWordType swLongitud = -1);

     SByteType getCaracter( UWordType luwNumCaracter ) const;
     Cadena& concatenarSeparadoPorComas( CPSByteType cpsbCharArray );

     Cadena aMayuscula( void );
     Cadena aMinuscula( void );

     bool recortar( UWordType uwLong );
     Cadena trim( void );

     Cadena lTrim( void );
     Cadena rTrim( void );

     bool empiezaCon( const Cadena& Str ) const;
     bool finalizaCon( const Cadena& str ) const;

     void insertaCaracter(const SByteType sbCaracter);
     void insertaCaracter(const UWordType uwPosicion, const SByteType sbCaracter, UWordType luwNumero = 1);

     void borraCaracter(const UWordType uwPosicion, const SByteType sbCaracter, UWordType luwNumero = 1);

     SDWordType toSDWordType (UByteType ubBase = 10) const;
     UDWordType toUDWordType (UByteType ubBase = 10) const;
     U64Type    toU64Type(UByteType ubBase = 10) const;



     // Retorna true si la cadena vale true, t, s, 1
     bool toBoolean() const;
 
   

     void subCadena(const UWordType uwPosicion,Cadena& mTexto);
     Cadena subCadena(const UWordType uwPosicion) const;
     Cadena subCadena(UWordType uwPosicion, UWordType uwFinal) const;
     void subCadena(const UWordType uwPosicion, const UWordType uwFinal, Cadena& mTexto);

     SWordType indiceDe(const SByteType sbCaracter, UWordType luwInicio = 0) const;

     SWordType ultimoIndiceDe(const SByteType sbCaracter) const;
     SWordType ultimoIndiceDe(const Cadena& lTexto, const UWordType luwInicio) const;
     SWordType primerIndiceDe(const Cadena cadCadena, UWordType luwOffset = 0) const;

     SWordType HayComodin() const;

     Cadena ReemplazarCadena( Cadena cadSubCadena, Cadena cadCadena );
     /* A diferencia del método ReemplazarCadena, que sobreescribe el contenido de la cadena,
     este método sustituye todas las ocurrencias de la cadena destino por la cadena fuente */
     Cadena sustituir( Cadena destino, Cadena fuente );
     Cadena hexToCadena(CPUByteType lcpsbDatos, UWordType luwLong);

     void dameParametro(UWordType uwNumParametro, Cadena& mTexto) const;
     void damePath(UWordType uwNumParametro, Cadena& mTexto) const;
     void dameTextoEntreSep(UWordType uwNumParametro, SByteType sbCaracter, Cadena& mTexto) const;

  
     bool ajustar( UWordType uwLong );

     float toFloat () const;
     int toInt ();

     Cadena& reemplazarCadena( const Cadena& cadSubCadena, const Cadena& cadCadena, bool bTodo=false );

};

// ================================================================
// DECLARACION DE CLASE
// ================================================================


#endif

