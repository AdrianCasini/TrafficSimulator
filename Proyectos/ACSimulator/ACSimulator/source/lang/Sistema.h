// =======================
//  FICHERO - SISTEMA.H
// =======================

#ifndef SISTEMA_H
#define SISTEMA_H

#include <libgen.h>
#include <stdlib.h>

#include <sys/statvfs.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "../lang/Types.h"
#include "../lang/Cadena.h"
#include "../util/STrz.h"

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: SISTEMA.H
  *
  * DESCRIPCION:
  *
  * FECHA CREACION: DD-MM-YYYY
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR:
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |        |                   |
  *    ---------------------------------------------------------------------
*
  * </PRE>
 */
class Sistema
{

public:

            /**
            * Obtener el porcentaje de disco libre.
            * @param PathFile Path de un fichero que esta en el disco a consultar.
            * @return porcentaje 0% a 100%.
            */
            static float getPorcentajeDiscoLibre(char *PathFile);
            /**
            * Obtener el porcentaje de disco usado.
            * @param PathFile Path de un fichero que esta en el disco a consultar.
            * @return porcentaje 0% a 100%.
            */
            static float getPorcentajeDiscoUsado(char *PathFile);

            /**
            * Obtener el porcentaje de memoria libre.
            * @return porcentaje 0% a 100%.
            */
            static float getPorcentajeMemoriaLibre(void);
            /**
            * Obtener el porcentaje de memoria usada.
            * @return porcentaje 0% a 100%.
            */
            static float getPorcentajeMemoriaUsada(void);

            /**
            * Obtener el porcentaje de swap libre.
            * @return porcentaje 0% a 100%.
            */
            static float getPorcentajeSwapLibre(void);
            /**
            * Obtener el porcentaje de swap usada.
            * @return porcentaje 0% a 100%.
            */
            static float getPorcentajeSwapUsado(void);

            /**
            * Obtener la carga del sistema.
            * @param f1min carga media durante el ultimo minuto.
            * @param f5min carga media durante los ultimos 5 minutos.
            * @param f15min carga media durante los ultimos 15 minutos.
            * @return si se ha realizado con exito.
            */
            static bool getCargaSistema(float *f1min,float *f5min,float *f15min);

           /**
            *  Busca el nombre del proceso dentro de la carpeta PROC, en el caso de que
            *  lo encuentre rotorna su PID y en caso negativo un -1.
            *  Solo busca todos los procesos que no sean el nuestro, es decir
            *  si se busca el pid de nuestro proceso retorna un -1.
            *  @param      lNombreProceso Cadena con el nombre del proceso
            *  @return     Resultado de la busqueda
            *  @exception  Ninguno
            */
            static SWordType getPid(const Cadena& lNombreProceso);

            /**
            *  Busca el nombre del proceso dentro de la carpeta PROC, en el caso de que
            *  lo encuentre rotorna su PID y en caso negativo un -1.
            *  Solo busca todos los procesos que no sean el nuestro, es decir
            *  si se busca el pid de nuestro proceso retorna un -1.
            *  @param      lNombreProceso Cadena con el nombre del proceso
            *  @return     Resultado de la busqueda
            *  @exception  Ninguno
            */
            static SWordType getPid(const Cadena& lNombreProceso,const Cadena& lPrimerParametro);





            /**
            * Devuelve el nombre de un proceso (ej: mainGestComs.run)
            *  @param pid El PID del proceso
            *  @return const PSByteType El nombre del proceso, "???" en caso de
            *  error. El resultado es un puntero a un buffer estatico interno de
            *  la funcion
            *  @exception  Ninguno
            */
            static const PSByteType getNombreProceso (pid_t pid);

            /**
            * Devuelve el padre de un proceso
            *  @param pid El PID del proceso
            *  @return pid_t El PID del proceso padre, o -1 en caso de error
            *  @exception  Ninguno
            */
            static pid_t getPadreProceso (pid_t pid);



private:

            static float contrario(float fPorcentaje);

            static bool EsDir(struct dirent *namelist);

};

#endif //SISTEMA_H

// ===========================
//  FIN DEL FICHERO - SISTEMA.H
// ===========================
