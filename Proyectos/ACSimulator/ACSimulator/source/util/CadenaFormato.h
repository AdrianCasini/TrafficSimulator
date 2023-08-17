
#ifndef CADENA_FORMATO_H
#define CADENA_FORMATO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL PROYECTO
#include "../lang/Cadena.h"
#include "../thread/Mutex.h"

// INCLUDES LOCALES
#include <sys/types.h>
#include <regex.h>
#include <stdarg.h>


// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: CadenaFormato.h
  *
  * DESCRIPCION: manipulacion de cadenas de texto especiales, que contienen
  * etiquetas de formato pseudo-HTML
  *
  * FECHA CREACION: 30-03-2006
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Marc Coll Carrillo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |      |               |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

class CadenaFormato
{
   ////////////////////   PUBLIC       ////////////////////
   public:

      /**
      Formatos que acepta la cadena. Los valores permiten indicar varios
      formatos mediante una mascara de bits. Por ejemplo:<br>
         CadenaFormato cad("EJEMPLO", CadenaFormato::NEGRITA | CadenaFormato::PARPADEO);
      */
      enum FormatoTypeE
      {
         NEGRITA = 0X01,
         PARPADEO = 0X02,
         SUBRAYADO = 0X04,
         VIDEO_INVERSO = 0X08
      };

      // ===============   CICLO DE VIDA   ==================

      /** Constructor por defecto */
      CadenaFormato ();

      /** Constructor por copia */
      CadenaFormato (const CadenaFormato& cadSrc);

      /**
      Constructor a partir de otra cadena y de uno o varios formatos opcionales
      <br>Ej:<br>
         CadenaFormato cad("EJEMPLO");<br>
         CadenaFormato cad("EJEMPLO", CadenaFormato::SUBRAYADO);<br>
         CadenaFormato cad("EJEMPLO", CadenaFormato::NEGRITA | CadenaFormato::PARPADEO);
      */
      CadenaFormato (const Cadena& cadSrc, UWordType uwFormato=0);

      /**
      Constructor a partir de un string de C y de uno o varios formatos opcionales
      <br>Ej:<br>
         CadenaFormato cad("EJEMPLO");<br>
         CadenaFormato cad("EJEMPLO", CadenaFormato::SUBRAYADO);<br>
         CadenaFormato cad("EJEMPLO", CadenaFormato::NEGRITA | CadenaFormato::PARPADEO);
      */
      CadenaFormato (CPSByteType psbSrc, UWordType uwFormato=0);

      /** Destructor */
      ~CadenaFormato ();

      // ===============   OPERADORES   ==================

      /**
      Operador para convertir la cadena en un string de C automagicamente.
      La cadena resultante incluye las etiquetas de formato */
      operator const char* () const;

      /** Operador de asignacion con otro CadenaFormato */
      CadenaFormato& operator= (const CadenaFormato& cadSrc);

      /** Operador de concatenacion con otro CadenaFormato */
      CadenaFormato& operator+= (const CadenaFormato& cadSrc);

      /** Operador de concatenacion con otro CadenaFormato */
      CadenaFormato operator+ (const CadenaFormato& cadSrc) const;

      /** Operador de comparacion con otra CadenaFormato */
      bool operator== (const CadenaFormato& cadSrc) const;

      /**
      Operador de comparacion con una Cadena. Compara ambas cadenas ignorando
      las etiquetas de formato del objeto CadenaFormato
      */
      bool operator== (const Cadena& cadSrc) const;

      /**
      Operador de comparacion con una Cadena. Compara ambas cadenas ignorando
      las etiquetas de formato del objeto CadenaFormato
      */
      bool operator!= (const Cadena& cadSrc) const;

      /**
      Operador de comparacion con una Cadena. Compara ambas cadenas ignorando
      las etiquetas de formato del objeto CadenaFormato
      */
      //bool operator>= (const Cadena& cadSrc) const;

      /**
      Operador de comparacion con una Cadena. Compara ambas cadenas ignorando
      las etiquetas de formato del objeto CadenaFormato
      */
      //bool operator<= (const Cadena& cadSrc) const;

      /**
      Operador de comparacion con una Cadena. Compara ambas cadenas ignorando
      las etiquetas de formato del objeto CadenaFormato
      */
      bool operator> (const Cadena& cadSrc) const;

      /**
      Operador de comparacion con una Cadena. Compara ambas cadenas ignorando
      las etiquetas de formato del objeto CadenaFormato
      */
      bool operator< (const Cadena& cadSrc) const;

