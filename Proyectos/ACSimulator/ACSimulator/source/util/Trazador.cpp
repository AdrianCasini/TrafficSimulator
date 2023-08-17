// =======================
//  FICHERO - Trazador.cpp
// =======================

#include "Trazador.h"  // clase implementada
#include "util/NivelTraza.h"
#include "thread/MutexLocker.h"

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
   Trazador::Trazador()
   {

   }

   Trazador::~Trazador()
   {

   }

   // ===============   OPERACIONES    ==================

   void Trazador::debug(SWordType lswTipoTraza, CPSByteType psbTraza)
   {
      MutexLocker mtx(mMutex);
      if( NivelTraza::TRAZAR_DEBUG == getNivel() )
      {
         mFechaHora.getAhora(true);
         FichTraza::registra(this->mNombreProc,lswTipoTraza,psbTraza,mFechaHora);
      }
   }

   void Trazador::basic(SWordType lswTipoTraza, CPSByteType psbTraza)
   {
      MutexLocker mtx(mMutex);
      if( NivelTraza::TRAZAR_NO != getNivel() )
      {
         mFechaHora.getAhora(true);
         FichTraza::registra(this->mNombreProc,lswTipoTraza,psbTraza,mFechaHora);
      }
   }

   void Trazador::inicializar()
   {
   }

   void Trazador::destruir()
   {
   }

   // =================   CONSULTA      ===================


   UWordType Trazador::getId()
   {
     return ( this->mswProceso );
   }

   UWordType Trazador::getLongMaxMensaje()
   {
     return ( FichTraza::getLongMaxMensaje() );
   }

   Cadena& Trazador::getNombreFichero( )
   {
     return ( FichTraza::getNombreFichero() );
   }

   UWordType Trazador::getNumFicheros()
   {
     return ( FichTraza::getNumMaxFicheros () );
   }

   UDWordType Trazador::getLongFicheros()
   {
      return ( FichTraza::getLongMaxFichero () );
   }

   Cadena& Trazador::getNombreProc()
   {
      return(this->mNombreProc);
   }

   NivelTraza::NivelTrazaTypeE  Trazador::getNivel ( )
   {
      return ( this->mTypeENivel );
   }


   // =================   ACCESO       ====================

   void Trazador::setProceso(const Cadena& lNombre,UWordType luwId)
   {
     mNombreProc = lNombre;
     mswProceso = luwId;
   }

   void Trazador::setLongMaxMensaje(UWordType luwLongMax)
   {
     FichTraza::setLongMaxMensaje( luwLongMax );
   }

   void Trazador::setNombreFichero(const Cadena& lFichAltern)
   {
     FichTraza::setNombreFichero( lFichAltern );
   }

   void Trazador::setNumFicheros(UWordType luwNumFichAltern)
   {
     FichTraza::setNumMaxFicheros ( luwNumFichAltern );
   }

   void Trazador::setLongFicheros(UDWordType ludwLongFichAltern)
   {
     FichTraza::setLongMaxFichero ( ludwLongFichAltern );
   }

   void Trazador::setNivel (  NivelTraza::NivelTrazaTypeE lNivel)
   {
     this->mTypeENivel = lNivel;
   }

// =======================
//  FIN DEL FICHERO - Trazador.cpp
// =======================
