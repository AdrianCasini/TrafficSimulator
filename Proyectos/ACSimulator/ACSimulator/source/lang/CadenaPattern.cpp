// =======================
//  FICHERO - CadenaPattern.cpp
// =======================

#include "CadenaPattern.h"  // clase implementada

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////

// ===============   CICLO DE VIDA    ==================
CadenaPattern::CadenaPattern()
{
    mbExpresion = false;
    mbMay = false;
    mbNoSubstring = false;
    mbNewLine = false;
    memset(&mRegex_t,0,sizeof(mRegex_t));
}

CadenaPattern::~CadenaPattern()
{

}

// =================   OPERACIONES   ===================
void CadenaPattern::setPattern(const Cadena& lPattern)
{
  SWordType lswFlags;

  lswFlags = 0;
  if ( true == mbExpresion ) lswFlags |= REG_EXTENDED;
  if ( true == mbMay ) lswFlags |= REG_ICASE;
  if ( true == mbNoSubstring ) lswFlags |= REG_NOSUB;
  if ( true == mbNewLine ) lswFlags |= REG_NEWLINE;

  switch(regcomp(&mRegex_t, lPattern.getCadena(), lswFlags) )
  {
    case REG_BADRPT:
        throw Excepcion(__AQUI__,"Invalid use of repetition operators such as using `*' as the first character.");
    case REG_BADBR:
        throw Excepcion(__AQUI__,"Invalid use of back reference operator.");
    case REG_EBRACE:
        throw Excepcion(__AQUI__,"Un-matched brace interval operators.");
    case REG_EBRACK:
        throw Excepcion(__AQUI__,"Un-matched bracket list operators.");
    case REG_ERANGE:
        throw Excepcion(__AQUI__,"Invalid use of the range operator, eg. the ending point of the range occurs prior to the starting point.");
    case REG_ECTYPE:
        throw Excepcion(__AQUI__,"Unknown character class name.");
    case REG_ECOLLATE:
        throw Excepcion(__AQUI__,"Invalid collating element.");
    case REG_EPAREN:
        throw Excepcion(__AQUI__,"Un-matched parenthesis group operators.");
    case REG_ESUBREG:
        throw Excepcion(__AQUI__,"Invalid back reference to a subexpression.");
//     case  REG_EEND:
//        throw Excepcion(__AQUI__,"Non specific error.  This is not defined by POSIX.2.");
    case REG_EESCAPE:
        throw Excepcion(__AQUI__,"Trailing backslash.");
    case REG_BADPAT:
        throw Excepcion(__AQUI__,"Invalid use of pattern operators such as group or list.");
//     case  REG_ESIZE:
//        throw Excepcion(__AQUI__,"Compiled regular expression requires a pattern buffer larger than 64Kb.  This is not defined by POSIX.2.");
    case REG_ESPACE:
        throw Excepcion(__AQUI__,"The regex routines ran out of memory.");
    default:
        throw Excepcion(__AQUI__,"Unknown error.");
  }
}

bool CadenaPattern::check(const Cadena& lTexto)
{
   if ( 0 != regexec(&mRegex_t, lTexto.getCadena(), 0, NULL, 0))
    return (false );
   else
    return (true);
}

// =================   ACCESO       ====================
void CadenaPattern::setExpresionExtendida(bool lOpcion)
{
      mbExpresion = lOpcion;
}

void CadenaPattern::setDiffMayusculas(bool lOpcion)
{
    mbMay = lOpcion;
}

void CadenaPattern::setNoSubString(bool lOpcion)
{
    mbNoSubstring = lOpcion;
}

void CadenaPattern::setNewLine(bool lOpcion)
{
    mbNewLine = lOpcion;
}
// =================   CONSULTA      ===================


bool CadenaPattern::checkIP ( const Cadena& lCadena )
{
   CadenaPattern lPattern;

   lPattern.setPattern ("^([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])(\\.([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])){3}");
   return (lPattern.check(lCadena));
}


// =======================
//  FIN DEL FICHERO - CadenaFormato.cpp
// =======================



