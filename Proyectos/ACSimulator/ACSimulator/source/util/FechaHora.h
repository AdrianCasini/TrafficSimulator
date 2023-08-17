
#if !defined(FechaHora_H)
  #define FechaHora_H

#include "../lang/Cadena.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: FechaHora.h
  *
  * DESCRIPCION: Clase para gestionar fechas, horas, tiempos, etc
  *
  * FECHA CREACION: 23-07-2012
  *
  * AUTOR DISENYO: Marc Coll
  *
  * PROGRAMADOR: Marc Coll
  *
  * </PRE>
  */
class FechaHora
{
   public:

      /**
      Unidades de tiempo, utiles para modificar el valor del objeto. Por ejemplo:
      \code
      FechaHora f1, f2;

      f1 = FechaHora::ahora ();

      #Sumar 5 minutos
      f1 += (5 * FechaHora::MINUTOS);

      #Restar 12 horas y media
      f2 = f1 - (12 * FechaHora::HORAS + 30 * FechaHora::MINUTOS);
      \endcode
      */
      enum Unidades
      {
         SEGUNDOS =  1,
         MINUTOS  = 60 * SEGUNDOS,
         HORAS    = 60 * MINUTOS,
         DIAS     = 24 * HORAS,
         SEMANAS  =  7 * DIAS
      };

      /** Formatos posibles para las fechas (tanto para leer como para formatear) */
      enum Formato
      {
         DD_MM_AA_HH_MM_SS_MMM = 0, ///< Ej: 31/12/13 12:34:56,123
         AAAAMMDDHHMMSS,            ///< Ej: 20131231123456
         AAAA_MM_DD_HH_MM_SS,       ///< Ej: 2013-12-31 12:34:56
         DD_MM_AAAA_HH_MM_SS,       ///< Ej: 31/12/2013 12:34:56
         DDD_HH_MM,                 ///< Ej: 365 12:34
         DDDHHMM,                   ///< Ej: 3651234
         AAAA_MM_DD,                ///< Ej: 2013-12-31
         AAAAMMDD,                  ///< Ej: 20131231
         AAMMDD,                    ///< Ej: 131231
         DD_MM_AAAA,                ///< Ej: 31/12/2013
         DD_MM_AA,                  ///< Ej: 31/12/13
         MMAA,                      ///< Ej: 12/13
         HH_MM_SS,                  ///< Ej: 12:34:56
         HHMMSS,                    ///< Ej: 123456
         HH_MM,                     ///< Ej: 12:34
         HHMM,                      ///< Ej: 1234
         NUM_FORMATOS,              //NO TOCAR

         //Sinonimos
         MOMENTO         = AAAAMMDDHHMMSS,        ///< Sinonimo de AAAAMMDDHHMMSS
         MOMENTO_BD      = AAAA_MM_DD_HH_MM_SS,   ///< Sinonimo de AAAA_MM_DD_HH_MM_SS
         MOMENTO_JULIANO = DDDHHMM,               ///< Sinonimo de DDDHHMM
         FECHA           = DD_MM_AAAA,            ///< Sinonimo de DD_MM_AAAA
         HORA            = HH_MM_SS,              ///< Sinonimo de HH_MM_SS
         FECHA_HORA      = DD_MM_AAAA_HH_MM_SS,   ///< Sinonimo de DD_MM_AAAA_HH_MM_SS
         CADUCIDAD       = MMAA,                  ///< Sinonimo de MMAA
         TRAZA           = DD_MM_AA_HH_MM_SS_MMM, ///< Sinonimo de DD_MM_AA_HH_MM_SS_MMM
      };

      /** Enum para facilitar el trabajo con los meses del anyo */
      enum MesAnyo
      {
         ENERO = 1,
         FEBRERO,
         MARZO,
         ABRIL,
         MAYO,
         JUNIO,
         JULIO,
         AGOSTO,
         SEPTIEMBRE,
         OCTUBRE,
         NOVIEMBRE,
         DICIEMBRE
      };

      /** Enum para facilitar el trabajo con los dias de la semana */
      enum DiaSemana
      {
         //Los valores estan puestos segun la funcion localtime. NO TOCAR
         DOMINGO = 0,
         LUNES,
         MARTES,
         MIERCOLES,
         JUEVES,
         VIERNES,
         SABADO
      };

      /**
      Constructor por defecto. Si no se pasa ningun parametro el resultado es un
      objeto FechaHora vacio
         @param segundos Numero de segundos transcurridos desde el 1 de enero de
         1970 a las 00:00
         @param microsegundos Numero de microsegundos
      */
      //FechaHora (time_t segundos=0, suseconds_t microsegundos=0);

      /**
      Constructor a partir de un struct timeval, que contiene segundos y microsegundos
         @see Consultar la documentacion de la funcion gettimeofday para mas detalles
      */
      FechaHora (const struct timeval& t);

      /**
      Constructor a partir de los datos suministrados
         @param anyo El anyo (>= 1900)
         @param mes El mes (1 - 12)
         @param dia El dia (1 - 31)
         @param hora La hora (0 - 23)
         @param minuto El minuto (0 - 59)
         @param segundo El segundo (0 - 59)
      */
      FechaHora (int anyo, int mes, int dia, int hora=0, int minuto=0, int segundo=0);

