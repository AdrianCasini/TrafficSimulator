// =======================
//  FICHERO - FichTraza.cpp
// =======================

#include "FichTraza.h"  // clase implementada
#include <libgen.h>

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

  FichTraza::FichTraza()
  {
    this->sbSeparadorCampos = '|';
    this->udwLongMaxFichero = 200000;
    this->uwNumMaxFicheros = 5;
    this->uwLongMaxMensaje = 140;
    this->mudwTamFichero = UDWordTypeMax; //Para forzar a que se abra el fichero la primera vez
  }

  FichTraza::~FichTraza()
  {

  }

  void FichTraza::registra(const Cadena& mProc, const Cadena& mTipoTr, const Cadena& mMsj,
     const FechaHora& lMomento)
  {
    bool bOutObligada;

    bOutObligada = false;
    if (this->mudwTamFichero >= this->udwLongMaxFichero)
    {
      try
      {
         if (isAbierto ())
            cerrar ();

         // Organizamos los ficheros, borramos los que sobran y creamos nuevos
         // en el caso de que el .log este lleno segun parametros.
         organizaFicheros ();
         setNombre (this->mNombreCompleto);

         // Abrimos el fichero y nos colocamos al final para la escritura de trazas
         setEscritura ( true );
         setLectura ( false );
         setAppend ( true );

         if( true == Directorio::existe(this->mNombreCompleto, Directorio::FICHERO) )
         {
            abrir();
            this->mudwTamFichero = getTamano ();
         }
         else
         {
            crear();
            this->mudwTamFichero = 0;
         }

         irFin();
      }
      catch(Excepcion& e)
      {
         bOutObligada = true;
      }
    }

    // Formateamos el tiempo y el mensaje a Trazar
    this->mProcRecortado = mProc;
    this->mTipoTrRecortado = mTipoTr;
    this->mProcRecortado.ajustar ( FichTraza::MAXIMA_LONGITUD_IDPROC );
    this->mTipoTrRecortado.ajustar ( FichTraza::MAXIMA_LONGITUD_TIPOTRAZA );
    this->mTexto.formCadena ("%s%c%s%c%s%c%s\n",
       lMomento.getCadena (FechaHora::TRAZA).getCadena (),
       sbSeparadorCampos, mProcRecortado.getCadena(), sbSeparadorCampos,
       mTipoTrRecortado.getCadena(), sbSeparadorCampos, mMsj.getCadena());
    if( this->mTexto.getLong () > this->uwLongMaxMensaje )
    {
       this->mTexto.recortar (this->uwLongMaxMensaje - 1);
       this->mTexto += '\n';
    }

    if( false == bOutObligada )
    {
      escribir (this->mTexto);
      fflush (this->mFichero);
      this->mudwTamFichero += this->mTexto.getLong();
    }
    else
    {
      // Como la salida a fichero no funciona lo imprimimos a pantalla
      std::cout << "FichTrz sin definir: " << mTexto.getCadena();
    }
  }


  void FichTraza::organizaFicheros()
  {
    UDWordType  udwIndice;
    UDWordType  udwNumFicheros;
    Cadena      mFichero;
    Cadena      mFicheroDestino;

    try
    {
      // Calculamos la longitud del fichero y lo cerramos.
      setNombre( this->mNombreCompleto);
      this->mudwTamFichero = getTamano();
      if( this->mudwTamFichero >= this->udwLongMaxFichero )
      {
        // Borramos el ultimo ficheros y si hay mas por configuraciones anteriores
        udwNumFicheros = dameNumeroFicheros( this->mNombreFichero);
        for (udwIndice = (this->uwNumMaxFicheros - 1);udwIndice <= udwNumFicheros;udwIndice++)
        {
          mFichero = this->mNombreFichero + udwIndice + this->mExtension;
          //Directorio::eliminarSeguro ( mFichero, Directorio::FICHERO );
          Directorio::eliminar ( mFichero, Directorio::FICHERO );
        }
        // renombramos el resto de ficheros numerados
        for(udwIndice = 0; udwIndice < udwNumFicheros; udwIndice++)
        {
           mFichero =  this->mNombreFichero + (udwNumFicheros - udwIndice) + this->mExtension;
           mFicheroDestino =  this->mNombreFichero + (udwNumFicheros - udwIndice + 1) + this->mExtension;
           try
           {
             Directorio::mover ( mFichero, mFicheroDestino );
           }
           catch(Excepcion& e)
           {
              // El fichero no existe o no se puede renombrar no importa
              // pasamos al siguiente.
           }
        }
         // Renombramos el fichero .log principal
         Cadena cadNombreDestino = this->mNombreFichero + 1 + this->mExtension;
         Directorio::mover ( this->mNombreCompleto, cadNombreDestino );
      }
    }
    catch(Excepcion& e)
    {
      // Da igual los errores durante la organizacion de ficheros
    }
  }

  UWordType FichTraza::dameNumeroFicheros(Cadena& mNombre)
  {
     bool      bExiste;
     UWordType uwIndice;
     Cadena    mTexto;

     uwIndice = 1;
     bExiste = true;
     while (bExiste)
     {
        // Formamos el nombre del fichero en Nombre1 ... Nombrenn
        mTexto = mNombre + uwIndice + this->mExtension;
        bExiste = Directorio::existe (mTexto, Directorio::FICHERO);
        if (!bExiste)
           return (uwIndice - 1);
        uwIndice ++;
     }

     return 0;
}

