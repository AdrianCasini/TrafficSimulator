#ifndef CTRLGRUPOS_H
#define CTRLGRUPOS_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/proceso/EventosProceso.h"
#include "util/proceso/EventosGrupo.h"
#include "util/proceso/OrdenesProceso.h"
#include "util/proceso/ListaGrupos.h"
#include "util/proceso/IteradorListaGrupos.h"
#include "lang/throwable/MemoriaExcepcion.h"
#include "util/ConstTraza.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   CTRLGRUPOS.H
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

class CtrlGrupos : public EventosProceso
{
  // ATRIBUTOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    enum ModoGrupoTypeE
    {
      EXPLOTACION =0 ,
      TEST,
      SIN_ASIGNAR,
    };

  ////////////////////   PRIVATE     ////////////////////
  private:
    OrdenesProceso* mpOrdenesProceso;
    EventosGrupo* mpEventos;
    static ModoGrupoTypeE mModo;
    ListaGrupos* mpLista;
    Grupo*  mpGrupoActual;
  protected:
    IteradorDLL<ProcesoHijo,Grupo::MAXIMOS_PROCESOS_HIJO> mIterador;
    IteradorListaGrupos mIteradorGrupos;

  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    // ===============   CICLO DE VIDA    ==================
    CtrlGrupos();
    virtual ~CtrlGrupos();
    // =================   OPERACIONES   ===================
    virtual void arranca(CtrlGrupos::ModoGrupoTypeE lModo);
    virtual Grupo* siguiente ();
    virtual void parado( SDWordType lpid, SDWordType idExit);
    virtual void arrancado ( SDWordType lpid );
    // =================   ACCESO       ====================
    void set(ListaGrupos* lLista);
    void set(OrdenesProceso* lOrdenes);
    void set(EventosGrupo* lEventos);
    void setGrupoAct( Grupo* lpGrupo);
    void setModo(ModoGrupoTypeE lModo);
    // =================   CONSULTA      ===================
    UWordType getNumGrupoActual();
    static CPSByteType getLiteral(CtrlGrupos::ModoGrupoTypeE lModo);
    OrdenesProceso* getOrdenes();
    EventosGrupo* getEventos();
    CtrlGrupos::ModoGrupoTypeE getModo();
    ListaGrupos* getLista();
    ProcesoHijo* primerProceso ();
    ProcesoHijo* siguienteProceso ();
    Grupo *getGrupoAct();
};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

