#ifndef ITERADORLISTAGRUPOS_H
#define ITERADORLISTAGRUPOS_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/proceso/Grupo.h"
#include "util/proceso/ListaGrupos.h"
#include "lang/throwable/RangoExcepcion.h"
#include "util/DLList.h"
#include "util/IteradorDLL.h"
// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   ITERADORLISTAGRUPOS.H
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

class IteradorListaGrupos
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PRIVATE     ////////////////////
  private:
   IteradorDLL<Grupo,ListaGrupos::MAXIMOS_GRUPOS> mIteradorGrupo;
   IteradorDLL<ProcesoHijo,Grupo::MAXIMOS_PROCESOS_HIJO> mIteradorProcesos;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    // =================   OPERACIONES   ===================
    UWordType getNumProcesos(ProcesoHijo::TiposProcesosTypeE lTipo);
    UWordType getNumParando();
    ProcesoHijo* getPrimero(ProcesoHijo::EstadoProcesoTypeE lEstado);
    ProcesoHijo* getSiguiente(ProcesoHijo::EstadoProcesoTypeE lEstado);
    ProcesoHijo* getPrimerActivo();
    ProcesoHijo* getSiguienteActivo();
    ProcesoHijo* getPrimero(ProcesoHijo::TiposProcesosTypeE lTipo);
    ProcesoHijo* getSiguiente(ProcesoHijo::TiposProcesosTypeE lTipo);
    bool getProcesosActivos();
    bool getProcesosPorParar();
    ProcesoHijo* get( SDWordType lswPid );
    // =================   ACCESO       ====================
    void set(DLList<Grupo,ListaGrupos::MAXIMOS_GRUPOS> *lpLista);
    // =================   CONSULTA      ===================
  private:
    ProcesoHijo* getSiguiente();
    ProcesoHijo* getPrimero();
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

