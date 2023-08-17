#ifndef CADENALIST_H
#define CADENALIST_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "../lang/Cadena.h"
#include "../lang/throwable/RangoExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO:   CadenaList.H
   *
   * DESCRIPCION: Tratamiento de cadenas anidadas
   *
   * FECHA CREACION: 24/4/2006
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

class CadenaList
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE     ////////////////////
   private:
      static const SByteType SEPARACION_DEF = ';';
      UWordType   muwNumItems;
      UWordType   muwMaxLong;
      Cadena      mValor;
      Cadena      mSeparador;
   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================
      CadenaList();
      CadenaList(const Cadena& lValores,const Cadena& lSeparacion = SEPARACION_DEF);
      virtual ~CadenaList();
      // =================   OPERADORES   ===================
      Cadena operator [] (UWordType luwNum) const;
      // =================   OPERACIONES   ===================
      void add(const Cadena& lValor);
      void inicializar(const Cadena& lValor);
      void borrar();
      // =================   ACCESO       ====================
      void setSeparador(const Cadena& lSeparacion);
      // =================   CONSULTA      ===================
      Cadena getSiguiente(const Cadena& lValor) const;
      Cadena getAnterior(const Cadena& lValor) const;
      Cadena getValor(const UWordType luwNum) const;
      UWordType getNumItems() const;
      Cadena getValor() const;
      Cadena getSeparador() const;
      bool   valorCorrecto(const Cadena& lValor) const;
      UWordType getMaximaLong() const;
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

