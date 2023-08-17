// =======================
//  FICHERO - Cadena.cpp
// =======================

#include "Cadena.h"  // clase implementada
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <sys/types.h>
#include <signal.h>
#include <assert.h>


#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/NoAplicaExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"
#include "../gestor/lang/Types.h"


//------------------------------------------------------------------------------
Cadena::Cadena( void ) : szString(){};
Cadena::Cadena( SByteType sbCaracter ) : szString(1, sbCaracter){};

//------------------------------------------------------------------------------
Cadena::Cadena( CPSByteType psbStr )
{
   if(NULL == psbStr)
      szString = "";
   else
      szString = psbStr;
}

//------------------------------------------------------------------------------
Cadena::Cadena( SDWordType sdwNum )
{
   stringstream ss;
   ss << sdwNum;
   szString = ss.str();
};

//------------------------------------------------------------------------------
Cadena::Cadena( UDWordType sdwNum )
{
   stringstream ss;
   ss << sdwNum;
   szString = ss.str();
};

//------------------------------------------------------------------------------
bool Cadena::ajustar( UWordType uwLong )
{
 if ( !recortar (uwLong) )
 {
   insertaCaracter(szString.size() + 1, ' ',uwLong - szString.size() );
   return (true);
 }
 return(false);
}

//------------------------------------------------------------------------------
Cadena::Cadena( float sdwNum )
{
   stringstream ss;
   ss << sdwNum;
   szString = ss.str();
};

//------------------------------------------------------------------------------
Cadena::Cadena( CPSByteType psbStr, UWordType uwLen )
{
   if(NULL == psbStr)
   {
      szString = "";
   }
   else
     {
      if (!memchr(psbStr, 0, uwLen))
        szString.assign(psbStr, uwLen);
      else
        szString = psbStr;
   }
}

//------------------------------------------------------------------------------
Cadena::Cadena ( const Cadena& str )
   : szString(str.szString)
{

}

//------------------------------------------------------------------------------
Cadena::~Cadena()
{
};

//------------------------------------------------------------------------------
Cadena& Cadena::operator = ( const Cadena& str )
{
   szString = str.szString;
   return (*this);
};

//------------------------------------------------------------------------------
bool Cadena::operator == ( const Cadena& str ) const
{
   return (szString == str.szString);
};

//------------------------------------------------------------------------------
bool Cadena::operator != ( const Cadena& str ) const
{
   return (szString != str.szString);
};

//------------------------------------------------------------------------------
Cadena Cadena::operator + (const Cadena& str) const
{
   Cadena lStr;

   lStr.szString = szString + str.szString;
   return lStr;
};

//------------------------------------------------------------------------------
Cadena& Cadena::operator += ( const Cadena& str )
{
   szString += str.szString;
   return (*this);
};

//------------------------------------------------------------------------------
bool Cadena::operator > ( const Cadena& str ) const
{
   return (szString.compare(str.szString) > 0);
};

//------------------------------------------------------------------------------
bool Cadena::operator < ( const Cadena& str ) const
{
   return (szString.compare(str.szString) < 0);
};

//------------------------------------------------------------------------------
SByteType Cadena::operator[] ( UDWordType udwCoI ) const
{
   try
   {  //Usamos el at para controlar la excepcion.
      return ( szString.at(udwCoI) );
   }
   catch(exception &e)
   {
      throw RangoExcepcion(__AQUI__,"El indice esta fuera de rango.");
   }
};

Cadena& Cadena::formCadena(const SByteType* psbParametros, ... )
{
   const int LON_MAX_STRING = 2048;
   SByteType buffer[ LON_MAX_STRING ];

   va_list ap;

   va_start (ap, psbParametros);
   vsnprintf (buffer, LON_MAX_STRING, psbParametros, ap);
   va_end (ap);

   szString = buffer;
   return ( *this );
}