      /**
      Constructor a partir de una cadena
         @param str La cadena (ej: 20120229)
         @param formato El formato de la cadena (ej: AAAAMMDD). Por defecto FECHA_HORA
      */
      FechaHora (const Cadena& str, enum Formato formato=FECHA_HORA);

      /** Constructor por copia */
      FechaHora (const FechaHora& f);

      /** Destructor */
      ~FechaHora ();

      FechaHora& operator = (const FechaHora& f);
      FechaHora& operator = (time_t t);
      FechaHora operator + (const FechaHora& f);
      FechaHora operator - (const FechaHora& f);
      FechaHora operator + (int t);
      FechaHora operator - (int t);
      void operator += (const FechaHora& f);
      void operator -= (const FechaHora& f);
      void operator += (int t);
      void operator -= (int t);

      bool operator == (const FechaHora& f) const;
      bool operator != (const FechaHora& f) const;
      bool operator > (const FechaHora& f) const;
      bool operator >= (const FechaHora& f) const;
      bool operator < (const FechaHora& f) const;
      bool operator <= (const FechaHora& f) const;

      /**
      Operador de conversion a time_t. Devuelve los segundos transcurridos desde
      el 01/01/1970, o -1 si la fecha no es valida
         @see getSegundos1970
      */
      operator time_t () const;

      /**
      Operador de conversion a struct timeval. Devuelve los segundos y microsegundos
      transcurridos desde el 01/01/1970
      */
      operator const struct timeval& () const;

      /**
      Operador de conversion a Cadena. Devuelve una cadena representando la
      fecha y hora actuales, en formato FECHA_HORA
      */
      operator Cadena () const;

      /** Establece la fecha a partir de los segundos (UTC) desde el 01/01/1970 */
      void setSegundos1970 (time_t segundos);

      /**
      Devuelve los segundos transcurridos (UTC) desde el 01/01/1970, o -1 si la
      fecha no es valida
      */
      time_t getSegundos1970 () const;

      /**
      Devuelve los segundos despues de realizar una operacion aritmetica entre
      fechas
      */
      time_t getTiempoEnSegundos () const;

      /** Establece a 0 las horas, minutos y segundos, dejando solo la fecha */
      void borrarHora ();

      /** Establece a 0 el dia, mes y anyo, dejando solo la hora */
      void borrarFecha ();

      /** Devuelve el anyo (de 1900 en adelante) */
      int getAnyo () const;

      /** Devuelve el mes (de 1 a 12) */
      int getMes () const;

      /** Devuelve el dia (de 1 a 31) */
      int getDia () const;

      /** Devuelve el dia de la semana */
      enum DiaSemana getDiaSemana () const;

      /** Devuelve el dia juliano (de 1 a 366) */
      int getDiaJuliano () const;

      /** Devuelve la hora (de 0 a 23) */
      int getHora () const;

      /** Devuelve el minuto (de 0 a 59) */
      int getMinuto () const;

      /** Devuelve el segundo (de 0 a 59) */
      int getSegundo () const;

      /** Devuelve los milisegundos (de 0 a 999) */
      int getMilisegundos () const;

      /** Devuelve los microsegundos (de 0 a 999999) */
      int getMicrosegundos () const;

      /**
      Devuelve el intervalo de tiempo expresado en milisegundos en lugar de
      segundos
         @return int64_t Un entero de 64 bits con el resultado
      */
      int64_t getTiempoEnMilisegundos () const;

      /** Devuelve una cadena con la zona horaria actual. Por ejemplo: 'CET (UTC +1)' */
      Cadena getZonaHoraria () const;

      /** Devuelve la diferencia horaria, en segundos, con respecto a UTC */
      time_t getDiferenciaUTC () const;

      /** Devuelve si actualmente hay horario de verano */
      bool hayHorarioDeVerano () const;

      /** Devuelve si el anyo actual es bisiesto */
      bool esAnyoBisiesto () const;

      /**
      Sumar a la fecha/hora la cantidad de tiempo indicada. A diferencia de
      otras funciones, los parametros no necesitan estar dentro de un rango
      determinado (podemos especificar 500 segundos, por ejemplo).
      @note Un metodo alternativo seria usar los operadores aritmeticos y el
      enum Unidades
         @param dias Numero de dias
         @param horas Numero de horas
         @param minutos Numero de minutos
         @param segundos Numero de segundos
      */
      void sumar (int dias, int horas, int minutos, int segundos);

      /**
      Funcion complementaria de sumar
      @see sumar
      */
      void restar (int dias, int horas, int minutos, int segundos);


      void restar_ms (int dias, int horas, int minutos, int segundos);
      void sumar_ms (int dias, int horas, int minutos, int segundos);

      /** Establece la fecha como vacia */
      void setVacia ();

      /** Devuelve si la fecha esta vacia */
      bool esVacia () const;

      /** Devuelve si la fecha es valida (ej: 31/02/2011 no es valida) */
      bool esValida () const;

