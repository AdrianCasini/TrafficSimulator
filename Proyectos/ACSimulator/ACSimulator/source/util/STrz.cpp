// =======================
//  FICHERO - STrz.CPP
// =======================

#ifdef _VIALINUX_SYSLOG

#include "SysTrz.h"

#else

#include "STrz.h"

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

   Trazable*   STrz::pmTrazable  = NULL;
   AutoMutex   STrz::mMutex;
   SByteType   STrz::sbBuffer[STrz::BUFFER_SIZE + 1];

// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================


   ////////////////////   PUBLIC       ////////////////////
   // =================   OPERACIONES      ===================

   STrz::STrz()
   {

   }

   STrz::~STrz()
   {

   }


   void STrz::debug( SWordType swTipoTraza,
      CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_debug( swTipoTraza, psbParametros, ap );
      va_end ( ap );
   }

   void STrz::va_debug( SWordType swTipoTraza,
      CPSByteType psbParametros, va_list ap )
   {
      vsnprintf (STrz::sbBuffer, BUFFER_SIZE, psbParametros, ap);
      STrz::sbBuffer[BUFFER_SIZE] = '\0';
      doDebug( swTipoTraza, STrz::sbBuffer);
   }

   void STrz::debug( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg,
         SWordType swTipoTraza, CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_debug( attribute, fg, bg, swTipoTraza, psbParametros, ap );
      va_end ( ap );
   }

   void STrz::va_debug( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg,
         SWordType swTipoTraza, CPSByteType psbParametros, va_list ap )
   {
      vsnprintf (STrz::sbBuffer, BUFFER_SIZE, psbParametros, ap);
      STrz::sbBuffer[BUFFER_SIZE] = '\0';
      doDebug( swTipoTraza, CadenaColor(STrz::sbBuffer).setColor(attribute, fg, bg).c_str() );
   };


   void STrz::debug( SWordType swTipoTraza, const Cadena &str )
   {
      MutexLocker lProteccion (mMutex);
      doDebug( swTipoTraza, (CPSByteType)str.getCadena () );
   }

   void STrz::debug( SWordType swTipoTraza, Excepcion& e )
   {
      for( UWordType i = 0; i < e.getNumeroExcepciones(); i++ )
      {
         STrz::debug( swTipoTraza ,"Excepcion: %s",e.getExcepcion(i));
      }
   };

   void STrz::basic( SWordType swTipoTraza,
      CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_basic( swTipoTraza, psbParametros, ap );
      va_end ( ap );
   }

   void STrz::va_basic( SWordType swTipoTraza,
      CPSByteType psbParametros, va_list ap )
   {
      vsnprintf (STrz::sbBuffer, BUFFER_SIZE, psbParametros, ap);
      STrz::sbBuffer[BUFFER_SIZE] = '\0';
      doBasic( swTipoTraza, STrz::sbBuffer );
   }

   void STrz::basic( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg,
         SWordType swTipoTraza, CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_basic( attribute, fg, bg, swTipoTraza, psbParametros, ap );
      va_end ( ap );
   }

   void STrz::va_basic( ColorTypeE attribute, ColorTypeE fg, ColorTypeE bg,
         SWordType swTipoTraza, CPSByteType psbParametros, va_list ap )
   {
      vsnprintf (STrz::sbBuffer, BUFFER_SIZE, psbParametros, ap);
      STrz::sbBuffer[BUFFER_SIZE] = '\0';
      doBasic( swTipoTraza, CadenaColor(STrz::sbBuffer).setColor(attribute, fg, bg).c_str() );
   }


   void STrz::basic( SWordType swTipoTraza, const Cadena &str )
   {
      MutexLocker lProteccion (mMutex);
      doBasic( swTipoTraza, (CPSByteType)str.getCadena () );
   }

   void STrz::basic( SWordType swTipoTraza, Excepcion& e )
   {
      for( UWordType i = 0; i < e.getNumeroExcepciones(); i++ )
      {
         STrz::basic( swTipoTraza ,"Excepcion: %s",e.getExcepcion(i));
      }
   };

   void STrz::error( SWordType swTipoTraza, CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_basic( FG_RED, BG_BLACK, BOLD_ON, swTipoTraza, psbParametros, ap );
      va_end ( ap );
   };

   void STrz::warning( SWordType swTipoTraza,  CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_basic( FG_YELLOW, BG_BLACK, BOLD_ON, swTipoTraza, psbParametros, ap );
      va_end ( ap );
   };

   void STrz::info( SWordType swTipoTraza,  CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_basic( INFO_COLOR, swTipoTraza, psbParametros, ap );
      va_end ( ap );
   };

   void STrz::trace( SWordType swTipoTraza,  CPSByteType psbParametros, ... )
   {
      va_list  ap;

      MutexLocker lProteccion (mMutex);
      va_start (ap,psbParametros);
      va_basic( TRACE_COLOR, swTipoTraza, psbParametros, ap );
      va_end ( ap );
   };

   void STrz::excepcion( SWordType swTipoTraza, Excepcion& e )
   {
      for( UWordType i = 0; i < e.getNumeroExcepciones(); i++ )
      {
         STrz::basic( FG_WHITE, BG_RED, BOLD_ON, swTipoTraza ,"Excepcion: %s",e.getExcepcion(i));
      }
   };

   Trazable*   STrz::getTrazable()
   {
      if( NULL != ( STrz::pmTrazable ) )
      return (STrz::pmTrazable);
      else
      throw MemoriaExcepcion(__AQUI__,"El puntero de traza es igual a NULL");
   }

