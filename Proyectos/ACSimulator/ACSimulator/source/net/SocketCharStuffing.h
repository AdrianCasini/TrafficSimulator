#ifndef SOCKETCHARSTUFFING_H
#define SOCKETCHARSTUFFING_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../net/SSLSocket.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   SOCKETCHARSTUFFING.H
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: 13-03-2006
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |           |                                        |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class SocketCharStuffing : public SSLSocket
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE     ////////////////////
   private:
      static const UWordType MAXIMOS_DATOS = 8196;
      static const UByteType DLE = 0x10;
      static const UByteType STX = 0x02;
      static const UByteType ETX = 0x03;
      enum EstadosTypeE
      {
        ESPERA_SINC_INICIO = 0,
        ESPERA_INICIO_MENSAJE,
        ESPERA_MENSAJE,
        ESPERA_SINC_FIN,
        MENSAJE_RECIBIDO,
      };
      EstadosTypeE         mEstado;
      UWordType            muwIndiceRx;
      UWordType            muwIndiceTx;
      UByteType            mBufferRx[MAXIMOS_DATOS];
      UByteType            mBufferTx[MAXIMOS_DATOS];
   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================
      SocketCharStuffing();
      virtual ~SocketCharStuffing();
      // =================   OPERACIONES   ===================
     /**
      *  Reeimplenta el metodo recibir de la clase SOcket, Realiza la recepcion de un
      *  mensaje completo formado con charstuffing. El campo longitud se mantiene, pero
      *  en realidad solo sirve para verificar que los datos caben en el bufer de destino
      *  @param      lpsbDatos Puntero a una zona de memoria donde se guardara los datos
      *  @param      lsdLongitud Longitud del puntero lpsbDatos
      *  @return     Longitud de los datos recibidos
      *  @exception  MemoriaExcepcion (__AQUI__,"El buffer de destino es mas pequeno que"\
      *          " el mensaje recibido");
      */
      SDWordType recibir(PSByteType lpsbDatos, const SDWordType& lsdLongitud );

     /**
      *  Realiza la misma llamada al metodo recibir(PSByteType lpsbDatos, const SDWordType& lsdLongitud )
      *  pero la grabacion de los datos se realiza sobre la cadena
      *  @param      lDatos Cadena donde se guardaran los datos
      *  @return     Longitud de los datos recibidos
      *  @exception  Las mismas que en SocketCharStuffing::recibir(PSByteType...)
      */
      SDWordType recibir( Cadena& ldatos );
      SDWordType enviar( CPSByteType lpsbDatos, const SDWordType& lsdLongitud ) ;
      SDWordType enviar( const Cadena& ldatos ) ;
   ////////////////////   PRIVATE       ////////////////////
   private:
      void tratarByteRx(UByteType lubDato) ;
      void guardarDatoRx(UByteType lubDato) ;
      void guardarDatoTx(UByteType lubDato) ;
      void limpiarDatosRx();
      void limpiarDatosTx();
      UByteType popRx();
      // =================   ACCESO       ====================
      // =================   CONSULTA      ===================
      UWordType getTamMenRx() const;
      UWordType getTamMenTx() const;
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

