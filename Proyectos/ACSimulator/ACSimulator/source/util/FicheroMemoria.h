// =======================
//  FicheroMemoria.h
// =======================

#ifndef FICHEROMEMORIA_H
#define FICHEROMEMORIA_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROYECTO
#include "Fichero.h"
#include "../thread/AutoMutex.h"


// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERODATOS: FicheroMemoria.h
   *
   * DESCRIPCION: clase de fichero en memoria mediante mmap
   *
   *
   * FECHA CREACION: 21-02-2007
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Marc Coll
   *
   * MODIFICACIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |            |                                         |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class FicheroMemoria : protected Fichero
{
   public:
   
      /**
       *  Constructor de la clase.
       *  @param   Ninguno
       *  @return  Ninguno
       */
      FicheroMemoria ();
      /**
       *  Destructor de la clase.
       *  @param   Ninguno
       *  @return  Ninguno
       */
      virtual ~FicheroMemoria ();

      /** 
       *  Abre el fichero, y lo mapea en memoria mediante <em>mmap</em> 
       *  @return  Ninguno
       */
      void abrir ();

      /**
       *  Crea el fichero con el tamanyo especificado, y lo mapea en memoria
       *  mediante <em>mmap</em>
       *  @param udwTamanyo -> El tamanyo del fichero a crear
       *  @return  Ninguno
       */
      void crear (UDWordType udwTamanyo);
      
      /** 
       * Cierra el fichero, sincroniza con el disco, y lo desmapea.
       *  @return  Ninguno
       */
      void cerrar ();

      /** 
       * Establece el nombre del fichero 
       *  @param cadNombre Cadena con la ruta del fichero.
       *  @return  Ninguno
       */
      void setNombre (const Cadena& cadNombre);

      /** 
       * Establece el modo de lectura sobre el fichero.
       *  @param bLectura True para el modo lectura.
       *  @return  Ninguno
       */
      void setLectura (bool bLectura);

      /** 
       * Establece el modo de escritura sobre el fichero. 
       *  @param bEscritura True para activar el modo escritura.
       *  @return  Ninguno
       */
      void setEscritura (bool bEscritura);

      /** 
       * Establece el modo exclusivo sobre el fichero.
       *  @param bExclusivo True para modo exclusivo.
       *  @return  Ninguno
       */
      void setExclusivo (bool bExclusivo);

      /** 
       * Establece el modo sincronizado para el fichero.
       *  @param bSincronizado True para sincronizar el valor.
       *  @return  Ninguno
       */
      void setSincronizado (bool bSincronizado);

      /** 
       * Devuelve el nombre del fichero 
       *  @return  La ruta del fichero.
       */
      Cadena getNombre () const;

      /** 
       * Devuelve el tamanyo del fichero 
       *  @return  El numero de bytes del fichero.
       */
      UDWordType getTamano () const;

      /** 
       * Devuelve el puntero a la memoria donde esta mapeado el fichero 
       *  @return  Un puntero de memoria.
       */
      void* getPunteroMemoria () const;

      /** 
       * Indica si el fichero esta abierto.
       *  @return  Si el fichero esta abierto.
       */
      bool isAbierto () const;

      /** 
       * Indica si es el final de la zona de memoria 
       *  @param udwOffset Offset de posicion del fichero.
       *  @return  Si es el final de la zona de memoria.
       */
      bool isFinal (UDWordType udwOffset) const;

      /**
       * Realiza un bloqueo sobre la memoria del fichero, para permitir accesos
       * concurrentes de forma segura. El bloqueo funciona tanto entre procesos,
       * como entre diferentes hilos de un mismo proceso
       */
      virtual void lock () const;

      /**
       * Desbloquea la memoria del fichero 
       */
      virtual void unlock () const;

      /**
       * Lee datos del fichero
       *  @param udwOffset -> Posicion del fichero a partir de la cual leer
       *  @param buf -> Buffer donde se guardaran los datos leidos
       *  @param udwCount -> Numero de bytes a leer
       *  @return  Los bytes leidos realmente.
       */
      UDWordType leer (UDWordType udwOffset, void *buf, UDWordType udwCount);

      /**
       * Lee un linea de datos del fichero de texto. 
       *  @param udwOffset -> Posicion del fichero a partir de la cual leer
       *  @param buf -> Buffer donde se guardaran los datos leidos
       *  @param udwCount -> Numero de bytes a leer
       *  @return Retorna el numero de caracteres leidos, incluyendo el \n.
       */
      SDWordType leerLinea (UDWordType udwOffset, void *buf, UDWordType udwCount);
      
      /**
       * Escribe datos en el fichero
       *  @param udwOffset -> Posicion del fichero a partir de la cual escribir
       *  @param buf -> Datos a escribir
       *  @param udwCount -> Numero de bytes a escribir
       */
      UDWordType escribir (UDWordType udwOffset, const void *buf, UDWordType udwCount);
      
      /**
       * Escribe una linea de datos en el fichero
       *  @param udwOffset -> Posicion del fichero a partir de la cual escribir
       *  @param buf -> Datos a escribir
       *  @param udwCount -> Numero de bytes a escribir
       */
      UDWordType escribirLinea (UDWordType udwOffset, const void *buf, UDWordType udwCount);

      /**
       * Sincroniza la memoria con el disco. Si el fichero es sincrono, cada vez
       * que se llame a esta funcion los datos se escribiran a disco sincronamente
       */
      void sincronizar ();

      /**
       * Reinicia el fichero al tamanyo especificado, y lo rellena con ceros
       *  @param udwTamanyo Tamano de la zona rellenada.
       */
      void reset (UDWordType udwTamanyo);

   private:

      void mapear ();
      void desmapear ();

      void*            mpMemoria;
      UDWordType       mudwTamanyo;
      static AutoMutex mThMutex;
};

#endif // FICHEROMEMORIA_H
