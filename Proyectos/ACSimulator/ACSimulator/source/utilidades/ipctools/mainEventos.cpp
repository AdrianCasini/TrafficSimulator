// =======================
//  FICHERO - mainEventos.cpp
// =======================

#include "gestor/ipc/conector/ConEventos.h"
#include "util/IteradorDLL.h"
#include "util/Observador/ObjObservadorGenerico.h"
#include <getopt.h>
// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================
enum OpcionesTypeE
{
   OPT_LIST,
   OPT_DELETE,
   OPT_HELP,
   FIN_OPCIONES,
};

struct option long_options[FIN_OPCIONES + 1] =
{
   {"list",       no_argument, NULL, 0},
   {"delete",      required_argument, NULL, 0},
   {"h",          no_argument, NULL, 0},
   {0, 0, 0, 0}
};

struct OpcionesTypeS
{
   bool mList;
   Cadena mDelete;
};

void help(int argc, char **argv)
{
   printf(
   "Uso: %s        [OPCIONES]     ]                                    \n"
   "Opciones:                                                          \n"
   "   -list  Lista la configuracion actual de los eventos             \n"
   "   -delete Delete del Id observador                                \n"
   "   -h     Muestra esta ayuda                                       \n"
   ,argv[0]);
}


struct OpcionesTypeS mOpciones;

bool getOpciones (int argc, char* argv[])
{
   int res, opt;
   while ((res = getopt_long_only (argc, argv, "", long_options, &opt)) != -1)
   {
      if (res == ':' || res == '?')
         return EXIT_FAILURE;
      switch ((enum OpcionesTypeE)opt)
      {
       case OPT_LIST:
           mOpciones.mList = true;
           break;
       case OPT_DELETE:
           mOpciones.mDelete= optarg;
           break;
       case OPT_HELP:
       default:
         return(EXIT_FAILURE);
      }
   }
   return EXIT_SUCCESS;
}
int main( int argc, char * argv[] )
{
 ConEventos mConEventos;
 ConEventos::listaObservadores lLista;
 ConEventos::datosObservadorTypeS *lpObser;
 UWordType luwIndice;
 IteradorDLL<ConEventos::datosObservadorTypeS,ConEventos::MAXIMOS_OBSERVADORES> lIterador;
 Cadena lTemp;

 if (EXIT_SUCCESS != getOpciones(argc,argv))
 {
   help(argc,argv);
   return(-1);
 }

 lIterador.set(&lLista);
 mConEventos.inicializar();

  printf("Lista de eventos:\n");
  luwIndice = 0;
  lTemp = EventoProc::getLiteral((EventoProc::datoTypeE)luwIndice);
  while ( lTemp != "??")
  {
   lLista.clear();
   mConEventos.get(lLista, (EventoProc::datoTypeE)luwIndice);
   if ( lLista.getNumItems() > 0 )
   {
    if ( mOpciones.mList )
    {
      printf("Evento número %d (%s) observadores:\n",luwIndice,EventoProc::getLiteral((EventoProc::datoTypeE)luwIndice));
      lpObser = lIterador.getPrimero();
      while(lpObser != NULL)
      {
       printf("|--Observador id: %x pid: %d tipo: %d %s\n",lpObser->luwId,lpObser->lPid,
             lpObser->lTipo,lpObser->lTipo == 0 ? "GENERICO" : "PROPIO");
       lpObser = lIterador.getSiguiente();
      }
   }
   else if ( !mOpciones.mDelete.isEmpty())
   {
      ObjObservadorGenerico lObjObservador;
     lpObser = lIterador.getPrimero();
     while(lpObser != NULL)
     {
      lTemp.formCadena("%x",lpObser->luwId );
      if ( lpObser->lTipo == 0 && lTemp == mOpciones.mDelete)
      {
       printf("Encontrado %x\n",lpObser->luwId );
       lObjObservador.inicializar(lpObser->luwId);
       mConEventos.sub(lObjObservador);
       return (0);
      }
      lpObser = lIterador.getSiguiente();
     }
   }
   else
   {
    help(argc,argv);
    return(-1);
   }
  }
  else if ( mOpciones.mList )
  {
    printf("No hay nadie suscrito al evento %s\n",lTemp.getCadena());
  }
  luwIndice++;
  lTemp = EventoProc::getLiteral((EventoProc::datoTypeE)luwIndice);
 }
 mConEventos.finalizar();
}

// =======================
//  FIN DEL FICHERO - mainEventos.cpp
// =======================

