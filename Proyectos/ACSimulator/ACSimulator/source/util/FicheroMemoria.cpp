// =======================
//  FicheroMemoria.cpp
// =======================

#include "FicheroMemoria.h"
#include <sys/mman.h>
#include <unistd.h>


AutoMutex FicheroMemoria::mThMutex;

//------------------------------------------------------------------------------
FicheroMemoria::FicheroMemoria()
   : mpMemoria(NULL), mudwTamanyo(0)
{
   this->mumask = 0;
   this->mmask = 0644;
   this->bLectura = true;
   this->bEscritura = false;
   this->bExclusivo = false;
}

//------------------------------------------------------------------------------
FicheroMemoria::~FicheroMemoria()
{
   if (isAbierto ())
      cerrar ();
}

//------------------------------------------------------------------------------
void FicheroMemoria::abrir ()
{
   Fichero::abrir ();
   this->mudwTamanyo = Fichero::getTamano ();
   mapear ();
}

//------------------------------------------------------------------------------
void FicheroMemoria::crear (UDWordType udwTamanyo)
{
   Fichero::crear ();

   SByteType sbAux[udwTamanyo];
   memset (sbAux, 0, udwTamanyo);
   Fichero::escribir (0, sbAux, udwTamanyo);
   Fichero::sincronizar ();

   this->mudwTamanyo = udwTamanyo;
   mapear ();
}

//------------------------------------------------------------------------------
void FicheroMemoria::cerrar ()
{
   sincronizar ();
   desmapear ();
   Fichero::cerrar ();
}

//------------------------------------------------------------------------------
void FicheroMemoria::setNombre (const Cadena& cadNombre)
{
   Fichero::setNombre (cadNombre);
}

//------------------------------------------------------------------------------
void FicheroMemoria::setLectura (bool bLectura)
{
   Fichero::setLectura (bLectura);
}

//------------------------------------------------------------------------------
void FicheroMemoria::setEscritura (bool bEscritura)
{
   Fichero::setEscritura (bEscritura);
}

//------------------------------------------------------------------------------
void FicheroMemoria::setExclusivo (bool bExclusivo)
{
   Fichero::setExclusivo (bExclusivo);
}

//------------------------------------------------------------------------------
void FicheroMemoria::setSincronizado (bool bSincronizado)
{
   Fichero::setSincronizado (bSincronizado);
}

//------------------------------------------------------------------------------
Cadena FicheroMemoria::getNombre () const
{
   return this->mNombre;
}

//------------------------------------------------------------------------------
UDWordType FicheroMemoria::getTamano () const
{
   return this->mudwTamanyo;
}

//------------------------------------------------------------------------------
void* FicheroMemoria::getPunteroMemoria () const
{
   return this->mpMemoria;
}

//------------------------------------------------------------------------------
bool FicheroMemoria::isAbierto () const
{
   return (this->fd >= 0);
}

bool FicheroMemoria::isFinal (UDWordType udwOffset) const
{
   return (udwOffset >= this->mudwTamanyo);
}

//------------------------------------------------------------------------------
void FicheroMemoria::lock () const
{
   this->mThMutex.lock ();
   lseek (this->fd, 0, SEEK_SET);
   lockf (this->fd, F_LOCK, 0);
}

//------------------------------------------------------------------------------
void FicheroMemoria::unlock () const
{
   lseek (this->fd, 0, SEEK_SET);
   lockf (this->fd, F_ULOCK, 0);
   this->mThMutex.unlock ();
}

//------------------------------------------------------------------------------
UDWordType FicheroMemoria::leer (UDWordType udwOffset, void *buf,
   UDWordType udwCount)
{
   if (this->mpMemoria != NULL)
   {
      memcpy (buf, ((char*)this->mpMemoria) + udwOffset, udwCount);
      return udwCount;
   }
   else
   {
      return 0;
   }
}

//------------------------------------------------------------------------------

SDWordType FicheroMemoria::leerLinea (UDWordType udwOffset, void *buf,
                                      UDWordType udwCount)
{
  char * nl, * bp;              // nl = pos of '\n', bp = puntero al inicio
  UDWordType lim;

  if (!this->mpMemoria && udwOffset > this->mudwTamanyo)
    return 0;

  lim = udwOffset + udwCount;
  if (lim > this->mudwTamanyo)      // restamos los bytes que nos salimos del
    udwCount -= lim - this->mudwTamanyo; // buffer

  bp = (char *) this->mpMemoria + udwOffset;
  nl = (char *) memchr(bp, '\n', udwCount);
  if (nl)
    udwCount = nl - bp;         // Copiamos solo hasta el \n (sin incluirlo)

  memcpy(buf, bp, udwCount);
  ((char *) buf)[udwCount] = '\0';

  return (nl == NULL) ? udwCount : udwCount + 1; // Hay que contar el '\n'
}


//------------------------------------------------------------------------------
UDWordType FicheroMemoria::escribir (UDWordType udwOffset, const void *buf,
   UDWordType udwCount)
{
   if (this->mpMemoria != NULL)
   {
      memcpy (((char*)this->mpMemoria) + udwOffset, buf, udwCount);
      return udwCount;
   }
   else
   {
      return 0;
   }
}

//------------------------------------------------------------------------------
UDWordType FicheroMemoria::escribirLinea (UDWordType udwOffset, const void *buf,
   UDWordType udwCount)
{
   if (this->mpMemoria != NULL && ((udwOffset + udwCount + 1) < this->mudwTamanyo))
   {
      memcpy (((char*)this->mpMemoria) + udwOffset, buf, udwCount);
      *(((char*)this->mpMemoria) + udwOffset + udwCount) = '\n';
      return udwCount + 1;
   }
   else
   {
      return 0;
   }
}

//------------------------------------------------------------------------------
void FicheroMemoria::sincronizar ()
{
   msync ((char*)this->mpMemoria, this->mudwTamanyo, MS_INVALIDATE |
      (this->bSincronizado ? MS_SYNC : MS_ASYNC));
}

//------------------------------------------------------------------------------
void FicheroMemoria::reset (UDWordType udwTamanyo)
{
   this->mudwTamanyo = udwTamanyo;
   ftruncate (this->fd, udwTamanyo);
   desmapear ();
   mapear ();
   memset (this->mpMemoria, 0, udwTamanyo);
   sincronizar ();
}


//------------------------------------------------------------------------------
void FicheroMemoria::mapear ()
{
   this->mpMemoria = mmap (NULL, this->mudwTamanyo,
      (this->bLectura ? PROT_READ : 0) | (this->bEscritura ? PROT_WRITE : 0),
      (this->bExclusivo ? MAP_PRIVATE : MAP_SHARED), this->fd, 0);

   if (this->mpMemoria == MAP_FAILED)
      throw MemoriaExcepcion(__AQUI__, "No se ha podido mapear el fichero");
}

//------------------------------------------------------------------------------
void FicheroMemoria::desmapear ()
{
   munmap ((char*)this->mpMemoria, this->mudwTamanyo);
}
