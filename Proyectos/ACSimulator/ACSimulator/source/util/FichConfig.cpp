// =======================
//  FICHERO - FichConfig.cpp
// =======================

#include "FichConfig.h"  // clase implementada

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


   FichConfig::FichConfig()
   {
     this->sbSeparador     =   DEF_SEPARADOR_VALOR;
     this->sbReservado     =   DEF_CARACTER_RESERVADO;
     this->udwUltimaPosicion = 0;
     this->udwUltimaLinea = 0;
   }

   FichConfig::~FichConfig()
   {

   }

// =================   OPERADORES    ===================

   void FichConfig::setFicheroCfg( const Cadena & nombreFichero )
   {
      mNombreFichero = nombreFichero;
      setUltLinea(0);
      setUltPos(0);
   }

   void  FichConfig::setCaracterSeparador(SByteType sbSeparador)
   {
     this->sbSeparador = sbSeparador;
   }

   void  FichConfig::setCaracterReservado(SByteType sbReservado)
   {
     this->sbReservado = sbReservado;
   }


// =================   ACCESO       ====================
// =================   CONSULTA      ===================

   SByteType  FichConfig::getCaracterSeparador()
   {
     return ( this->sbSeparador );
   }

   SByteType  FichConfig::getCaracterReservado()
   {
     return ( this->sbReservado );
   }


   bool FichConfig::getId( const UWordType numLinea, Cadena & ident )
   {
    //  UWordType  udIndice;
      bool       bResultado;

      try
      {
         if ( numLinea > 0 )
         {
            bResultado = false;
            // Abrimos el fichero
            setNombre(mNombreFichero);
            setEscritura ( false );
            setLectura ( true );
            abrir();
            // Nos colocamos al inicio del fichero
            setPosicion(0);
            // Leemos el numero de lineas considerando que cada linea corresponde
            // a un identificador
            bResultado = true;
            IrLinea(numLinea - 1);
               // Si el resultado es correcto
            if ( false == esFinalFichero() )
            {
              // Leemos el identificador de la linea deseada
              if ( dameIdentificador(  ident ) == false )
              {
                 // Elemento no encontrado
                 bResultado = false;
              }
              else
              {
                 bResultado = true;
              }
              cerrar();
            }
            else
            {
              bResultado = false;
              cerrar();
              // No se puede leer el id deseado
              throw ParametroExcepcion(__AQUI__, "Parametros incorrectos No existe la linea.");
            }
         }
         else
         {
            throw ParametroExcepcion(__AQUI__, "Parametros incorrectos no se puede leer "\
             " la linea Nº0 (Empieza por la 1).");
         }
      }
      catch(NoExisteExcepcion& e)
      {
          // No existe el fichero
          bResultado = false;
      }
      return ( bResultado );
   }

   bool FichConfig::getParam( const UWordType numLinea, const SDWordType iCampo, Cadena & Param ,const Cadena& mDefecto)
   {
      bool       bResultado;
      Cadena     ident;

      try
      {
         if ( numLinea > 0 )
         {
            bResultado = false;
            // Abrimos el fichero
            setNombre(mNombreFichero);
            setEscritura ( false );
            setLectura ( true );
            abrir();
            // Nos colocamos al inicio del fichero
            setPosicion( 0 );
            // Leemos el numero de lineas considerando que cada linea corresponde
            // a un identificador
            bResultado = true;
            IrLinea(numLinea - 1);
               // Si el resultado es correcto
            if ( false == esFinalFichero() )
            {
              // Primero leemos el identificador
              if ( dameIdentificador(  ident ) == true )
              {
                // Leemos el identificador de la linea deseada
                if ( dameParametro( iCampo, Param) == false )
                {
                   // Elemento no encontrado
                   bResultado = false;
                   Param = mDefecto;
                }
                else
                {
                   bResultado = true;
                }
                cerrar();
              }
              else
              {
                bResultado = false;
                Param = mDefecto;
                cerrar();
              }
            }
            else
            {
              bResultado = false;
              cerrar ();
              // No se puede leer el id deseado
              throw ParametroExcepcion(__AQUI__, "Parametros incorrectos No existe la linea.");
            }
         }
         else
         {
           throw ParametroExcepcion(__AQUI__, "Parametros incorrectos no se puede leer "\
            " la linea Nº0 (Empieza por la 1).");
         }
       }
       catch(NoExisteExcepcion& e)
       {
          // No existe el fichero
          bResultado = false;
          Param = mDefecto;
       }
       return ( bResultado );
   }

   bool FichConfig::getParam( const Cadena & IdParam, const SDWordType iCampo, Cadena & Param ,const Cadena & mDefecto)
   {
      // Abrimos el fichero
      try
      {
         setNombre(mNombreFichero);
         setEscritura ( false );
         setLectura ( true );
         abrir();
         // Nos colocamos al inicio del fichero
         setPosicion( 0 );
         // Leemos el identificador de la linea deseada
         if ( dameParametro( IdParam, iCampo, Param ) == false )
          {
             // No se encuentra el elemento asignamos el elemento por defecto
             Param = mDefecto;
             cerrar();
             return ( false );
          }
          else
          {
            // Elemento encontrado
            cerrar();
            return ( true );
          }
       }
       catch(NoExisteExcepcion& e)
       {
          // No existe el fichero
          Param = mDefecto;
          return(false);
       }
   }

   bool FichConfig::setParam(const Cadena& mIdParam,const SDWordType sdwCampo,const Cadena& mValor)
   {
      Cadena lValorLinea;
      UWordType luwPosActual;
      UWordType luwLineaActual;

      // Abrimos el fichero
      try
      {
         setNombre(mNombreFichero);
         setEscritura ( true );
         setLectura ( true );
         if(false == Directorio::existe (mNombreFichero, Directorio::FICHERO ) )
          crear();
         else
          abrir();
         // Nos colocamos al inicio del fichero
         luwPosActual = 0;
         setPosicion( luwPosActual );
         while(true == leerLinea(lValorLinea,false) )
         {
           if(true == lineaCorrecta(lValorLinea) )
           {
              if(getIdLinea(lValorLinea) == mIdParam)
              {
                escribirParametro(lValorLinea,sdwCampo,mValor);
                setPosicion(luwPosActual);
                borrarLinea(true);
                setPosicion(luwPosActual);
                setPosicion(luwPosActual + insertarLinea(lValorLinea,true) );
                setUltLinea(0);
                setUltPos(0);
                cerrar();
                return(true);
              }
           }
           luwPosActual = getPosicion();
           luwLineaActual++;
         }
         // No se encuentra el parametro escribimos el nuevo
         irFin();
         lValorLinea = mIdParam + getCaracterSeparador();
         escribirParametro(lValorLinea,sdwCampo,mValor);
         escribir(lValorLinea + "\n");
         cerrar();
         return(false);
       }
       catch(NoExisteExcepcion& e)
       {
          // No existe el fichero
          return(false);
       }
   }

   CUDWordType FichConfig::getNumTotalLineas()
   {
     UWordType uwNumLineas;

      uwNumLineas = 0;
      // Abrimos el fichero
      setNombre(mNombreFichero);
      setEscritura ( false );
      setLectura ( true );
      abrir();
      // Nos colocamos al inicio del fichero
     setPosicion(0);

      while(false == esFinalFichero() )
      {
        saltaLinea();
        uwNumLineas ++;
      }

      cerrar();
      return(uwNumLineas);
   }

