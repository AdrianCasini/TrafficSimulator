
#include "CadenaFormato.h"
#include "lang/throwable/RangoExcepcion.h"
#include "lang/throwable/FormatoExcepcion.h"
#include "util/Timer.h"


static CPSByteType FMT_CNEGRITA   = "<B>";
static CPSByteType FMT_CPARPADEO  = "<BLINK>";
static CPSByteType FMT_CSUBRAYADO = "<U>";
static CPSByteType FMT_CINVERSO   = "<INV>";
static CPSByteType FMT_NEGRITA   = "B";
static CPSByteType FMT_PARPADEO  = "BLINK";
static CPSByteType FMT_SUBRAYADO = "U";
static CPSByteType FMT_INVERSO   = "INV";
static const SByteType INI_TAG = '<';
static const SByteType FIN_TAG = '>';
static const SByteType FIN = '/';

regex_t CadenaFormato::msRegexpValidar;
regex_t CadenaFormato::msRegexpFormato;
bool CadenaFormato::mbooRegexp = false;
Mutex CadenaFormato::mmtxRegexp;

CadenaFormato::CadenaFormato ()
   : muwLong(0)
{
   CadenaFormato::inicializarRegexps ();
}

CadenaFormato::CadenaFormato (const CadenaFormato& cadSrc)
   : mcadStr(cadSrc.mcadStr), muwLong(cadSrc.muwLong)
{
   CadenaFormato::inicializarRegexps ();
}

CadenaFormato::CadenaFormato (const Cadena& cadSrc, UWordType uwFormato)
   : mcadStr(cadSrc)
{
   CadenaFormato::inicializarRegexps ();
   this->muwLong = cadSrc.getLong ();
   if (uwFormato != 0)
      setFormato (uwFormato);
}

CadenaFormato::CadenaFormato (CPSByteType psbSrc, UWordType uwFormato)
   : mcadStr(psbSrc)
{
   CadenaFormato::inicializarRegexps ();
   this->muwLong = this->mcadStr.getLong ();
   if (uwFormato != 0)
      setFormato (uwFormato);
}

CadenaFormato::~CadenaFormato ()
{

}


CadenaFormato::operator const char* () const
{
   return this->mcadStr.getCadena ();
}

CadenaFormato& CadenaFormato::operator= (const CadenaFormato& cadSrc)
{
   this->mcadStr = cadSrc.mcadStr;
   this->muwLong = cadSrc.muwLong;
   return *this;
}

CadenaFormato& CadenaFormato::operator+= (const CadenaFormato& cadSrc)
{
   this->mcadStr += cadSrc.mcadStr;
   this->muwLong += cadSrc.muwLong;
   return *this;
}

CadenaFormato CadenaFormato::operator+ (const CadenaFormato& cadSrc) const
{
   CadenaFormato cadAux(*this);
   cadAux += cadSrc;
   return cadAux;
}

bool CadenaFormato::operator== (const CadenaFormato& cadSrc) const
{
   return (this->mcadStr == cadSrc.mcadStr);
}

bool CadenaFormato::operator== (const Cadena& cadSrc) const
{
   return (getCadena () == cadSrc);
}

bool CadenaFormato::operator!= (const Cadena& cadSrc) const
{
   return (getCadena () != cadSrc);
}
/*
bool CadenaFormato::operator>= (const Cadena& cadSrc) const
{
   return (getCadena () >= cadSrc);
}

bool CadenaFormato::operator<= (const Cadena& cadSrc) const
{
   return (getCadena () <= cadSrc);
}
*/
bool CadenaFormato::operator> (const Cadena& cadSrc) const
{
   return (getCadena () > cadSrc);
}

bool CadenaFormato::operator< (const Cadena& cadSrc) const
{
   return (getCadena () < cadSrc);
}


void CadenaFormato::concatenarCadena (const Cadena& cadSrc, UWordType uwFormato)
{
   CadenaFormato cadAux(cadSrc, uwFormato);
   this->mcadStr += cadAux.mcadStr;
   this->muwLong += cadAux.muwLong;
}

