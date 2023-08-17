/*****************************************************************************/
/*                                                                           */
/*   PEAJE DINAMICO                                                          */
/*                                                                           */
/*   FICHERO      : carascii.lit                                             */
/*                                                                           */
/*   DESCRIPCION  : Literales correspondientes a los caracteres especiales   */
/*                  utilizados                                               */
/*                                                                           */
/*   AUTOR        : J. Galera                                                */
/*                                                                           */
/*   FECHA        : 22 Nov 1990                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef _Carascii
#define _Carascii

/*
    --------------- GENERALES ----------------------
*/
                             /* Nulo            */
#define NUL             0x00
                             /* Inicio de cabecera */
#define SOH             0x01
                             /* Inicio de texto */
#define STX             0x02
                             /* Fin de texto */
#define ETX             0x03
                             /* Fin de transmision */
#define EOT             0x04
                             /* Pregunta           */
#define ENQ             0x05
                             /* Respuesta */
#define ACK             0x06
                             /* Bell      */
#define BEL             0x07
                             /* Back space */
#define BS              0x08
                             /* Tabulador horizontal */
#define HT              0x09
                             /* Avance de linea */
#define LF              0x0a
                             /* Tabulador vertical */
#define VT              0x0b
                             /* Avance pagina */
#define FF              0x0c
                             /* Retorno de carro */
#define CR              0x0d
                             /* Desplazamiento inactivo */
#define SO              0x0e
                             /* Desplazamiento activo */
//ncg. ojo! provoca ambiguedad
//#define SI              0x0f
                             /* Data link escape */
#define DLE             0x10
                             /* Control dispositivo 1 */
#define DC1             0x11
                             /* Control dispositivo 2 */
#define DC2             0x12
                             /* Control dispositivo 3 */
#define DC3             0x13
                             /* Control dispositivo 4 */
#define DC4             0x14
                             /* Respuesta negativa */
#define NACK            0x15
                             /* Sincronismos de idle */
#define SYN             0x16
                             /* Fin de bloque de transmision */
#define ETB             0x17
                             /* Cancelar */
#define CAN             0x18
                             /* Fin de medio */
#define EM              0x19
                             /* Sustituir    */
#define SUB             0x1a
                             /* Escape */
#define ESC             0x1b
                             /* Separador de fichero */
#define FS              0x1c
                             /* Separador de grupo */
#define GS              0x1d
                             /* Separador de registro */
#define RS              0x1e
                             /* Separador de unidad */
#define US              0x1f
                                                                                /* Wake up */
#define WUP                                     0x12

#endif
