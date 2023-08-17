
//------------------------------------------------------------------------------
// GestorSICK.cpp
//------------------------------------------------------------------------------
#include "GestorSICK.h" 


//------------------------------------------------------------------------------
GestorSICK::GestorSICK()
{
   mbConnected=false;
}
//------------------------------------------------------------------------------
GestorSICK::~GestorSICK()
{
   finalizar();
}
//------------------------------------------------------------------------------
void GestorSICK::inicializar()
{
	/*
   mConfig.inicializar();
   mConfig.mostrarInfo();

   mcadIP=mConfig.getIP();
   mudwTcpport=mConfig.getTcpPort();
  
   thConectar.setMetodo(this,&GestorSICK::conectar);
   thConectar.iniciar();

   thLeer.setMetodo(this,&GestorSICK::leer);
   thLeer.iniciar();
   */

}

//------------------------------------------------------------------------------
void GestorSICK::finalizar()
{/*
   thConectar.destruir();
   thConectar.esperarFinalHilo();
   thLeer.destruir();
   thLeer.esperarFinalHilo();

   mSocket.desconectar();
   mSocket.cerrar();
   */
}


//------------------------------------------------------------------------------
void GestorSICK::conectar()
{
/*
   char bufferRx[SK_BUFFER_MAX];
   SICK::sSICKMsgHeader* ptrH;
   char * ptr;
   UDWordType udwNumChr=0;

   while (true)
   {
   
      while (!mbConnected)
      {
         printf("**Inicio de la conexion con el SICK [%s]:[%d]**",mcadIP.getCadena(), mudwTcpport );
   
         try
         {
            mSocket.setDireccionRemota(mcadIP.getCadena());
            mSocket.setPuertoRemoto(mudwTcpport);
            mSocket.setProtocolo(Socket::TCP);
            mSocket.setTimeoutConectar(10000);
            mSocket.abrir();
            mSocket.conectar();
            mSocket.unsetTimeoutConectar();
            STrz::basic(ConstTraza::T_TRAZA_SICK, "Socket Conectado");
   
            //*********** Inicializacion************
            enviando_inicializacion();
            //********* Receive Information ********
            memset(bufferRx, 0, SK_BUFFER_MAX);
            udwNumChr = mSocket.recibir(bufferRx,SK_BUFFER_MAX);
            ptrH = (SICK::sSICKMsgHeader*) bufferRx;
            ptr  = (char *) bufferRx;
   
            if (ptrH->uwMsgType == SICK::INFORMATION)
            {
               parse_informacion(ptr);
               //******** Envio Configuracion*******
               mbConnected=true;
               sleep(2);
               enviando_configuracion();
               mbConnected=true;
               DB_conectar ();
            }
            else if (ptrH->uwMsgType == SICK::ERROR)
            {
               STrz::basic(ConstTraza::T_TRAZA_SICK, "Recepcion de Msg:Error luego de Inicializacion");
               parse_informacion(ptr);
               mbConnected=false;
            }
            else
            {
               STrz::basic(ConstTraza::T_TRAZA_SICK, "Recepcion de Msg:Desconocido");
               mbConnected=false;
            }  
         }
         catch (Excepcion& e )
         {
            mbConnected=false;
            STrz::basic(ConstTraza::T_TRAZA_SICK, "Excepcion: Error en la conexion con el SICK");
         }
      
         if (!mbConnected)
         {
            STrz::basic(ConstTraza::T_TRAZA_SICK, "Esperamos 20 segundos y lo intentamos de nuevo");
            try {mSocket.desconectar();} catch (Excepcion& e){}
            try {mSocket.cerrar();} catch (Excepcion& e){}
            DB.desconectar();
            sleep(20);
         }

         STrz::basic(ConstTraza::T_TRAZA_SICK, "Fin Inicializacion");
      }
      sleep(1);
   }
   */
}