// =================   OPERADORES    ===================
// =================   ACCESO       ====================

  void FichTraza::setLongMaxMensaje(UWordType uwLongMaxMensaje)
  {
    if( 0 == uwLongMaxMensaje )
      throw ParametroExcepcion(__AQUI__,"ERROR: La asignacion de la longitud maxima de la traza es 0");
    this->uwLongMaxMensaje = uwLongMaxMensaje;
  }

  void FichTraza::setNumMaxFicheros(UWordType uwNumMaxFicheros)
  {
    if( 0 == uwNumMaxFicheros )
      throw ParametroExcepcion(__AQUI__,"ERROR: La asignacion del numero maximo de ficheros es 0");
    this->uwNumMaxFicheros = uwNumMaxFicheros;
  }

  void FichTraza::setLongMaxFichero(UDWordType udwLongMaxFichero)
  {
    if( 0 == udwLongMaxFichero )
      throw ParametroExcepcion(__AQUI__,"ERROR: La asignacion de la longitud maxima del fichero de traza es 0");
    this->udwLongMaxFichero = udwLongMaxFichero;
  }

  void FichTraza::setSeparadorCampos(SByteType sbSeparadorCampos)
  {
    this->sbSeparadorCampos = sbSeparadorCampos;
  }

  void FichTraza::setNombreFichero(const Cadena& lFichero)
  {
      SWordType   swPosicion;
      Cadena      lNombreMasExt;
      Cadena      lDirectorio;
      Cadena      lNombreFichero;
      Cadena      lNombreCompleto;

      lNombreCompleto = lFichero;
      lNombreMasExt = basename(const_cast<char*>(lFichero.getCadena()));
      lNombreCompleto.subCadena(1,lNombreCompleto.getLong() - lNombreMasExt.getLong(),lDirectorio);
      // Buscamos la extension
      swPosicion = lNombreMasExt.ultimoIndiceDe('.');
      if( -1 != swPosicion)
      {
         // Le quitamos la extension
         lNombreMasExt.subCadena(1,swPosicion - 1,lNombreFichero);
         lNombreMasExt.subCadena(swPosicion,this->mExtension);
         this->mNombreFichero = lDirectorio  + lNombreFichero;
      }
      else
      {
         // El fichero no tiene extension
         this->mNombreFichero = lFichero;
         this->mExtension = "";
      }
      this->mNombreCompleto = lFichero;
  }

// =================   CONSULTA      ===================

  Cadena& FichTraza::getNombreFichero()
  {
    return ( this->mNombreCompleto);
  }


  SByteType FichTraza::getSeparadorCampos()
  {
    return ( this->sbSeparadorCampos );
  }


  UDWordType FichTraza::getLongMaxFichero()
  {
    return ( this->udwLongMaxFichero );
  }


  UWordType FichTraza::getNumMaxFicheros()
  {
    return ( this->uwNumMaxFicheros );
  }


  UWordType FichTraza::getLongMaxMensaje()
  {
    return ( this->uwLongMaxMensaje );
  }

  ////////////////////   PROTECTED    /////////////////////
  ////////////////////   PRIVATE      ////////////////////

// =======================
//  FIN DEL FICHERO - FichTraza.cpp
// =======================
