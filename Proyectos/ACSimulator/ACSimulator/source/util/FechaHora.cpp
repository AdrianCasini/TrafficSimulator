
#include "FechaHora.h"
#include <limits.h>


const char* FechaHora::FORMATOS[NUM_FORMATOS] =
{
   "%02d/%02d/%02d %02d:%02d:%02d,%03d", //DD_MM_AA_HH_MM_SS_MMM
   "%04d%02d%02d%02d%02d%02d",           //AAAAMMDDHHMMSS
   "%04d-%02d-%02d %02d:%02d:%02d",      //AAAA_MM_DD_HH_MM_SS
   "%02d/%02d/%04d %02d:%02d:%02d",      //DD_MM_AAAA_HH_MM_SS
   "%03d %02d:%02d",                     //DDD_HH_MM
   "%03d%02d%02d",                       //DDDHHMM
   "%04d-%02d-%02d",                     //AAAA_MM_DD
   "%04d%02d%02d",                       //AAAAMMDD
   "%02d%02d%02d",                       //AAMMDD
   "%02d/%02d/%04d",                     //DD_MM_AAAA
   "%02d/%02d/%02d",                     //DD_MM_AA
   "%02d%02d",                           //MMAA
   "%02d:%02d:%02d",                     //HH_MM_SS
   "%02d%02d%02d",                       //HHMMSS
   "%02d:%02d",                          //HH_MM
   "%02d%02d"                            //HHMM
};


