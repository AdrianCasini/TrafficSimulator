#ifndef CHARENUM_H
#define CHARENUM_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include "stdarg.h"
#include "stdio.h"

// INCLUDES DEL PROJECTO

#include "../lang/Cadena.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   CHARENUM.H
  *
  * DESCRIPCION: Tratamiento de los enumerados con texto.
  *    o textos (literales) como enumerados.
  *
  * FECHA CREACION: 15-07-2005
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Josep Oriol  Herrera Mari
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |                |                                                             |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class CharEnum
{

// ATRIBUTOS
// =====================================================
////////////////////   PRIVATE      ////////////////////
  private:

    struct Obj
    {
      Cadena      cadCad;
      SDWordType  sdNum;
    };

    static SDWordType const MAX_LONG=50;
    struct Obj aGeneral[MAX_LONG];



// METODOS
// =====================================================
////////////////////   PUBLIC      ////////////////////
  public:

// =================   CICLO DE VIDA      ===================

    /**
     * Destructor
     *  @param      Ninguno
     *  @return     Ninguno
     *  @exception  Ninguno
    */
    virtual ~CharEnum();

    /**
     * Constructor
     *  @param      Caracter de relleno (Para los textos).
     *  @return     Ninguno
     *  @exception  Ninguno
    */
    CharEnum(char Car=0);


// =================   ACCESO      ===================

    /**
     *  Guarda en el "enumerado" el texto cadena referenciado por el indice associado
     *  @param      sdIndex -> Indice associado al texto. Indice del enumerado.
     +  @param      Cad -> Cadena con el texto que queremos referenciar.
     *  @return     Ninguno
     *  @exception  Ninguna
     */
     void set(UDWordType udIndex, Cadena cadCad);

    /**
     *  Devuelve el texto previamente almacenado con el indice parametro.
     *  @param      sdIndex -> Indice associado al texto. Indice del enumerado.
     *  @return     Cadena de texto con el contenido almacenado previamente.
     *  @exception  Ninguna
     */
     Cadena get(UDWordType udIndex);

    /**
     *  Devuelve el indice previmanete almacenado con la cadena parametro.
     +  @param      Cad -> Cadena con el texto que queremos buscar.
     *  @return     Indice almacenado previamente con el texto del parametro.
     *  @exception  Ninguna
     */
     SDWordType get(Cadena cadCad);

};


// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif // CHARENUM_H
