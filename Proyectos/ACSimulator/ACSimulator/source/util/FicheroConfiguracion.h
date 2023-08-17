// =======================
//  FICHERO - FicheroConfiguracion.h
// =======================

#ifndef FICHERO_CONFIGURACION_H
#define FICHERO_CONFIGURACION_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include "stdio.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


// INCLUDES DEL PROJECTO
#include "lang/Cadena.h"


// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO:  FicheroConfiguracion.h
   *
   * DESCRIPCION: Esta clase implementa todos los valores que permiten la escritura
   *                 , lectura, apennf .... de un fichero.
   *
   * FECHA CREACION: 28-11-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Javier Tarradas
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |            |                                         |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class FicheroConfiguracion
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PRIVATE      ////////////////////
   protected:

      Cadena    mNombre;

      bool          bLectura;
      bool          bEscritura;
      bool          bExclusivo;
      bool          bAppend;
      bool          bSincronizado;
      bool          bNoctty;
      bool          bNoBloqueante;

      mode_t        mumask;
      mode_t        mmask;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:

      // ===============   CICLO DE VIDA    ==================

      FicheroConfiguracion();
      virtual  ~FicheroConfiguracion();

       // ===============   OPERACIONES    ==================

      /**
      * Establece el nombre fichero.
      * @param mFichero Nombre del fichero con su path.
      * @return Nada.
      */
      void setNombre(const Cadena& mFichero);
      /**
      * Establece el modo de lectura del fichero.
      * @param bLectura Si es de lectura o no.
      * @return Nada.
      */
      void setLectura(const bool bLectura);
      /**
      * Establece el modo de escritura del fichero.
      * @param bEscritura Si es de escritura o no.
      * @return Nada.
      */
      void setEscritura(const bool bEscritura);
      /**
      * Establece el modo exclusivo/compartido del fichero.
      * @param bExclusivo true si es exclusivo o false si es compartido.
      * @return Nada.
      */
      void setExclusivo(const bool bExclusivo);
      /**
      * Establece el modo append del fichero.
      * @param bAppend Si es de modo append o no.
      * @return Nada.
      */
      void setAppend(const bool bAppend);
      /**
      * Determina si el fichero se actualiza en disco en cada operacion de escritura
      * @param bSincronizado Si es de modo sync o no.
      * @return Nada.
      */
      void setSincronizado(const bool bSincronizado);
      /**
      * Establece el modo NOCTTY del fichero.
      * @param bNoctty Si es de modo Noctty o no.
      * @return Nada.
      */
      void setNoctty(const bool bNoctty);
      /**
      * Establece el modo NONBLOCK del fichero.
      * @param bNoBloqueante Si es de modo NoBloqueante o no.
      * @return Nada.
      */
      void setNoBloqueante(const bool bNoBloqueante);
      /**
      * Establece los permisos umask en la creacion del fichero.
      * @param mumask Permisos umask del fichero.
      * @return Nada.
      */
      void setUMask(mode_t mumask);
      /**
      * Establece los permisos en la creacion del fichero.
      * @param mmask Permisos del fichero.
      * @return Nada.
      */
      void setMask(mode_t mmask);

      /**
      * Retorna el nombre del archivo
      * @return Nombre del archivo
      */
      Cadena getNombre ();

      /**
      * Retorna el modo de lectura del fichero.
      * @param Ninguno
      * @return Si es de lectura o no.
      */
      const bool getLectura ( );
      /**
      * Retorna el modo de escritura del fichero.
      * @param Ninguno
      * @return Si es de escritura o no.
      */
      const bool getEscritura ( );
      /**
      * Retorna el modo exclusivo/compartido del fichero.
      * @param Ninguno.
      * @return true si es exclusivo o false si es compartido.
      */
      const bool getExclusivo ( );
      /**
      * Retorna el modo append del fichero.
      * @param Ninguno
      * @return Si es de modo append o no.
      */
      const bool getAppend ( );
      /**
      * Retorna si el fichero se actualiza en disco en cada operacion de escritura
      * @param Ninguno
      * @return Si es de modo sync o no.
      */
      const bool getSincronizado ( );
      /**
      * Retorna el modo NOCTTY del fichero.
      * @param Ninguno
      * @return Si es de modo Noctty o no..
      */
      const bool getNoctty ( );



      const bool getNoBloqueante();
      /**
      * Retorna los permisos umask para la creacion del fichero.
      * @param Ninguno
      * @return Permisos umask del fichero.
      */
      const mode_t getUMask ( );
      /**
      * Retorna los permisos para la creacion del fichero.
      * @param Ninguno.
      * @return Permisos del fichero.
      */
      const mode_t getMask ( );

   private:
};

#endif // FICHERO_CONFIGURACION_H
