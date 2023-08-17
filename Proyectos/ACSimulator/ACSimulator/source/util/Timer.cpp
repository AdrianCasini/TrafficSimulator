// =======================
//  FICHERO - Timer.cpp
// =======================

#include "Timer.h"

Timer::Timer()
{
   bExit=false;
   bDone=true;
   udwMsec=10;
}

Timer::~Timer()
{
}

void Timer::inicializar(UDWordType udwMsec)
{
   this->udwMsec=udwMsec;
   Timers.inicializar();
   iniciar();
}

void Timer::armar(const UDWordType udwTiempo, EjecutorTimer* pEjecTimer,
const UDWordType udwNumTemporiz, UDWordType uwId,bool bEjecutarYa)
{
   _Timer timer;

   desarmar(uwId);

   timer.uwId=uwId;
   timer.udMode=MODO_RELATIVO;
   timer.sdSteps=udwNumTemporiz;
   timer.udBaseTicks=(udwTiempo/udwMsec == 0) ? 1 : (udwTiempo/udwMsec);
   timer.udTicks=(bEjecutarYa) ? 1 : timer.udBaseTicks;
   timer.pEjecTimer=pEjecTimer;
   timer.udEjecuciones=0;
   timer.bPaused=false;

   Timers.add(timer, uwId);
}

void Timer::armar(const UByteType ubHora,const UByteType ubMinuto,const UByteType ubSegundo,
       EjecutorTimer* pEjecTimer,
      const UDWordType udwNumTemporiz, UDWordType uwId)
{
   _Timer timer;

   desarmar(uwId);

   timer.uwId=uwId;
   timer.udMode=MODO_ABSOLUTO;
   timer.sdSteps=udwNumTemporiz;
   timer.udBaseTicks=(500/udwMsec == 0) ? 1 : (500/udwMsec);;
   timer.udTicks=timer.udBaseTicks;
   timer.pEjecTimer=pEjecTimer;
   timer.udEjecuciones=0;
   timer.bPaused=false;

   timer.ubHora=ubHora;
   timer.ubMinuto=ubMinuto;
   timer.ubSegundo=ubSegundo;
   timer.sdwUltimaEjecucion=0;

   Timers.add(timer, uwId);
}

void Timer::armar(const UDWordType udwDesfase, EjecutorTimer* pEjecTimer,UDWordType uwId) {
   _Timer timer;

   desarmar(uwId);

   timer.uwId=uwId;
   timer.udMode=MODO_AVISO_CAMBIO_HORA;
   timer.udBaseTicks=(500/udwMsec == 0) ? 1 : (500/udwMsec);;
   timer.udTicks=timer.udBaseTicks;
   timer.pEjecTimer=pEjecTimer;
   timer.bPaused=false;
   timer.udwDesfase=((udwDesfase<500) ? 500 : udwDesfase);
   timerclear(&timer.nexttime);

   Timers.add(timer, uwId);
}

void Timer::desarmar(UDWordType uwId)
{
   HITEM htimer;

   Timers.lock();
   htimer=Timers.get(uwId);
   if (htimer!=NULL)
   {
      Timers.remove(htimer);
   }
   Timers.unlock();
}

bool Timer::setPausa(UDWordType uwId,bool bValor)
{
   HITEM htimer;
   _Timer *timer;

   Timers.lock();
   htimer=Timers.get(uwId);
   if (htimer!=NULL)
   {
      timer=Timers.getObject(htimer);
      timer->bPaused=bValor;
      Timers.unlock();
      return true;
   }
   Timers.unlock();
   return false;
}

bool Timer::pausar(UDWordType uwId)
{
   return(setPausa(uwId,true));
}

bool Timer::continuar(UDWordType uwId)
{
   return(setPausa(uwId,false));
}


bool Timer::armado(UDWordType uwId)
{
   return(Timers.get(uwId)!=NULL);
}

SDWordType Timer::getNumTimers(void)
{
   return(Timers.getNumItems());
}

void Timer::esperar( UDWordType udMilis)
{
   usleep( 1000 * udMilis );
}


