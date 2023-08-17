// =======================
//  FICHERO - CfgGestorProcesos.cpp
// =======================

#include "CfgGestorProcesos.h"  // clase implementada

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
CfgGestorProcesos::CfgGestorProcesos()
{
  sdwTInicioProcesos = 0;
  sdwTEInicioProcesos = 0;
  sdwTReInicioProcesos = 0;
  sdwTMaxKill = 0;
  sdwTCfgWatchDog = 0;
  sdwEntreKeepAlive = 0;
  sdwAckKeepAlive = 0;
  sdwIntentosArranque = 0;
  mbWatchDog = false;
  mNivel = NivelTraza::TRAZAR_DEBUG;
}

CfgGestorProcesos::~CfgGestorProcesos()
{
}
// =================   OPERACIONES   ===================

void CfgGestorProcesos::leer(Trazador* lTraza)
{
  Cadena      mParametro;

  if( true ==  getParam("FICHERO_PROCESOS",1,mParametro,"") )
  {
     if(true == mParametro.contieneVariableEntorno() )
      mParametro.resuelveVariableEntorno();

     mFichProcesos = mParametro;
     getParam("PATH_PROCESOS",1,mParametro,"$FF_BIN/");
     if(true == mParametro.contieneVariableEntorno() )
       mParametro.resuelveVariableEntorno();
     mPathProcesos = mParametro;

     if ( lTraza  != NULL )
     {
      getParam("FICHERO_LOG",1,mParametro,"$FF_LOG/gestDDorprocesos.log");
      if(true == mParametro.contieneVariableEntorno() )
        mParametro.resuelveVariableEntorno();
      lTraza->setNombreFichero(mParametro);

      getParam("NUM_FICHEROS_LOG",1,mParametro,"10");
      if ( false == mParametro.isInteger() )
        mParametro =  10 ;
      lTraza->setNumFicheros((UDWordType)mParametro.toSDWordType());

      getParam("LONG_MAX_LOG",1,mParametro,"200");
      if ( false == mParametro.isInteger() )
        mParametro =  200;
      lTraza->setLongFicheros(mParametro.toSDWordType() * 1024);

      getParam("LONG_MAX_MENSAJE_LOG",1,mParametro,"150");
      if ( false == mParametro.isInteger() )
        mParametro =  150 ;
      lTraza->setLongMaxMensaje(mParametro.toSDWordType());

      getParam("NIVEL_LOG",1,mParametro,"BASIC");
      if(!strcmp(mParametro.getCadena(),"BASIC"))
       mNivel = NivelTraza::TRAZAR_BASIC;
      else if(!strcmp(mParametro.getCadena(),"DEBUG"))
        mNivel = NivelTraza::TRAZAR_DEBUG;
      else if(!strcmp(mParametro.getCadena(),"NO"))
       mNivel = NivelTraza::TRAZAR_NO;
      else
       mNivel = NivelTraza::TRAZAR_BASIC;
     }
     getParam("TIEMPO_INICIO_PROCESOS",1,mParametro,"1000");
     if ( true == mParametro.isInteger() )
       sdwTInicioProcesos =  mParametro.toSDWordType() ;
     else
       sdwTInicioProcesos = 1000;

     getParam("TIEMPO_ENTRE_INICIO_PROCESOS",1,mParametro,"1000");
     if ( false == mParametro.isInteger() )
       mParametro =  1000 ;
     sdwTEInicioProcesos = mParametro.toSDWordType();

     getParam("TIEMPO_ENTRE_REINICIO_PROCESOS",1,mParametro,"5000");
     if ( false == mParametro.isInteger() )
       mParametro =  5000 ;
     sdwTReInicioProcesos = mParametro.toSDWordType();

     getParam("TIEMPO_MAX_SIGKILL",1,mParametro,"10000");
     if ( false == mParametro.isInteger() )
       mParametro =  10000 ;
     sdwTMaxKill= mParametro.toSDWordType();

     getParam("TIEMPO_CFG_WATCHDOG",1,mParametro,"50000");
     if ( (true == mParametro.isInteger()) &&  ( mParametro.toSDWordType() < 6000 ) )
       sdwTCfgWatchDog = 6000;
     else if ( false == mParametro.isInteger() )
       sdwTCfgWatchDog = 6000;
     else
       sdwTCfgWatchDog = mParametro.toSDWordType();

     getParam("TIEMPO_ENTRE_KEEPALIVE",1,mParametro,"40000");
     if ( false == mParametro.isInteger() )
       mParametro =  40000 ;
     sdwEntreKeepAlive = mParametro.toSDWordType();

     getParam("TIEMPO_ACK_KEEPALIVE",1,mParametro,"30000");
     if ( false == mParametro.isInteger() )
       mParametro =  30000 ;
     sdwAckKeepAlive = mParametro.toSDWordType();

     getParam("NUMERO_INTENTOS_ARRANCAR",1,mParametro,"5");
     if ( false == mParametro.isInteger() )
       mParametro =  5 ;
     sdwIntentosArranque = mParametro.toSDWordType();

     getParam("WATCHDOG",1,mParametro,"SI");
     if(!strcmp(mParametro.getCadena(),"SI")) mbWatchDog = true;
     else if(!strcmp(mParametro.getCadena(),"NO")) mbWatchDog = false;
     else mbWatchDog = true;
  }
  else
  {
    throw NoExisteExcepcion(__AQUI__,"No existe el FICHERO_PROCESOS: en " \
     " gestorprocesos.cfg este fichero es obligatorio para el gestor de procesos.");
  }
}


// =================   ACCESO       ====================
// =================   CONSULTA      ===================
SDWordType  CfgGestorProcesos::getTEInicioProcesos ()
{
  return(sdwTEInicioProcesos);
}

SDWordType  CfgGestorProcesos::getTInicioProcesos ()
{
 return(sdwTInicioProcesos);
}

SDWordType  CfgGestorProcesos::getTReInicioProcesos()
{
 return(sdwTReInicioProcesos);
}

SDWordType  CfgGestorProcesos::getTMaxKill ()
{
 return(sdwTMaxKill);
}

SDWordType  CfgGestorProcesos::getTCfgWatchDog ()
{
 return(sdwTCfgWatchDog);
}

SDWordType  CfgGestorProcesos::getEntreKeepAlive ()
{
 return(sdwEntreKeepAlive);
}

SDWordType  CfgGestorProcesos::getAckKeepAlive ()
{
 return(sdwAckKeepAlive);
}

bool CfgGestorProcesos::getWatchDog()
{
 return(mbWatchDog);
}

SDWordType CfgGestorProcesos::getIntentosArranque()
{
 return(sdwIntentosArranque);
}

Cadena&  CfgGestorProcesos::getPathProcesos()
{
 return(mPathProcesos);
}

Cadena&  CfgGestorProcesos::getFichProcesos()
{
  return(mFichProcesos);
}

NivelTraza::NivelTrazaTypeE CfgGestorProcesos::getNivel()
{
  return(mNivel);
}

bool CfgGestorProcesos::getDebug()
{
  if ( mNivel ==  NivelTraza::TRAZAR_DEBUG )
   return( true);
  else
   return(false);
}

// =======================
//  FIN DEL FICHERO - CfgGestorProcesos.cpp
// =======================