////////////////////   PROTECTED    /////////////////////

////////////////////   PRIVATE      ////////////////////



   bool FichConfig::caracterValidoId( const SByteType sbCaracter )
   {
      // Mientras todos los caracteres que sean letras mayusculas, y diferentes de ;
      // y #
      if ( ( (  sbCaracter  != sbSeparador ) && isprint(sbCaracter) &&
             (  sbCaracter  != '#' )) ||
           ( ( isalpha( sbCaracter ) != 0 ) && ( isupper( sbCaracter ) != 0 ) ) )
             return ( true );
      else
         return ( false );
   }

   bool FichConfig::caracterValidoParam( const SByteType sbCaracter )
   {
      // Mientras no sea un ';'
      if (((( sbCaracter != sbSeparador ) && isprint(sbCaracter)&& (  sbCaracter  != '#' )) ||
            ( sbCaracter == sbReservado ) )|| (sbCaracter == '\t') )
         return ( true );
      else
         return ( false );
   }

   bool FichConfig::dameIdentificador(  Cadena& str )
   {
      Cadena mIdentificador;
      SByteType sbCaracter;

      // Leemos el primer elemento
      sbCaracter = getCaracter();
      // Mientras no se acabe el fichero
      while ( ( false == esFinalFichero() ) && ( caracterValidoId( sbCaracter ) == true ) )
      {
         // Guardamos el caracter leido dentro del objeto cadena
         if ( sbCaracter != sbReservado )
            mIdentificador += sbCaracter;
         else
            getCaracter(); // Ignoramos el siguiente caracter
         sbCaracter = getCaracter();
      }
      // Aqui impedimos que la lectura de un id signifique el cambio de linea en
      // un fichero. Dejando que los finales de fichero se traten solo por el
      // salta linea
      if( sbCaracter == '\n' )
         mover(-1);

      // Miramos si el identificador leido es correcto
      if ( strlen( mIdentificador.getCadena() ) > 0 )
      {
         // La longitud del identificador es superior a 1 se considera un
         // identificador correcto
         str = mIdentificador;
         return ( true );
      }
      return ( false );
   }

   bool FichConfig::dameParametro(  const Cadena & id, const UDWordType iCampo, Cadena & str )
   {
      UDWordType udIndice;
      SByteType sbCaracter;
      Cadena mParametro;
      Cadena mIdentificador;

      if ( iCampo > 0 )
      {
         // Buscamos el parametro identificador dentro del fichero
         dameIdentificador( mIdentificador );
         while ( ( esFinalFichero() == false ) && ( mIdentificador != id ) )
         {
            // Saltamos una linea
            saltaLinea();
            // Leeemos el siguiente elemento
            dameIdentificador( mIdentificador );
         }
         if (  mIdentificador == id  )
         {
            // Mientras no se acabe el fichero o encontremos el parametro
            for ( udIndice = 0; udIndice < iCampo; udIndice++ )
            {
               // Leemos el primer elemento
               sbCaracter = getCaracter();
               while ( ( false == esFinalFichero() ) && ( caracterValidoParam( sbCaracter ) == true ) )
               {
                  // Guardamos el caracter leido dentro del objeto cadena
                  // solo el parametro que queremos
                  if ( ( udIndice == ( iCampo - 1 ) ) )
                     if ( sbCaracter != sbReservado )
                     {
                      if (sbCaracter != '\t' )
                        mParametro += sbCaracter;
                      }
                     else
                        getCaracter(); // Ignoramos el siguiente caracter
                  sbCaracter = getCaracter();
               }
            }
            // Miramos si el identificador leido es correcto
            if ( strlen( mParametro.getCadena() ) > 0 )
            {
               // La longitud del identificador es superior a 1 se considera un
               // identificador correcto
               str = mParametro.trim();
               return ( true );
            }
         }
      }
      return ( false );
   }

   bool FichConfig::dameParametro( const UDWordType iCampo, Cadena & str )
   {
      UDWordType udIndice;
      SByteType sbCaracter;
      Cadena mParametro;
      Cadena mIdentificador;

      if ( iCampo > 0 )
      {
        // Mientras no se acabe el fichero o encontremos el parametro
        for ( udIndice = 0; udIndice < iCampo; udIndice++ )
        {
           // Leemos el primer elemento
           sbCaracter = getCaracter();
           while ( ( false == esFinalFichero() ) && ( caracterValidoParam( sbCaracter ) == true ) )
           {
              // Guardamos el caracter leido dentro del objeto cadena
              // solo el parametro que queremos
              if ( ( udIndice == ( iCampo - 1 ) ) )
                 if ( sbCaracter != sbReservado )
                 {
                  if (sbCaracter != '\t' )
                    mParametro += sbCaracter;
                  }
                 else
                    getCaracter(); // Ignoramos el siguiente caracter
              sbCaracter = getCaracter();
           }
        }
        // Miramos si el parametro leido es correcto
        if ( strlen( mParametro.getCadena() ) > 0 )
        {
           // La longitud del identificador es superior a 1 se considera un
           // identificador correcto
           str = mParametro.trim();
           return ( true );
        }
      }
      return ( false );
   }

   UDWordType FichConfig::getUltPos()
   {
      return(this->udwUltimaPosicion);
   }

   void FichConfig::setUltPos(UDWordType udwUltiPos)
   {
      this->udwUltimaPosicion = udwUltiPos;
   }

   void FichConfig::setUltLinea(UDWordType udwUltLinea)
   {
      this->udwUltimaLinea = udwUltLinea;
   }

   UDWordType FichConfig::getUltLinea()
   {
      return(this->udwUltimaLinea);
   }

   void FichConfig::IrLinea(UDWordType udwNumLinea)
   {
      UDWordType udwFaltan;
      UDWordType udwIndice;

      if(getUltLinea()  <= udwNumLinea  )
      {
         setPosicion(getUltPos());
         udwFaltan =  udwNumLinea - getUltLinea();
         for(udwIndice = 0;udwIndice < udwFaltan;udwIndice++)
            saltaLinea();
      }
      else
      {
         for(udwIndice = 0;udwIndice < udwNumLinea;udwIndice++)
            saltaLinea();
      }
      setUltLinea(udwNumLinea);
      setUltPos(getPosicion ());
   }

