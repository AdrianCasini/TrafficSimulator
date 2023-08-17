#ifdef _NOSSL
#warning *ATENCION* (_NOSSL) Compilado sin conexion SSL.
#define _SSLSOCKET_H
#define SSLSocket Socket
#include "Socket.h"
#endif

#ifndef _SSLSOCKET_H
#define _SSLSOCKET_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "Socket.h"

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: SSLSOCKET.H
   *
   * DESCRIPCION:
   *
   * FECHA CREACION: 30-07-2007
   *
   * AUTOR : Toni Bassas
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |            |                                              |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class SSLSocket : public Socket
{

   public:

      SSL_CTX *ctx;
      SSL *ssl;
      BIO *sbio;
#ifdef _DEBUG
      BIO *bio_err;
#endif

   public:
      SSLSocket();
      virtual ~SSLSocket();

      void abrir();
      void cerrar();
      void conectar();
      void destruir();
      SDWordType enviar( const SByteType* sbDatos, const SDWordType& sdLongitud )
      const;
      SDWordType recibir(SByteType* sbDatos, const SDWordType& sdLongitud );
      void aceptarConexion( SSLSocket& sslsocket) const;

      bool setClavePrivada(SByteType *Fichero,SDWordType szFichero);
      bool setCertificado(SByteType *Fichero,SDWordType szFichero);

   private:

      #define MAX_SSL_BUFFER     2048

      SByteType   PrivateKeyPemFile[MAX_SSL_BUFFER];
      SByteType   CertificatePemFile[MAX_SSL_BUFFER];

      UByteType   ssl_buff[MAX_SSL_BUFFER];
      SDWordType  ssl_buff_size;
      SDWordType  ssl_buff_readed;

      SDWordType recibirSSL(SByteType* sbDatos, const SDWordType& sdLongitud );
};

#endif
