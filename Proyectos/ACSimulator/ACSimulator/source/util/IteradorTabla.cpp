// =======================
//  FICHERO - IteradorTabla.cpp
// =======================

#include "IteradorTabla.h"  // clase implementada
#include "../lang/throwable/LlenoExcepcion.h"
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"

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

IteradorTabla::IteradorTabla()
   : mptrLimite(NULL), mswTipo(0), meTipo(SET)
{
   for (int i = 0; i < MAX_TIPOS; i++)
      mswTipos[i] = 0;
}

IteradorTabla::IteradorTabla(void* ptrPrimero)
   : mptrPrimero(ptrPrimero), mptrLimite(NULL), mswTipo(0), meTipo(SET)
{
   for (int i = 0; i < MAX_TIPOS; i++)
      mswTipos[i] = 0;
}

IteradorTabla::~IteradorTabla()
{

}


void IteradorTabla::limpiarZona ()
{
   //Inicializar la memoria a un valor conocido
   memset (mptrPrimero, 8, (long int)mptrLimite - (long int)mptrPrimero);
}


// =================   OPERADORES    ===================

void* IteradorTabla::operator ++ ()
{
   //Comprobar que no hemos iterado hasta el final
   if (this->meTipo == GET && this->mptrActual > this->mptrUltimo)
      return NULL;

   //Comprobar que no nos salimos del banco de memoria
   if (this->meTipo == SET && this->mptrActual > this->mptrLimite)
      throw MemoriaExcepcion (__AQUI__, "Se ha llegado al limite del banco de memoria");

   if (mswTipo > 0)
      this->mptrActual = siguienteRegistro (mswTipo, this->mubTam, this->mptrActual);
   else
      this->mptrActual = ((PSByteType)this->mptrActual) + this->mubTam;

   if (this->meTipo == SET)
      this->mptrUltimo = this->mptrActual;

   return this->mptrActual;
}

void* IteradorTabla::operator -- ()
{
   this->mptrActual = ((PSByteType)this->mptrActual) - this->mubTam;

   return( this->mptrActual );
}


// =================   OPERACIONES   ===================

void* IteradorTabla::getPrimero() const
{
   return( this->mptrPrimero );
}

void IteradorTabla::setPrimero(void* ptrPrimero)
{
   this->mptrPrimero=ptrPrimero;
   if (mswTipo > 0)
   {
      //Mover el iterador hasta la posicion de su primer registro
      this->mptrPrimero = siguienteRegistro (mswTipo, this->mubTam, this->mptrPrimero);
   }
}

void* IteradorTabla::getUltimo() const
{
   return( this->mptrUltimo );
}

bool IteradorTabla::esUltimo() const
{
   return (this->mptrActual == this->mptrUltimo);
}

void IteradorTabla::setUltimo(void* ptrUltimo)
{
   this->mptrUltimo=ptrUltimo;
}

void IteradorTabla::setActual(void* ptrActual)
{
   this->mptrActual=ptrActual;
}

void IteradorTabla::setLimite(void* ptrLimite)
{
   this->mptrLimite=ptrLimite;
}


// ==========================     ACCESO        ===============

//Sets para acceder a las variables principales.
void IteradorTabla::setTam(UWordType uwTam)
{
   this->mubTam=uwTam;
}

void IteradorTabla::setTipo(SWordType swTipo)
{
   this->mswTipo=swTipo;
}

void IteradorTabla::addOtro (SWordType swTipo, SWordType swTam)
{
   SWordType i;

   for (i = 0; mswTipos[0] != 0 && i < MAX_TIPOS; i++);
   if (i >= MAX_TIPOS)
      throw LlenoExcepcion(__AQUI__, "Demasiados tipos de registro");
   mswTipos[i] = swTipo;
   mswTamTipos[i] = swTam;
}

void IteradorTabla::setTipoIterador (enum TipoIterador eTipo)
{
   meTipo = eTipo;
}

enum IteradorTabla::TipoIterador IteradorTabla::getTipoIterador ()
{
   return meTipo;
}

void* IteradorTabla::getActual() const
{
   return (this->mptrActual);
}

//Gets para acceder a las variables principales.
UWordType IteradorTabla::getTam() const
{
   return(this->mubTam);
}

SWordType IteradorTabla::buscarTipo (SWordType swTipo)
{
   SWordType i;

   for (i = 0; mswTipos[0] != swTipo && i < MAX_TIPOS; i++);
   if (i >= MAX_TIPOS)
      return 0;

   return mswTamTipos[i];
}

void* IteradorTabla::siguienteRegistro (SWordType swTipo, SWordType swTam,
   void* psbFrom)
{
   SWordType swRegTipo;
   SWordType pos;
   SWordType inc;

   pos = 0;
   inc = 0;
   do {
      pos += inc;
      swRegTipo = *((SWordType*)&((PSByteType)psbFrom)[pos]);
      inc = buscarTipo (swRegTipo);
   } while (swRegTipo != swTipo && inc > 0);

   return &((PSByteType)psbFrom)[pos];
}


// =======================
//  FICHERO - IteradorTabla.cpp
// =======================
