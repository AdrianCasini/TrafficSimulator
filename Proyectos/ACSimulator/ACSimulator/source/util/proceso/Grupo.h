#ifndef GRUPO_H
#define GRUPO_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "util/proceso/ProcesoHijo.h"
#include "util/DLList.h"
#include "util/IteradorDLL.h"
#include "ipc/SemIPC.h"
// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   GRUPO.H
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

class Grupo
{
  // ATRIBUTOS
  // =====================================================
  public:
    static const UWordType MAXIMOS_PROCESOS_HIJO = 40;
  ////////////////////   PRIVATE     ////////////////////
  private:
   DLList<ProcesoHijo,MAXIMOS_PROCESOS_HIJO> mLista;
   IteradorDLL<ProcesoHijo,MAXIMOS_PROCESOS_HIJO> mIterador;
   SWordType mswNumero;
   bool mbArrancado;
  // METODOS
  // =====================================================
  ////////////////////   PUBLIC       ////////////////////
  public:
    // ===============   CICLO DE VIDA    ==================
    Grupo();
    virtual ~Grupo();
    // =================   OPERACIONES   ===================
    void add(ProcesoHijo& lpProceso);
    void activar ( SemIPC& lSemaforo );
    // =================   ACCESO       ====================
    void setId(SWordType lswNumero);
    void setArrancado ( bool lbArrancado );
    // =================   CONSULTA      ===================
    bool getArrancado();
    bool getArrancando();
    bool getProcNoViaParados();
    SWordType getNum();
    SWordType getId();
    ProcesoHijo* getPrimero();
    ProcesoHijo* getSiguiente();
    ProcesoHijo* getPrimero(ProcesoHijo::EstadoProcesoTypeE lEstado);
    ProcesoHijo* getSiguiente(ProcesoHijo::EstadoProcesoTypeE lEstado);
    void getIterador(IteradorDLL<ProcesoHijo,Grupo::MAXIMOS_PROCESOS_HIJO> &lIterador);

};

// ================================================================
// DEFINICI\uFFFDN DE M\uFFFDTODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================


#endif

