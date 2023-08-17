

//---------------------------------------------------------------------------
// ConfigSICK.cpp
//---------------------------------------------------------------------------
#include "ConfigSICK.h"  

//------------------------------------------------------------------------------
ConfigSICK::ConfigSICK()
{
}
//------------------------------------------------------------------------------
ConfigSICK::~ConfigSICK()
{
}

//------------------------------------------------------------------------------
void ConfigSICK::inicializar()
{
   FicheroMemoria fileMap;
   SByteType buf[1024];
   SDWordType cantChars = 0;
   SDWordType flagFinalFichero = 0;
   Cadena cadParametro;
   Cadena cadValor;
   Cadena cadAux;

   seteoDefault();

   Cadena cadCFG = getValor("$FF_CFG");
   if (!cadCFG.isEmpty())
   {
      Cadena cadFichConfig = cadCFG + Cadena("/") + Cadena("gestorSICK.cfg");
      if (Directorio::existe(cadFichConfig, Directorio::FICHERO))
      {
         try
         {
            fileMap.setNombre(cadFichConfig);
            fileMap.setLectura(true);
            fileMap.setExclusivo(false);
            fileMap.abrir();
            while(!flagFinalFichero)
            {
               cantChars +=  fileMap.leerLinea (cantChars, buf, 1024);
               if (SICK::validarRegexp("^#", buf)) {/*Comentario*/}
               else if (SICK::validarRegexp("^\\s+", buf)) {/*Linea vacia*/}
               else if (SICK::validarRegexp("^[A-Z]+", buf))
               {
                  cadAux.formCadena("%s", buf);
                  cadAux.dameTextoEntreSep(1, ';', cadParametro);
                  cadParametro.resuelveVariableEntorno();
                  cadAux.dameTextoEntreSep(2, ';', cadValor);
                  cadValor.resuelveVariableEntorno();
               }
               if ((!cadParametro.isEmpty()) && (!cadValor.isEmpty()))
               {mMapParametros[cadParametro] = cadValor;}
               if (cantChars >= (int)fileMap.getTamano()) {flagFinalFichero = 1;}
            }
            fileMap.cerrar();
         }
         catch (Excepcion& e )
         {
            STrz::basic( ConstTraza::T_TRAZA_SICK, e );
            STrz::basic( ConstTraza::T_TRAZA_SICK,
            "No se ha podido obtener informacion del fichero[%s]",cadFichConfig.getCadena() );
            fileMap.cerrar();
            seteoDefault();
         }
      }
      else
      {
         STrz::basic( ConstTraza::T_TRAZA_SICK,"Error No existe Fichero Configuracion: [%s]",cadFichConfig.getCadena());
         seteoDefault();
      }
   }
   else
   {
      STrz::basic( ConstTraza::T_TRAZA_SICK,"Variable Entorno $FF_CFG vacia");
      seteoDefault();
   }
}

//------------------------------------------------------------------------------
void ConfigSICK::seteoDefault()
{
   STrz::basic( ConstTraza::T_TRAZA_SICK,"WARNING!! Tomo Valores por defecto");
   
   mMapParametros["IP"]               = "127.0.0.1";
   mMapParametros["PORT"]             = "54345";
   mMapParametros["DIR_IMAGENES"]     = "/fflow/imagenes";

   mMapParametros["NAME"]             = "ABERTIS";
   mMapParametros["HB_INTERVAL"]      = "20000"; //miliseconds
   mMapParametros["ST_INTERVAL"]      = "50000"; //miliseconds
   mMapParametros["BINARY_DATA"]      = "JPEG";  //VEHICLE_DATA - BINARY_3D
   mMapParametros["FORMAT"]           = "SIMPLE";//DISABLED - EXTENDED
   mMapParametros["OBJ_ST"]           = "SI";
   mMapParametros["OBJ_LOC"]          = "SI";
   mMapParametros["STATUS"]           = "SI";
}

//------------------------------------------------------------------------------
Cadena ConfigSICK::getIP()               {return mMapParametros["IP"];}
//------------------------------------------------------------------------------
UDWordType ConfigSICK::getTcpPort()      {return mMapParametros["PORT"].toUDWordType();}
//------------------------------------------------------------------------------
Cadena ConfigSICK::getDirImagenes()      {return mMapParametros["DIR_IMAGENES"];}
//------------------------------------------------------------------------------
Cadena ConfigSICK::getName()             {return mMapParametros["NAME"];}
//------------------------------------------------------------------------------
UDWordType ConfigSICK::getHBInterval()   {return mMapParametros["HB_INTERVAL"].toUDWordType();}
//------------------------------------------------------------------------------
UDWordType ConfigSICK::getSTInterval()   {return mMapParametros["ST_INTERVAL"].toUDWordType();}
//------------------------------------------------------------------------------
bool ConfigSICK::getOBJSt()              {return ((mMapParametros["OBJ_ST"]   == "SI") ? true : false);}
//------------------------------------------------------------------------------
bool ConfigSICK::getOBJLoc()             {return ((mMapParametros["OBJ_LOC"]  == "SI") ? true : false);}
//------------------------------------------------------------------------------
bool ConfigSICK::getStatus()             {return ((mMapParametros["STATUS"]   == "SI") ? true : false);}
//------------------------------------------------------------------------------
UDWordType ConfigSICK::getFormat() 
{
    if      (mMapParametros["FORMAT"] == "DISABLED")
      return SICK::DISABLED;
    else if (mMapParametros["FORMAT"] == "SIMPLE")
      return SICK::SIMPLE;
    else if (mMapParametros["FORMAT"] == "EXTENDED")
      return SICK::EXTENDED;

   return SICK::SIMPLE;
}
//------------------------------------------------------------------------------
UDWordType ConfigSICK::getBinaryData() 
{
    if      (mMapParametros["BINARY_DATA"] == "JPEG")
      return SICK::JPEG;
    else if (mMapParametros["BINARY_DATA"] == "VEHICLE_DATA")
      return SICK::VEHICLE_DATA;
    else if (mMapParametros["BINARY_DATA"] == "BINARY_3D")
      return SICK::BINARY_3D;

   return SICK::VEHICLE_DATA;
}
   


//------------------------------------------------------------------------------
void ConfigSICK::mostrarInfo()
{
   MapParametros::const_iterator it;

   STrz::basic(ConstTraza::T_TRAZA_SICK, "==============================================");
   STrz::basic(ConstTraza::T_TRAZA_SICK, "==             CONFIGURACION                ==");
   STrz::basic(ConstTraza::T_TRAZA_SICK, "==============================================");

   for (it = mMapParametros.begin(); it != mMapParametros.end(); ++it)
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK, "  |->[%s][%s]",(it->first).getCadena(),(it->second).getCadena());
   }

}

//------------------------------------------------------------------------------
Cadena ConfigSICK::getValor(Cadena cadVarEntorno)
{
   try {return cadVarEntorno.resuelveVariableEntorno();}
   catch (Excepcion& e)
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK, "No existe variable:[%s]", cadVarEntorno.getCadena() ); return "";
   }
}


