// =======================
//  FICHERO - FORMATOEXCEPCION.H
// =======================

#ifndef FORMATOEXCEPCION_H
#define FORMATOEXCEPCION_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "Excepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: FORMATOEXCEPCION.H
   *
   * DESCRIPCION:
   *   Excepciones lanzadas donde el formato del dato no sea correcto.
   *
   * FECHA CREACION: 05-05-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Oscar Lopez Calvo
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |       |                   |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class FormatoExcepcion : public Excepcion
{
  public:

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
      FormatoExcepcion(const UWordType uwNumLinea,const char* uwNomFichero,
                  const char* uwNomFuncion,const Cadena& mStr,
                  const SDWordType sdwNumError = -1,Excepcion* peExcepcion = NULL);

     /**
      *   Destructor, no realiza ninguna accion.
      *   @param     Ninguno
      *   @return    Ninguno
      *   @exception Ninguno
      */
      virtual ~FormatoExcepcion();
};

#endif // FORMATOEXCEPCION_H

// ===========================
//  FIN DEL FICHERO - FORMATOEXCEPCION.H
// ===========================