//------------------------------------------------------------------------------
void GestorSICK::leer()
{
	/*
   char bufferRx[SK_BUFFER_MAX];
   UDWordType udwNumChr;
   SICK::sSICKMsgHeader*  ptrH;   
   char * ptr;



   while (true)
   {

      while (! mbConnected){sleep(1);}
      
      while(mbConnected)
      {
         try
         {
            memset(&bufferRx, 0, SK_BUFFER_MAX);
            udwNumChr = mSocket.recibir(bufferRx,SK_BUFFER_MAX);
            ptrH = (SICK::sSICKMsgHeader*) bufferRx;
            ptr = (char *) bufferRx;
      
            STrz::basic(ConstTraza::T_TRAZA_SICK, "Recibiendo respuesta Server: recibido [%d] bytes",udwNumChr);
            STrz::basic(ConstTraza::T_TRAZA_SICK, "Requeridos: [%d]",ptrH->udwLength);
            STrz::basic(ConstTraza::T_TRAZA_SICK, "--------------------------------------");
            
            if (ptrH->uwMsgType == SICK::HEARTBEAT)                {parse_heart_beat(ptr);}
            else if (ptrH->uwMsgType == SICK::VEHICLE_RESULT)      {parse_vehicle_result(ptr);}
            else if (ptrH->uwMsgType == SICK::VEHICLE_EXT_RESULT)  {parse_vehicle_ext_result(ptr);}
            else if (ptrH->uwMsgType == SICK::OBJECT_START)        {parse_object_start(ptr);}
            else if (ptrH->uwMsgType == SICK::OBJECT_STOP)         {parse_object_stop(ptr);}
            else if (ptrH->uwMsgType == SICK::OBJECT_LOCATION)     {parse_object_location(ptr);}
            else if (ptrH->uwMsgType == SICK::STATUS)              {parse_status(ptr);}
            else {STrz::basic(ConstTraza::T_TRAZA_SICK, "Desconocido[%d]",ptrH->uwMsgType);}
         }
         catch(Excepcion& e)
         {
            mbConnected=false;
            DB.desconectar();
            STrz::basic(ConstTraza::T_TRAZA_SICK, "Excepcion al Recibir mensaje");
         }
      }
   } 
   */
}



