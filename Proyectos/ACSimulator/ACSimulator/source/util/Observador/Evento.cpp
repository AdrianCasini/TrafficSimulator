// =======================
//  FICHERO - Evento.cpp
// =======================

#include "Evento.h"  // clase implementada

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
Evento::Evento()
{
   muwOffset = 0;
   memset((PUByteType)&mMensaje, 0, sizeof(mMensaje));
}

Evento::Evento( const SDWordType lId,const SDWordType lParametro)
{
  mMensaje.muwLong = sizeof(lParametro);
  mMensaje.msdwId = lId;
  muwOffset = 0;
  setDato((CPSByteType)&lParametro,sizeof(lParametro));
}

Evento::~Evento()
{

}
// =================   OPERACIONES   ===================
void Evento::inicioDatos(const SDWordType lId)
{
  muwOffset = 0;
  mMensaje.msdwId = lId;
}

void Evento::inicioLectura()
{
  muwOffset = 0;
}

// =================   ACCESO       ====================
void Evento::setId(SDWordType lId)
{
  mMensaje.msdwId = lId;
}

void Evento::set(const Evento& lEvento)
{
  memcpy((PUByteType)&mMensaje,(PUByteType)&lEvento.mMensaje,sizeof(mMensaje));
}

void Evento::setParametro(const SDWordType lParametro)
{
  setDato((SByteType*)&lParametro,sizeof(lParametro));
}

void Evento::setDato(CPSByteType psbDatos,const UWordType luwLong)
{
   if ( luwLong > SIZE_DATOS )
    throw RangoExcepcion(__AQUI__,"El dato es superior al tamanyo del buffer");
   else
   {
    memset(mMensaje.msbDatos, 0, sizeof(mMensaje.msbDatos));
    memcpy(mMensaje.msbDatos,psbDatos,luwLong);
    mMensaje.muwLong = luwLong;
   }
}

void Evento::addDato(CPSByteType psbDatos,const UWordType luwLong)
{
   if ( (luwLong+muwOffset) > SIZE_DATOS )
    throw RangoExcepcion(__AQUI__,"El dato es superior al tamanyo del buffer");
   else
   {
    if (muwOffset == 0)
    {
     memset(mMensaje.msbDatos, 0, sizeof(mMensaje.msbDatos));
     mMensaje.muwLong = 0;
    }
    memcpy(mMensaje.msbDatos + muwOffset,psbDatos,luwLong	);
    mMensaje.muwLong += luwLong;
    muwOffset += luwLong;
   }
}

void Evento::addDato(const Cadena& lDato)
{
  UWordType luwLong;
  luwLong =lDato.getLong();
  setDato((PSByteType)&luwLong,sizeof(UWordType));
  setDato(lDato.getCadena(),lDato.getLong());
}
// =================   CONSULTA      ===================
SDWordType Evento::getId() const
{
   return(mMensaje.msdwId);
}

SDWordType Evento::getParametro() const
{
   SDWordType lsdwTemp;

   memcpy(&lsdwTemp,mMensaje.msbDatos,sizeof(lsdwTemp));
   return(lsdwTemp);
}

void Evento::copyDatos(PSByteType psbDatos,UWordType luwLong) const
{
   memcpy (psbDatos,mMensaje.msbDatos,luwLong);
}

void Evento::getDatos(PSByteType psbDatos,UWordType luwLong)
{
  if ( (luwLong + muwOffset) < SIZE_DATOS )
  {
   memcpy (psbDatos,mMensaje.msbDatos + muwOffset,luwLong);
   muwOffset += luwLong;
  }
  else
   throw RangoExcepcion(__AQUI__,"El dato es superior al tamanyo del buffer");
}

void Evento::getDatos(Cadena& lTexto)
{
  UWordType lLong;
  SByteType lTmp[Cadena::LON_MAX_STRING];

  getDatos((PSByteType)&lLong,sizeof(UWordType));
  if ( lLong < Cadena::LON_MAX_STRING)
  {
   getDatos(lTmp,lLong);
   lTexto = lTmp;
  }
  else
   throw RangoExcepcion(__AQUI__,"El dato es superior al tamanyo del buffer");
}

UWordType Evento::getLong() const
{
  return(mMensaje.muwLong);
}

// =======================
//  FIN DEL FICHERO - Evento.cpp
// =======================