void  mygettimeofday(struct timeval *tv, struct timezone *tz)
{
   SDWordType sdErr=0;

   sdErr=gettimeofday(tv,tz);
   if (FAIL==sdErr)
   {
      switch(errno)
      {
         case EPERM:
            throw NoAplicaExcepcion(__AQUI__,
            "settimeofday is called by someone other than the superuser.",
            errno);
            break;
         case EINVAL:
            throw FormatoExcepcion(__AQUI__,
            "Timezone (or something else) is invalid.",
            errno);
            break;
         case EFAULT:
            throw ParametroExcepcion(__AQUI__,
            "One of tv or tz pointed outside your accessible address space.",
            errno);
            break;
         default:
            throw DesconocidoExcepcion(__AQUI__,
            "Error desconocido en mygettimeofday.",
            errno);
            break;
      }
   }
}

void  Timer::tarea(void)
{
   _Timer *timer;
   HITEM hCurrent;
   HITEM hLast;

   EjecutorTimer *pEjecutor;
   UDWordType   uwId;

   struct timeval Before;
   struct timeval After;
   struct timeval Now;
   UDWordType udTicks;
   SDWordType sdDif;

   struct tm ptm;
   time_t now;

   mygettimeofday(&Before,NULL);
   adjust(&Before);
   udTicks=0;

   bDone=false;
   while (!bExit)
   {
      mygettimeofday(&After,NULL);
      adjust(&After);
      /*Calculamos el tiempo perdido por los cambios de contexto
        y por tener un kernel no real time. (desfase) */
      sdDif=(After.tv_sec-Before.tv_sec)*MICROSEC+(After.tv_usec-Before.tv_usec);
      sdDif-=(1000*udwMsec*udTicks);
      /*Si el desfase es mayor que el sleep del timer lo compensamos no haciendo
        el sleep que toca.*/
      if (sdDif<(1000*udwMsec))
      {
         /*Para evitar overflows en el calculo de dif reiniciamos el tiempo
           de calculo de desfase si se dan los siguientes casos:

            1) mejor de los casos:
            El desfase es menor de 10 microsegundos podemos
            considerar que no hay desfase y por tanto empezar de nuevo el calculo.

            2) cambio horario:
            La diferencia es negativa y eso solo puede suceder con un cambio horario.
            Iniciamos el ajuste.
         */
         if (sdDif<10)
         {
            mygettimeofday(&Before,NULL);
            adjust(&Before);
            udTicks=0;
         }
         /*Seguimos el procedimiento normal de esperar un tick del timer*/
         usleep(1000*udwMsec);
      }
      else {
         /* Si la cpu no ha podido atendernos en mas de 10 veces la resolucion
            del timer consideramos que es un cambio horario y por tanto re-iniciamos el ajuste.*/
         if (sdDif > (10000*udwMsec))
         {
            mygettimeofday(&Before,NULL);
            adjust(&Before);
            udTicks=0;
            usleep(1000*udwMsec);
         }
      }
      udTicks++;
      /*Procesamos los timers*/
      Timers.lock();
      hLast=NULL;

      now=time(NULL);
      localtime_r(&now,&ptm);

      while ((hCurrent=Timers.getNext(hLast))!=NULL)
      {  //obtenemos el handle del siguiente timer
         //obtenemos el timer a traves del handle
         timer=Timers.getObject(hCurrent);
         if (!timer->bPaused)
         {  //si no esta en pausa ...
            //decrementamos el contador propio de ticks
            timer->udTicks--;
            if (timer->udTicks==0)
            {  //si ya toca ejecutar ....
               uwId=timer->uwId;
               pEjecutor=timer->pEjecTimer;
               switch (timer->udMode) {
                  case MODO_AVISO_CAMBIO_HORA:
                     if (timerisset(&timer->nexttime)) {
                        gettimeofday(&Now,NULL);
                        adjust(&Now);
                        sdDif=(Now.tv_sec-timer->nexttime.tv_sec)*MICROSEC+(Now.tv_usec-timer->nexttime.tv_usec);
                        if (abs(sdDif)>(timer->udwDesfase*1000)) {
                           if (pEjecutor!=NULL) pEjecutor->tareaTimer(uwId);
                        }
                     }
                     gettimeofday(&timer->nexttime,NULL);
                     timer->nexttime.tv_usec+=(timer->udBaseTicks*udwMsec*1000);
                     adjust(&timer->nexttime);
                     timer->udTicks=timer->udBaseTicks;
                     break;
                  case MODO_ABSOLUTO:
                     if ((timer->ubHora==(UByteType) ptm.tm_hour) &&
                           (timer->ubMinuto==(UByteType) ptm.tm_min) &&
                           (timer->ubSegundo==(UByteType) ptm.tm_sec)) {

                              if (abs(now-timer->sdwUltimaEjecucion)>2) {

                                      if (timer->pEjecTimer!=NULL)
                                      {
                                              timer->sdwUltimaEjecucion=now;
                                      }
                                      timer->udEjecuciones++;
                                      if (timer->sdSteps!=0)
                                      {
                                              timer->sdSteps--;
                                              if (timer->sdSteps==0)
                                              {
                                                      Timers.remove(hCurrent);
                                                      hCurrent=hLast;
                                                      if (pEjecutor!=NULL) pEjecutor->tareaTimer(uwId);
                                                      break;
                                              }
                                      }
                                      else {
                                              if (pEjecutor!=NULL) pEjecutor->tareaTimer(uwId);
                                      }
                              }
                     }
                     else {
                        timer->sdwUltimaEjecucion=0;
                     }
                     timer->udTicks=timer->udBaseTicks;
                     break;
                  case MODO_RELATIVO:
                  default:

                     timer->udEjecuciones++;
                     if (timer->sdSteps==0)
                     {  //si tiene temporizaciones infinitas
                        //reiniciamos el contador propio de ticks
                        timer->udTicks=timer->udBaseTicks;
                     }
                     else
                     {   //si tiene temporizaciones finitas
                        //decrementamos numero de temporizaciones
                        timer->sdSteps--;
                        if (timer->sdSteps==0)
                        {  //si no quedan temporizaciones
                           //borramos el timer de la lista
                           Timers.remove(hCurrent);
                           //el timer actual es el anterior (para el Timers.getNext)
                           //dado que este timer ya no existe en la lista.
                           hCurrent=hLast;
                        }
                        else
                        {  //si todavia quedan temporizaciones
                           //reiniciamos el contador propio de ticks
                           timer->udTicks=timer->udBaseTicks;
                        }
                     }
                     if (pEjecutor!=NULL) pEjecutor->tareaTimer(uwId);
                     break;
               }
            }
         }
         hLast=hCurrent;
      }
      Timers.unlock();
   }
   bDone=true;
}

