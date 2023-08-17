// =======================
//  FICHERO - SocketCharStuffing.cpp
// =======================

#include "SocketCharStuffing.h"  // clase implementada

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
SocketCharStuffing::SocketCharStuffing()
{
    this->mEstado = ESPERA_SINC_INICIO;
    memset(mBufferRx,0,sizeof(mBufferRx));
    memset(mBufferTx,0,sizeof(mBufferTx));
    this->muwIndiceRx = 0;
    this->muwIndiceTx = 0;
}

SocketCharStuffing::~SocketCharStuffing()
{

}
// =================   OPERACIONES   ===================

SDWordType SocketCharStuffing::recibir(PSByteType lpsbDatos, const SDWordType& lsdLongitud )
{
   SByteType lsbDato;
   UWordType luwLongRecibida;

   limpiarDatosRx();
   // Esperamos la recepcion de un mensaje
   while(MENSAJE_RECIBIDO != mEstado)
   {
     luwLongRecibida = SSLSocket::recibir(&lsbDato,1);
     if( 1 == luwLongRecibida )
     {
        tratarByteRx (lsbDato);
     }
     if( 0 == luwLongRecibida )
     {
        // Conexion perdida retornamos 0 bytes leidos
        return(0);
     }
   };

   if(mEstado == MENSAJE_RECIBIDO)
   {
      if(muwIndiceRx < lsdLongitud)
      {
        memcpy(lpsbDatos,mBufferRx,muwIndiceRx);
        return(muwIndiceRx);
      }
      else
      {
         throw MemoriaExcepcion (__AQUI__,"El buffer de destino es mas pequeno que"\
          " el mensaje recibido");
      }
   }
   return(0);
}

SDWordType SocketCharStuffing::recibir( Cadena& ldatos )
{
   return(SocketCharStuffing::recibir((PSByteType)ldatos.getCadena(),Cadena::LON_MAX_STRING));
}

SDWordType SocketCharStuffing::enviar( CPSByteType lpsbDatos, const SDWordType& lsdLongitud )
{
   UWordType luwIndice;

   limpiarDatosTx();
   guardarDatoTx(DLE);
   guardarDatoTx(STX);
   for(luwIndice = 0;luwIndice < lsdLongitud;luwIndice++)
   {
      guardarDatoTx(lpsbDatos[luwIndice]);
      if( DLE == lpsbDatos[luwIndice] )
      {
        // Duplicamos todos los caracteres DLE
        guardarDatoTx(DLE);
      }
   }
   guardarDatoTx(DLE);
   guardarDatoTx(ETX);
   return(SSLSocket::enviar((PSByteType)mBufferTx,muwIndiceTx));
}

SDWordType SocketCharStuffing::enviar( const Cadena& ldatos )
{
 return(SocketCharStuffing::enviar((PSByteType)ldatos.getCadena(),ldatos.getLong()));
}

////////////////////   PRIVATE       ////////////////////
void SocketCharStuffing::tratarByteRx(UByteType lubDato)
{
   switch(mEstado)
   {
     case ESPERA_SINC_INICIO:
         if(DLE == lubDato)
         {
            mEstado = ESPERA_INICIO_MENSAJE;
         }
         break;
     case ESPERA_INICIO_MENSAJE:
         if(STX == lubDato)
         {
             mEstado = ESPERA_MENSAJE;
         }
         else if(DLE == lubDato)
         {
            break;
         }
         else
         {
            limpiarDatosRx();
         }
         break;
     case ESPERA_MENSAJE:
         if(DLE == lubDato)
         {
            // Primer DLE lo guardamos en buffer
            guardarDatoRx(lubDato);
            mEstado = ESPERA_SINC_FIN;
         }
         else
         {
            // Guardamos el dato
            guardarDatoRx(lubDato);
         }
         break;
     case ESPERA_SINC_FIN:
         if(DLE == lubDato)
         {
            // Segundo DLE no lo guardamos en el buffer
            mEstado = ESPERA_MENSAJE;
         }
         else if(ETX == lubDato)
         {
            // Final del mensaje
            // Extraemos el utimo DLE del mensaje
            popRx();
            mEstado = MENSAJE_RECIBIDO;
         }
         else if(STX == lubDato)
         {
            // Inicio de un mensaje sin acabar el anterior
            limpiarDatosRx();
            mEstado = ESPERA_MENSAJE;
         }
         else
         {
            // El mensaje esta mal construido detras de un DLE en datos
            // o hay otro DLE o un ETX.
            limpiarDatosRx();
         }
         break;
     case MENSAJE_RECIBIDO:
         throw ComunicacionExcepcion(__AQUI__,"Recibidos nuevos datos sin tratar"\
          " mensaje anterior");
         break;
   }
}

void SocketCharStuffing::guardarDatoTx(UByteType lubDato)
{
   if(muwIndiceTx < MAXIMOS_DATOS)
   {
      mBufferTx[muwIndiceTx] = lubDato;
      muwIndiceTx++;
   }
   else
   {
      throw MemoriaExcepcion(__AQUI__,"El mensaje transmitido es mas grande"\
       " de lo permitido");
   }
}

void SocketCharStuffing::guardarDatoRx(UByteType lubDato)
{
   if(muwIndiceRx < MAXIMOS_DATOS)
   {
      mBufferRx[muwIndiceRx] = lubDato;
      muwIndiceRx++;
   }
   else
   {
      // No hay memoria para recibir un mensaje tan grande volvemos
      // a resetear la maquina de estados
      limpiarDatosRx();
   }
}

UByteType SocketCharStuffing::popRx()
{
   if(muwIndiceRx > 0)
   {
      muwIndiceRx--;
   }
   return(mBufferRx[muwIndiceRx]);
}

void SocketCharStuffing::limpiarDatosRx()
{
   muwIndiceRx  = 0;
   memset(mBufferRx,0,sizeof(mBufferRx));
   mEstado = ESPERA_SINC_INICIO;
}

void SocketCharStuffing::limpiarDatosTx()
{
   memset(mBufferTx,0,sizeof(mBufferTx));
   muwIndiceTx  = 0;
}


// =================   ACCESO       ====================
// =================   CONSULTA      ===================

UWordType SocketCharStuffing::getTamMenRx() const
{
   return(muwIndiceRx);
}

UWordType SocketCharStuffing::getTamMenTx() const
{
   return(muwIndiceTx);
}

// =======================
//  FIN DEL FICHERO - SocketCharStuffing.cpp
// =======================