void Cadena::formCadenaArg( const SByteType* psbParametros, va_list va)
{
   const int LON_MAX_STRING = 2048;
   SByteType buffer[ LON_MAX_STRING ];

   vsnprintf(buffer, LON_MAX_STRING, psbParametros, va);

   szString = buffer;
}

//------------------------------------------------------------------------------
bool Cadena::isInteger() const
{
   UDWordType udIndice;

   if(szString.empty())
      return false;

   // Mientras no encontremos un caracter no valido
   for(udIndice = 0;udIndice < szString.size(); udIndice++)
   {
      if(isdigit(szString[udIndice]) == 0)
         return false;
   }

   return true;
}

//------------------------------------------------------------------------------
bool Cadena::isEmpty() const
{
   return szString.empty();
};

//------------------------------------------------------------------------------
void Cadena::setEmpty()
{
   return szString.clear();
};

//-----------------------------------------------------------------------------
UDWordType Cadena::getLong() const
{
   return szString.size();
};

//-----------------------------------------------------------------------------
CPSByteType Cadena::getCadena() const
{
   return szString.c_str();
};

//-----------------------------------------------------------------------------
CPSByteType Cadena::c_str() const
{
   return szString.c_str();
};

//-----------------------------------------------------------------------------
Cadena Cadena::setCharArray( CPSByteType szCharArray, SWordType swLongitud )
{
  assert(szCharArray);
  try
    {
      if (!memchr(szCharArray, 0, swLongitud) && swLongitud != -1)
        szString.assign(szCharArray, swLongitud);
      else
        szString = szCharArray;
    }
  catch(exception& e)
    {
      throw DesconocidoExcepcion(__AQUI__, e.what());
    }

  return *this;
}


//------------------------------------------------------------------------------
SByteType Cadena::getCaracter( UWordType luwNumCaracter ) const
{
   try
   {  //Usamos el at para controlar la excepcion.
      return ( szString.at(luwNumCaracter) );
   }
   catch(exception &e)
   {
      throw RangoExcepcion(__AQUI__,"El indice esta fuera de rango.");
   }
};

//------------------------------------------------------------------------------
Cadena& Cadena::concatenarSeparadoPorComas( CPSByteType cpsbCharArray )
{
   if( 1 < strlen( cpsbCharArray ) )
   {
      if ( false == szString.empty() )
          szString += ( ", " );

      szString += cpsbCharArray;
   }
   return (*this);
};

//------------------------------------------------------------------------------
Cadena Cadena::aMayuscula( void )
{
   transform(szString.begin(), szString.end(), szString.begin(), (int(*)(int))toupper);
   return(*this);
};

//------------------------------------------------------------------------------
Cadena Cadena::aMinuscula( void )
{
   transform(szString.begin(), szString.end(), szString.begin(), (int(*)(int))tolower);
   return(*this);
};

//------------------------------------------------------------------------------
bool Cadena::recortar( UWordType uwLong)
{
   if(uwLong <= szString.size())
   {
      try
      {
         szString.resize(uwLong);
      }
      catch(exception& e)
      {
         throw DesconocidoExcepcion(__AQUI__, e.what());
      }
      return true;
   }
   else
      return false;
};

//------------------------------------------------------------------------------
Cadena Cadena::trim( void )
{
   lTrim();  // Quitamos los espacios de la izquierda
   rTrim();  // Quitamos los espacios de la derecha

   return(*this);
};

//------------------------------------------------------------------------------
Cadena Cadena::lTrim( void )
{
   SDWordType sdIndice = szString.find_first_not_of(' ');

   if(sdIndice != (SDWordType) string::npos)
      szString = szString.substr( sdIndice, szString.length() - sdIndice);
   else
      szString = "";

   return(*this);
};

//------------------------------------------------------------------------------
Cadena Cadena::rTrim( void )
{
   SDWordType sdIndice = szString.find_last_not_of(' ');

   if(sdIndice !=  (SDWordType) string::npos)
      szString = szString.substr( 0, sdIndice + 1);
   else
      szString = "";

   return(*this);
};

