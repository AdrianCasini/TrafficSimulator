// =======================
//  FICHERO - Excepcion.cpp
// =======================

#include "Excepcion.h"  // clase implementada
#include <errno.h>

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

Excepcion::Excepcion(const UWordType uwNumLinea, const char* psbNomFichero,
                     const char* psbNomFuncion, const Cadena& mStr,
                     const SDWordType sdwNumError, Excepcion* peExcepcion)
{
   SByteType  sbTexto[Cadena::LON_MAX_STRING];
   PSByteType psbAux;

   psbAux = const_cast<PSByteType> (strrchr (psbNomFichero, '/'));
   if (psbAux != NULL)
      psbAux++;
   else
      psbAux = const_cast<PSByteType> (psbNomFichero);

   snprintf (sbTexto, Cadena::LON_MAX_STRING, "%c%02d%c%s%c%s:%d%c%s()",
      mcsbSeparador, sdwNumError, mcsbSeparador, mStr.getCadena (),
      mcsbSeparador, psbAux, uwNumLinea, mcsbSeparador, psbNomFuncion);

   // Inicializamos las variables
   LimpiarStackTrace();
   // Copiamos las posibles excepciones anteriores
   CopiarStackExcepciones(peExcepcion);
   // Insertamos el nuevo elemento
   InsertaElemento(sbTexto);
}

Excepcion::~Excepcion()
{

}

// =================   OPERADORES    ===================

////////////////////   PROTECTED    /////////////////////

////////////////////   PRIVATE      ////////////////////

// =================   ACCESO       ====================


// =================   CONSULTA      ===================

   UDWordType Excepcion::getNumeroExcepciones()
   {
      return ( this->muwIndice );
   }

   const char*  Excepcion::getExcepcion(CUDWordType cudNumeroExcepcion)
   {
      if(cudNumeroExcepcion < this->muwIndice)
      {
        return(mCadenas[cudNumeroExcepcion]);
      }
      return(NULL);
   }

   const char* Excepcion::getTextoUltimaExcepcion()
   {
      if(this->muwIndice > 0)
       return(mCadenas[this->muwIndice -1]);
      else
       return(NULL);
   }

   Cadena Excepcion::getTextoUsuarioUltimaExcepcion()
   {
      Cadena lTexto;
      UWordType luwInicio;

      if(this->muwIndice > 0)
      {
         lTexto = mCadenas[this->muwIndice -1];
         luwInicio = lTexto.indiceDe(mcsbSeparador);
         lTexto = lTexto.subCadena(luwInicio + 1,lTexto.indiceDe(mcsbSeparador,luwInicio) - 1);
      }
      return(lTexto);
   }

// =================   OPERACIONES   ===================

   void Excepcion::ImprimirStack()
   {
      UDWordType udIndice;

      if(this->muwIndice > 0)
      {
         std::cout << "Lista de Excepciones generadas: Pid: "  << "\n";  //<< getpid()
         std::cout << '\t'
                   << " - N-Error - (Texto usuario/Texto sistema)"
                   << " - N-LINEA - FICHERO - FUNCION\n";
         for(udIndice = 0;udIndice < this->muwIndice;udIndice++)
         {
          std::cout << '\t' << "    " <<
            mCadenas[this->muwIndice - udIndice - 1] << '\n';
         }

      }
      else
      {
         std::cout << "El stack de Excepciones esta vacio.\n";
      }

      std::cout.flush();
   }

   const char* Excepcion::getStack()
   {
      stringstream ret;
      UDWordType udIndice;

      if(this->muwIndice > 0)
      {
         ret << "Lista de Excepciones generadas: Pid: "  << "\n"; /*getpid()*/
         ret << "\t"
             << " - N-Error - (Texto usuario/Texto sistema)"
             << " - N-LINEA - FICHERO - FUNCION" << endl;

         for(udIndice = 0;udIndice < this->muwIndice;udIndice++)
         {
            ret << "\t    " << mCadenas[this->muwIndice - udIndice - 1] << endl;
         }
      }
      else
      {
         ret << "El stack de Excepciones esta vacio.\n";
      }

      ret.flush();

      return ret.str().c_str();
   }

   void Excepcion::LimpiarStackTrace()
   {
      this->muwIndice = 0;
   }

   void Excepcion::InsertaElemento(const char* str)
   {

      // Si la pila esta llena ignoramos la excepcion entrante
       if(this->muwIndice < LON_MAX_PILA)
       {
          memcpy(&mCadenas[this->muwIndice],str,Cadena::LON_MAX_STRING);
          this->muwIndice++;
       }
   }

   void Excepcion::CopiarStackExcepciones( Excepcion* peExcepcion)
   {
      UDWordType lNumExcepciones;
      UDWordType lIndice;

      if(peExcepcion != NULL)
      {
         // Miramos el numero de excepciones que nos entra la anterior
         // excepcion.
         lNumExcepciones = peExcepcion->getNumeroExcepciones();
         for(lIndice = 0;lIndice < lNumExcepciones; lIndice++)
         {
          // Insertamos el nuevo elemento
           InsertaElemento( peExcepcion->getExcepcion (lIndice) );
         }
      }
   }


// =======================
//  FIN DEL FICHERO - Excepcion.cpp
// =======================