void Timer::adjust(struct timeval *tmv)
{
   /* Linux no te garantiza que tv_usec sea menor de 1000000
      y todavia haya que pasar la parte correspondiente a segundos*/
   while(tmv->tv_usec>MICROSEC)
   {
      tmv->tv_usec-=MICROSEC;
      tmv->tv_sec++;
   }
}

UDWordType Timer::getTiempoRestante(UDWordType uwId) {
   struct _Timer *_timer;
   UDWordType udwRet=0;

   Timers.lock();
   _timer=(struct _Timer *) Timers.getObject(Timers.get(uwId));
   if ((_timer!=NULL) && (_timer->udMode==MODO_RELATIVO)) {
      udwRet=_timer->udTicks*udwMsec;
   }
   Timers.unlock();

   return(udwRet);
}

bool Timer::execNow(UDWordType uwId,UDWordType udwTiempo,bool block) {
   struct _Timer *_timer;

   if (block) {
      Timers.lock();
   }
   else {
      if (Timers.tryLock()) return false;
   }
   _timer=(struct _Timer *) Timers.getObject(Timers.get(uwId));
   if ((_timer!=NULL) && (_timer->udMode==MODO_RELATIVO)) {
      _timer->udTicks=(udwTiempo/udwMsec == 0) ? 1 : (udwTiempo/udwMsec);;
   }
   Timers.unlock();
   return true;
}

void Timer::finalizar(void)
{
   bExit=true;
   destruir();
   esperarFinalHilo();
}

