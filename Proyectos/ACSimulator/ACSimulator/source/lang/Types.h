

//**************************************************************************//

// TIPOS A UTILIZAR EN PROGRAMAS                                            //

// ------------------------------                                           //

//                                                                          //

//    UByteType    1 byte  [0..255]                                         //

//    SByteType    1 byte  [-128..127]                                      //

//    UWordType    2 bytes [0..65535]                                       //

//    SWordType    2 bytes [-32768..32767]                                  //

//    UDWordType   4 bytes [0..4294967295]                                  //

//    SDWordType   4 bytes [-2147483648..2147483647]                        //

//    bool         1 byte  [true, false]                                    //

//    float        4 bytes [3.4x10^-38..3.4x10^+38]                         //

//    double       8 bytes [1.7x10^-308..1.7x10^308]                        //

//    long double 80 bits  [3.4x10^-4932..3.4x10^+4932]                     //

//                                                                          //

// TIPOS QUE NO SE DEBEN UTILIZAR                                           //

// ------------------------------                                           //

//                                                                          //

//    char     unsigned char     signed char                                //

//    short    unsigned short    signed short                               //

//    int      unsigned int      signed int                                 //

//    long     unsigned long     signed long                                //

//                                                                          //

// TIPOS FUTUROS                                                            //

// ------------                                                             //

//                                                                          //

//    SQWordType    8 bytes [-9223372036854775808..9223372036854775807]     //

//                                                                          //

// TIPOS ACTUALES EN LINUX ( C++ FOR RED HAT LINUX 6.0 IN INTEL PLATFORM )  //

// -----------------------                                                  //

//                                                                          //

//    bool           1 byte                                                 //

//    char           1 byte                                                 //

//    short          2 bytes                                                //

//    int            4 bytes                                                //

//    long           4 bytes                                                //

//    float          4 bytes                                                //

//    double         8 bytes                                                //

//    long double    80 bits                                                //

//                                                                          //

//**************************************************************************//

#ifndef Types_H

#define Types_H


    typedef unsigned char             UByteType;
    typedef const char                CSByteType;
    typedef char                      SByteType;

    typedef const unsigned short int  CUWordType;
    typedef unsigned short int        UWordType;
    typedef const short int           CSWordType;
    typedef short int                 SWordType;

    typedef const unsigned int        CUDWordType;
    typedef unsigned int              UDWordType;
    typedef const int                 CSDWordType;
    typedef int                       SDWordType;

    typedef const char*               CPSByteType;
    typedef char*                     PSByteType;