//------------------------------------------------------------------------------
bool Cadena::empiezaCon( const Cadena& str ) const
{
   SByteType sbIndice;

   if(!str.szString.empty())
   {
      sbIndice = szString.find(str.szString);
      if(sbIndice == 0)  //Si nos indica la posicion 0 es que esta al principio
         return true;
   }

   return false;
};

//------------------------------------------------------------------------------
bool Cadena::finalizaCon( const Cadena& str ) const
{
   SByteType sbIndice;

   if(!str.szString.empty())
   {
      sbIndice = szString.find(str.szString);
      if(sbIndice != (SByteType)string::npos)
      {
         if(szString[sbIndice + str.szString.size()] == '\0')
            return true;
      }
   }

   return false;
};

//------------------------------------------------------------------------------
void Cadena::insertaCaracter(const SByteType sbCaracter)
{
   szString += sbCaracter;
};

//------------------------------------------------------------------------------
void Cadena::insertaCaracter(const UWordType uwPosicion, const SByteType sbCaracter, UWordType luwNumero)
{
  if(uwPosicion > 0)
  {
     try
     {
        szString.insert((uwPosicion-1) , luwNumero, sbCaracter);
     }
     catch(exception& e)
     {
        throw DesconocidoExcepcion(__AQUI__, e.what());
     }
  }
  else
  {
      throw RangoExcepcion(__AQUI__, "La posicion tiene que ser mayor que 0.");
  }
};

//------------------------------------------------------------------------------
void Cadena::borraCaracter(const UWordType uwPosicion,const SByteType sbCaracter,UWordType luwNumero)
{
   if(uwPosicion > 0)
   {
      try
      {
         szString.replace((uwPosicion-1) , luwNumero, luwNumero, sbCaracter);
      }
      catch(exception& e)
      {
         throw DesconocidoExcepcion(__AQUI__, e.what());
      }
   }
   else
   {
      throw RangoExcepcion(__AQUI__, "La posicion y el numero de caracteres tiene que ser mayor que 0.");
   }
};

//------------------------------------------------------------------------------
SDWordType Cadena::toSDWordType(UByteType ubBase) const
{
   return SDWordType(strtol (szString.c_str(), NULL, ubBase));
};

//------------------------------------------------------------------------------
UDWordType Cadena::toUDWordType(UByteType ubBase) const
{
   return UDWordType(strtoul (szString.c_str(), NULL, ubBase));
};

//------------------------------------------------------------------------------
U64Type Cadena::toU64Type(UByteType ubBase) const
{
   return U64Type(strtoull (szString.c_str(), NULL, ubBase));
};

//------------------------------------------------------------------------------
bool Cadena::toBoolean() const
{
   Cadena lValor = *this;

   lValor.aMayuscula();
   if ( lValor == "S" || lValor == "SI"        ||
        lValor == "T" || lValor == "TRUE"      ||
        lValor == "V" || lValor == "VERDADERO" ||
        lValor.toSDWordType())
      return true;
   else
      return false;
};



//------------------------------------------------------------------------------
void Cadena::subCadena(const UWordType uwPosicion, Cadena& mTexto)
{
   if(uwPosicion <= szString.size())
   {
      if(uwPosicion > 0)
      {
         try
         {
            mTexto.szString = szString.substr(uwPosicion - 1);
         }
         catch(exception& e)
         {
            throw DesconocidoExcepcion(__AQUI__, e.what());
         }
      }
      else
      {
         throw RangoExcepcion(__AQUI__, "La posicion tiene que ser mayor que 0.");
      }
   }
   else
      throw RangoExcepcion(__AQUI__, "La posicion es superior al tamano de la cadena.");
};

