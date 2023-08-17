
#ifndef SICK_H
#define SICK_H

#include "../../lang/Types.h"
#include "../../lang/Cadena.h"
//#include <regex.h>
#include <map>
#include <vector>

//------------------------------------------------------------------------------
/**
 * <PRE>
  *
  * FICHERO: SICK.h
  *
  * DESCRIPCION:  Datos para el Protocolo de comunicacion LASER-SICK
  *
  * FECHA CREACION: 20/01/2015
  *
  * AUTOR DISENYO: Adrian Casini
  *
  * PROGRAMADOR:   Adrian Casini
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
//------------------------------------------------------------------------------

class SICK
{

   public:
   
      //Header
      static const UByteType   SICK_ST_MK   = 0x02;
      static const UByteType   SICK_UNUSED  = 0x00;
      //Version
      static const UDWordType  SICK_VERSION = 0x00010002;
      //Tipo de errores en la conexion
      static const SDWordType  SICK_ERR_UNKNOWN         = 0;
      static const SDWordType  SICK_ERR_VMISTMATCH      = 1;
      static const SDWordType  SICK_ERR_TO_MANY_CLIENTS = 2;
      static const SDWordType  SICK_ERR_ACESS_DENIED    = 3;
      
      
      enum eSICKMsgType 
      {
         INITIALIZATION     = 0,
         INFORMATION        = 1,
         ERROR              = 2,
         CONFIGURATION      = 3,
         HEARTBEAT          = 4,
         VEHICLE_RESULT     = 100,
         VEHICLE_EXT_RESULT = 101,
         OBJECT_START       = 200,
         OBJECT_STOP        = 201,
         OBJECT_LOCATION    = 202,
         STATUS             = 300
      };
      
      struct sSICKMsgHeader
      {
         UByteType  ubst_mk;
         UByteType  ubUnused;
         UWordType  uwMsgType;
         UDWordType udwLength;
      
      };
      
      //--------------------------------------------------------------------
      //------        Error                                         --------
      //--------------------------------------------------------------------
      struct sSICKMsgError
      {
         SDWordType  swType;
         char        cDescription[300];
      };


      //--------------------------------------------------------------------
      //------        Init                                          --------
      //--------------------------------------------------------------------
      struct sMsgInit
      {
         sSICKMsgHeader sHeader;
         UDWordType     udwVersion;
      };
      
      //--------------------------------------------------------------------
      //------        Config                                        --------
      //--------------------------------------------------------------------

      struct sSICKMsgConfig
      {
         char               client_identification[100];
         SDWordType         heart_beat_interval;//miliseconds
         UByteType          msg_format;
         UDWordType         binary_data_section;
         bool               object_st;
         bool               object_lc;
         bool               status;
         SDWordType         status_update_interval;//miliseconds
      };

      struct sMsgConfig
      {
         sSICKMsgHeader sHeader;
         sSICKMsgConfig sInfo;
      };


      enum eBinData 
      {
         BINARY_3D    = 0x0001,
         VEHICLE_DATA = 0x0002,
         JPEG         = 0x0004,
      };

      enum eFormat 
      {
         DISABLED    = 0x0000,
         SIMPLE      = 0x0001,
         EXTENDED    = 0x0002,
      };

      //--------------------------------------------------------------------
      //------        Heart Beat                                    --------
      //--------------------------------------------------------------------

      struct sSICKMsgHeartBeat
      {
         UDWordType  sequence;
      };

      struct sMsgHeartBeat
      {
         sSICKMsgHeader    sHeader;
         sSICKMsgHeartBeat sHBeat;
      };
      
      //--------------------------------------------------------------------
      //------        Status                                         --------
      //--------------------------------------------------------------------

      struct sSICKMsgStatus
      {
         U64Type     msg_date_time;
         UByteType   system_state;
         SDWordType  nro_heads;
      };

      struct sSICKMsgStatus_head
      {
         UByteType   state_head;
         UByteType   state_lms1_ort;
         UByteType   state_lms2_long;
      };

      struct sMsgStatus
      {
         sSICKMsgHeader    sHeader;
         sSICKMsgStatus    sStatus;
      };
   
      //--------------------------------------------------------------------
      //------        Object Location                               --------
      //--------------------------------------------------------------------

      enum eMovingDir
      {
         MD_UNKNOWN=0,
         MD_TOWARDS=1,
         MD_AWAY=2,
      };
   
      enum eAccuracy  
      {
         AC_UNKNOWN=0,
         AC_ACCURATE=1,
         AC_ESTIMATED=2,
         AC_PHANTOM=3,
      };
   
      struct sSICKMsgObjectLocation
      {
         Cadena      cadFecha_snd;
         Cadena      cadFecha_evt;
         S64Type     obj_id;
         SDWordType  road_nro;
         SDWordType  head_nro;
         float       trigger_location;
         float       location;
         SByteType   moving_direction;
         SByteType   accuracy;
      };

      //--------------------------------------------------------------------
      //------        Object Start                                  --------
      //--------------------------------------------------------------------
      struct sSICKMsgObjectStart
      {
         Cadena      cadFecha_snd;
         Cadena      cadFecha_evt;
         S64Type     obj_id;
         SDWordType  road_nro;
         SDWordType  head_nro;
         float       start_x;
         float       stop_x;
      };

      //--------------------------------------------------------------------
      //------        Object Stop                                   --------
      //--------------------------------------------------------------------
      struct sSICKMsgObjectStop
      {
         Cadena      cadFecha_snd;
         Cadena      cadFecha_evt;
         S64Type     obj_id;
         SDWordType  road_nro;
         SDWordType  head_nro;
         SDWordType  result;      
         float       start_x;
         float       stop_x;
         SDWordType  nro_trk_ft_tgg;    //uno o mas U64Type
      };


      //--------------------------------------------------------------------
      //------        Vehicule--result                              --------
      //--------------------------------------------------------------------
      struct sSICKMsgVehicleResult
      {
         S64Type     obj_id;
      
         SDWordType  road_nro;
         SDWordType  lane_nro;
         SDWordType  head_nro;
      
         Cadena      date_time_front;
         Cadena      date_time_rear;
      
         UByteType   direction;
      
         float       ancho;
         float       alto;
         float       largo;
      
         bool        has_trailer;
         float       trailer_position;
      
         float       speed;
      
         SDWordType  idle_time;
         SDWordType  lane_prop;
      
         SDWordType  categoria_int;
         SDWordType  categoria_ext;
      
         float       head_center;
         float       lane_center;  
         float       road_center;
      };


      //--------------------------------------------------------------------
      //------        Vehicule-Extended-result                      --------
      //--------------------------------------------------------------------
      struct sSICKMsgVehicleExtResult
      {
         float       i_width;
         float       i_height;  
         float       i_length;
         float       i_has_a_trailer;
         float       i_trailer_pos;
         float       i_speed;
         float       i_idle_time;
         float       i_category;
         float       i_ext_category;
         float       i_axels_hauler;
         float       i_axels_trailer;
         float       i_axels;
         float       i_weight_hauler;
         float       i_weight_trailer;
         float       i_weight;
         SDWordType  nro_binarydata;

      };


      struct sSICKMsgVehicleExtResult_Bin
      {  
         UByteType   type;
         UByteType   title[200];
         SDWordType  size;
      };


      //--------------------------------------------------------------------
      //------        Information Message                           --------
      //--------------------------------------------------------------------
      struct sSICKMsgInfo_part1
      {
         SDWordType  server_version;
         char        name[100];
         SDWordType  number_of_roads;
      };
      
      struct sSICKMsgInfo_Road
      {
         char        name[100];
         char        direction[100];
         char        location[100];
      
         float       gps_latitude;
         float       gps_longitud;
      
         SDWordType  number_of_lanes;
      };
      
      
      struct sSICKMsgInfo_Lane
      {
         float       lane_width;
         UByteType   lane_direction;
      };
      
      
      struct sSICKMsgInfo_part2
      {
         UDWordType  number_of_lasers_heads;
      };
      
      struct sSICKMsgInfo_LaserHeads_part1
      {
         SDWordType sensor_type;
         SDWordType road_num;
         float      location;
      
         SDWordType number_of_devices;
      };
      
      struct sSICKMsgInfo_device
      {
         char       device_type[100];
         char       firmware_version[100];
         char       serial_number[100];
         char       ip_address[15];
         char       ip_mask[15];
         char       ip_gateway[15];
         SDWordType ip_host_port;
      };
      
      struct sSICKMsgInfo_LaserHeads_part2
      {
         SDWordType number_of_triggers;
      };
      
      struct sSICKMsgInfo_trigger
      {
         float triggers_position;
      };

   



   public:

      SICK();
      ~SICK();

      static bool validarRegexp (CPSByteType formato, CPSByteType str);
      static Cadena getCadPuntos(Cadena cadTexto);

};



//---------------------------

#endif