      // ===============   OPERACIONES   =================

      /**
      Concatena una Cadena con el formato (o formatos) indicado
      <br>Ej:<br>
         cad.concatenarCadena ("EJEMPLO", CadenaFormato::SUBRAYADO);<br>
         cad.concatenarCadena ("EJEMPLO", CadenaFormato::NEGRITA | CadenaFormato::PARPADEO);
         @param uwFormato -> Mascara de bits que indica el formato (o formatos)
         que se quiere establecer. Por defecto es 0 (sin formato)
      */
      void concatenarCadena (const Cadena& cadSrc, UWordType uwFormato=0);

      /**
      Concatena una cadena que ya contiene etiquetas de formato
         @param cadFormato -> La cadena, que puede contener etiquetas de formato
         @exception FormatoExcepcion -> Si la cadena no es valida
      */
      void concatenarCadenaFormato (const Cadena& cadFormato);

      /** Elimina todas las etiquetas de formato de la cadena */
      void borrarFormatos ();

      /**
      Funcion equivalente al sprintf, pero con un parametro extra para indicar
      el formato (0 si no se desea ningun formato)
         @param psbStr -> Formato de la cadena (como en la funcion sprintf)
         @param uwFormato -> Mascara de bits que indica el formato (o formatos)
         que se quiere establecer (0 para ningun formato)
      */
      void formCadena (CPSByteType psbStr, UWordType uwFormato, ...);

      /**
      Funcion que devuelve la cadena interna mediante subcadenas de formato.
      Util para mostrar en pantalla la cadena formateada. Para usarla, primero
      hay que llamar a la funcion pasandole un 0 como indice, y a partir de ahi
      basta con iterar (usando el mismo valor de indice que se devuelve) para
      que la funcion nos vaya dando una subcadena cada vez que cambie el
      formato
         @param cadStr -> Cadena en la que se devolvera la subcadena del objeto
         @param uwFormato -> devuelve el formato de dicha subcadena (o 0 si no
         tiene formato).
         @param uwIndice -> Indice a partir del cual se debe iniciar la busqueda
         de la siguiente subcadena. Hay que pasar un 0 al inicio, y a partir de
         ahi la propia funcion ya devuelve el indice siguiente cada vez
         @return bool -> Devuelve true mientras no se haya llegado al final de
         la cadena, y false una vez terminado
      */
      bool getSubcadenaFormato (Cadena& cadStr, UWordType& uwFormato,
         UWordType& uwIndice) const;

      // ===============   GETS   ==================

      /** Devuelve la longitud de la cadena, sin contar las etiquetas de formato */
      UWordType getLong () const;

      /** Devuelve la longitud de la cadena, contando las etiquetas de formato */
      UWordType getLongFormato () const;

      /** Devuelve la cadena, sin las etiquetas de formato */
      Cadena getCadena () const;

      /** Devuelve la cadena, con las etiquetas de formato */
      Cadena getCadenaFormato () const;

      /** Devuelve si la cadena contiene realmente alguna etiqueta de formato */
      bool hayFormato () const;

      // ===============   FORMATO   ==================

      /**
      Establece el formato(s) especificado(s)
      <br>Ej:<br>
         cad.setFormato (CadenaFormato::SUBRAYADO);<br>
         cad.setFormato (CadenaFormato::NEGRITA | CadenaFormato::PARPADEO);
         @param uwFormato -> Mascara de bits que indica el formato (o formatos)
         que se quiere establecer. Por defecto es 0 (sin formato)
      */
      void setFormato (UWordType uwFormato);

      /** Establece el formato de negrita */
      void setNegrita ();

      /** Establece el formato de parpadeo */
      void setParpadeo ();

      /** Establece el formato de subrayado */
      void setSubrayado ();

      /** Establece el formato de video inverso */
      void setVideoInverso ();
      bool hayFormatoCompleto(const Cadena& lFormato)const ;

   ////////////////////   PRIVATE      ////////////////////
   private:

      inline void setFormato (CPSByteType psbFmt);
      inline bool validarCadena (const Cadena& cadStr) const;
      UWordType calcularLongSinFormato (const Cadena& cadFrmt) const;
      static void inicializarRegexps ();
      inline UWordType getFormato (const Cadena& cadFormato) const;

      Cadena           mcadStr;
      UWordType        muwLong;
      static regex_t   msRegexpValidar;
      static regex_t   msRegexpFormato;
      static bool      mbooRegexp;
      static Mutex     mmtxRegexp;
};

#endif //CADENA_FORMATO_H