void CadenaFormato::concatenarCadenaFormato (const Cadena& cadFormato)
{
   if (validarCadena (cadFormato))
   {
      this->mcadStr += cadFormato;
      this->muwLong += calcularLongSinFormato (cadFormato);
   }
   else
   {
      throw FormatoExcepcion(__AQUI__, "Formato de cadena incorrecto");
   }
}

void CadenaFormato::borrarFormatos ()
{
   Cadena cadAux (getCadena ());
   this->mcadStr = cadAux;
   this->muwLong = cadAux.getLong ();
}

void CadenaFormato::formCadena (CPSByteType psbStr,
   UWordType uwFormato, ...)
{
   SByteType sbAux[Cadena::LON_MAX_STRING];
   UWordType uwAux;
   va_list   ap;

   va_start (ap, uwFormato);
   uwAux = vsnprintf (sbAux, Cadena::LON_MAX_STRING, psbStr, ap);
   va_end (ap);

   this->mcadStr.setCharArray (sbAux, uwAux);
   this->muwLong = uwAux;
   if (uwFormato != 0)
      setFormato (uwFormato);
}

bool CadenaFormato::getSubcadenaFormato (Cadena& cadStr, UWordType& uwFormato,
   UWordType& uwIndice) const
{
   Cadena    cadFormato;
   SByteType sbAux;
   UWordType uwPos;
   bool      booFormato;

   if (uwIndice >= this->mcadStr.getLong ())
      return false;

   //Avanzar hasta uwIndice, guardando los formatos
   uwPos = 0;
   uwFormato = 0;
   booFormato = false;
   while (uwPos < uwIndice && uwPos < this->mcadStr.getLong ())
   {
      sbAux = this->mcadStr[uwPos];
      uwPos++;

      if (sbAux == INI_TAG)
      {
         booFormato = true;
         cadFormato.setEmpty ();
      }
      else if (sbAux == FIN_TAG)
      {
        booFormato = false;
         if (cadFormato[0] == FIN)
            uwFormato &= ~(getFormato (cadFormato));
         else
            uwFormato |= getFormato (cadFormato);
      }
      else if (booFormato)
      {
         cadFormato += sbAux;
      }
   }

   //Avanzar hasta encontrar el siguiente tag de formato (apertura o cierre)
   sbAux = '\0';
   cadStr.setEmpty ();
   while (sbAux != FIN_TAG && uwIndice < this->mcadStr.getLong ())
   {
      sbAux = this->mcadStr[uwIndice];
      uwIndice++;

      if (sbAux == INI_TAG)
         booFormato = true;
      else if (sbAux == FIN_TAG)
         booFormato = false;
      else if (!booFormato)
         cadStr += sbAux;
   }

   //Si la subcadena esta vacia (dos tags de formato seguidos), volver a llamar
   //a la funcion para avanzar hasta el siguiente tag
   if (cadStr.getLong () == 0)
      return getSubcadenaFormato (cadStr, uwFormato, uwIndice);
   else
      return true;
}


UWordType CadenaFormato::getLong () const
{
   return this->muwLong;
}

UWordType CadenaFormato::getLongFormato () const
{
   return this->mcadStr.getLong ();
}

Cadena CadenaFormato::getCadena () const
{
   Cadena    lFormato;
   Cadena    lTexto;
   UWordType luwIndice;
   UWordType luwTam;
   bool      lbTag;
   SByteType lbCaracter;

   lbTag = false;
   luwTam = mcadStr.getLong();
   for(luwIndice = 0;luwIndice < luwTam; luwIndice++)
   {
      lbCaracter = mcadStr.getCaracter(luwIndice);
      if( INI_TAG ==  lbCaracter)
         lbTag = true;

      if( false == lbTag )
      {
         lTexto += lbCaracter;
      }
      else
      {
         if(lbCaracter != FIN)
          lFormato += lbCaracter;
      }

      if( (FIN_TAG == lbCaracter) && (true == lbTag) )
      {
         lbTag = false;
         if( false == hayFormatoCompleto(lFormato) )
         {
            lTexto += lFormato;
         }
         lFormato.setEmpty();
      }
   }
   if( true == lbTag )
   {
      // NO hay fin de tag
      lTexto += lFormato;
   }
   return (lTexto);
}

