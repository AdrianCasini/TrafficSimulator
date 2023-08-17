//---------------------------------------------------------------------------
// ControlTiempos.cpp
//---------------------------------------------------------------------------

#include "ControlTiempos.h"

//---------------------------------------------------------------------------
ControlTiempos::ControlTiempos(SDWordType sdwMaxTiempos, bool booSum)
{
   booSuma=booSum;
   if (MAX_TIEMPOS>=sdwMaxTiempos)
   {
      msdwMaxTiempos = sdwMaxTiempos;
      booMax=false;
   }
   else
   {
      msdwMaxTiempos = MAX_TIEMPOS;
      booMax=true;
   }
}


//---------------------------------------------------------------------------
ControlTiempos::~ControlTiempos()
{

}


//---------------------------------------------------------------------------
void ControlTiempos::set(SDWordType mTiempo, Cadena cadCad)
{
   aTiempo[mTiempo].cadTitulo=cadCad;
}


//---------------------------------------------------------------------------
void ControlTiempos::reset(SDWordType mTiempo)
{
   this->On(mTiempo);
   this->Off(mTiempo);
}


//---------------------------------------------------------------------------
void ControlTiempos::On(SDWordType mTiempo)
{
   gettimeofday( &aTiempo[mTiempo].tmTiempoOn , NULL );
   aTiempo[mTiempo].isOn = true;
}


//---------------------------------------------------------------------------
void ControlTiempos::Off(SDWordType mTiempo)
{
   gettimeofday( &aTiempo[mTiempo].tmTiempoOff , NULL );
   aTiempo[mTiempo].isOn = false;
}
//---------------------------------------------------------------------------
bool ControlTiempos::isOn(SDWordType mTiempo)
{
   return aTiempo[mTiempo].isOn;
}

//---------------------------------------------------------------------------
double ControlTiempos::getOn(SDWordType mTiempo)
{
   return( (double) aTiempo[mTiempo].tmTiempoOn.tv_sec
      + ( (double) aTiempo[mTiempo].tmTiempoOn.tv_usec / 1000000.0 ) );
}

//---------------------------------------------------------------------------
double ControlTiempos::getOff(SDWordType mTiempo)
{
   return( (double) aTiempo[mTiempo].tmTiempoOff.tv_sec
      + ( (double) aTiempo[mTiempo].tmTiempoOff.tv_usec / 1000000.0) );
}

//---------------------------------------------------------------------------
double ControlTiempos::get(SDWordType mTiempo)
{
   if ( isOn (mTiempo) ) {
     struct timeval   tmTiempo;   // ver tiempo sin pararlo
     gettimeofday( &tmTiempo , NULL );
     return ((double) tmTiempo.tv_sec + ( (double) tmTiempo.tv_usec / 1000000.0) -
             getOn(mTiempo) );
   }
   else {
     return( getOff(mTiempo) - getOn(mTiempo) );
   }
}


//---------------------------------------------------------------------------
void ControlTiempos::getTiempo(SDWordType mTiempo)
{
   if (aTiempo[mTiempo].cadTitulo.getLong() > 0)
   {
      printf("\t");
      if ( 1 < msdwMaxTiempos )
      {
         printf( "%d.- ", mTiempo );
      }
      printf( " < %02.3f >", get(mTiempo) );
      printf( "  %s", aTiempo[mTiempo].cadTitulo.getCadena());
      //printf( "\tOn:<%02.3f>", getOn() );
      //printf( " Off:<%02.3f>", getOff() );
      printf( "\n");
   }
}


//---------------------------------------------------------------------------
void ControlTiempos::getTiempos()
{
   SDWordType sdwX;

   dSumaTotal=0;
   printf("\n");
   printf("\tControl de tiempos:\n");
   printf("\t===================\n");
   for (sdwX=0; ((sdwX < msdwMaxTiempos)&&(aTiempo[sdwX].cadTitulo.getLong() > 0)) ; sdwX++)
   {
      if ((booSuma)&&(aTiempo[sdwX+1].cadTitulo.getLong() > 0))
      {
         dSumaTotal= dSumaTotal + get(sdwX);
      }
      getTiempo(sdwX);
   }
   if (booSuma)
   {
      printf("\t");
      printf( "     < %02.3f >", dSumaTotal );
      printf( "  %s", "Suma total\n");
   }
   printf("\t===================\n");
   if (booMax)
   {
      printf("\tNo se pueden mostrar mas tiempos. Maximo <%d>.\n", MAX_TIEMPOS);
      printf("\t===================\n");
   }

}



//---------------------------------------------------------------------------
// ControlTiempos.cpp
//---------------------------------------------------------------------------