//------------------------------------------------------------------------------
Cadena Cadena::subCadena(const UWordType uwPosicion) const
{
   Cadena mTexto;

   if(uwPosicion <= szString.size())
   {
      if(uwPosicion > 0)
      {
         try
         {
            mTexto.szString = szString.substr(uwPosicion - 1);
         }
         catch(exception& e)
         {
            throw DesconocidoExcepcion(__AQUI__, e.what());
         }
      }
      else
      {
         throw RangoExcepcion(__AQUI__, "La posicion tiene que ser mayor que 0.");
      }
   }
   else
      throw RangoExcepcion(__AQUI__, "La posicion es superior al tamano de la cadena.");

   return mTexto;
};

//------------------------------------------------------------------------------
Cadena Cadena::subCadena(UWordType uwPosicion, UWordType uwFinal) const
{
   Cadena mTexto;

   if(uwPosicion <= szString.size())
   {
      if(uwPosicion > 0 && uwFinal > 0)
      {
         try
         {
            mTexto.szString = szString.substr(uwPosicion - 1, (uwFinal - uwPosicion + 1));
         }
         catch(exception& e)
         {
            throw DesconocidoExcepcion(__AQUI__, e.what());
         }
      }
      else
      {
         throw RangoExcepcion(__AQUI__, "La posicion tiene que ser mayor que 0.");
      }
   }
   else
      throw RangoExcepcion(__AQUI__, "La posicion es superior al tamano de la cadena.");

   return mTexto;
};

//------------------------------------------------------------------------------
void Cadena::subCadena(const UWordType uwPosicion, const UWordType uwFinal, Cadena& mTexto)
{

   if(uwPosicion <= szString.size())
   {
      if(uwPosicion > 0)
      {
         try
         {
            mTexto.szString = szString.substr(uwPosicion - 1, (uwFinal - uwPosicion + 1));
         }
         catch(exception& e)
         {
            throw DesconocidoExcepcion(__AQUI__, e.what());
         }
      }
      else
      {
         throw RangoExcepcion(__AQUI__, "La posicion tiene que ser mayor que 0.");
      }
   }
   else
   {
      throw RangoExcepcion(__AQUI__, "La posicion es superior al tamano de la cadena.");
   }
};

//------------------------------------------------------------------------------
SWordType Cadena::indiceDe(const SByteType sbCaracter, UWordType luwInicio) const
{
   SWordType lswValue;
   try
   {
       lswValue = szString.find(sbCaracter, luwInicio);
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }
   return (lswValue!=(SWordType)string::npos) ? (lswValue + 1) : (-1);
};

//------------------------------------------------------------------------------
SWordType Cadena::ultimoIndiceDe(const Cadena& lTexto, const UWordType luwInicio) const
{
   SWordType lswValue;
   try
   {
      lswValue = szString.rfind(lTexto.szString, luwInicio);
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }
   return (lswValue!=(SWordType)string::npos) ? (lswValue+1) : (lswValue);
};

//------------------------------------------------------------------------------
SWordType Cadena::ultimoIndiceDe(const SByteType sbCaracter) const
{
   SWordType lswValue;
   try
   {
      lswValue = szString.rfind(sbCaracter);
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }
   return (lswValue!=(SWordType)string::npos) ? (lswValue+1) : (lswValue);
};

//------------------------------------------------------------------------------
SWordType Cadena::primerIndiceDe(const Cadena cadCadena, UWordType luwOffset) const
{
   SWordType lswValue;
   try
   {
      lswValue = szString.find(cadCadena.szString, luwOffset);
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }
   return (lswValue!=(SWordType)string::npos) ? (lswValue+1) : (lswValue);
};

//------------------------------------------------------------------------------
SWordType Cadena::HayComodin() const
{
  return (primerIndiceDe("<@@@>"));
};

