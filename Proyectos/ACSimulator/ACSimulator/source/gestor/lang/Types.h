#ifndef TypesINT_H
#define TypesINT_H

#include "../../lang/Types.h"
#include <stdint.h>

#define VALOR_INVALIDO -1
#define LITERAL_INVALIDO "<INVALIDO>"
#define SIZE(a) sizeof(a) / sizeof(a[0])

typedef UByteType  NumeroConcesionaria;
typedef UWordType  NumeroEstacion;
typedef UByteType  NumeroPortico;
typedef UDWordType NumeroEmpleado;
typedef UWordType  CodigoAlarma;
typedef UByteType  NumeroTramo;
typedef uint64_t   IdentificadorOBE;

enum EstadoTelecargasTypeE {
   INICIALIZANDO = 0,
   ACTUALIZANDO = 1,
   ACTUALIZADAS = 2,
   ACTIVADAS = 3,
   ACTIVADAS_SIN_ACK = 4
};

enum EstadoComsTypeE {
   COMS_ON_LINE = 0,
   COMS_OFF_LINE = 1,
   COMS_DETENIDAS = 2,
   COMS_RECUPERACION
};

enum MotivosCambioEstadoTypeE {
   SIN_MOTIVO,
   AUSENCIA_TELECARGAS,
   ESPERA_PERIODOS,
   ALARMA_GRAVE,
   POR_PETICION_COBRADOR,
   BLOQUEO_ESTACION,
   PERIODOS_KO,
   SESION_CAJERO,
};

enum EstadoPorticoTypeE {
   ESTADO_ARRANQUE,
   ESTADO_CERRADO,
   ESTADO_FUERA_SERVICIO,
   ESTADO_ABIERTO,
   ESTADO_BLOQUEADO
};



enum EstadoArranqueTypeE {
   ARRANQUE_SIN_DATOS = 'S',
   ARRANQUE_GW_VACIO = 'C',
   ARRANQUE_GW_LLENO = 'G',
   ARRANQUE_CON_DATOS = 'E'
};

//enum EstadoPeriodoTypeE {PERIODO_PENDIENTE = 'P', PERIODO_TRANSMITIDO = 'T', PERIODO_ACK = 'K'};

struct VersionTypeS {
   UByteType mubCampo1;
   UByteType mubCampo2;
   UByteType mubCampo3;
};


enum TipoCobradorTypeE
{
   SIN_COBRADOR      = ' ',
   COBRADOR_LOCAL    = 'L',
   COBRADOR_REMOTO   = 'R',
   COBRADOR_GENERICO = 'G'
};

#endif

//ignoreSoap
#ifndef TypesFUNC_H
#define TypesFUNC_H
//__attribute__((__used__)) es para que el compilador no se queje de que no
//se usa la variable
//static const char* __attribute__((__used__)) TIPOS_VIA[NUM_TIPOS_VIAS] = {
//      "??", "CE", "AM", "CM", "AG", "CG", "AA", "CA", "AC", "CC", "AT", "CT",
//      "AF", "CF", "AV", "CV", "PK", "EE" };

static const ColorTypeE HOST_COLORS[][3] =  {
   { ALL_ATTRIBUTES_OFF, FG_RED,     BG_BLACK },
   { ALL_ATTRIBUTES_OFF, FG_GREEN,   BG_BLACK },
   { ALL_ATTRIBUTES_OFF, FG_YELLOW,  BG_BLACK },
   { ALL_ATTRIBUTES_OFF, FG_MAGENTA, BG_BLACK },
   { ALL_ATTRIBUTES_OFF, FG_CYAN,    BG_BLACK },

   { ALL_ATTRIBUTES_OFF, FG_RED,     BG_WHITE },
   { ALL_ATTRIBUTES_OFF, FG_BLUE,    BG_WHITE },
   { ALL_ATTRIBUTES_OFF, FG_MAGENTA, BG_WHITE },
   { ALL_ATTRIBUTES_OFF, FG_BLACK,   BG_WHITE },

   { ALL_ATTRIBUTES_OFF, FG_RED,     BG_BLUE },
   { ALL_ATTRIBUTES_OFF, FG_GREEN,   BG_BLUE },
   { ALL_ATTRIBUTES_OFF, FG_YELLOW,  BG_BLUE },
   { ALL_ATTRIBUTES_OFF, FG_MAGENTA, BG_BLUE },
   { ALL_ATTRIBUTES_OFF, FG_CYAN,    BG_BLUE },
   { BOLD_ON,            FG_WHITE,   BG_BLUE },

   { ALL_ATTRIBUTES_OFF, FG_RED,     BG_CYAN },
   { ALL_ATTRIBUTES_OFF, FG_YELLOW,  BG_CYAN },
   { ALL_ATTRIBUTES_OFF, FG_BLUE,    BG_CYAN },
   { ALL_ATTRIBUTES_OFF, FG_MAGENTA, BG_CYAN },
   { ALL_ATTRIBUTES_OFF, FG_BLACK,   BG_CYAN },

   { ALL_ATTRIBUTES_OFF, FG_RED,     BG_YELLOW },
   { ALL_ATTRIBUTES_OFF, FG_BLUE,    BG_YELLOW },
   { ALL_ATTRIBUTES_OFF, FG_CYAN,    BG_YELLOW },
   { ALL_ATTRIBUTES_OFF, FG_WHITE,   BG_YELLOW },
   { ALL_ATTRIBUTES_OFF, FG_BLACK,   BG_YELLOW },

   { ALL_ATTRIBUTES_OFF,  FG_GREEN,   BG_MAGENTA },
   { ALL_ATTRIBUTES_OFF,  FG_BLUE,    BG_MAGENTA },
   { ALL_ATTRIBUTES_OFF,  FG_CYAN,    BG_MAGENTA },
   { BOLD_ON,             FG_WHITE,   BG_MAGENTA },
   { ALL_ATTRIBUTES_OFF,  FG_BLACK,   BG_MAGENTA },

   { ALL_ATTRIBUTES_OFF, FG_GREEN,   BG_RED },
   { ALL_ATTRIBUTES_OFF, FG_BLUE,    BG_RED },
   { ALL_ATTRIBUTES_OFF, FG_CYAN,    BG_RED },
   { BOLD_ON,            FG_WHITE,   BG_RED },
   { ALL_ATTRIBUTES_OFF, FG_BLACK,   BG_RED },

   { ALL_ATTRIBUTES_OFF, FG_RED,     BG_GREEN },
   { ALL_ATTRIBUTES_OFF, FG_BLUE,    BG_GREEN },
   { ALL_ATTRIBUTES_OFF, FG_MAGENTA, BG_GREEN },
   { ALL_ATTRIBUTES_OFF, FG_BLACK,   BG_GREEN },

   { BOLD_ON, FG_RED,     BG_BLUE },
   { BOLD_ON, FG_GREEN,   BG_BLUE },
   { BOLD_ON, FG_YELLOW,  BG_BLUE },
   { BOLD_ON, FG_MAGENTA, BG_BLUE },
   { BOLD_ON, FG_CYAN,    BG_BLUE },
   { BOLD_ON, FG_WHITE,   BG_BLUE },
};

static const int HOST_COLOR_COUNT = sizeof( HOST_COLORS ) / sizeof( HOST_COLORS[ 0 ] );
const char* toStr(EstadoPorticoTypeE estado);

#endif
//fignoreSoap
