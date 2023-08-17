
#include "util/STrz.h"
#include "SSLSocket.h"

extern bool bSSL_initiated;

#define __TRAZA__ STrz::debug (ConstTraza::T_TRAZA_SOCKETS, "SSLSocket::%s", __FUNCTION__);

SSLSocket::SSLSocket()
{
   ssl = NULL;
   ctx = NULL;
   resetAtributos();
   ssl_buff_size=0;
   ssl_buff_readed=0;
   setProtocolo( Socket::TCP );

   sprintf(PrivateKeyPemFile,"%s/priv.pem",getenv("FF_DAT"));
   sprintf(CertificatePemFile,"%s/cert.pem",getenv("FF_DAT"));
}

SSLSocket::~SSLSocket()
{

}


void SSLSocket::abrir(void)
{

   __TRAZA__

   if (!bSSL_initiated) {
      bSSL_initiated=true;
      STrz::debug( ConstTraza::T_TRAZA_SOCKETS, "Iniciando libreria SSL");
      SSL_library_init();
      SSL_load_error_strings();
   }
   Socket::abrir();
   ctx=SSL_CTX_new(SSLv23_method());
}

void SSLSocket::destruir()
{
   __TRAZA__

   if (ssl!=NULL) {
    SSL_free(ssl);
    ssl=NULL;
   }
}

void SSLSocket::cerrar()
{
   __TRAZA__

   if (ssl!=NULL) {
     SSL_shutdown(ssl);
   }
   if (ctx!=NULL) {
    SSL_CTX_free(ctx);
    ctx=NULL;
   }
   Socket::cerrar();
}

void SSLSocket::conectar(void)
{
   __TRAZA__

   Socket::conectar();

   ssl=SSL_new(ctx);
   sbio=BIO_new_socket(getId(),BIO_NOCLOSE);
   SSL_set_bio(ssl,sbio,sbio);
#ifdef _DEBUG
   bio_err=BIO_new_fp(stderr,BIO_NOCLOSE);
#endif

   if(SSL_connect(ssl)<=0) {
#ifdef _DEBUG
      BIO_printf(bio_err,"%s\n","SSL SSLSocket conectando ERROR");
      ERR_print_errors(bio_err);
#endif
      throw DesconocidoExcepcion( __AQUI__, "SSL connect error");
   }
}

SDWordType SSLSocket::enviar( const SByteType* sbDatos,const SDWordType& sdLongitud ) const
{

   SDWordType          lsdNumBytesEnviados;

   lsdNumBytesEnviados=SSL_write(ssl,sbDatos,sdLongitud);

   switch(SSL_get_error(ssl,lsdNumBytesEnviados)){
      /* We wrote something*/
      case SSL_ERROR_NONE:
      break;

      /* We would have blocked */
      case SSL_ERROR_WANT_WRITE:
      lsdNumBytesEnviados=0;
      break;

      /* We get a WANT_READ if we're
         trying to rehandshake and we block on
         write during the current connection.

         We need to wait on the socket to be readable
         but reinitiate our write when it is */
      case SSL_ERROR_WANT_READ:
      lsdNumBytesEnviados=0;
      break;

         /* Some other error */
      default:
      throw SinRecursosExcepcion( __AQUI__, "Error SSL_write", errno );
      break;
   }

   return lsdNumBytesEnviados;

}

SDWordType SSLSocket::recibir( SByteType* sbDatos,const SDWordType& sdLongitudMax )
{

   SDWordType         lsdLonAddr;
   struct timeval     lTimeout;
   struct timeval*    lpTimeout;
   fd_set             lReadfds; // socket de lectura
   SDWordType         lswResultadoSelect;
   SDWordType         lsdNumBytesRecibidos;

   // Timeout
   FD_ZERO( &lReadfds );
   FD_SET( getId() ,&lReadfds );
   if ( true == hasTimeoutRecibir() )
   {
      lTimeout = milisegundosToTimeval( getTimeoutRecibir() );
      lpTimeout = &lTimeout;
   }
   else
      lpTimeout = NULL;
   // Esperamos que el socket reciba algun dato
   if (ssl_buff_size-ssl_buff_readed>0) {
      lswResultadoSelect = 1;
   }
   else {
      lswResultadoSelect = select( getId() + 1, &lReadfds,  NULL, NULL, lpTimeout );
   }
   if ( FAIL == lswResultadoSelect )
      throwSelectExcepcion( errno );
   if ( 0 == lswResultadoSelect )
   {
      throw TimeoutExcepcion( __AQUI__, "Se ha superado el tiempo maximo para"\
      " recibir datos por el socket" );
   }

   // Lectura de los datos recibidos
   lsdLonAddr = sizeof(struct sockaddr_in);

   lsdNumBytesRecibidos=recibirSSL(sbDatos,sdLongitudMax);
   if ( FAIL == lsdNumBytesRecibidos)
      throwRecvExcepcion( errno );

   return(lsdNumBytesRecibidos);


};


