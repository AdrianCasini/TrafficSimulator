// =======================
//  FICHERO - Programa.cpp
// =======================

#include "Programa.h"  // clase implementada
#include <sys/types.h>
#include <signal.h>


// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////

// ===============   CICLO DE VIDA    ==================
Programa::Programa()
{
  mbBack = false;
  mPid = -1;
  mstatus = 0;
}

Programa::~Programa()
{

}
// =================   OPERACIONES   ===================
void Programa::exec(const Cadena& lPrograma,const Cadena& arg1,const Cadena& arg2)
{
   mPid = fork();

   if (mPid == -1)
       throw IOExcepcion(__AQUI__,"Error en el fork");
   if (mPid == 0) {
       execl (lPrograma.getCadena(),lPrograma.getCadena(),arg1.getCadena(),arg2.getCadena(),NULL);
       exit(-1);
   }
   // Si estamos en foreground nos esperamos
   if ( mbBack == false)
    wait(&mstatus);
}

void Programa::exec(const Cadena& lPrograma,const Cadena& arg1)
{
   mPid = fork();
   if (mPid == -1)
       throw IOExcepcion(__AQUI__,"Error en el fork");
   if (mPid == 0) {
       execl (lPrograma.getCadena(),lPrograma.getCadena(),arg1.getCadena(),NULL);
       exit(-1);
   }
   // Si estamos en foreground nos esperamos
   if ( mbBack == false)
    wait(&mstatus);
}

void Programa::stop()
{
  if ( mPid != -1 )
  {
   kill ( mPid,SIGKILL );
   wait(&mstatus);
  }
}

// =================   ACCESO       ====================
void Programa::setBackground(bool lbBackground)
{
  mbBack = lbBackground;
}

// =================   CONSULTA      ===================
bool Programa::getbackground()
{
  return(mbBack);
}

SDWordType Programa::getExit()
{
  if ( WIFEXITED(mstatus) ) {
    return 0;
  }
  else {
    return -1;
  }
}
// =======================
//  FIN DEL FICHERO - Programa.cpp
// =======================