//------------------------------------------------------------------------------
FechaHora::FechaHora (time_t segundos, suseconds_t microSegundos)
{
   tzset ();
   this->mSegundos.tv_sec = segundos;

   this->m
   this->mSegundos.tv_usec = microSegundos;
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
FechaHora::FechaHora (int anyo, int mes, int dia, int hora, int minuto, int segundo)
{
   tzset ();
   resetTiempo ();
   this->mTiempo.tm_year = (anyo - ANYO_INICIAL);
   this->mTiempo.tm_mon = (mes - 1);
   this->mTiempo.tm_mday = dia;
   this->mTiempo.tm_hour = hora;
   this->mTiempo.tm_min = minuto;
   this->mTiempo.tm_sec = segundo;
   mktime (&this->mTiempo);
   actualizarSegundos ();
}

//------------------------------------------------------------------------------
FechaHora::FechaHora (const struct timeval& t)
   : mSegundos(t)
{
   tzset ();
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
FechaHora::FechaHora (const Cadena& str, enum Formato formato)
{
   tzset ();
   setFechaHora (str, formato);
}

//------------------------------------------------------------------------------
FechaHora::FechaHora (const FechaHora& f)
   : mSegundos(f.mSegundos), mTiempo(f.mTiempo), mValida(f.mValida)
{

}

//------------------------------------------------------------------------------
FechaHora::~FechaHora ()
{

}

//------------------------------------------------------------------------------
FechaHora& FechaHora::operator = (const FechaHora& f)
{
   this->mSegundos.tv_sec = f.mSegundos.tv_sec;
   this->mSegundos.tv_usec = f.mSegundos.tv_usec;
   memcpy (&this->mTiempo, &f.mTiempo, sizeof (this->mTiempo));
   this->mValida = f.mValida;
   return *this;
}

//------------------------------------------------------------------------------
FechaHora& FechaHora::operator = (time_t t)
{
   this->mSegundos.tv_sec = t;
   this->mSegundos.tv_usec = 0;
   actualizarTiempo ();
   return *this;
}

//------------------------------------------------------------------------------
FechaHora FechaHora::operator + (const FechaHora& f)
{
   FechaHora res(*this);
   res += f;
   return res;
}

//------------------------------------------------------------------------------
FechaHora FechaHora::operator - (const FechaHora& f)
{
   FechaHora res(*this);
   res -= f;
   return res;
}

//------------------------------------------------------------------------------
FechaHora FechaHora::operator + (int t)
{
   FechaHora res(*this);
   res += t;
   return res;
}

//------------------------------------------------------------------------------
FechaHora FechaHora::operator - (int t)
{
   FechaHora res(*this);
   res -= t;
   return res;
}

//------------------------------------------------------------------------------
void FechaHora::operator += (const FechaHora& f)
{
   this->mSegundos.tv_sec += f.mSegundos.tv_sec;
   this->mSegundos.tv_usec += f.mSegundos.tv_usec;
   redondearSegundos ();
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
void FechaHora::operator -= (const FechaHora& f)
{
   if (this->mSegundos.tv_usec < f.mSegundos.tv_usec)
   {
      this->mSegundos.tv_sec--;
      this->mSegundos.tv_usec += 1000000;
   }
   this->mSegundos.tv_sec -= f.mSegundos.tv_sec;
   this->mSegundos.tv_usec -= f.mSegundos.tv_usec;
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
void FechaHora::operator += (int t)
{
   this->mSegundos.tv_sec += t;
   redondearSegundos ();
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
void FechaHora::operator -= (int t)
{
   this->mSegundos.tv_sec -= t;
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
bool FechaHora::operator == (const FechaHora& f) const
{
   return (this->mSegundos.tv_sec == f.mSegundos.tv_sec &&
      this->mSegundos.tv_usec == f.mSegundos.tv_usec);
}

//------------------------------------------------------------------------------
bool FechaHora::operator != (const FechaHora& f) const
{
   return !(operator == (f));
}

//------------------------------------------------------------------------------
bool FechaHora::operator > (const FechaHora& f) const
{
   return (this->mSegundos.tv_sec > f.mSegundos.tv_sec || (
      this->mSegundos.tv_sec == f.mSegundos.tv_sec &&
      this->mSegundos.tv_usec > f.mSegundos.tv_usec)
   );
}

//------------------------------------------------------------------------------
bool FechaHora::operator >= (const FechaHora& f) const
{
   return (operator > (f) || operator == (f));
}

//------------------------------------------------------------------------------
bool FechaHora::operator < (const FechaHora& f) const
{
   return (this->mSegundos.tv_sec < f.mSegundos.tv_sec || (
      this->mSegundos.tv_sec == f.mSegundos.tv_sec &&
      this->mSegundos.tv_usec < f.mSegundos.tv_usec)
   );
}

//------------------------------------------------------------------------------
bool FechaHora::operator <= (const FechaHora& f) const
{
   return (operator < (f) || operator == (f));
}

//------------------------------------------------------------------------------
FechaHora::operator time_t () const
{
   return getSegundos1970 ();
}

//------------------------------------------------------------------------------
FechaHora::operator const struct timeval& () const
{
   return this->mSegundos;
}

//------------------------------------------------------------------------------
FechaHora::operator Cadena () const
{
   return getCadena (FECHA_HORA);
}

//------------------------------------------------------------------------------
void FechaHora::setSegundos1970 (time_t segundos)
{
   this->mSegundos.tv_sec = segundos;
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
time_t FechaHora::getSegundos1970 () const
{
   return (this->mValida ? this->mSegundos.tv_sec : -1);
}

//------------------------------------------------------------------------------
time_t FechaHora::getTiempoEnSegundos () const
{
   return (this->mValida ? this->mSegundos.tv_sec : 0);
}

//------------------------------------------------------------------------------
void FechaHora::borrarHora ()
{
   this->mTiempo.tm_hour = 0;
   this->mTiempo.tm_min = 0;
   this->mTiempo.tm_sec = 0;
   actualizarSegundos ();
}

//------------------------------------------------------------------------------
void FechaHora::borrarFecha ()
{
   tzset ();
   this->mSegundos.tv_sec = (
      (this->mTiempo.tm_hour * HORAS) - getDiferenciaUTC() +
      this->mTiempo.tm_min * MINUTOS +
      this->mTiempo.tm_sec * SEGUNDOS
   );

   this->mSegundos.tv_usec = 0;
   actualizarTiempo ();
}

//------------------------------------------------------------------------------
int FechaHora::getAnyo () const
{
   return (this->mTiempo.tm_year + ANYO_INICIAL);
}

//------------------------------------------------------------------------------
int FechaHora::getMes () const
{
   return (this->mTiempo.tm_mon + 1);
}

//------------------------------------------------------------------------------
int FechaHora::getDia () const
{
   return this->mTiempo.tm_mday;
}

//------------------------------------------------------------------------------
enum FechaHora::DiaSemana FechaHora::getDiaSemana () const
{
   return (enum DiaSemana)this->mTiempo.tm_wday;
}

//------------------------------------------------------------------------------
int FechaHora::getDiaJuliano () const
{
   return (this->mTiempo.tm_yday + 1);
}

//------------------------------------------------------------------------------
int FechaHora::getHora () const
{
   return this->mTiempo.tm_hour;
}

//------------------------------------------------------------------------------
int FechaHora::getMinuto () const
{
   return this->mTiempo.tm_min;
}

//------------------------------------------------------------------------------
int FechaHora::getSegundo () const
{
   return this->mTiempo.tm_sec;
}

//------------------------------------------------------------------------------
int FechaHora::getMilisegundos () const
{
   int milis;

   //Redondeamos los milisegundos excepto si estamos en el 999, pues entonces
   //podria ser 1000 y estariamos en el segundo siguiente
   milis = (this->mSegundos.tv_usec / 1000 +
      ((this->mSegundos.tv_usec % 1000) >= 500 ? 1 : 0));

   return (milis <= 999 ? milis : 999);
}

//------------------------------------------------------------------------------
int FechaHora::getMicrosegundos () const
{
   return this->mSegundos.tv_usec;
}

//------------------------------------------------------------------------------
int64_t FechaHora::getTiempoEnMilisegundos () const
{
   int64_t t;

   t = this->mSegundos.tv_sec;
   t *= 1000;
   t += (this->mSegundos.tv_usec / 1000);

   return t;
}

//------------------------------------------------------------------------------
Cadena FechaHora::getZonaHoraria () const
{
   Cadena str;

   str.formCadena ("%s (UTC %+d)", tzname[hayHorarioDeVerano () ? 1 : 0],
      getDiferenciaUTC () / 3600);

   return str;
}

//------------------------------------------------------------------------------
time_t FechaHora::getDiferenciaUTC () const
{
   time_t diff;
#if defined (ab_linux) || defined (ab_sparc)
   diff = -timezone;
#else
   diff = -altzone;
#endif
   return (hayHorarioDeVerano () ? diff : diff - HORAS);
}

//------------------------------------------------------------------------------
bool FechaHora::hayHorarioDeVerano () const
{
   return (this->mTiempo.tm_isdst != 0);
}

//------------------------------------------------------------------------------
bool FechaHora::esAnyoBisiesto () const
{
   return bisiesto (getAnyo ());
}

//------------------------------------------------------------------------------
void FechaHora::sumar (int dias, int horas, int minutos, int segundos)
{
   this->mTiempo.tm_mday += dias;
   this->mTiempo.tm_hour += horas;
   this->mTiempo.tm_min  += minutos;
   this->mTiempo.tm_sec  += segundos;

   actualizarSegundos ();
}

//------------------------------------------------------------------------------
void FechaHora::restar (int dias, int horas, int minutos, int segundos)
{
   sumar (-dias, -horas, -minutos, -segundos);
}

//------------------------------------------------------------------------------
void FechaHora::sumar_ms (int dias, int horas, int minutos, int segundos)
{
   this->mTiempo.tm_mday += dias;
   this->mTiempo.tm_hour += horas;
   this->mTiempo.tm_min  += minutos;
   this->mTiempo.tm_sec  += segundos;

   time_t difAfter, difBefore = getDiferenciaUTC ();

   this->mSegundos.tv_sec = mktime (&this->mTiempo);

   difAfter = getDiferenciaUTC ();
   this->mSegundos.tv_sec -= (difAfter-difBefore);
   //this->mSegundos.tv_usec = 0;
   this->mValida = (this->mSegundos.tv_sec >= 0);

   resetTiempo ();
   this->mValida = (localtime_r (&this->mSegundos.tv_sec, &this->mTiempo) != NULL);
}
//------------------------------------------------------------------------------
void FechaHora::restar_ms (int dias, int horas, int minutos, int segundos)
{
   sumar_ms (-dias, -horas, -minutos, -segundos);
}

//------------------------------------------------------------------------------
void FechaHora::setVacia ()
{
   resetTiempo ();
   this->mSegundos.tv_sec = 0;
   this->mSegundos.tv_usec = 0;
   actualizarTiempo ();
   this->mValida = true;
}

//------------------------------------------------------------------------------
bool FechaHora::esVacia () const
{
   return (this->mSegundos.tv_sec == 0 && this->mSegundos.tv_usec == 0);
}

//------------------------------------------------------------------------------
bool FechaHora::esValida () const
{
   return this->mValida;
}

//------------------------------------------------------------------------------
bool FechaHora::esMismoDia (const FechaHora& f) const
{
   return (this->mTiempo.tm_year == f.mTiempo.tm_year &&
      this->mTiempo.tm_mon == f.mTiempo.tm_mon &&
      this->mTiempo.tm_mday == f.mTiempo.tm_mday);
}

//------------------------------------------------------------------------------
void FechaHora::setFecha (int anyo, int mes, int dia)
{
   this->mTiempo.tm_year = (anyo - ANYO_INICIAL);
   this->mTiempo.tm_mon = (mes - 1);
   this->mTiempo.tm_mday = dia;
   actualizarSegundos ();
}

//------------------------------------------------------------------------------
void FechaHora::setHora (int hora, int minuto, int segundo)
{
   this->mTiempo.tm_hour = hora;
   this->mTiempo.tm_min = minuto;
   this->mTiempo.tm_sec = segundo;
   actualizarSegundos ();
}

//------------------------------------------------------------------------------
void FechaHora::setFechaHora (int anyo, int mes, int dia, int hora, int minuto,
   int segundo, int milis)
{
   resetTiempo ();
   this->mTiempo.tm_year = (anyo - ANYO_INICIAL);
   this->mTiempo.tm_mon = (mes - 1);
   this->mTiempo.tm_mday = dia;
   this->mTiempo.tm_hour = hora;
   this->mTiempo.tm_min = minuto;
   this->mTiempo.tm_sec = segundo;
   mktime ( &this->mTiempo );
   actualizarSegundos ();
   this->mSegundos.tv_usec = (milis * 1000);

}

//------------------------------------------------------------------------------
void FechaHora::setFechaHora (const Cadena& str, enum Formato formato)
{
   int anyo, mes, dia, hora, minuto, segundo, milis;

   if (str.isEmpty ())
   {
      setVacia ();
   }
   else
   {
      parse (str, formato, &anyo, &mes, &dia, &hora, &minuto, &segundo, &milis);
      if (anyo < 100) //Anyo con 2 digitos
         anyo += 2000;

      if (formato == DDD_HH_MM || formato == DDDHHMM) //Momento juliano
      {
         FechaHora ahora = hoy ();
         setMomentoJuliano (
            dia > ahora.getDiaJuliano () ? ahora.getAnyo () - 1 : ahora.getAnyo (),
            dia, hora, minuto);
      }
      else
      {
         setFechaHora (anyo, mes, dia, hora, minuto, segundo, milis);
      }
   }
}

//------------------------------------------------------------------------------
void FechaHora::setMomentoJuliano (int anyo, int dia, int hora, int minuto)
{
   resetTiempo ();
   this->mTiempo.tm_year = (anyo - ANYO_INICIAL);
   this->mTiempo.tm_mday = dia;
   this->mTiempo.tm_mon = 0;
   this->mTiempo.tm_hour = hora;
   this->mTiempo.tm_min = minuto;
   mktime ( &this->mTiempo );
   actualizarSegundos ();
}

//------------------------------------------------------------------------------
Cadena FechaHora::getCadena (enum Formato formato) const
{
   Cadena str;

   if (esVacia ())
      return "";
   else if (!esValida ())
      return "****";

   const char* strformato = getFormato (formato);
   switch (formato)
   {
      case DD_MM_AA_HH_MM_SS_MMM:
         str.formCadena (strformato, getDia (), getMes (), getAnyo () % 100,
            getHora (), getMinuto (), getSegundo (), getMilisegundos ());
         break;

      case AAAA_MM_DD_HH_MM_SS:
      case AAAAMMDDHHMMSS:
         str.formCadena (strformato, getAnyo (), getMes (), getDia (), getHora (),
            getMinuto (), getSegundo ());
         break;

      case DD_MM_AAAA_HH_MM_SS:
         str.formCadena (strformato, getDia (), getMes (), getAnyo (), getHora (),
            getMinuto (), getSegundo ());
         break;

      case DDD_HH_MM:
      case DDDHHMM:
         str.formCadena (strformato, getDiaJuliano (), getHora (), getMinuto ());
         break;

      case AAAAMMDD:
      case AAAA_MM_DD:
         str.formCadena (strformato, getAnyo (), getMes (), getDia ());
         break;

      case AAMMDD:
         str.formCadena (strformato, getAnyo () % 100, getMes (), getDia ());
         break;

      case DD_MM_AAAA:
         str.formCadena (strformato, getDia (), getMes (), getAnyo ());
         break;

      case DD_MM_AA:
         str.formCadena (strformato, getDia (), getMes (), getAnyo () % 100);
         break;

      case MMAA:
         str.formCadena (strformato, getMes (), getAnyo () % 100);
         break;

      case HH_MM_SS:
      case HHMMSS:
      case HH_MM:
      case HHMM:
         str.formCadena (strformato, getHora (), getMinuto (), getSegundo ());
         break;

      case NUM_FORMATOS: break;
   }

   return str;
}

//------------------------------------------------------------------------------
bool FechaHora::validarMomento (const Cadena& momento, enum Formato formato)
{
   int  anyo, mes, dia, hora, minuto, segundo, milis;

   parse (momento, formato, &anyo, &mes, &dia, &hora, &minuto, &segundo, &milis);
   switch (formato)
   {
      case DD_MM_AA_HH_MM_SS_MMM:
         return (checkFecha2 (anyo, mes, dia) && checkHora (hora, minuto, segundo) &&
            milis >= 0 && milis <= 999);

      case DD_MM_AAAA_HH_MM_SS:
      case AAAA_MM_DD_HH_MM_SS:
      case AAAAMMDDHHMMSS:
         return (checkFecha4 (anyo, mes, dia) && checkHora (hora, minuto, segundo));

      case DDD_HH_MM:
      case DDDHHMM:
         return (dia >= 1 && dia <= 366 && checkHora (hora, minuto, 0));

      case DD_MM_AAAA:
      case AAAAMMDD:
      case AAAA_MM_DD:
         return checkFecha4 (anyo, mes, dia);

      case DD_MM_AA:
      case AAMMDD:
         return checkFecha2 (anyo, mes, dia);

      case MMAA:
         return (anyo >= 0 && anyo <= 99 && mes >= 1 && mes <= 12);

      case HH_MM_SS:
      case HHMMSS:
      case HH_MM:
      case HHMM:
         return checkHora (hora, minuto, segundo);

      case NUM_FORMATOS: break;
   }

   return false;
}

//------------------------------------------------------------------------------
void FechaHora::getAhora (bool conMicrosegundos)
{
   operator = (ahora (conMicrosegundos));
}

//------------------------------------------------------------------------------
FechaHora FechaHora::ahora (bool conMicrosegundos)
{
   struct timeval tv;
   gettimeofday (&tv, NULL);
   return FechaHora(tv.tv_sec, conMicrosegundos ? tv.tv_usec : 0);
}

//------------------------------------------------------------------------------
FechaHora FechaHora::hoy ()
{
   FechaHora hoy;
   hoy = ahora ();
   hoy.borrarHora ();
   return hoy;
}

//------------------------------------------------------------------------------
FechaHora FechaHora::ayer ()
{
   FechaHora _hoy = hoy ();
   FechaHora _ayer = (_hoy - (1 * DIAS));
   _ayer -= (_ayer.getDiferenciaUTC() - _hoy.getDiferenciaUTC());
   return _ayer;
}

//------------------------------------------------------------------------------
FechaHora FechaHora::manyana ()
{
   return (hoy () + (1 * DIAS));
}

//------------------------------------------------------------------------------
int FechaHora::getUltimoDiaMes (int anyo, int mes)
{
   switch (mes)
   {
      //Meses de 31 dias
      case ENERO:
      case MARZO:
      case MAYO:
      case JULIO:
      case AGOSTO:
      case OCTUBRE:
      case DICIEMBRE:
         return 31;

      //Meses de 30 dias
      case ABRIL:
      case JUNIO:
      case SEPTIEMBRE:
      case NOVIEMBRE:
         return 30;

      //Febrero
      case FEBRERO:
         return (bisiesto (anyo) ? 29 : 28);
   }

   return 0;
}

//------------------------------------------------------------------------------
bool FechaHora::esAnyoBisiesto (int anyo)
{
   return bisiesto (anyo);
}
//------------------------------------------------------------------------------
void FechaHora::actualizarSegundos ()
{
time_t difAfter, difBefore = getDiferenciaUTC ();

   this->mSegundos.tv_sec = mktime (&this->mTiempo);

   difAfter = getDiferenciaUTC ();
   this->mSegundos.tv_sec -= (difAfter-difBefore);
   this->mSegundos.tv_usec = 0;
   this->mValida = (this->mSegundos.tv_sec >= 0);
   actualizarTiempo ();
}
//------------------------------------------------------------------------------
void FechaHora::actualizarTiempo ()
{
   resetTiempo ();
   this->mValida = (localtime_r (&this->mSegundos.tv_sec, &this->mTiempo) != NULL);
}
//------------------------------------------------------------------------------
void FechaHora::resetTiempo ()
{
   memset (&this->mTiempo, 0, sizeof (this->mTiempo));
   this->mTiempo.tm_isdst = -1; //Unknown
}
//------------------------------------------------------------------------------
void FechaHora::redondearSegundos ()
{
   if (this->mSegundos.tv_usec > 1000000)
   {
      this->mSegundos.tv_sec += (this->mSegundos.tv_usec / 1000000);
      this->mSegundos.tv_usec %= 1000000;
   }
}

//------------------------------------------------------------------------------
const char* FechaHora::getFormato (enum Formato formato)
{
   return (formato >= NUM_FORMATOS ? "" : FORMATOS[formato]);
}

//------------------------------------------------------------------------------
void FechaHora::parse (const Cadena& str, enum Formato formato, int* anyo,
   int* mes, int* dia, int* hora, int* minuto, int* segundo, int* milis)
{
   //*anyo = 1970; *mes = 1; *dia = 1;
   *anyo = 0; *mes = 0; *dia = 0;
   *hora = 0;
   *minuto = 0;
   *segundo = 0;
   *milis = 0;

   const char* strformato = getFormato (formato);
   switch (formato)
   {
      case DD_MM_AA_HH_MM_SS_MMM:
         sscanf (str.getCadena (), strformato, anyo, mes, dia, hora, minuto, segundo, milis);
         break;

      case AAMMDD:
      case AAAAMMDD:
      case AAAA_MM_DD:
      case AAAAMMDDHHMMSS:
      case AAAA_MM_DD_HH_MM_SS:
         sscanf (str.getCadena (), strformato, anyo, mes, dia, hora, minuto, segundo);
         break;

      case DD_MM_AAAA:
      case DD_MM_AA:
      case DD_MM_AAAA_HH_MM_SS:
         sscanf (str.getCadena (), strformato, dia, mes, anyo, hora, minuto, segundo);
         break;

      case DDD_HH_MM:
      case DDDHHMM:
         sscanf (str.getCadena (), strformato, dia, hora, minuto);
         break;

      case MMAA:
         sscanf (str.getCadena (), strformato, mes, anyo);
         break;

      case HH_MM_SS:
      case HHMMSS:
      case HH_MM:
      case HHMM:
         sscanf (str.getCadena (), strformato, hora, minuto, segundo);
         break;

      case NUM_FORMATOS: break;
   }
}

//------------------------------------------------------------------------------
bool FechaHora::checkFecha2 (int anyo, int mes, int dia)
{
   return (anyo >= 0 && anyo <= 99 && mes >= 1 && mes <= 12 &&
      checkDiaMes (2000 + anyo, mes, dia));
}

//------------------------------------------------------------------------------
bool FechaHora::checkFecha4 (int anyo, int mes, int dia)
{
   return (anyo > ANYO_INICIAL && mes >= 1 && mes <= 12 && checkDiaMes (anyo, mes, dia));
}

//------------------------------------------------------------------------------
bool FechaHora::checkHora (int hora, int minuto, int segundo)
{
   return (hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59 &&
      segundo >= 0 && segundo <= 59);
}

//------------------------------------------------------------------------------
bool FechaHora::checkDiaMes (int anyo, int mes, int dia)
{
   return (dia >= 1 && dia <= getUltimoDiaMes (anyo, mes));
}

//------------------------------------------------------------------------------
bool FechaHora::bisiesto (int anyo)
{
   return (anyo % 4 == 0 && (anyo % 100 != 0 || anyo % 400 == 0));
}
