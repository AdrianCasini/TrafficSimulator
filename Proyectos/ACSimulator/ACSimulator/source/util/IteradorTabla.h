//---------------------------------------------------------------------------
// IteradorTabla
//---------------------------------------------------------------------------

#if !defined(ITERADORTABLA_H)
  #define ITERADORTABLA_H


// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL PROJECTO
#include "../lang/Types.h"

// INCLUDES LOCALES


// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: IteradorTabla.h
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: 11-08-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oriol Herrera
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |             |                                              |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
 */

class IteradorTabla
{

   // CLASES INTERNAS
   // =====================================================

   ////////////////////   PUBLIC       ////////////////////

   ////////////////////   PROTECTED    ////////////////////
   ////////////////////   PRIVATE      ////////////////////

   public:

   enum TipoIterador {
      GET,
      SET
   };

   private:

      static const SWordType MAX_TIPOS = 5;

      void*  mptrActual;
      void*  mptrPrimero;
      void*  mptrUltimo;
      void*  mptrLimite;

      SDWordType        mubTam;
      SWordType         mswTipo;
      SWordType         mswTipos[MAX_TIPOS];
      SWordType         mswTamTipos[MAX_TIPOS];
      bool              mbooFinal;
      enum TipoIterador meTipo;

   // METODOS
   // =====================================================

   ////////////////////   PUBLIC       ////////////////////
   public:
      IteradorTabla();

      /** Constructor a partir del puntero al priemr registro de la tabla */
      IteradorTabla(void* ptrPrimero);

      virtual ~IteradorTabla();


      /**
      Itera hasta el siguiente registro. Si el iterador tiene que iterar sobre
      diferentes tipos e registro, se mueve hasta el siguiente de su tipo
         @return void* -> Puntero al registro (o NULL si se ha llegado al
         final
      */
      void* operator ++ ();

      /**
      Itera hasta el registro anterior
         @return void* -> Puntero al registro
      */
      void* operator -- ();


      /**
      Devuelve el puntero al primer registro de la tabla
         @return void* -> Puntero al registro
      */
      void* getPrimero() const;

      /**
      Devuelve el puntero al ultimo registro de la tabla
         @return void* -> Puntero al registro
      */
      void* getUltimo() const;

      /**
      Comprueba si el iterador ha llegado al final
         @return bool -> Si estamos en el ultimo registro o no
      */
      bool esUltimo() const;

      /**
      Establece el puntero al primer registro de la tabla
         @param ptrPrimero -> Puntero al registro
      */
      void setPrimero(void* ptrPrimero);

      /**
      Establece el puntero al ultimo registro de la tabla
         @param ptrPrimero -> Puntero al registro
      */
      void setUltimo(void* ptrUltimo);

      /**
      Establece el puntero al final de la tabla, para evitar salirse y
      sobreescribir memoria de otra tabla
         @param ptrPrimero -> Puntero al registro del final de la tabla
      */
      void setLimite(void* ptrLimite);

      /**
      Devuelve el puntero al registro actual
         @return void* -> Puntero al registro
      */
      void* getActual() const;

      /**
      Establece el puntero al registro actual
         @param ptrActual -> Puntero al registro
      */
      void setActual(void* ptrActual);

      /** Inicializa toda la memoria de la tabla a un valor conocido (8) */
      void limpiarZona ();

      /**
      Establece el tamano del registro a iterar
         @param uwTam -> Tamano del registro
      */
      void setTam(UWordType uwTam);

      /**
      Establece el tipo de registro (el campo tipo registro, que es el primero
      de todos), de forma que el iterador pueda reconocer sus registros en
      tablas con mas de un tipo de registro
         @param swTipo -> Tipo de registro
      */
      void setTipo(SWordType swTipo);

      /**
      Establece el tipo de iterador (GET o SET), dependiendo de si el iterador
      se va a usar para iterar sobre los datos de una tabla llena, o para
      hacer sets de los datos en una tabla vacia
         @param eTipo -> Tipo de iterador (GET o SET)
      */
      void setTipoIterador (enum TipoIterador eTipo);

      /**
      Anadir otros tipos de registros (para iterar sobre tablas con varios
      tipos de registros de diferentes tamanos)
         @param swTipo -> Tipo de registro
         @param swTam -> Tamano del registro
      */
      void addOtro (SWordType swTipo, SWordType swTam);

      /**
      Devuelve el tamano del registro
         @return UWordType -> El tamano del registro
      */
      UWordType getTam() const;

      /**
      Devuelve el tipo de iterador
         @return enum TipoIterador -> El tipo de iterador (GET o SET)
      */
      enum TipoIterador getTipoIterador ();

   private:

      SWordType buscarTipo (SWordType swTipo);
      void* siguienteRegistro (SWordType swTipo, SWordType swTam, void* psbFrom);

////////////////////   PROTECTED    ////////////////////
////////////////////   PRIVATE      ////////////////////

};

#endif // IteradorTabla
