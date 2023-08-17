// =======================
//  FICHERO - Excepcion.H
// =======================

#ifndef EXCEPCION_H
#define EXCEPCION_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdio.h>

// INCLUDES DEL PROJECTO

#include "../../lang/Types.h"
#include "../Cadena.h"

// INCLUDES LOCALES

#define __AQUI__      __LINE__,__FILE__,__FUNCTION__

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: Excepcion.H
  *
  * DESCRIPCION: Tratamiento de las excepciones del sistema
  *
  * FECHA CREACION: 18-04-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |      |                 |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
 */
class Excepcion
{

   // CLASES INTERNAS
   // =====================================================

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   private:
      static     CUWordType LON_MAX_PILA = 30;        // Maxima longitud de la cola
      static     CSByteType mcsbSeparador ='|';
      SByteType  mCadenas[LON_MAX_PILA][Cadena::LON_MAX_STRING + 1];     // variable que contiene el string
      UDWordType muwIndice;              // Indice a la pila de excepciones

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

      /**
      Constructor con  un comentario de texto, un numero de linea, un
      nombre de fichero, y un nombre de funcion. La excepcion se guarda
      automaticamente en el stack de excepciones. Los argumentos uwNumLinea,
      uwNomFichero, y uwNomFuncion (correspondientes a las macros __LINE__,
      __FILE__, y __FUNCTION__), se pueden sustituir por la definicion __AQUI__
      declarada en Excepcion.h.
         @param mStr -> Cadena de texto con la  descripcion de la Excepcion
         @param uwNumLinea -> Corresponde al numero de linea donde se
            genera la excepcion (__LINE__)
         @param uwNomFichero -> Corresponde al nombre del fichero donde se
            genera la excepcion. (__FILE__)
         @param uwNomFuncion -> Corresponde al nombre de la funcion donde se
            genera la excepcion. (__FUNCTION__)
         @param sdwNumError (opcional) -> Parametro con el codigo de error del
            sistema en el caso de que las excepciones generadas sean por metodos
            del sistema. En el caso de omitir el parametro se tiene que colocar
            el valor -1.
         @param peExcepcion (opcional) -> Puntero opcional a un objeto de tipo
            Excepcion, en el caso de que sea diferente a NULL, todas las
            excepciones que tenga seran copiadas a la nueva excepcion.
      */
      Excepcion(const UWordType uwNumLinea,const char* uwNomFichero,
                      const char* uwNomFuncion,const Cadena& mStr,
                      const SDWordType sdwNumError = -1,
                      Excepcion* peExcepcion = NULL);

     /**
      *   Destructor, no se realiza ninguna accion.
      *   @param      Ninguno
      *   @return     Ninguno
      *   @exception  Ninguno
      */
      virtual ~Excepcion();

      // =================   OPERACIONES   ===================

     /**
      *  Realiza una salida por el cout con el contenido formateado del stack
      *  de excepciones.
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      void ImprimirStack();

      /** Devuelve el stack en un const char*
       */
      const char* getStack();

     /**
      *   Devuelve el numero de Excepciones que contiene el objeto.
      *   @param      Ninguno
      *   @return     Ninguno
      *   @exception  Ninguno
      */
      UDWordType  getNumeroExcepciones();

     /**
      *   Devuelve el texto del numero de excepcion que se pasa como parametro
      *
      *   @param   udNumeroExcepcion-> Numero de excepcion que se lee. El 0 es la excepcion
      *            actual y la LON_MAX_PILA es la ultima.
      *   @param   mTexto -> Texto de la excepcion que se solicita, en caso de no encontrase
      *            el elemento, no se modifica el texto de mTexto.
      *   @return  Ninguno
      *   @exception  Ninguno
      */
      const char*  getExcepcion(CUDWordType cudNumeroExcepcion);


     /**
      *  Retorna el ultimo texto de la excepcion
      *  @param      Ninguno
      *  @return     Ninguno
      *  @exception  Ninguno
      */
      const char* getTextoUltimaExcepcion();

     /**
      *  Retorna el texto de usuario, de la ultima excepcion generada
      *  @param      Ninguno
      *  @return     Cadena con el texto de la excepcion
      *  @exception  Ninguno
      */
      Cadena getTextoUsuarioUltimaExcepcion();

   ////////////////////   PRIVATE       ////////////////////
   private:

     /**
      *   Borra todo el contenido de la pila de stack.
      *   @param      Ninguno
      *   @return     Ninguno
      *   @exception  Ninguno
      */
      void LimpiarStackTrace();

     /**
      * Inserta un texto de excepcion dentro de la pila de stack de la excepcion
      * @param mStr-> Cadena de texto de la excepcion que se quiere insertar.
      * @return     Ninguno
      * @exception  Ninguno
      */
      void  InsertaElemento(const char* mStr);

     /**
      *  Copia todas las excepciones generadas en la excepcion que recibe como
      *  parametro.
      *
      *  @param      peExcepcion -> Puntero a la excepcion que se quiere copiar.
      *  @return     Ninguno
      *  @exceptin   Ninguno
      */
      void  CopiarStackExcepciones(Excepcion* peExcepcion);

};


// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //Excepcion_H

// ===========================
//  FIN DEL FICHERO - Excepcion.H
// ===========================