//------------------------------------------------------------------------------
void  GestorSICK::parse_vehicle_result(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Vehicle Result] <<<");

   SICK::sSICKMsgVehicleResult sMsgVR;
   Cadena cadSQL;
   FechaHora ahora;

   ptr += 8;
   ahora.getAhora();

   sMsgVR.obj_id             = *((unsigned long long*) (ptr));              ptr += sizeof(S64Type);
   sMsgVR.road_nro           = *((SDWordType*) (ptr));                      ptr += sizeof(SDWordType);
   sMsgVR.lane_nro           = *((SDWordType*) (ptr));                      ptr += sizeof(SDWordType);
   sMsgVR.head_nro           = *((SDWordType*) (ptr));                      ptr += sizeof(SDWordType);
   sMsgVR.date_time_front    = getTimeStamp((unsigned long long *) (ptr));  ptr += sizeof(S64Type);
   sMsgVR.date_time_rear     = getTimeStamp((unsigned long long *) (ptr));  ptr += sizeof(S64Type);
   sMsgVR.direction          = *((UByteType*)(ptr));                        ptr += sizeof(UByteType);
   sMsgVR.ancho              = *((float*) ptr);                             ptr += sizeof(float);
   sMsgVR.alto               = *((float*) ptr);                             ptr += sizeof(float);
   sMsgVR.largo              = *((float*) ptr);                             ptr += sizeof(float);
   sMsgVR.has_trailer        = *((UByteType*)  ptr);                        ptr += sizeof(UByteType);
   sMsgVR.trailer_position   = *((float*) ptr);                             ptr += sizeof(float);ptr=ptr + 3;
   sMsgVR.speed              = *((float*) ptr);                             ptr += sizeof(float);
   sMsgVR.idle_time          = *((SDWordType*) ptr);                        ptr += sizeof(SDWordType);
   sMsgVR.lane_prop          = *((SDWordType*) ptr);                        ptr += sizeof(SDWordType);
   sMsgVR.categoria_int      = *((SDWordType*) ptr);                        ptr += sizeof(SDWordType);
   sMsgVR.categoria_ext      = *((SDWordType*) ptr);                        ptr += sizeof(SDWordType);
   sMsgVR.head_center        = *((float*) ptr);                             ptr += sizeof(float);
   sMsgVR.lane_center        = *((float*) ptr);                             ptr += sizeof(float);
   sMsgVR.road_center        = *((float*) ptr);                             ptr += sizeof(float);


   //show_msg_vehicle 
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-obj_id:.................[%lld]",sMsgVR.obj_id);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-road_nro:...............[%d]",sMsgVR.road_nro);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-lane_nro:...............[%d]",sMsgVR.lane_nro);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-head_nro:...............[%d]",sMsgVR.head_nro);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-date_time_front:........[%s]",sMsgVR.date_time_front.getCadena());
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-date_time_rear:.........[%s]",sMsgVR.date_time_rear.getCadena());
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-direction:..............[%d]",sMsgVR.direction);  
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-ancho:..................[%f]",sMsgVR.ancho);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-alto:...................[%f]",sMsgVR.alto);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-largo:..................[%f]",sMsgVR.largo);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-has_trailer:............[%d]",sMsgVR.has_trailer);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-trailer_position:.......[%f]",sMsgVR.trailer_position);
   float speed_k_h = sMsgVR.speed * 3600/1000;
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-speed:..................[%f m/s] [%f km/h] ",sMsgVR.speed, speed_k_h);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-idle_time:..............[%d]",sMsgVR.idle_time);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-lane_prop:..............[%d]",sMsgVR.lane_prop);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-categoria_int:..........[%d]",sMsgVR.categoria_int);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-categoria_ext:..........[%d]",sMsgVR.categoria_ext);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-head_center:............[%f]",sMsgVR.head_center);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-lane_center:............[%f]",sMsgVR.lane_center);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-road_center:............[%f]",sMsgVR.road_center);


   cadSQL.formCadena("INSERT INTO vehiculo_sick_laser (obj_id,event_reg_time,road_nro,lane_nro,head_nro,date_time_front,date_time_rear,direction,ancho,alto,largo,has_trailer,trailer_position,speed_m_s,speed_k_h,idle_time_veh,lane_prop,categoria_int,categoria_ext,head_center,lane_center,road_center) values (%lld,%s,%d,%d,%d,'%s','%s',%d,%f,%f,%f,%d,%f,%f,%f,%d,%d,%d,%d,%f,%f,%f);",
   sMsgVR.obj_id,
   BaseDatos::fechaHora (ahora).getCadena(),
   sMsgVR.road_nro,
   sMsgVR.lane_nro,
   sMsgVR.head_nro,
   sMsgVR.date_time_front.getCadena(),
   sMsgVR.date_time_rear.getCadena(),
   sMsgVR.direction,
   sMsgVR.ancho,
   sMsgVR.alto,
   sMsgVR.largo,
   sMsgVR.has_trailer ? 1 : 0 ,
   sMsgVR.trailer_position,
   sMsgVR.speed,
   speed_k_h,
   sMsgVR.idle_time,
   sMsgVR.lane_prop,
   sMsgVR.categoria_int,
   sMsgVR.categoria_ext,
   sMsgVR.head_center,
   sMsgVR.lane_center,
   sMsgVR.road_center);

   DB_consulta(cadSQL); 
         */
}

