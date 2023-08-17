// =======================
//  FICHERO - CharEnum.cpp
// =======================

#include "CharEnum.h"  // clase implementada

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


//---------------------------------------------------------------------------
CharEnum::~CharEnum()
{
}


//---------------------------------------------------------------------------
// Iniciliza los campos. Los textos con el caracter pasado por parametro.
CharEnum::CharEnum(char Car)
{
  SDWordType sdX;

  for (sdX=0 ; sdX < MAX_LONG ; sdX++)
  {
    aGeneral[sdX].cadCad=Cadena(Car);
    aGeneral[sdX].sdNum=-1;
  }
}


// =================   OPERADORES    ===================


// =================   ACCESO       ====================

//---------------------------------------------------------------------------
// Anade una cadena en el primer espacio vacio que encuentra
//  y le asocia un indice pasado por parametro.
void CharEnum::set(UDWordType udIndex, Cadena cadCadena)
{
  SDWordType sdX;

  for (sdX=0 ; sdX < MAX_LONG ; sdX++)
  {
    if ((*this).aGeneral[sdX].sdNum==-1)
    {
      (*this).aGeneral[sdX].sdNum=udIndex;
      (*this).aGeneral[sdX].cadCad=cadCadena;
      break;
    }
  }
}



// =================   CONSULTA      ===================


//---------------------------------------------------------------------------
// Encuentra y devuelve una cadena correspondiente al indice relacionado.
Cadena CharEnum::get(UDWordType udIndex)
{
  SDWordType sdX;

  for (sdX=0 ; sdX < MAX_LONG ; sdX++)
  {
    if ((*this).aGeneral[sdX].sdNum==(SDWordType)udIndex)
    {
      return((*this).aGeneral[sdX].cadCad);
    }
  }
  return(Cadena(""));
}


//---------------------------------------------------------------------------
// Encuentra y devuelve el indice correcpondiente al texto relacionado.
SDWordType CharEnum::get(Cadena cadCadena)
{
  SDWordType sdX;

  for (sdX=0 ; sdX < MAX_LONG ; sdX++)
  {
    if ((strcmp((*this).aGeneral[sdX].cadCad.getCadena(),cadCadena.getCadena()))==0)
    {
      return( (UDWordType) ((*this).aGeneral[sdX].sdNum) );
    }
  }
  return(-1);
}



// =======================
//  FIN DEL FICHERO - CharEnum.cpp
// =======================
