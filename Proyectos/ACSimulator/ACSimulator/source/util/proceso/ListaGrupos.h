#ifndef LISTAGRUPOS_H
#define LISTAGRUPOS_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/proceso/Grupo.h"
#include "lang/throwable/RangoExcepcion.h"
#include "util/DLList.h"
#include "util/IteradorDLL.h"
#include "util/ConstTraza.h"

class IteradorListaGrupos;

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   LISTAGRUPOS.H
  *
  * DESCRIPCION:
  *
  * FECHA CREACION:
  *
  * AUTOR DISE\uFFFDO:
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

class ListaGrupos
{
  // ATRIBUTOS
  // =====================================================
  public:
   static const UWordType MAXIMOS_GRUPOS = 10;
  ////////////////////   PRIVATE     ////////////////////
  private:
   DLList<Grupo,MAXIMOS_GRUPOS>  mGrupos;
   IteradorDLL<Grupo,MAXIMOS_GRUPOS> mIterador;
   Grupo* mpGrupoAct;

  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    // ===============   CICLO DE VIDA    ==================
    ListaGrupos();
    virtual ~ListaGrupos();
    // =================   OPERACIONES   ===================
    void inicializar();
    void add(ProcesoHijo& lProceso);
    void toString();
    // =================   ACCESO       ====================
    // =================   CONSULTA      ===================
    Grupo* getPrimero();
    Grupo* getSiguiente();
    UWordType getNumGrupos();
    UWordType getNumProcesos();
    Grupo* getGrupoAct();
    bool getArrancado();
    void getIterador(IteradorListaGrupos &lIterador);
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