// =================   ACCESO       ====================

   UDWordType STrz::getLongFicheros()
   {
      return (STrz::getTrazable()->getLongFicheros());
   }

   UWordType STrz::getNumFicheros()
   {
      return (STrz::getTrazable()->getNumFicheros());
   }

   Cadena& STrz::getNombreFichero()
   {
      return (STrz::getTrazable()->getNombreFichero());
   }

   UWordType STrz::getLongMaxMensaje()
   {
      return (STrz::getTrazable()->getLongMaxMensaje());
   }

   SWordType STrz::getProceso()
   {
      return (STrz::getTrazable()->getId());
   }

   void STrz::setProceso(const Cadena& lNombre,SWordType swCodProc)
   {
      STrz::getTrazable()->setProceso(lNombre,swCodProc);
   }

   void STrz::setLongFicheros(UDWordType udwLongFichTrazaAltern)
   {
      STrz::getTrazable()->setLongFicheros(udwLongFichTrazaAltern);
   }

   void STrz::setNumFicheros(UWordType uwNumFichTrazaAltern)
   {
      STrz::getTrazable()->setNumFicheros(uwNumFichTrazaAltern);
   }

   void STrz::setNombreFichero(const Cadena& mFichTrazaAltern)
   {
      STrz::getTrazable()->setNombreFichero(mFichTrazaAltern);
   }

   void STrz::setLongMaxMensaje(UWordType uwLongMaxMensaje)
   {
      STrz::getTrazable()->setLongMaxMensaje(uwLongMaxMensaje);
   }

   void STrz::setTrazable(Trazable *pTrazable)
   {
      STrz::pmTrazable = pTrazable;
   }

////////////////////   PRIVATE       ////////////////////
   void STrz::salirPantalla(SWordType swTipoTraza, CPSByteType str)
   {
      try
      {
         Cadena mTraza;

         // Montamos la traza
         mTraza = FechaHora::ahora (true).getCadena (FechaHora::TRAZA);
         mTraza += '|';
         mTraza += swTipoTraza;
         mTraza += '|';
         mTraza += str;
         mTraza += '\n';
         std::cout << "TrzDef: " << mTraza.getCadena ();
         fflush( stdout );
      }
      catch( Excepcion &e)
      {
        std::cout << "TrzDef: Error generando traza: %s" << e.getTextoUltimaExcepcion();
        fflush( stdout );
      }
   }

   void STrz::doDebug( SWordType swTipoTraza, CPSByteType str )
   {
      if( STrz::pmTrazable )
         STrz::pmTrazable->debug( swTipoTraza, str );
      else
         STrz::salirPantalla( swTipoTraza, str );
   }

   void STrz::doBasic( SWordType swTipoTraza, CPSByteType str )
   {
      if( STrz::pmTrazable )
         STrz::pmTrazable->basic( swTipoTraza, str );
      else
         STrz::salirPantalla( swTipoTraza, str );
   }
#endif
// ===============================
//  FIN DEL FICHERO - STrz.CPP
// ===============================

