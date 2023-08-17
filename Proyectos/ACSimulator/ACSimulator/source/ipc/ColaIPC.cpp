// =======================
//  FICHERO - ColaIPC.cpp
// =======================

#include "ColaIPC.h"

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////
// =================   OPERACIONES   ===================
void ColaIPC::crear(const SDWordType& sdKey, const SDWordType& sdFlags)
{
   SDWordType lsdId;

   lsdId = msgget( sdKey, sdFlags );
   if ( lsdId == FAIL) // la creacion del recurso ha fallado
   {
      switch ( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "A message queue exists for"\
            " key, but the calling process has no access permissions to the"\
            " queue.",errno );
         case EEXIST:
            throw OcupadoExcepcion( __AQUI__, "A message queue exists for key"\
            " and msgflg was asserting both IPC_CREAT and IPC_EXCL.", errno );
         case ENOENT:
            throw NoExisteExcepcion( __AQUI__, "No message queue exists for key"\
            " and msgflg wasn't asserting IPC_CREAT.", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "A message queue has to be"\
            " created but the system has not enough memory for the new data"\
            " structure.", errno );
         case ENOSPC:
            throw LlenoExcepcion( __AQUI__, "A  message  queue  has  to  be"\
            " created but the system limit for the maximum number of message"\
            " queues (MSGMNI) would be exceeded.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror(errno), errno );
      }
   }
   else // el recurso se ha creado correctamente
   {
      setKey( sdKey );
      setId( lsdId );
      setFlg( sdFlags );
   }
}

//------------------------------------------------------------------------------
void ColaIPC::escribir(const void *pMsj, const SDWordType& sdLongMsj,
   const SDWordType& sdFlags) const
{
   if ( FAIL == msgsnd(getId(), pMsj, sdLongMsj, sdFlags) )
   {
      switch ( errno )
      {
         case EAGAIN: // operacion invocada con el parametro NO_BLOQUEAR
           throw LlenoExcepcion(__AQUI__,"The message can't be sent due to the "\
              " msg_qbytes limit for the  queue  and  IPC_NOWAIT was asserted in mgsflg.");
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The calling process has no"\
            " write permission on the message queue.", errno );
         case EFAULT:
            throw ParametroExcepcion( __AQUI__, "The address pointed to by"\
            " msgp isn't accessible.", errno );
         case EIDRM:
            throw NoExisteExcepcion( __AQUI__, "The message queue was removed.",
            errno );
         case EINTR:
            throw InterrupcionExcepcion( __AQUI__, "Sleeping on a full message"\
            " queue condition, the process caught a signal.", errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "Invalid msqid value, or"\
            " nonpositive mtype value, or invalid msgsz value (less than 0 or"\
            " greater  than the system value MSGMAX).", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "The system has not enough"\
               " memory to make a copy of the supplied msgbuf.", errno );
         default:
           throw DesconocidoExcepcion( __AQUI__, strerror(errno), errno);
      }
   }
};

//------------------------------------------------------------------------------
SDWordType ColaIPC::leer(void *pMsj, const SDWordType& sdLongMsj,
   const SDWordType& sdTipo, const SDWordType& sdFlags) const
{
   SDWordType sdBytesRecibidos;

   sdBytesRecibidos = msgrcv(getId(), pMsj, sdLongMsj, sdTipo, sdFlags);
   if ( FAIL == sdBytesRecibidos )
   {
      switch ( errno )
      {
         case ENOMSG:
            return 0; // no hay mensajes en la cola
         case E2BIG:
            throw RangoExcepcion( __AQUI__, "The message text length is greater"\
            " than msgsz and MSG_NOERROR isn't asserted in msgflg.", errno );
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The calling process does not"\
            " have read permission on the message queue.", errno );
         case EFAULT:
            throw ParametroExcepcion( __AQUI__, "The address pointed to by msgp"\
            " isn't accessible.", errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "Illegal msgqid value, or msgsz"\
            " less than 0.", errno );
         case EIDRM:
            throw NoExisteExcepcion( __AQUI__, "While the process was sleeping"\
            " to receive a message, the message queue was removed.", errno );
         case EINTR:
            throw InterrupcionExcepcion( __AQUI__, "While the process was"\
            " sleeping to receive a message, the process received a signal"\
            " that had to be caught.", errno );
         case 514:
            throw InterrupcionExcepcion( __AQUI__, "While the process was"\
            " sleeping to receive a message something happend.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror(errno), errno );
      }
   }
   else
      return sdBytesRecibidos; // se ha leido un mensaje correctamente
};

//------------------------------------------------------------------------------
void ColaIPC::destruir()
{
   if ( FAIL == msgctl(getId(), IPC_RMID, 0) )
   {
      switch( errno )
      {
         case EPERM:
            throw PermisoExcepcion( __AQUI__, "The calling process has"\
            " insufficient privileges to execute the command.", errno );
         case EIDRM:
            break;
         case EINVAL:
            throw NoExisteExcepcion( __AQUI__, "Invalid value for msqid.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror(errno), errno );
      }
   }
};

//------------------------------------------------------------------------------
void ColaIPC::borrar() const
{
   struct msgbuf lBuffer;
   UDWordType    ludLonBuffer;

#ifdef __sun
  ludLonBuffer = sizeof( lBuffer._mtext );
#else
   ludLonBuffer = sizeof( lBuffer.mtext );
#endif
   /*
   Leemos la cola hasta que no que quede ningun mensaje
   IPC_NOWAIT evita el bloqueo cuando ya se ha leido el ultimo mensaje
   MSG_NOERROR permite consumir un mensaje aunque no quepa en el buffer sin que
   se produzca un error E2BIG
   */
   while ( 0 != leer( &lBuffer, ludLonBuffer, 0, IPC_NOWAIT | MSG_NOERROR ) );
}