//------------------------------------------------------------------------------
void  GestorSICK::parse_vehicle_ext_result(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Vehicle Extended Result] <<<");

   SICK::sSICKMsgVehicleExtResult sMsgVExR;
   SICK::sSICKMsgVehicleExtResult_Bin sMsgVExR_BIN;
   ptr += 8;

   sMsgVExR.i_width               = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_height              = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_length              = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_has_a_trailer       = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_trailer_pos         = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_speed               = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_idle_time           = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_category            = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_ext_category        = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_axels_hauler        = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_axels_trailer       = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_axels               = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_weight_hauler       = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_weight_trailer      = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.i_weight              = *((float*) ptr);          ptr += sizeof(float);
   sMsgVExR.nro_binarydata        = *((SDWordType*) ptr);     ptr += sizeof(SDWordType);

   
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_width:...........[%f]",sMsgVExR.i_width);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_height:..........[%f]",sMsgVExR.i_height);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_length:..........[%f]",sMsgVExR.i_length);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_has_a_trailer:...[%f]",sMsgVExR.i_has_a_trailer);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_trailer_pos:.....[%f]",sMsgVExR.i_trailer_pos);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_speed:...........[%f]",sMsgVExR.i_speed);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_idle_time:.......[%f]",sMsgVExR.i_idle_time);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_category:........[%f]",sMsgVExR.i_category);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_ext_category:....[%f]",sMsgVExR.i_ext_category);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_axels_hauler:....[%f]",sMsgVExR.i_axels_hauler);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_axels_trailer:...[%f]",sMsgVExR.i_axels_trailer);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_axels:...........[%f]",sMsgVExR.i_axels);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_weight_hauler:...[%f]",sMsgVExR.i_weight_hauler);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_weight_trailer:..[%f]",sMsgVExR.i_weight_trailer);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-i_weight:..........[%f]",sMsgVExR.i_weight);

   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-nro_binarydata:....[%d]",sMsgVExR.nro_binarydata);

   for (int i=0; i < sMsgVExR.nro_binarydata; i++)
   {  
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-----------------------------------");
      sMsgVExR_BIN.type           = *((UByteType*) ptr);      ptr += sizeof(UByteType);
      sMsgVExR_BIN.size           = *((SDWordType*) ptr);     ptr += sizeof(SDWordType);

      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-type:........[%d]", sMsgVExR_BIN.type);
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-title:.......[%s]", sMsgVExR_BIN.title);
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-size:........[%d]", sMsgVExR_BIN.size);

   }
   */
}


//------------------------------------------------------------------------------
void  GestorSICK::parse_object_start(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Object Start] <<<");

   FechaHora ahora;
   Cadena cadSQL;
   SICK::sSICKMsgObjectStart sMsgOS;

   ptr += 8;
   ahora.getAhora();


   sMsgOS.cadFecha_snd   = getTimeStamp((unsigned long long *) (ptr));   ptr += sizeof(U64Type);
   sMsgOS.cadFecha_evt   = getTimeStamp((unsigned long long *) (ptr));   ptr += sizeof(U64Type); 

   sMsgOS.obj_id         = *((unsigned long long*) (ptr));               ptr += sizeof(S64Type);
   sMsgOS.road_nro       = *((SDWordType*)(ptr));                        ptr += sizeof(SDWordType);
   sMsgOS.head_nro       = *((SDWordType*)(ptr));                        ptr += sizeof(SDWordType);
   sMsgOS.start_x        = *((float*)(ptr));                             ptr += sizeof(float);
   sMsgOS.stop_x         = *((float*)(ptr));

   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-snd_date_time:....[%s]",  sMsgOS.cadFecha_snd.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-evt_date_time:....[%s]",  sMsgOS.cadFecha_evt.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-obj_id:...........[%lld]",sMsgOS.obj_id);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-road_nro:.........[%d]",  sMsgOS.road_nro);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-head_nro:.........[%d]",  sMsgOS.head_nro);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-start_x:..........[%f]",  sMsgOS.start_x);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-stop_x:...........[%f]",  sMsgOS.stop_x);


   cadSQL.formCadena("INSERT INTO sick_object_start (obj_id,event_time,event_snd_time,event_reg_time,road_nro,head_nro,start_x,stop_x) values (%lld,'%s','%s',%s,%d,%d,%f,%f);",
   sMsgOS.obj_id,
   sMsgOS.cadFecha_evt.getCadena(),
   sMsgOS.cadFecha_snd.getCadena(),
   BaseDatos::fechaHora (ahora).getCadena(),
   sMsgOS.road_nro,
   sMsgOS.head_nro,
   sMsgOS.start_x,
   sMsgOS.stop_x);


   DB_consulta(cadSQL); 
   */

}

