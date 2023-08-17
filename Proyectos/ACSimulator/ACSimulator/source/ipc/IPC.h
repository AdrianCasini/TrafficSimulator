// =======================
//  FICHERO - IPC.H
// =======================

#ifndef IPC_H
#define IPC_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA
#include <sys/types.h>
#include <sys/ipc.h>

// INCLUDES DEL PROJECTO
#include "../lang/Cadena.h"
#include "../lang/throwable/Excepcion.h"
#include "../lang/throwable/PermisoExcepcion.h"
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/FormatoExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
<PRE>
*     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
*     ---------------------------------------
*
* FICHERO: IPC.H
*
* DESCRIPCION:
*  Clase utilidad para el uso de recusos compartidos IPC
*
* FECHA CREACION: 21-04-2005
*
* AUTOR DISENYO:
*
* PROGRAMADOR: Jorge Martinez
*
* MODIFICATIONES:
*
*    |   FECHA    |   AUTOR   |  RAZON                                   |
*    ---------------------------------------------------------------------
*    | DD-MM-YYYY |               |                                        |
*    ---------------------------------------------------------------------
*
* </PRE>
*/

class IPC
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      /**
      * Modo de adquisicion del recurso. VINCULAR vincula el objeto a un recurso
      * IPC existente. CREAR vincula el objeto al recurso, creandolo si no existe.
      * Un recurso creado con CREAR_EXCLUSIVO retornara error si el recurso ya
      * existe
      */
      static const SWordType VINCULAR        = 0;
      static const SWordType CREAR           = IPC_CREAT;
      static const SWordType CREAR_EXCLUSIVO = IPC_CREAT | IPC_EXCL;

      /**
      * Constantes para controlar el comportamiento de las operaciones sobre
      * los recursos IPC
      */
      static const SDWordType BLOQUEAR    = 0;
      static const SDWordType NO_BLOQUEAR = IPC_NOWAIT;

      /**
      * Constantes para asignar permisos de usuarios y acceso
      */
      // Usuarios
      static const SDWordType USUARIO   = 0700;
      static const SDWordType GRUPO     = 0070;
      static const SDWordType OTROS     = 0007;

      // Accessos
      static const SDWordType LECTURA           = 0444;
      static const SDWordType ESCRITURA         = 0222;


   ////////////////////   PRIVATE      ////////////////////
   private :

     /// Archivo utilizado por ftok para generar la clave del recurso IPC
     static const SByteType ARCHIVO_FTOK[];

     /// Key del recurso
     SDWordType msdKey;

     /// Identificador del recurso en el sistema
     SDWordType msdId;

     /// Flags para la creacion recurso compatido
     SDWordType msdFlg;

   // METODOS
   // =====================================================

   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================
      IPC();
      // ===============   CONSULTA   ========================
      inline SDWordType getFlg() const
      {
        return msdFlg;
      };

      inline SDWordType getId() const
      {
        return msdId;
      };

      inline SDWordType getKey() const
      {
        return msdKey;
      };

   ////////////////////   PROTECTED    ////////////////////
   protected:
      // ===============   ACCESO   ==========================
      void setFlg( const SDWordType& sdFlg );
      void setId( const SDWordType& sdId );
      void setKey( const SDWordType& sdKey );
      void setCnt( const SDWordType& sdCnt );

      virtual void resetAtributos();

   public:
      // ===============   OPERACIONES  ======================
      /**
      *<PRE>
      *    descripcion
      *
      * PARAMETROS:
      *    uwIdentificador -> Identificador a partir del cual se genera la clave.
      *    Dos identificadores diferentes siempre generan claves diferentes
      *
      * VALORES RETORNO:
      *    Clave numerica generada
      *
      * EXCEPCIONES:
      *    PermisoExcepcion, NoExisteExcepcion, FormatoExcepcion,
      *    MemoriaExcepcion, RangoExcepcion, DesconocidoExcepcion
      *
      * NOTAS:
      *
      * </PRE>
      */
      static SDWordType generaClave( const UWordType& uwIdentificador,
                                                           CPSByteType lcpsbFichero = NULL );
};

#endif //IPC_H

// ===========================
//  FIN DEL FICHERO - IPC.H
// ===========================
