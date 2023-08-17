// =======================
//  FICHERO - GestorFirmware.cpp
// =======================

#include "GestorFirmware.h"  // clase implementada

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
GestorFirmware::GestorFirmware()
{

}

GestorFirmware::~GestorFirmware()
{

}

// =================   OPERACIONES   ===================
void GestorFirmware::inicializar()
{
    STrz::setTrazable (&mTrazador);
    mTrazador.inicializar();
    mTrazador.mFichTraza.setNombreFichero("$FF_LOG/firmware.log");
    mTrazador.mFichTraza.setNumMaxFicheros(10);
    mTrazador.mFichTraza.setLongMaxFichero(10000 * 1024);
    mTrazador.setNivel(Trazable::TRAZAR_BASIC);
}

bool GestorFirmware::version()
{
  STrz::basic(ConstTraza::T_TRAZA_DISPOSITIVO,"Consulta de la version de software" );
  return(false);
}

bool GestorFirmware::download( const Cadena& lFichero)
{
  STrz::basic(ConstTraza::T_TRAZA_DISPOSITIVO,"Descarga del fichero %s",lFichero.getCadena() );
  return(false);
}

// =================   ACCESO       ====================
void GestorFirmware::setVersion(const Cadena& lVersion)
{
  mVersion = lVersion;
}

void GestorFirmware::setPuerto(const Cadena& lPuerto)
{
  mPuerto = lPuerto;
}

// =================   CONSULTA      ===================
Cadena GestorFirmware::getVersion()
{
  return(mVersion);
}

Cadena GestorFirmware::getPuerto()
{
 return(mPuerto);
}



// =======================
//  FIN DEL FICHERO - GestorFirmware.cpp
// =======================