//------------------------------------------------------------------------------
void  GestorSICK::parse_object_stop(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Object Stop] <<<");

   FechaHora ahora;
   Cadena cadSQL;
   SICK::sSICKMsgObjectStop sMsgOStp;
   S64Type objLoctaionID[20];

   memset(objLoctaionID,0,20 * sizeof(S64Type));
   ahora.getAhora();
   ptr += 8;


   sMsgOStp.cadFecha_snd     = getTimeStamp((unsigned long long *) (ptr));   ptr += sizeof(U64Type);
   sMsgOStp.cadFecha_evt     = getTimeStamp((unsigned long long *) (ptr));   ptr += sizeof(U64Type);

   sMsgOStp.obj_id         = *((unsigned long long*) (ptr));               ptr += sizeof(S64Type);
   sMsgOStp.road_nro       = *((SDWordType*)(ptr));                        ptr += sizeof(SDWordType);
   sMsgOStp.head_nro       = *((SDWordType*)(ptr));                        ptr += sizeof(SDWordType);
   sMsgOStp.result         = *((SDWordType*)(ptr));                        ptr += sizeof(SDWordType);
   sMsgOStp.start_x        = *((float*)(ptr));                             ptr += sizeof(float);
   sMsgOStp.stop_x         = *((float*)(ptr));                             ptr += sizeof(float);
   sMsgOStp.nro_trk_ft_tgg = *((SDWordType*)(ptr));                        ptr += sizeof(SDWordType);

   for (int i=0; i< sMsgOStp.nro_trk_ft_tgg; i++)
   {
      objLoctaionID[i] = *((unsigned long long *)(ptr));                   ptr += sizeof(S64Type);
   } 


   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-snd_date_time:....[%s]",  sMsgOStp.cadFecha_snd.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-evt_date_time:....[%s]",  sMsgOStp.cadFecha_evt.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-obj_id:...........[%lld]",sMsgOStp.obj_id);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-road_nro:.........[%d]",  sMsgOStp.road_nro);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-head_nro:.........[%d]",  sMsgOStp.head_nro);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-result:...........[%d]",  sMsgOStp.result);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-start_x:..........[%f]",  sMsgOStp.start_x);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-stop_x:...........[%f]",  sMsgOStp.stop_x);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-nro trk frnt tgg:.[%d]",  sMsgOStp.nro_trk_ft_tgg);

   for (int i=0; i< sMsgOStp.nro_trk_ft_tgg; i++)
   {
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |--Id Object Location:....[%lld]",objLoctaionID[i]);
   }



   cadSQL.formCadena("INSERT INTO sick_object_stop (obj_id,event_time,event_snd_time,event_reg_time,road_nro,head_nro,start_x,stop_x,nro_trk_ft_tgg,trk_ft_1,trk_ft_2,trk_ft_3) values (%lld,'%s','%s',%s,%d,%d,%f,%f,%d,%lld,%lld,%lld);",
   sMsgOStp.obj_id,
   sMsgOStp.cadFecha_evt.getCadena(),
   sMsgOStp.cadFecha_snd.getCadena(),
   BaseDatos::fechaHora (ahora).getCadena(),
   sMsgOStp.road_nro,
   sMsgOStp.head_nro,
   sMsgOStp.start_x,
   sMsgOStp.stop_x,
   sMsgOStp.nro_trk_ft_tgg,
   objLoctaionID[0],
   objLoctaionID[1],
   objLoctaionID[2]);


   DB_consulta(cadSQL); 
   */
}