//------------------------------------------------------------------------------
Cadena Cadena::ReemplazarCadena( Cadena cadSubCadena, Cadena cadCadena )
{
   try
   {
      SWordType lswValue = szString.find(cadSubCadena.szString);

      if(lswValue!=(SWordType)string::npos)
         szString.replace(lswValue, cadCadena.szString.size(), cadCadena.szString);
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }

   return (*this);
};

//------------------------------------------------------------------------------
Cadena Cadena::sustituir( Cadena destino, Cadena fuente )
{
   int n;

   while ( string::npos != ( n = szString.find( destino.szString ) ) )
   {
      szString.erase( n, destino.szString.length() );
      szString.insert( n, fuente.szString );
   }

   return (*this);
};

//------------------------------------------------------------------------------
Cadena Cadena::hexToCadena(CPUByteType lcpsbDatos, UWordType luwLong)
{
   UWordType luwIndice;
   stringstream ss;

   try
   {
      ss << uppercase << hex;

      for(luwIndice = 0;luwIndice < luwLong;luwIndice++)
         ss << " " << setfill('0') << setw(2) << (int) lcpsbDatos[luwIndice];

      szString = ss.str();
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }

   return (*this);
};

//------------------------------------------------------------------------------
void Cadena::dameParametro(UWordType uwNumParametro, Cadena& mTexto) const
{
   dameTextoEntreSep(uwNumParametro,' ',mTexto);
};

//------------------------------------------------------------------------------
void Cadena::damePath(UWordType uwNumParametro, Cadena& mTexto) const
{
   dameTextoEntreSep(uwNumParametro,':',mTexto);
};

//------------------------------------------------------------------------------
void Cadena::dameTextoEntreSep(UWordType uwNumParametro, SByteType sbCaracter, Cadena& mTexto) const
{
   UWordType uwIndice;
   SWordType swValueNew;
   SWordType swValueOld;
   SWordType swResult;

   swResult = 0;
   swValueNew = 0;
   swValueOld = 0;
   uwIndice = 0;

   //Protegemos que si nos piden el parametro cero devolvemos una Kadena vacia.
   if(0 == uwNumParametro)
   {
      mTexto = "";
      return;
   }

   try
   {
      //Para compatibilizar con la cadena antigua
      //Saltamos todos los caracteres primeros iguales al sbCaracter
      for(uwIndice = 0; uwIndice < szString.size(); uwIndice++)
      {
         if(szString[uwIndice] == sbCaracter)
            swValueNew++;
         else
            break;
      }

      uwIndice = 0;

      for(uwIndice = 0; uwIndice < uwNumParametro; uwIndice ++)
      {
         swValueOld = swValueNew;
         //Si no encuentra algun sbCaracter significa que no hay o que es mayor de
         //los que tenemos.
         if(((SWordType)string::npos) == (swResult = szString.find(sbCaracter, swValueOld)))
            break;

         swValueNew = swResult + 1;
      }

      //Contemplamos 2 casos: que termine en sbCaracter o que no termine.
      if((uwNumParametro - uwIndice) <= 1)
         mTexto.szString = szString.substr(swValueOld, swValueNew - swValueOld - 1);
      else
         mTexto = "";
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }
}



//------------------------------------------------------------------------------
float Cadena::toFloat () const
{
   return strtof(szString.c_str(), NULL);
};

//------------------------------------------------------------------------------
Cadena& Cadena::reemplazarCadena( const Cadena& cadSubCadena, const Cadena& cadCadena, bool bTodo )
{
   size_t lswValue;

   try
   {
      do
      {
         lswValue = szString.find(cadSubCadena.szString);
         if(lswValue!=string::npos)
            szString.replace(lswValue, cadSubCadena.szString.size(), cadCadena.szString);
      } while (bTodo && lswValue!=string::npos);
   }
   catch(exception& e)
   {
      throw DesconocidoExcepcion(__AQUI__, e.what());
   }

   return (*this);
};

//------------------------------------------------------------------------------
int Cadena::toInt () 
{
  return atoi(getCadena());
}

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

