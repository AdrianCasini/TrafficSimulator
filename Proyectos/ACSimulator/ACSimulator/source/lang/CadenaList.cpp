// =======================
//  FICHERO - CadenaList.cpp
// =======================

#include "CadenaList.h"  // clase implementada

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
CadenaList::CadenaList()
{
 this->muwNumItems = 0;
 this->mSeparador  = SEPARACION_DEF;
 this->muwMaxLong = 0;
}

CadenaList::CadenaList(const Cadena& lValores,const Cadena& lSeparacion)
{
  this->muwNumItems = 0;
  this->mSeparador  = SEPARACION_DEF;
  this->muwMaxLong = 0;

  mSeparador = lSeparacion;
  inicializar(lValores);
}

CadenaList::~CadenaList()
{

}
// =================   OPERADORES   ===================
Cadena CadenaList::operator [] (UWordType luwNum) const
{
   return(getValor(luwNum));
}
// =================   OPERACIONES   ===================
void CadenaList::borrar()
{
   muwNumItems    = 0;
   mValor.setEmpty();
   mSeparador.setEmpty();
}

void CadenaList::setSeparador(const Cadena& lSeparacion)
{
   mSeparador = lSeparacion;
}

void CadenaList::add(const Cadena& lValor)
{
   if(muwNumItems > 0)
    mValor += mSeparador + lValor;
   else
    mValor = lValor;
   muwNumItems ++;
   if(lValor.getLong() > muwMaxLong )
    muwMaxLong = lValor.getLong();
}

/*void CadenaList::inicializar(const Cadena& lValor)
{
   SWordType lswPos;
   UWordType luwIni;

   mValor = lValor;

   #warning error al parsear cadenas con mSeparador contiguos, ya que primerIndiceDe retorna la posicion encontrada+1
   //fixme error al parsear cadenas con mSeparador contiguos, ya que primerIndiceDe retorna la posicion encontrada+1
   //ex. VALOR1;VALOR2;;VALOR3;;;
   //No calculara correctamente el muwNumItems.
   muwNumItems = 0;
   if(mValor.getLong() > 0)
   {
      muwNumItems = 1;
      lswPos = mValor.primerIndiceDe ( mSeparador , 0);
      luwIni = 1;
      while( -1 !=  lswPos)
      {
         muwNumItems++;
         if( (lswPos - luwIni ) > muwMaxLong )
         {
            muwMaxLong = (lswPos - luwIni );
         }
         luwIni = lswPos;
         lswPos = mValor.primerIndiceDe ( mSeparador , lswPos + mSeparador.getLong());
      }
   }
}*/


void CadenaList::inicializar(const Cadena& lValor)
{
   SWordType lswPos;
   UWordType luwIni;

   mValor = lValor;
   muwNumItems = 0;
   if(mValor.getLong() > 0)
   {
      lswPos = mValor.primerIndiceDe ( mSeparador , 0);
        muwNumItems = 1; // No hay mas datos solo 1
      luwIni = 1;
      while( (-1 !=  lswPos) && (lswPos != lValor.getLong()) )
      {
         muwNumItems++;
         if( (lswPos - luwIni ) > muwMaxLong )
         {
            muwMaxLong = (lswPos - luwIni );
         }
         luwIni = lswPos;
         lswPos = mValor.primerIndiceDe ( mSeparador , lswPos + mSeparador.getLong());
      }
   }
}









// =================   ACCESO       ====================
// =================   CONSULTA      ===================
UWordType CadenaList::getNumItems() const
{
   return(muwNumItems);
}

Cadena CadenaList::getValor(const UWordType luwNum) const
{
   Cadena    lTemp;
   SWordType lswPosFinal;
   SWordType lswPosInicial;
   UWordType luwNumParam;

   if( luwNum < muwNumItems )
   {
      // Averiguamos en que posicion esta la cadena
      lswPosInicial = 0;
      lswPosFinal = mValor.primerIndiceDe ( mSeparador , 0);
      luwNumParam = 0;
      while( ( -1 != lswPosFinal) && (luwNumParam <  luwNum) )
      {
         luwNumParam++;
         lswPosInicial = lswPosFinal;
         lswPosFinal = mValor.primerIndiceDe ( mSeparador , lswPosInicial );
      }
      // Miramos los resultados
      if ( luwNum == luwNumParam  )
      {
         if( -1 != lswPosFinal)
          lTemp = mValor.subCadena(lswPosInicial + 1,lswPosFinal - 1 );
         else
          lTemp = mValor.subCadena(lswPosInicial + 1,mValor.getLong());
         return ( lTemp );
      }
   }
   throw RangoExcepcion(__AQUI__,"No existe la cadena");
}

Cadena CadenaList::getValor() const
{
   return(mValor);
}

Cadena CadenaList::getSeparador() const
{
   return(mSeparador);
}

Cadena CadenaList::getSiguiente(const Cadena& lValor) const
{
   SWordType lswPosInicio;
   SWordType lswPosFinal;
   Cadena    lTemp;

   if(getNumItems() > 0)
   {
      // Por defecto el primer valor
      lTemp = getValor(0);
      // Calculamos su inicio
      lswPosInicio = mValor.primerIndiceDe (lValor);
      if( -1 != lswPosInicio)
      {
         lswPosInicio +=  lValor.getLong() + mSeparador.getLong();
         // Calculamos su final
         lswPosFinal = mValor.primerIndiceDe (mSeparador,lswPosInicio );
         if( -1 != lswPosFinal )
         {
            lTemp = mValor.subCadena(lswPosInicio,lswPosFinal - 1);
         }
         else
         {
            if( (UWordType)lswPosInicio < mValor.getLong() )
            {
               lTemp = mValor.subCadena(lswPosInicio,mValor.getLong());
            }
         }
      }
   }
   return(lTemp);
}

Cadena CadenaList::getAnterior(const Cadena& lValor) const
{
  SWordType lswPosInicio;
  SWordType lswPosFinal;
  Cadena    lTemp;

   if(getNumItems() > 0)
   {
     // El valor por defecto es el ultimo
     lTemp = getValor(getNumItems() - 1);
     // Calculamos su inicio
     lswPosInicio = mValor.ultimoIndiceDe (lValor,mValor.getLong());
     if( -1 != lswPosInicio)
     {
        // Calculamos su final
        lswPosFinal = mValor.ultimoIndiceDe (mSeparador,lswPosInicio - 1);
        if( -1 != lswPosFinal )
        {
           lTemp = mValor.subCadena(lswPosFinal + 1,lswPosInicio - 2);
        }
        else
        {
           if( lswPosInicio > 1 )
           {
              lTemp = mValor.subCadena(1,lswPosInicio - 2);
           }
        }
     }
  }
  return(lTemp);
}


bool  CadenaList::valorCorrecto(const Cadena& lValor) const
{
   if(-1 != mValor.primerIndiceDe (lValor) )
    return(true);
   else
    return(false);
}

UWordType CadenaList::getMaximaLong() const
{
   return(muwMaxLong);
}

// =======================
//  FIN DEL FICHERO - CadenaList.cpp
// =======================