//------------------------------------------------------------------------------
void  GestorSICK::parse_object_location(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Object Location] <<<");

   FechaHora ahora;
   Cadena cadSQL;
   SICK::sSICKMsgObjectLocation sMsgOLoc;

   ahora.getAhora();
   ptr += 8;

   sMsgOLoc.cadFecha_snd         = getTimeStamp((unsigned long long *) (ptr));   ptr += sizeof(U64Type);
   sMsgOLoc.cadFecha_evt         = getTimeStamp((unsigned long long *) (ptr));   ptr += sizeof(U64Type);
   sMsgOLoc.obj_id             = *((unsigned long long*) (ptr));                 ptr += sizeof(S64Type);
   sMsgOLoc.road_nro           = *((SDWordType*)(ptr));                          ptr += sizeof(SDWordType);
   sMsgOLoc.head_nro           = *((SDWordType*)(ptr));                          ptr += sizeof(SDWordType);
   sMsgOLoc.trigger_location   = *((float*)(ptr));                               ptr += sizeof(float);
   sMsgOLoc.location           = *((float*)(ptr));                               ptr += sizeof(float);
   sMsgOLoc.moving_direction   = *((SByteType*)(ptr));                           ptr += sizeof(SByteType);
   sMsgOLoc.accuracy           = *((SByteType*)(ptr));                           ptr += sizeof(SByteType);


   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-snd_date_time:....[%s]",  sMsgOLoc.cadFecha_snd.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-evt_date_time:....[%s]",  sMsgOLoc.cadFecha_evt.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-obj_id:...........[%lld]",sMsgOLoc.obj_id);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-road_nro:.........[%d]",  sMsgOLoc.road_nro);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-head_nro:.........[%d]",  sMsgOLoc.head_nro);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-trigger_location:.[%f]",  sMsgOLoc.trigger_location);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-location:.........[%f]",  sMsgOLoc.location);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-moving_direction:.[%d]",  sMsgOLoc.moving_direction);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-accuracy:.........[%d]",  sMsgOLoc.accuracy);


   cadSQL.formCadena("INSERT INTO sick_object_location (obj_id,event_time,event_snd_time,event_reg_time,road_nro,head_nro,trigger_location,location,moving_direction,accuracy) values (%lld,'%s','%s',%s,%d,%d,%f,%f,%d,%d);",
   sMsgOLoc.obj_id,
   sMsgOLoc.cadFecha_evt.getCadena(),
   sMsgOLoc.cadFecha_snd.getCadena(),
   BaseDatos::fechaHora (ahora).getCadena(),
   sMsgOLoc.road_nro,
   sMsgOLoc.head_nro,
   sMsgOLoc.trigger_location,
   sMsgOLoc.location,
   sMsgOLoc.moving_direction,
   sMsgOLoc.accuracy);


   DB_consulta(cadSQL); 

   */
}



//------------------------------------------------------------------------------
void  GestorSICK::parse_error(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Error] <<<");

   SICK::sSICKMsgError*      ptrEr;

   ptr   = ptr+sizeof(SICK::sSICKMsgHeader);
   ptrEr = (SICK::sSICKMsgError*) ptr;

   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-error_code:......[%d]", ptrEr->swType);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-error_message:...[%s]", ptrEr->cDescription);
   */
}
//------------------------------------------------------------------------------
void  GestorSICK::parse_informacion(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Informacion] <<<");

   SICK::sSICKMsgInfo_part1* ptrIP1;
   SICK::sSICKMsgInfo_Road*  ptrIRd;
   SICK::sSICKMsgInfo_Lane*  ptrILn;

   ptr = ptr+sizeof(SICK::sSICKMsgHeader);
   ptrIP1 = (SICK::sSICKMsgInfo_part1*) ptr;

   UDWordType vp1,vp2;
   vp1 =  ptrIP1->server_version; vp1 = vp1 >> 16;
   vp2 =  ptrIP1->server_version; vp2 = vp2 & 0x00FF;

   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-server_version: [%d.%d]",  vp1,vp2);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-name: [%s]",            ptrIP1->name);
   STrz::basic(ConstTraza::T_TRAZA_SICK, "|-number_of_roads: [%d]", ptrIP1->number_of_roads);
   
   ptr = ptr + sizeof(SICK::sSICKMsgInfo_part1);
   ptrIRd = (SICK::sSICKMsgInfo_Road *) ptr;

   //--------------------------------------------------------
   //--------------------Road Section------------------------
   //--------------------------------------------------------
   for (int i=0; i< ptrIP1->number_of_roads; i++)
   {
      if (i!=0) {ptr = ptr + sizeof(SICK::sSICKMsgInfo_Road);};
      STrz::basic(ConstTraza::T_TRAZA_SICK, "-------Road Nro:[%d]---------------",i);
      STrz::basic(ConstTraza::T_TRAZA_SICK, "   |-name:...........[%s]",   ptrIRd->name);
      STrz::basic(ConstTraza::T_TRAZA_SICK, "   |-direction:......[%s]",   ptrIRd->direction);
      STrz::basic(ConstTraza::T_TRAZA_SICK, "   |-location:.......[%s]",   ptrIRd->location);
      STrz::basic(ConstTraza::T_TRAZA_SICK, "   |-gps_latitude:...[%f]",   ptrIRd->gps_latitude);
      STrz::basic(ConstTraza::T_TRAZA_SICK, "   |-gps_longitud:...[%f]",   ptrIRd->gps_longitud);
      STrz::basic(ConstTraza::T_TRAZA_SICK, "   |-number_of_lanes:[%d]",   ptrIRd->number_of_lanes);

      ptr = ptr + sizeof(SICK::sSICKMsgInfo_Road);
      ptrILn = (SICK::sSICKMsgInfo_Lane *) ptr;
      //Lane per Road---------
      for (int j=0; j< ptrIRd->number_of_lanes; j++)
      {
         if (j!=0) {ptr = ptr + sizeof(SICK::sSICKMsgInfo_Lane);};
         STrz::basic(ConstTraza::T_TRAZA_SICK, "   ---------Lane Nro:[%d]---------",j);
         STrz::basic(ConstTraza::T_TRAZA_SICK, "      |-width:.........[%f]",   ptrILn->lane_width);
         STrz::basic(ConstTraza::T_TRAZA_SICK, "      |-direction:.....[%c]",   ptrILn->lane_direction);
      }
   }*/
}               