//ignoreSoap
    typedef const unsigned char       CUByteType;

    typedef const unsigned char*      CPUByteType;

    typedef unsigned char*            PUByteType;

    typedef const char*               CPSByteType;

    typedef char*                     PSByteType;



    typedef const unsigned short int* CPUWordType;

    typedef unsigned short int*       PUWordType;

    typedef const short int*          CPSWordType;

    typedef short int*                PSWordType;



    typedef const unsigned int*       CPUDWordType;

    typedef unsigned int*             PUDWordType;

    typedef const int*                CPSDWordType;

    typedef int*                      PSDWordType;

    typedef unsigned long long        U64Type;

    typedef signed long long          S64Type;



    CSDWordType    UByteTypeMin  = 0;

    CSDWordType    UByteTypeMax  = 255;

    CSDWordType    SByteTypeMin  = -128;

    CSDWordType    SByteTypeMax  = 127;

    CSDWordType    UWordTypeMin  = 0;

    CSDWordType    UWordTypeMax  = 65535;

    CSDWordType    SWordTypeMin  = -32768;

    CSDWordType    SWordTypeMax  = 32767;

    CSDWordType    UDWordTypeMin = 0;

    CUDWordType    UDWordTypeMax = 4294967295U;

    CSDWordType    SDWordTypeMin = -2147483647; // Debera ser -2147483648 pero el

    CSDWordType    SDWordTypeMax = 2147483647;  // compilador lo cambia por unsigned.

    //const float    FloatMin      = 3.4e-38;

    //const float    FloatMax      = 3.4e+38;

    //const double   DoubleMin     = 1.7e-308;

    //const double   DoubleMax     = 1.7e+308;

   static const   CSDWordType FAIL = -1;
   static const   CSByteType  LF           = '\n';
   static const   CSByteType  CHAR_BLANCO  = ' ';
   static const   SByteType   CERO_UNO_DOS[] = { '0', '1', '2' };
   static const   SByteType   CERO_UNO_BLANCO[] = { '0', '1', ' ' };
   static const   SByteType   CERO_UNO[] = { '0', '1' };
   static const   SByteType   NO_SI[] = { 'N', 'S' };
   static const   SByteType   NO_SI_BLANCO[] = { 'N', 'S', ' ' };
   static const   SByteType   OFF_ON[][ 3+1 ] = { "OFF", "ON" };
   static const   SByteType   ACK_NACK[][ 2+1 ] = { "15", "06" };

   enum ColorTypeE
   {
      // Text Attributes
      ALL_ATTRIBUTES_OFF = 0,
      BOLD_ON            = 1,
      UNDERSCORE         = 4, // on monochrome display adapter only
      BLINK_ON           = 5,
      REVERSE_VIDEO_ON   = 7,
      CONCEALED_ON       = 8,

      // Foreground colors
      FG_BLACK   = 30,
      FG_RED     = 31,
      FG_GREEN   = 32,
      FG_YELLOW  = 33,
      FG_BLUE    = 34,
      FG_MAGENTA = 35,
      FG_CYAN    = 36,
      FG_WHITE   = 37,

      // Background colors
      BG_BLACK   = 40,
      BG_RED     = 41,
      BG_GREEN   = 42,
      BG_YELLOW  = 43,
      BG_BLUE    = 44,
      BG_MAGENTA = 45,
      BG_CYAN    = 46,
      BG_WHITE   = 47
   };

   #define BLACK          ALL_ATTRIBUTES_OFF, FG_BLACK,   BG_BLACK
   #define RED            ALL_ATTRIBUTES_OFF, FG_RED,     BG_BLACK
   #define GREEN          ALL_ATTRIBUTES_OFF, FG_GREEN,   BG_BLACK
   #define YELLOW         ALL_ATTRIBUTES_OFF, FG_YELLOW,  BG_BLACK
   #define BLUE           ALL_ATTRIBUTES_OFF, FG_BLUE,    BG_BLACK
   #define MAGENTA        ALL_ATTRIBUTES_OFF, FG_MAGENTA, BG_BLACK
   #define CYAN           ALL_ATTRIBUTES_OFF, FG_CYAN,    BG_BLACK
   #define WHITE          ALL_ATTRIBUTES_OFF, FG_WHITE,   BG_BLACK
   #define BRIGHT_BLACK   BOLD_ON,            FG_BLACK,   BG_BLACK
   #define BRIGHT_RED     BOLD_ON,            FG_RED,     BG_BLACK
   #define BRIGHT_GREEN   BOLD_ON,            FG_GREEN,   BG_BLACK
   #define BRIGHT_YELLOW  BOLD_ON,            FG_YELLOW,  BG_BLACK
   #define BRIGHT_BLUE    BOLD_ON,            FG_BLUE,    BG_BLACK
   #define BRIGHT_MAGENTA BOLD_ON,            FG_MAGENTA, BG_BLACK
   #define BRIGHT_CYAN    BOLD_ON,            FG_CYAN,    BG_BLACK
   #define BRIGHT_WHITE   BOLD_ON,            FG_WHITE,   BG_BLACK

   #define ERROR_COLOR     BOLD_ON, FG_RED,     BG_BLACK
   #define WARNING_COLOR   BOLD_ON, FG_YELLOW,  BG_BLACK
   #define INFO_COLOR      BOLD_ON, FG_CYAN,    BG_BLACK
   #define TRACE_COLOR     BOLD_ON, FG_MAGENTA, BG_BLACK
   #define EXCEPCION_COLOR BOLD_ON, FG_WHITE,   BG_RED

   // Restricciones en los valores de los campos del tipo VersionTypeS
   static const UByteType MAX_VERSION = 99;

   static const UDWordType MIN_NUM_TICKET =    1;
   static const UDWordType MAX_NUM_TICKET = 9999;
//fignoreSoap

#endif