      /** Compara la fecha con otro objeto FechaHora sin tener en cuenta la hora */
      bool esMismoDia (const FechaHora& f) const;

      /**
      Establece la fecha a partir de los datos suministrados
         @param anyo El anyo (>= 1900)
         @param mes El mes (1 - 12)
         @param dia El dia (1 - 31)
      */
      void setFecha (int anyo, int mes, int dia);

      /**
      Establece la hora a partir de los datos suministrados
         @param hora La hora (0 - 23)
         @param minuto El minuto (0 - 59)
         @param segundo El segundo (0 - 59)
      */
      void setHora (int hora, int minuto, int segundo);

      /**
      Establece la fecha y la hora a partir de los datos suministrados
         @param anyo El anyo (>= 1900)
         @param mes El mes (1 - 12)
         @param dia El dia (1 - 31)
         @param hora La hora (0 - 23)
         @param minuto El minuto (0 - 59)
         @param segundo El segundo (0 - 59)
         @param milis Los milisegundos (0 - 999). Por defecto 0
      */
      void setFechaHora (int anyo, int mes, int dia, int hora, int minuto,
         int segundo, int milis=0);

      /**
      Establece la fecha a partir de una cadena
         @param str La cadena (ej: 20120229)
         @param formato El formato de la cadena (ej: AAAAMMDD)
      */
      void setFechaHora (const Cadena& str, enum Formato formato=FECHA_HORA);

      /**
      Establece la fecha y la hora a partir de la hora y el dia juliano
         @param anyo El anyo (>= 1900)
         @param dia El dia (1 - 366)
         @param hora La hora (0 - 23)
         @param minuto El minuto (0 - 59)
      */
      void setMomentoJuliano (int anyo, int dia, int hora, int minuto);

      /**
      Devuelve una cadena indicando la fecha/hora en un formato predefinido
         @param formato El formato a usar. Por defecto FECHA_HORA
      */
      Cadena getCadena (enum Formato formato=FECHA_HORA) const;

      /**
      Valida una cadena indicando una fecha/hora en un formato predefinido
         @param momento La cadena a validar
         @param formato El formato de la cadena
      */
      static bool validarMomento (const Cadena& momento, enum Formato formato);

      /**
      Establece la fecha y hora actuales
         @param conMicrosegundos Si es necesario usar precision de microsegundos,
         o basta con los segundos. Por defecto es false (sin microsegundos)
      */
      void getAhora (bool conMicrosegundos=false);

      /**
      Devuelve la fecha y hora actuales
         @param conMicrosegundos Si es necesario usar precision de microsegundos,
         o basta con los segundos. Por defecto es false (sin microsegundos)
      */
      static FechaHora ahora (bool conMicrosegundos=false);

      /** Devuelve la fecha actual, sin la hora */
      static FechaHora hoy ();

      /** Devuelve la fecha actual menos un dia, sin la hora */
      static FechaHora ayer ();

      /** Devuelve la fecha actual mas un dia, sin la hora */
      static FechaHora manyana ();

      /**
      Devuelve cual es el ultimo dia del mes en el anyo indicado.
      Ej: getUltimoDiaMes (2012, FEBRERO) = 29
         @param anyo El anyo
         @param mes El mes
      */
      static int getUltimoDiaMes (int anyo, int mes);

      /**
      Devuelve si el anyo indicado es bisiesto
         @param anyo El anyo
      */
      static bool esAnyoBisiesto (int anyo);

   private:

      static const int ANYO_INICIAL = 1900;

      static const char* FORMATOS[];

      void resetTiempo ();
      void actualizarTiempo ();
      void actualizarSegundos ();
      void redondearSegundos ();

      static const char* getFormato (enum Formato formato);
      static void parse (const Cadena& str, enum Formato formato,
         int* anyo, int* mes, int* dia, int* hora, int* minuto, int* segundo,
         int* milis);
      static bool checkFecha2 (int anyo, int mes, int dia);
      static bool checkFecha4 (int anyo, int mes, int dia);
      static bool checkHora (int hora, int minuto, int segundo);
      static bool checkDiaMes (int anyo, int mes, int dia);
      static bool bisiesto (int anyo);

      //struct timeval mSegundos; //Esto siempre representa el tiempo en UTC
	  struct _timeb mSegundos;

      struct tm      mTiempo;   //Esto siempre representa el tiempo en GMT
      bool           mValida;

      /*
      Estructuras de datos:

      struct timeval {
         time_t      tv_sec;     // seconds
         suseconds_t tv_usec;    // microseconds
      };

      struct tm
      {
         int tm_sec;         // seconds (0 - 59)
         int tm_min;         // minutes (0 - 59)
         int tm_hour;        // hours (0 - 23)
         int tm_mday;        // day of the month (1 - 31)
         int tm_mon;         // month (0 - 11)
         int tm_year;        // year (since 1900)
         int tm_wday;        // day of the week (0 - 6)
         int tm_yday;        // day in the year (0 - 365)
         int tm_isdst;       // daylight saving time
      };
      */
};

#endif