//------------------------------------------------------------------------------
void  GestorSICK::parse_heart_beat(char * ptr)
{
	
	/*STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [HeartBeat] <<<");

   SICK::sMsgHeartBeat* ptrHB;
   ptrHB = (SICK::sMsgHeartBeat*) ptr;
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-secuencia[%d]",ptrHB->sHBeat.sequence);
   */
}



//------------------------------------------------------------------------------
void  GestorSICK::parse_status(char * ptr)
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, "<<< [Object Status] <<<");

   SICK::sSICKMsgStatus sMsgSt;
   SICK::sSICKMsgStatus_head sMsgST_h;
   Cadena cadFecha;

   cadFecha = getTimeStamp((unsigned long long *) (ptr+8));
   sMsgSt.system_state  = *(ptr+8+8);
   sMsgSt.nro_heads     = *(ptr+8+8+1);

   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-date:[%s]",cadFecha.getCadena());
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-system state[%d]",sMsgSt.system_state);
   STrz::debug(ConstTraza::T_TRAZA_SICK, "|-nro heads [%d]",sMsgSt.nro_heads);

   ptr = ptr + 8 + 8 + 1 + 4;
   for (int i= 0; i < sMsgSt.nro_heads; i++)
   {
      sMsgST_h.state_head = *(ptr);
      sMsgST_h.state_lms1_ort = *(ptr+1);
      sMsgST_h.state_lms2_long = *(ptr+2);
      
      if (i!=0) {ptr = ptr+3;}
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-head[%d]:.....[%d]",i,sMsgST_h.state_head);
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-laser ort:....[%d]",sMsgST_h.state_lms1_ort);
      STrz::debug(ConstTraza::T_TRAZA_SICK, "   |-laser long:...[%d]",sMsgST_h.state_lms2_long);
   }*/
}

//------------------------------------------------------------------------------
void  GestorSICK::enviando_inicializacion()
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, ">>> [Inicializacion] >>>");

   char bufferTx[SK_BUFFER_MAX];
   memset(bufferTx, 0, SK_BUFFER_MAX);

   SICK::sMsgInit sInit;
   sInit.sHeader.ubst_mk   = SICK::SICK_ST_MK;
   sInit.sHeader.ubUnused  = SICK::SICK_UNUSED;
   sInit.sHeader.uwMsgType = (UWordType) SICK::INITIALIZATION;
   sInit.sHeader.udwLength = sizeof(UDWordType);
   sInit.udwVersion        = (UDWordType) SICK::SICK_VERSION;

   memcpy (bufferTx,&sInit,sizeof(SICK::sMsgInit));
   mSocket.enviar(bufferTx, sizeof(SICK::sMsgInit));
   */

}

