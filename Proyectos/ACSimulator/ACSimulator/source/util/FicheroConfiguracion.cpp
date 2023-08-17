// ============================
//  FICHERO - FicheroConfiguracion.cpp
// ============================

#include "FicheroConfiguracion.h"

FicheroConfiguracion::FicheroConfiguracion()
{
   setUMask( 0 );
   setMask ( 0644 );
   setLectura ( true );
   setEscritura ( false );
   setExclusivo ( false );
   setAppend ( false );
   setSincronizado ( false );
   setNoctty ( false );
   setNoBloqueante ( false );

}

FicheroConfiguracion::~FicheroConfiguracion()
{

}

void FicheroConfiguracion::setNombre(const Cadena& mNombre)
{
   this->mNombre = mNombre;
   this->mNombre.resuelveVariableEntorno();
}

void FicheroConfiguracion::setLectura(const bool bLectura)
{
   this->bLectura=bLectura;
}

void FicheroConfiguracion::setEscritura(const bool bEscritura)
{
   this->bEscritura=bEscritura;
}

void FicheroConfiguracion::setExclusivo(const bool bExclusivo)
{
   this->bExclusivo=bExclusivo;
}

void FicheroConfiguracion::setAppend(const bool bAppend)
{
   this->bAppend=bAppend;
}

void FicheroConfiguracion::setSincronizado(const bool bSincronizado)
{
   this->bSincronizado=bSincronizado;
}

void FicheroConfiguracion::setNoctty(const bool bNoctty)
{
   this->bNoctty=bNoctty;
}

void FicheroConfiguracion::setNoBloqueante(const bool bNoBloqueante)
{
   this->bNoBloqueante=bNoBloqueante;
}


void FicheroConfiguracion::setUMask(mode_t mumask)
{
   this->mumask=mumask;
}

void FicheroConfiguracion::setMask(mode_t mmask)
{
   this->mmask=mmask;
}

Cadena FicheroConfiguracion::getNombre ( )
{
   return this->mNombre;
}

const bool FicheroConfiguracion::getLectura ( )
{
   return this->bLectura;
}

const bool FicheroConfiguracion::getEscritura ( )
{
   return this->bEscritura;
}

const bool FicheroConfiguracion::getExclusivo ( )
{
   return this->bExclusivo;
}

const bool FicheroConfiguracion::getAppend ( )
{
   return this->bAppend;
}

const bool FicheroConfiguracion::getSincronizado ( )
{
   return this->bSincronizado;
}

const bool FicheroConfiguracion::getNoctty ( )
{
   return this->bNoctty;
}

const bool FicheroConfiguracion::getNoBloqueante ( )
{
   return this->bNoBloqueante;
}

const mode_t FicheroConfiguracion::getUMask ( )
{
   return this->mumask;
}

const mode_t FicheroConfiguracion::getMask ( )
{
   return this->mmask;
}

// ============================
//  FIN DEL FICHERO - FicheroConfiguracion.cpp
// ============================
