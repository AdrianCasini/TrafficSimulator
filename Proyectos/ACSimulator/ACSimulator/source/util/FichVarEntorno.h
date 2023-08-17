#ifndef FICHVARENTORNO_H
#define FICHVARENTORNO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../util/Directorio.h"
#include "../util/FicheroDatos.h"
#include "../util/ConstTraza.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO:   FichVarEntorno.H
   *
   * DESCRIPCION: Herramiento para modificar, insertar o eliminar la asignacion
   *  de valores en las variables de condicion, en un fichero. Es decir las
   *  declaraciones del tipo export VARA=VALORY
   *
   * FECHA CREACION: 11/4/2006
   *
   * AUTOR DISENO:
   *
   * PROGRAMADOR: Oscar Lopez Calvo
   *
   * MODIFICATIONES:
   *
   *       FECHA       AUTOR     RAZON
   *    ---------------------------------------------------------------------
   *     DD-MM-YYYY
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */

class FichVarEntorno : private FicheroDatos
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE     ////////////////////
   private:
      bool mbCrear;
   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================
      FichVarEntorno();
      virtual ~FichVarEntorno();
      // =================   OPERACIONES   ===================
     /**
      *  Asigna el nombre del fichero, si lbCrear esta igual a false, no
      *  se podra abrir en el caso de que no existe. Si queremos que se
      *  cree debemos colocar el flag lbCrear a true.
      *  @param      lNombre Cadena con el nombre del fichero
      *  @param      lbCrear true indica que si no existe el fichero se crear
      *               false que el fichero ya existe
      *  @return     Ninguno
      *  @exception  las mismas que FicheroDatos::setNombre
      */
      void setNombre(const Cadena& lNombre,bool lbCrear = true);
      // =================   ACCESO       ====================
     /**
      *  Asigna un valor a una variable de entorno, en el caso de no encontrar
      *  la variable de entorno, la funcion la anade al final del fichero.
      *  @param      lNombreVarEntorno Cadena de texto con el nombre de la variable
      *               de entorno
      *  @param      lValor Valor de la variable de entorno
      *  @return     Ninguno
      *  @exception  Las mismas que FicheroDatos::escribir y FicheroDatos::leerLinea
      */
      void setValor(const Cadena& lNombreVarEntorno,const Cadena& lValor);
      // =================   CONSULTA      ===================
     /**
      *  Retorna una cadena de texto con el valor de la variable de entorno
      *  en el caso de que no se encuentre la variable de entorno retorna
      *  una cadena vacia
      *  @param      lNombreVarEntorno Cadena de texto con el nombre de la variable
      *               de entorno
      *  @return     Cadena con el nombre del valor
      *  @exception  Las mismas que FicheroDatos::leerLinea
      */
      Cadena getValor(const Cadena& lNombreVarEntorno);
   ////////////////////   PRIVATE       ////////////////////
   private:
     /**
      *  Recorre desde el inicio del fichero todas las lineas en busca
      *  de lNombreVarEntorno una vez encontrada retorna la primera posicion
      *  al inicio del valor de la variable de entorno es decir lNombreVarEntorno=
      *  @param      lNombreVarEntorno Cadena de texto con el nombre de la variable
      *               de entorno
      *  @return     SDWordType Posicion dentro del fichero donde se encuenta
      *               el valor, o -1 en caso de que no la encuentre
      *  @exception  Las mismas que FicheroDatos::leerLinea
      */
      SDWordType getPosicionValor(const Cadena& lNombreVarEntorno);
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