//------------------------------------------------------------------------------
void  GestorSICK::enviando_configuracion()
{
	/*
   STrz::basic(ConstTraza::T_TRAZA_SICK, ">>> [Configuracion] >>>");

   char bufferTx[SK_BUFFER_MAX];
   memset(bufferTx, 0, SK_BUFFER_MAX);

   //Header
   SICK::sMsgConfig sConfig;
   sConfig.sHeader.ubst_mk   = SICK::SICK_ST_MK;
   sConfig.sHeader.ubUnused  = SICK::SICK_UNUSED;
   sConfig.sHeader.uwMsgType = (UWordType) SICK::CONFIGURATION;
   sConfig.sHeader.udwLength = 116;

   //Body
   strcpy (sConfig.sInfo.client_identification,mConfig.getName().getCadena());
   sConfig.sInfo.heart_beat_interval    = mConfig.getHBInterval();
   sConfig.sInfo.msg_format             = mConfig.getFormat();
   sConfig.sInfo.binary_data_section    = mConfig.getBinaryData();
   sConfig.sInfo.object_st              = 1;//mConfig.getOBJSt();
   sConfig.sInfo.object_lc              = 1;//mConfig.getOBJLoc();
   sConfig.sInfo.status                 = 1;//mConfig.getHBInterval();
   sConfig.sInfo.status_update_interval = mConfig.getSTInterval();

   char * p = (char *) bufferTx;
   memcpy (p,&sConfig.sHeader,8);p=p+sizeof(sConfig.sHeader);

   memcpy (p,&sConfig.sInfo.client_identification,100);  p += 100;

   memcpy (p,&sConfig.sInfo.heart_beat_interval,4);      p += 4;
   memcpy (p,&sConfig.sInfo.msg_format,1);               p += 1;
   memcpy (p,&sConfig.sInfo.binary_data_section,4);      p += 4;
   memcpy (p,&sConfig.sInfo.object_st,1);                p += 1;
   memcpy (p,&sConfig.sInfo.object_lc,1);                p += 1;
   memcpy (p,&sConfig.sInfo.status,1);                   p += 1;
   memcpy (p,&sConfig.sInfo.status_update_interval,4);   p += 4;

   mSocket.enviar(bufferTx, 124);
   */
}

//------------------------------------------------------------------------------
Cadena  GestorSICK::getTimeStamp( unsigned long long * ptr )
{
	
   Cadena cadFecha;
   /*
   unsigned long long segundos_; 
   unsigned long long microsegs_; 
   unsigned long long deci_micros_from_1970 = 621355968000000000ULL;
                                              
   segundos_ = *ptr - deci_micros_from_1970;

   segundos_ = segundos_ / 10000000;
   microsegs_ = *ptr % 10000000;microsegs_ = microsegs_/10;

   FechaHora fecha(segundos_,microsegs_);
   cadFecha = fecha.getCadena(FechaHora::DD_MM_AA_HH_MM_SS_MMM);

   cadFecha.ReemplazarCadena(",",".");
   */
   return cadFecha;
   
}

//------------------------------------------------------------------------------
bool GestorSICK::DB_conectar()
{
	/*
   try
   {
      DB.conectar (DBcfg.getHostname (), DBcfg.getPuerto (), DBcfg.getNombreBaseDatos (),
       DBcfg.getUsuario (), DBcfg.getContrasenya (),DBcfg.getTimeout ());
   }
   catch (Excepcion& e)
   {
      STrz::basic (ConstTraza::T_TRAZA_SICK,"Error: Problemas conectando con la Base de Datos");
      return false;
   }*/
   return true;
   

}


//------------------------------------------------------------------------------
bool GestorSICK::DB_consulta(Cadena cadSQL)
{
	
   bool bResp=true;
   /*
   try
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK, "DB->[%s]", cadSQL.getCadena());
      bResp=DB.ejecutarConsulta(cadSQL);

      if (!SICK::validarRegexp ("UPDATE", cadSQL.getCadena()))
         STrz::basic(ConstTraza::T_TRAZA_SICK, "Registros afectados[%d]",DB.getNumRegistros());
      else
         STrz::basic(ConstTraza::T_TRAZA_SICK, "Registros afectados[%d]",DB.getNumRegistrosAfectados());
   }
   catch (Excepcion& e)
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK, "Error: Problemas consultando la base de datos");
      bResp=false;
   }
   */
   return bResp;

   
}


//---------------------------FIN------------------------------------------------

