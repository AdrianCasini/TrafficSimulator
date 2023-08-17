// =======================
//  FICHERO - ProcesoHijo.CPP
// =======================

  #include "ProcesoHijo.h"

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

  ProcesoHijo::ProcesoHijo()
  {
     this->uwGrupoProcesos  =   0;
     this->sdwPID = -1;
     this->uwEstadoProc = EST_PARADO;
     this->uwOrdenParada = 0;
     this->sbTipoProceso = 0;
     this->sdwNumArranques = 0;
     this->udwTiempo = 0;
     this->mTipoProcTypeE = PROC_SIEMPRE;
     this->mbInterrogado = false;
     mbBackground = false;
     mbProcVia = false;
     mbFinaliza = false;
     mbPermitirRearranque = false;
     mpEjecutorCtrlProceso = NULL;
     msdwIdProc = -1;
  }

  ProcesoHijo::~ProcesoHijo()
  {

  }

  void ProcesoHijo::lanzarEjec()
  {
    SDWordType    sdwResultado;
    Cadena        mArg[ProcesoHijo::MAXIMOS_ARGUMENTOS];
    UWordType     uwIndice;
    CPSByteType   psbParam[ProcesoHijo::MAXIMOS_ARGUMENTOS];
    Cadena        lNomProc;

    sdwResultado = fork();
    if( -1 != sdwResultado)
    {
      if( 0 != sdwResultado)
      {
        // Ejecucion del padre
        setPID( sdwResultado );
        setRecepcionACK ( 0 );
        setHoraInicio();
      }
      if( 0 == sdwResultado)
      {
        // Ejecucion del hijo.
        if (true == getProcVia() )
         ignorarSenales();
        setPID( getpid() );
         // Separamos los argumentos
         for(uwIndice = 0;uwIndice <ProcesoHijo::MAXIMOS_ARGUMENTOS;uwIndice++)
         {
           mParametros.dameParametro(uwIndice + 1,mArg[uwIndice]);
           if(mArg[uwIndice].getLong() == 0)
            psbParam[uwIndice] = NULL;
           else
            psbParam[uwIndice] = mArg[uwIndice].getCadena();
         }
          // Lanzamos la aplicacion
          lNomProc = mNombreProceso + ".run";
          generarRun();
          sdwResultado = execl (lNomProc.getCadena(), // Nombre y path del ejecutable
                                 lNomProc.getCadena(), // Nombre y path del ejecutable como argumento
                                 psbParam[0],  // Id del semaforo
                                 psbParam[1],  // Id del semaforo
                                 psbParam[2],  // 3 argumento del programa
                                 psbParam[3],  // 4 argumento del programa
                                 psbParam[4],  // 5 argumento del programa
                                 NULL);
          if( -1 == sdwResultado)
          {
            switch(errno)
            {
              case EACCES:
                throw PermisoExcepcion(__AQUI__,mNombreProceso + " The file or a script interpreter is"\
                 " not  a  regular file. Execute  permission  is  denied  for  the "\
                 " file or a script or ELF interpreter. The file system is mounted noexec.",errno);
              case  EPERM:
                throw PermisoExcepcion(__AQUI__,mNombreProceso + " The file system is mounted nosuid, "\
                  " the user is  not the superuser, and the file has an SUID or SGID "\
                  " bit set. The process is being traced, the user  is  not  the "\
                  " superuser and the file has an SUID or SGID bit set.",errno);
              case  E2BIG:
                throw FormatoExcepcion(__AQUI__,mNombreProceso + " The argument list is too big.",errno);
              case ENOEXEC:
                throw FormatoExcepcion(__AQUI__,\
                       mNombreProceso +  " An executable is not in a recognised format, is for "\
                         " the  wrong  architecture,  or has some other format "\
                         " error that means it cannot be executed.",errno);
              case   EFAULT:
                throw MemoriaExcepcion(__AQUI__,mNombreProceso + " filename points  outside  your "\
                  "  accessible  address space.",errno);
              case  ENAMETOOLONG:
                throw RangoExcepcion(__AQUI__,mNombreProceso + " filename is too long.",errno);
              case  ENOENT:
                throw NoExisteExcepcion(__AQUI__,\
                 mNombreProceso +  " The  file  filename  or a script or ELF interpreter "\
                  " does not exist, or a shared library needed for file "\
                  " or interpreter cannot be found.",errno);
              case  ENOMEM:
                throw MemoriaExcepcion(__AQUI__,mNombreProceso + " Insufficient kernel memory was available.",errno);
              case  ENOTDIR:
                throw NoExisteExcepcion(__AQUI__,\
                 mNombreProceso + " A  component  of  the  path prefix of filename or a "\
                 " script or ELF interpreter is not a directory.",errno);
              case   ELOOP:
                throw NoAplicaExcepcion(__AQUI__,mNombreProceso + " Too many symbolic links were encountered "\
                  " in resolving  filename or the name of a script or ELF interpreter.",errno);
              case ETXTBSY:
                throw OcupadoExcepcion(__AQUI__,\
                   mNombreProceso + " Executable was open for writing by one or more processes.",errno);
              case     EIO:
                throw IOExcepcion(__AQUI__,mNombreProceso + " An I/O error occurred.",errno);
              case  ENFILE:
                throw RangoExcepcion(__AQUI__,mNombreProceso + " The  limit on the total number of files "\
                   "pen on the system has been reached.The process has the maximum "\
                   " number of files open.",errno);
              case  EINVAL:
                throw NoAplicaExcepcion(__AQUI__,\
                   mNombreProceso +  " An ELF executable had more than one PT_INTERP segment  (i.e., "\
                    " tried  to  name  more than one interpreter).",errno);
              case  EISDIR:
                throw NoAplicaExcepcion(__AQUI__,"An ELF interpreter was a directory.",errno);
              case  ELIBBAD:
                throw CorrompidoExcepcion(__AQUI__,mNombreProceso + " An ELF interpreter was not in a recognised format.",errno);
              default:
                throw DesconocidoExcepcion(__AQUI__,mNombreProceso + " Error en execv",errno);
           }
            }
     }
    }
    else
    {
      setPID( sdwResultado );
      // Errores del fork
      switch(errno)
      {
        case EAGAIN:
          throw OcupadoExcepcion(__AQUI__,"fork  cannot allocate sufficient" \
            " memory to copy the parent's page tables and allocate a task  "
            " structure for the child.",errno);
          break;
        case ENOMEM:
          throw MemoriaExcepcion(__AQUI__,"fork failed to allocate the necessary" \
             " kernel structures because memory is tight.",errno);
          break;
        default:
            throw DesconocidoExcepcion(__AQUI__,"Error en execv.",errno);
      }
    }
  }

  void ProcesoHijo::enviarKill(SDWordType sdwSignal)
  {
    MutexLocker lProteccion (mMutex);
    try
    {
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Envio FIN por signal %d"\
       " al proceso %s",sdwSignal,getNombreProceso().getCadena());
#ifdef _DEBUG
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Compilado en debug enviamos -> SIGSEGV");
      enviarSignal (SIGSEGV);
#else
      enviarSignal (sdwSignal);
#endif
    }
    catch(Excepcion& e)
    {
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Error al enviar el SIGKILL");
    }
  }

  void ProcesoHijo::enviarSignal(SDWordType sdwSignal)
  {
    if( getPID() > 0)
    {
      if( -1 == kill (getPID(),sdwSignal))
      {
        switch(errno)
        {
         case EINVAL:
          throw NoAplicaExcepcion(__AQUI__,"An invalid signal was specified.",errno);
         case  ESRCH:
          throw NoExisteExcepcion(__AQUI__,\
           " The pid or process group does not exist.  Note that "\
           " an existing process might be a  zombie,  a  process "\
           " which  already  committed  termination, but has not "\
           "yet been wait()ed for.",errno);
         case EPERM:
          throw PermisoExcepcion(__AQUI__,\
           " The process does not have permission  to  send  the "\
           " signal  to  any  of the receiving processes.  For a "\
           " process to have permission to send a signal to pro- "\
           " cess  pid  it  must either have root privileges, or "\
           " the real or effective user ID of the  sending  pro- "\
           " cess  must  equal  the real or saved set-user-ID of "\
           " the receiving process.  In the case of  SIGCONT  it "\
           " suffices  when  the sending and receiving processes "\
           " belong to the same session.",errno);
          default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en la funcion kill",errno);
        }
      }
    }
    else
    {
      throw ParametroExcepcion(__AQUI__,"El pId del proceso no es correcto.");
    }
  }

  void ProcesoHijo::setHoraMuerte()
  {
    if(((time_t)-1) == time(&horaMuerte))
    {
      switch(errno)
      {
        case EFAULT:
         throw MemoriaExcepcion(__AQUI__,"t points outside your accessible address space.");
         break;
        default:
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
         break;
      }
    }

  }

  void ProcesoHijo::setHoraInicio()
  {
    if(((time_t)-1) == time(&horaArranque))
    {
      switch(errno)
      {
        case EFAULT:
         throw MemoriaExcepcion(__AQUI__,"t points outside your accessible address space.");
         break;
        default:
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en time().",errno);
         break;
      }
    }
    else
       mbPermitirRearranque = true;
  }

  bool ProcesoHijo::esHoraResucitar(UWordType uwOffset)
  {
    time_t  Resultado;
    time_t  HoraActual;

    if(((time_t)-1) == time(&HoraActual))
    {
      switch(errno)
      {
        case EFAULT:
         throw MemoriaExcepcion(__AQUI__,"t points outside your accessible address space.");
         break;
        default:
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en time().",errno);
         break;
      }
    }

    Resultado = HoraActual - horaArranque;
    if( Resultado >= (uwOffset / 1000) )
     return ( true );
    else
     return ( false );
  }

  void ProcesoHijo::ignorarSenales()
  {
    if(signal(SIGHUP,SIG_IGN) == SIG_ERR)
     throw NoAplicaExcepcion(__AQUI__,"Error en el signal del SIGHUP");
    if(signal(SIGUSR1,SIG_IGN) == SIG_ERR)
     throw NoAplicaExcepcion(__AQUI__,"Error en el signal del SIGUSR1");
    if(signal(SIGUSR2,SIG_IGN) == SIG_ERR)
     throw NoAplicaExcepcion(__AQUI__,"Error en el signal del SIGUSR2");
    if(signal(SIGINT,SIG_IGN) == SIG_ERR)
     throw NoAplicaExcepcion(__AQUI__,"Error en el signal del SIGINT");
    if(signal(SIGQUIT,SIG_IGN) == SIG_ERR)
     throw NoAplicaExcepcion(__AQUI__,"Error en el signal del SIGQUIT");

  }

  bool ProcesoHijo::parar()
  {
    MutexLocker lProteccion (mMutex);
    if( (EST_PARADO != getEstadoProc()) && (EST_BLOQUEADO != getEstadoProc()) )
       {
//        Timer::esperar(getTiempoEsperaParada());
        setEstadoProc(ProcesoHijo::EST_PARANDO);
        if (true == getProcVia() )
        {
          // Enviamos la peticion por la cola IPC
          STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Envio FIN por cola IPC "\
           " al proceso %s",getNombreProceso().getCadena());
          try
          {
           getServicio()->pararEjecucion();
          }
          catch(Excepcion& e)
          {
          }
          mbInterrogado = false;
        }
        else
        {
          STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Envio FIN por signal %d "\
           " al proceso %s (orden parar)",SIGINT,getNombreProceso().getCadena());
          enviarSignal(SIGINT);
          setHoraMuerte();
        }
        return (true);
      }
      return(false);
  }