bool FichConfig::lineaCorrecta(const Cadena& lValorLinea)
{
   UWordType luwIndice;
   UWordType luwLongTotal;

   luwLongTotal = lValorLinea.getLong();
   luwIndice = 0;
   while( (luwIndice < luwLongTotal) &&
        ( (true == caracterValidoId (lValorLinea.getCaracter(luwIndice) ) ) ) )
   {
      luwIndice++;
   }
   if( (luwIndice > 0 ) && (lValorLinea.getCaracter(luwIndice) == getCaracterSeparador() ) )
   {
      return(true);
   }
   return(false);
}

void FichConfig::escribirParametro(Cadena& lValorLinea,SWordType sdwCampo,const Cadena &mValor)
{
   SWordType lswPosActual;
   SWordType lswPosFinal;
   UWordType luwNumParam;
   Cadena    lTemp;
   Cadena    lSobrante;

   if( sdwCampo > 0 )
   {
   lswPosActual = 0;
   luwNumParam = 0;
   // Buscamos la posicion del parametro sdwCampo
   lswPosActual = lValorLinea.indiceDe(getCaracterSeparador(),lswPosActual);
   while( (-1 != lswPosActual) && (luwNumParam < (sdwCampo - 1) ) && ((UWordType)lswPosActual < lValorLinea.getLong()) )
   {
      luwNumParam ++;
      lswPosActual = lValorLinea.indiceDe(getCaracterSeparador(),lswPosActual);
   }
   lTemp = lValorLinea;
   // Miramos el resultado de la busqueda
   if( (-1 != lswPosActual) && (luwNumParam == (sdwCampo - 1) ) && ((UWordType)lswPosActual < lValorLinea.getLong()))
   {
      // El campo se encuentra borramos el parametro entre
      // DEF_SEPARADOR_VALOR   nnnnnnnnnn   DES_SEPARADOR_VALOR
      // Buscamos el inicio del valor del parametro para conservar comentarios
      while( true == esSeparadorParametro(lValorLinea.getCaracter(lswPosActual) ) )
      {
         lswPosActual++;
      }
      lswPosFinal = lTemp.indiceDe(getCaracterSeparador(),lswPosActual);
      if ( -1 != lswPosFinal )
       lSobrante = lTemp.subCadena ( lswPosFinal  );
      lTemp.recortar(lswPosActual);
      lTemp += mValor + lSobrante;
   }
   else
   {
      // El campo no se encuentra lo insertamos
      // Si no se encuentra el parametro insertamos los que faltan
      while(luwNumParam < (sdwCampo - 1) )
      {
         lTemp.insertaCaracter (getCaracterSeparador());
         luwNumParam++;
      }
      lTemp += mValor + getCaracterSeparador();
   }
   lValorLinea = lTemp;
 }
}

Cadena FichConfig::getIdLinea(const Cadena& lValorLinea)
{
   Cadena lTemp;
   SWordType lswPosActual;

   lTemp = lValorLinea;

   lswPosActual = lValorLinea.indiceDe(getCaracterSeparador(),0);
   if( -1 != lswPosActual)
   {
      lTemp.recortar(lswPosActual - 1);
   }
   return(lTemp);
}

bool FichConfig::esSeparadorParametro(const SByteType sbCaracter)
{
   if( (' ' == sbCaracter ) || ('\t' == sbCaracter ) )
    return(true);
   else
    return(false);
}

// =======================
//  FIN DEL FICHERO - FichConfig.cpp
// =======================