bool CadenaFormato::hayFormatoCompleto(const Cadena& lFormato) const
{
   if(lFormato ==   FMT_CNEGRITA )
    return(true);
   if(lFormato ==   FMT_CPARPADEO )
    return(true);
   if(lFormato ==   FMT_CSUBRAYADO )
    return(true);
   if(lFormato ==   FMT_CINVERSO )
    return(true);
   return(false);
}

Cadena CadenaFormato::getCadenaFormato () const
{
   return this->mcadStr;
}

bool CadenaFormato::hayFormato () const
{
   return (regexec (&CadenaFormato::msRegexpFormato, this->mcadStr.getCadena (),
      0, NULL, 0) == 0);
}


void CadenaFormato::setFormato (UWordType uwFormato)
{
   if (uwFormato & NEGRITA)
      setNegrita ();
   if (uwFormato & PARPADEO)
      setParpadeo ();
   if (uwFormato & SUBRAYADO)
      setSubrayado ();
   if (uwFormato & VIDEO_INVERSO)
      setVideoInverso ();
}

void CadenaFormato::setNegrita ()
{
   setFormato (FMT_NEGRITA);
}

void CadenaFormato::setParpadeo ()
{
   setFormato (FMT_PARPADEO);
}

void CadenaFormato::setSubrayado ()
{
   setFormato (FMT_SUBRAYADO);
}

void CadenaFormato::setVideoInverso ()
{
   setFormato (FMT_INVERSO);
}


////////////////////   PRIVATE      ////////////////////

inline void CadenaFormato::setFormato (CPSByteType psbFmt)
{
   Cadena cadAux(this->mcadStr);
   this->mcadStr.formCadena ("%c%s%c%s%c%c%s%c", INI_TAG, psbFmt, FIN_TAG,
      cadAux.getCadena (), INI_TAG, FIN, psbFmt, FIN_TAG);
}

inline bool CadenaFormato::validarCadena (const Cadena& cadStr) const
{
   return (regexec (&CadenaFormato::msRegexpValidar, cadStr.getCadena (),
      0, NULL, 0) == 0);
}

UWordType CadenaFormato::calcularLongSinFormato (const Cadena& cadFrmt) const
{
   UWordType uwLong;
   UWordType uwAux;
   UWordType i;
   bool      count;

   uwLong = 0;
   count = true;
   uwAux = cadFrmt.getLong ();
   CPSByteType psbStr = cadFrmt.getCadena ();
   for (i = 0; i < uwAux; i++)
   {
      if (psbStr[i] == INI_TAG)
         count = false;
      else if (count)
         uwLong++;
      else if (psbStr[i] == FIN_TAG)
         count = false;
   }

   return uwLong;
}

void CadenaFormato::inicializarRegexps ()
{
   //*TODO: fer que aquestes dues expressions regulars es construeixin a partir
   //*de INI_TAG, FIN_TAG, i FIN
   static CPSByteType psbRegexpValidar =
      "^(.*<[(B|U|BLINK|INV)]>.*</[(B|U|BLINK|INV)]>.*)*$";
   static CPSByteType psbRegexpFormato =
      "^.*<([(B|U|BLINK|INV)])>.*</\\1>.*$";

   if (!CadenaFormato::mbooRegexp)
   {
      CadenaFormato::mmtxRegexp.lock ();
      regcomp (&CadenaFormato::msRegexpValidar, psbRegexpValidar, REG_EXTENDED);
      regcomp (&CadenaFormato::msRegexpFormato, psbRegexpFormato, REG_EXTENDED);
      CadenaFormato::mbooRegexp = true;
      CadenaFormato::mmtxRegexp.unlock ();
   }
}

inline UWordType CadenaFormato::getFormato (const Cadena& cadFormato) const
{
   if (cadFormato.finalizaCon (FMT_NEGRITA))
      return NEGRITA;
   else if (cadFormato.finalizaCon (FMT_PARPADEO))
      return PARPADEO;
   else if (cadFormato.finalizaCon (FMT_SUBRAYADO))
      return SUBRAYADO;
   else if (cadFormato.finalizaCon (FMT_INVERSO))
      return VIDEO_INVERSO;
   else
      return 0;
}