// =================   CONSULTA      ===================

  SByteType ProcesoHijo::getTipoProceso()
  {
    return sbTipoProceso;
  }

  Cadena& ProcesoHijo::getParametros()
  {
    return mParametros;
  }

  Cadena& ProcesoHijo::getNombreProceso()
  {
    return mNombreProceso;
  }

  UWordType ProcesoHijo::getOrdenParada()
  {
    return uwOrdenParada;
  }

  ProcesoHijo::EstadoProcesoTypeE ProcesoHijo::getEstadoProc()
  {
    MutexLocker lProteccion (mMutex);
    return uwEstadoProc;
  }

  UWordType ProcesoHijo::getGrupoProcesos()
  {
    return uwGrupoProcesos;
  }

  SDWordType ProcesoHijo::getPID()
  {
    return this->sdwPID;
  }

  SByteType ProcesoHijo::getRecepcionACK()
  {
    return ( this->sbRecepcionACK );
  }

  SDWordType ProcesoHijo::getNumArranques()
  {
    return ( this->sdwNumArranques );
  }

  UWordType ProcesoHijo::getIdSemaforo()
  {
    return ( this->uwIdSemaforo );
  }

  EjecutorCtrlProceso* ProcesoHijo::getServicio()
  {
   if (mpEjecutorCtrlProceso != NULL)
    return( this->mpEjecutorCtrlProceso );
   else
    throw NoAplicaExcepcion(__AQUI__,"No ha servicio CtrlProceso asignado");
  }

  bool ProcesoHijo::getBackground()
  {
    return(mbBackground);
  }

  // =================   ACCESO       ====================

  void ProcesoHijo::setNombreProceso(const Cadena& mNombreProceso)
  {
    this->mNombreProceso = mNombreProceso;
    this->mNombreProceso.resuelveVariableEntorno();
  }

  void ProcesoHijo::setParametros(const Cadena& mParametros)
  {
    this->mParametros = mParametros;
  }

  void ProcesoHijo::setBackground(bool lEstado)
  {
    mbBackground = lEstado;
  }

  void ProcesoHijo::setTipoProceso(SByteType sbTipoProceso)
  {
    this->sbTipoProceso = sbTipoProceso;
  }

  void ProcesoHijo::setOrdenParada(UWordType uwOrdenParada)
  {
    this->uwOrdenParada = uwOrdenParada;
  }

  void ProcesoHijo::setEstadoProc(EstadoProcesoTypeE uwEstadoProc)
  {
    MutexLocker lProteccion (mMutex);
    this->uwEstadoProc = uwEstadoProc;
  }

  void ProcesoHijo::setGrupoProcesos(UWordType uwGrupoProcesos)
  {
    this->uwGrupoProcesos = uwGrupoProcesos;
  }

  void ProcesoHijo::setRecepcionACK(SByteType sbRecepcionACK)
  {
    if( sbRecepcionACK > 0 )
     this->sbRecepcionACK = sbRecepcionACK;
    else
     this->sbRecepcionACK = 0;
  }

  void ProcesoHijo::setPID(SDWordType sdwPID)
  {
    this->sdwPID = sdwPID;
  }

  void ProcesoHijo::setNumArranques(SDWordType sdwNumArranques)
  {
    this->sdwNumArranques = sdwNumArranques;
  }

  void ProcesoHijo::setIdSemaforo(UWordType uwIdSemaforo)
  {
    this->uwIdSemaforo = uwIdSemaforo;
  }

  void ProcesoHijo::setTimeoutResucitacion(UDWordType udwTiempo)
  {
    this->udwTiempo = udwTiempo;
  }

  bool ProcesoHijo::hayTimeoutResucitacion(UDWordType udwTiempo)
  {
    if( (this->udwTiempo) == 0)
     return (false);  // Timeout anulado
    if( (this->udwTiempo) < udwTiempo)
    {
      this->udwTiempo = 0;
      return ( true );
    }
    else
    {
      setTimeoutResucitacion((this->udwTiempo) - udwTiempo);
      if( (this->udwTiempo) == 0)
       return ( true );
    }
    return ( false );
  }

  void ProcesoHijo::setFinaliza(bool lbFinaliza)
  {
    mbFinaliza = lbFinaliza;
  }

  void ProcesoHijo::setProcVia(bool lbProcVia)
  {
    mbProcVia = lbProcVia;
  }

  void ProcesoHijo::setTiempoEsperaParada(UDWordType udwTiempoEsperaParada)
  {
    this->udwTiempoEsperaParada = udwTiempoEsperaParada;
  }

  UDWordType  ProcesoHijo::getTiempoEsperaParada()
  {
    return (this->udwTiempoEsperaParada);
  }

  void ProcesoHijo::setTipoProc(TiposProcesosTypeE lTipoProcTypeE)
  {
    this->mTipoProcTypeE = lTipoProcTypeE;
  }

  ProcesoHijo::TiposProcesosTypeE ProcesoHijo::getTipoProc()
  {
    return(this->mTipoProcTypeE);
  }

  Cadena ProcesoHijo::getNombreTipoProc()
  {
    Cadena lTexto;

    switch(this->mTipoProcTypeE)
    {
      case PROC_EXPLOTACION: lTexto = "Explotacion"; break;
      case PROC_TEST: lTexto = "Test"; break;
      case PROC_SIEMPRE: lTexto = "Siempre"; break;
    }
    return(lTexto);
  }

  bool  ProcesoHijo::getInterrogado()
  {
     return(mbInterrogado);
  }

  void ProcesoHijo::setInterrogado(bool lbInterrogado)
  {
    mbInterrogado = lbInterrogado;
  }

  void ProcesoHijo::generarRun()
  {
    Cadena lTemp;
//    STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Generamos el fichero %s.run",getNombreProceso().getCadena());
    lTemp.formCadena("cp %s %s.run",getNombreProceso().getCadena(),getNombreProceso().getCadena());
    if (-1 == system(lTemp.getCadena()) )
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Error en la copia del fichero");
  }

  bool ProcesoHijo::getMaster()
  {
   if( (getTipoProceso() == 'm') || (getTipoProceso() == 'M'))
    return( true );
   else
    return( false );
  }

  bool  ProcesoHijo::getProcVia()
  {
    return (mbProcVia);
  }

  bool  ProcesoHijo::getActivo()
  {
    if ( ProcesoHijo::EST_PARADO == getEstadoProc())
     return(false);
    else
     return(true);
  }

  bool  ProcesoHijo::getFinaliza()
  {
    return(mbFinaliza);
  }

  CPSByteType ProcesoHijo::getTexto(ProcesoHijo::TiposProcesosTypeE lTipo)
  {
    switch(lTipo)
    {
      case PROC_EXPLOTACION: return("Explotacion");
      case PROC_TEST: return("Test");
      case PROC_SIEMPRE: return("Siempre");
      default: return("Desconocido");
    }
  }

  CPSByteType ProcesoHijo::getLiteral(ProcesoHijo::EstadoProcesoTypeE lEstado)
  {
    switch(lEstado)
    {
     case EST_ARRANCADO: return("EST_ARRANCADO");
     case EST_ARRANCANDO: return("EST_ARRANCANDO");
     case EST_PARANDO: return("EST_PARANDO");
     case EST_PARADO: return("EST_PARADO");
     case EST_RESUCITANDO: return("EST_RESUCITANDO");
     case EST_BLOQUEADO: return("EST_BLOQUEADO");
     default: return("Desconocido");
    }
  }

  bool ProcesoHijo::getPermitirReArranque()
  {
    return(mbPermitirRearranque);
  }
  void ProcesoHijo::setPermitirRearranque(bool lbAccion)
  {
    mbPermitirRearranque = lbAccion;
  }

  void ProcesoHijo::set(EjecutorCtrlProceso *lpEjecutorCtrlProceso)
  {
   mpEjecutorCtrlProceso = lpEjecutorCtrlProceso;
  }

  void ProcesoHijo::setIdProc (SDWordType lsdwIdProc)
  {
    msdwIdProc = lsdwIdProc;
  }


// ===============================
//  FIN DEL FICHERO - PROCESOHIJO.CPP
// ===============================



