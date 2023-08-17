// =======================
//  FICHERO - TipoTraza
// =======================

#ifndef NivelTraza_H
#define NivelTraza_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

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
  * FICHERO: TipoTraza.h
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: 13:35:58
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
class NivelTraza
{

   // CLASES INTERNAS
   // =====================================================

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
    // Tipo de configuracion de las trazas
    enum NivelTrazaTypeE
    {
       DESCONOCIDO           =  -1,
       TRAZAR_NO             =   0,
       TRAZAR_BASIC          =   1,
       TRAZAR_DEBUG          =   2
    };
   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
     static NivelTrazaTypeE getNivel(const Cadena& lTexto);
     static CPSByteType getLiteral(NivelTrazaTypeE lNivel);
   // ===============   CICLO DE VIDA    ==================
   // =================   OPERACIONES   ===================
   ////////////////////   PRIVATE       ////////////////////
   private:
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //TipoTraza.h

// ===========================
//  FIN DEL FICHERO - TipoTraza.h
// ===========================