SDWordType SSLSocket::recibirSSL(SByteType* sbDatos, const SDWordType& sdLongitudMax ) {
   SDWordType readed;
   SDWordType sdwBufToRead;
   SDWordType sdwBufReaded;
   SDWordType ret;
   bool read_blocked;

   while (true) {

      sdwBufToRead=ssl_buff_size-ssl_buff_readed;
      if (sdwBufToRead>0) {
         sdwBufReaded=(sdLongitudMax < sdwBufToRead ) ? sdLongitudMax : sdwBufToRead;
         memcpy(sbDatos,ssl_buff+ssl_buff_readed,sdwBufReaded);
         ssl_buff_readed+=sdwBufReaded;
         if (ssl_buff_readed>=ssl_buff_size) {
            ssl_buff_size=0;
            ssl_buff_readed=0;
         }
         return sdwBufReaded;
      }

      do {
         read_blocked=false;

         readed=SSL_read(ssl,ssl_buff,MAX_SSL_BUFFER-ssl_buff_size);
         switch((ret=SSL_get_error(ssl,readed))){
            case SSL_ERROR_NONE:
               ssl_buff_size+=readed;
               break;
            case SSL_ERROR_ZERO_RETURN:
               /* End of data */
               return(0);
               break;
            case SSL_ERROR_WANT_READ:
               read_blocked=true;
               break;
            case SSL_ERROR_WANT_WRITE:
               break;
            case SSL_ERROR_SYSCALL: /* look at error stack/return value/errno */
               STrz::debug (ConstTraza::T_TRAZA_SOCKETS, "SSLSocket::%s SSL_ERROR_SYSCALL err=%d ret=%d errno=%d",
                               __FUNCTION__,ERR_get_error(),readed,errno);
               return(FAIL);
               break;
            default:
               STrz::debug (ConstTraza::T_TRAZA_SOCKETS, "SSLSocket::%s Error SSL_read %d", __FUNCTION__,ret);
               return(FAIL);
               break;
         }
      } while (SSL_pending(ssl) && !read_blocked);

   }

   return (FAIL);
}

bool SSLSocket::setClavePrivada(SByteType *Fichero,SDWordType szFichero) {

   __TRAZA__

   if (szFichero<MAX_SSL_BUFFER) {
      strcpy(PrivateKeyPemFile,Fichero);
      return true;
   }
   return false;
}

bool SSLSocket::setCertificado(SByteType *Fichero,SDWordType szFichero) {

   __TRAZA__

   if (szFichero<MAX_SSL_BUFFER) {
      strcpy(CertificatePemFile,Fichero);
      return true;
   }
   return false;
}

void SSLSocket::aceptarConexion( SSLSocket& sslsocket) const
{

   __TRAZA__
   SDWordType ret;

   Socket::aceptarConexion(sslsocket);

   sslsocket.ctx=SSL_CTX_new(SSLv23_method());

   //openssl genrsa 2048 > priv.pem

   if(!(SSL_CTX_use_PrivateKey_file(sslsocket.ctx,PrivateKeyPemFile,SSL_FILETYPE_PEM)))
      throw DesconocidoExcepcion( __AQUI__, "Can't read private key file", errno);

   //openssl req -new -x509 -key priv.pem -keyform PEM -out cert.pem -batch

   if(!(SSL_CTX_use_certificate_file(sslsocket.ctx,CertificatePemFile,SSL_FILETYPE_PEM)))
      throw DesconocidoExcepcion( __AQUI__, "Can't read cetificate file", errno);

   sslsocket.ssl=SSL_new(sslsocket.ctx);
   sslsocket.sbio=BIO_new_socket(sslsocket.getId(),BIO_NOCLOSE);
   SSL_set_bio(sslsocket.ssl,sslsocket.sbio,sslsocket.sbio);
#ifdef _DEBUG
   sslsocket.bio_err=BIO_new_fp(stderr,BIO_NOCLOSE);
#endif
   if((ret=SSL_accept(sslsocket.ssl))<=0) {
#ifdef _DEBUG
      BIO_printf(sslsocket.bio_err,"%s\n","SSL Socket accept ERROR");
      ERR_print_errors(sslsocket.bio_err);
#endif
      SSL_free(sslsocket.ssl);
      sslsocket.ssl=NULL;
      throw DesconocidoExcepcion( __AQUI__, "Error SSL_accept.", errno );
   }
   else if (ret==1) {
      STrz::debug (ConstTraza::T_TRAZA_SOCKETS, "SSLSocket::%s Conexion establecida.", __FUNCTION__,ret);
   }
}

