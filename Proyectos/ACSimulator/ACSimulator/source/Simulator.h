#ifndef _SIMULATOR_H
#define _SIMULATOR_H

// GLEW
#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLUT
#include "GL/glut.h"

// GLM -------------
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

// FreeType Fonts---
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include "lodepng.h"
#define LODEPNG_COMPILE_DISK

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <regex>

//---gral-includes--
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <map>
#include <vector>
#include "resource.h"
#include <Commdlg.h>

#include <math.h>
#include <cmath>
#include <new>
#include <cassert>
#include <tchar.h>
#include <curl.h>
#include <ctime>


//---Simulator Clases----
#include "lang/Cadena.h"
#include "Shader.h"
#include "BaseDatos.h"
#include "SOIL.h"
#include "texture.hpp"

//------------------------------------------------------------------------------
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace glm;
using namespace rapidjson;

//------------------------------------------------------------------------------
//----- DATA BASE---------
#define SIM_DB_NAME "traffic_simulator"
#define SIM_DB_HOST "localhost"
#define SIM_DB_PORT 5432
#define SIM_DB_USER "adria"
#define SIM_DB_TIMEOUT 1000

//----- GRAL--------------
#define SIM_WINDOW_TITLE       "Traffic Simulator"
#define SIM_WINDOW_SUB_TITLE   "AdrianCasini@2022"
#define SIM_MAX_POSITION_Z 10500.0
#define SIM_MIN_POSITION_Z 0.2
#define SIM_COLOUR_SEA_DAY       (168.0/255.0), (234.0/255.0), (255.0/255.0), (255.0/255.0)  
#define SIM_COLOUR_SEA_NIGHT     (20.0/255.0),  (40.0/255.0),  (40.0/255.0),  (255.0/255.0) 
#define SIM_COLOUR_EARTH_DAY     (253.0/255.0), (253.0/255.0), (253.0/255.0), (255.0/255.0) 
#define SIM_COLOUR_EARTH_NIGHT   (20.0/255.0),  (20.0/255.0),  (20.0/255.0),  (255.0/255.0) 

/*

			foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47

Additionally, you can use these :

reset             0  (everything back to normal)
bold / bright     1  (often a brighter shade of the same colour)
underline         4
inverse           7  (swap foreground and background colours)
bold / bright off 21
underline off     24
inverse off       27
*/

#define SIM_TXOF_BLUE std::string  SIMF_BLUE("\033[0;34m");
#define SIM_TXOF_BLACK std::string SIMF_BLACK("\033[0;30m");


#define SIM_TXOB_CYAN std::string SIMB_CYAN("\033[0;46m");
#define SIM_TXO_RESET std::string SIM_RESET("\033[0m");



#define SIM_POSZ_INITIAL 20.0
#define SIM_POSX_INITIAL_BCN  -20.0  //Barcelona
#define SIM_POSY_INITIAL_BCN  -24.0  //Barcelona

#define SIM_SCREEN_W 1920
#define SIM_SCREEN_H 1080
#define SIM_SCROLL_FACTOR 0.8

#define SIM_EARTH_RADIUS_KM 6371.0
#define SIM_PI 3.14159265358979323846
#define SIM_LON_REF 2.31368
#define SIM_LAT_REF 41.5942
#define SIM_LON_MOVE 1.853005
#define SIM_LAT_MOVE 41.244897
#define SIM_KM2MILLAS 0.621371

#define SIM_MAX_INSTRUCTION_NUMBER 1000
#define SIM_MAP_QUADS 1500

//------------------------------------------------------------------------------
#define SIM_GPS_SAMPLE 1000
//------------------------------------------------------------------------------
//----- MAPPER_TEXTER
#define SIM_MAX_TEXTER 8
#define SIM_MAX_TEXTER_QUADS 9
#define SIM_MAX_TEXTER_PLACES 1000
#define SIM_MAX_TEXTER_NAME_SIZE 50
#define SIM_MAX_VH_X_NODE 500
#define SIM_MAX_MAPPER_X 5
#define SIM_MAX_MAPPER_Y 3
#define SIM_MAX_MAPPER  SIM_MAX_MAPPER_X * SIM_MAX_MAPPER_Y

#define SIM_MAX_MAPPER_DB_X 6
#define SIM_MAX_MAPPER_DB_Y 3
#define SIM_MAX_MAPPER_DB  SIM_MAX_MAPPER_DB_X * SIM_MAX_MAPPER_DB_Y
//------------------------------------------------------------------------------
#define SIM_MAX_TRIP_POINTS  200
#define SIM_MAX_TRACE_POINTS 30000
#define SIM_MAX_SNAP_POINTS 1000
//------------------------------------------------------------------------------
#define SIM_MAX_GPSPOINTS 5000
//------------------------------------------------------------------------------
//----- TOLLER DEFINITIONS
#define SIM_TOLLER_LINE_WIDTH   0.002
#define SIM_TOLLER_POINT_ZIZE   0.009
#define SIM_TOLLER_POINT_IN_PW  0.75
#define SIM_TOLLER_TARIFA       0.09

//------------------------------------------------------------------------------
//----- TOOLS DEFINITIONS
#define SIM_TOOLS_LINE_WIDTH   0.0009
#define SIM_TOOLS_POINT_ZIZE   0.013
#define SIM_TOOLS_POINT_IN_PW  0.65
#define SIM_TOOL_DELTA_SPEED   10.0 //Km/h

#define SIM_MAX_POLYGONS 10
#define SIM_MAX_POLYG_PNTS 10

//------------------------------------------------------------------------------
//----- DRAWING DEFINITIONS
#define SIM_DRAWING_POINT_IN_PW 0.65
#define SIM_DRAWING_POINT_ZIZE 0.013
#define SIM_DRAWING_LINE_WIDTH 0.0020

//------------------------------------------------------------------------------
//-----POFI DEFINITIONS
#define SIM_MAX_ITEM_PNTS 300
#define SIM_ITEM_POINT_ZIZE 0.005

//------------------------------------------------------------------------------
//----- ROUTE/SNAP DEFINITIONS
#define SIM_MAX_ROUTE_PNTS 100
#define SIM_MAX_ROUTE_PNTS_ROUTE 10000
//------------------------------------------------------------------------------
//----- SIMULATOR DEFINITIONS
#define SIM_MAX_VEHICLES 8000
#define SIM_MAX_SOURCES 500
#define SIM_MAX_VEH_X_SOURCE 500
#define SIM_MAX_VEHICLES_WAY 300
#define SIM_MAX_TRIPS 100
#define SIM_MAX_NODE_ORDER 50000
#define SIM_MAX_3D_MODEL_POINTS 12000

//------------------------------------------------------------------------------
//----- HIGHWAY (Ap7) --- DEFINITIONS
#define SIM_MAX_NODES_HIGHWAY  10000
#define SIM_MAX_POSSIBLE_TRIPS 350
#define SIM_MAX_NODES_TRIP     2500
#define SIM_HIGHWAY_W          0.002
#define SIM_SPEED_VH           4000.0
#define SIM_MAX_HW_POINTS      50000
//------------------------------------------------------------------------------
//----- DIRECTORIES ------------------------------------------------------------
#define SIM_DIR_ORIGIN_        "/TrafficSimulator/"

#define SIM_DIR_PROJECT_       SIM_DIR_ORIGIN_"Proyectos/ACSimulator/ACSimulator/"
#define SIM_DIR_PRJT_SHADERS_  SIM_DIR_PROJECT_"shaders/"
#define SIM_DIR_PRJT_OBJECTS_  SIM_DIR_PROJECT_"objects/"
#define SIM_DIR_PRJT_ETC_      SIM_DIR_PROJECT_"etc/"


#define SIM_DIR_FONTS_         SIM_DIR_ORIGIN_"Fonts/"
#define SIM_DIR_IMAGES_        SIM_DIR_ORIGIN_"Images/"
#define SIM_DIR_SHADERS_       SIM_DIR_ORIGIN_"Shaders/"
#define SIM_DIR_MAPS_          SIM_DIR_ORIGIN_"Maps/"
#define SIM_DIR_DB_NAME        "DataBase/traffic_simulator"

#define SIM_DIR_OBJECTS_       SIM_DIR_ORIGIN_"Objects/"
#define SIM_DIR_PQ_            SIM_DIR_ORIGIN_"PostgresSQL/"
#define SIM_DIR_PQ_BIN_        SIM_DIR_PQ_"bin/"
#define SIM_PQ_PGCTL           "pg_ctl.exe"
#define SIM_PQ_INIT            "initdb.exe"
#define SIM_PQ_PSQL            "psql.exe"
#define SIM_PQ_CRT             "createdb.exe"


//------------------------------------------------------------------------------
#define SIM_DIR_ETC_           "/Etc/"
#define SIM_DIR_ETC_SCRIPTS_   SIM_DIR_ETC_"scripts/"
#define SIM_DIR_ETC_CONFIG_    SIM_DIR_ETC_"config/"
#define SIM_DIR_ETC_BINES_     SIM_DIR_ETC_"bines/"


#define SIM_DIR_INFO_          "/Info/"
#define SIM_DIR_INFO_MAPS_     SIM_DIR_INFO_"Maps/"

//----------DIRECTORIES TEST----------------------------------------------------
#define SIM_DIR_TEST_           "Test/"
#define SIM_DIR_TEST_SIMUL_     SIM_DIR_TEST_"simulation"
#define SIM_DIR_TEST_SRC_       SIM_DIR_TEST_"sources/"
#define SIM_DIR_TEST_TRP_       SIM_DIR_TEST_"trips/"
#define SIM_DIR_TEST_ROUTES_    SIM_DIR_TEST_"routes/"
#define SIM_DIR_TEST_SNAPS_     SIM_DIR_TEST_"snaps/"
#define SIM_DIR_TEST_IN_OUT_    SIM_DIR_TEST_"in_out"
#define SIM_DIR_TEST_LABELS_    SIM_DIR_TEST_"labels/"
#define SIM_DIR_TEST_POLY_      SIM_DIR_TEST_"polygons/"
#define SIM_DIR_TEST_TRACE_     SIM_DIR_TEST_"trace/"
#define SIM_DIR_TEST_POSITIONS_ SIM_DIR_TEST_"positions/"
#define SIM_DIR_TEST_FLLRT_     SIM_DIR_TEST_"follow_route/"
#define SIM_DIR_TEST_SECTIONS_  SIM_DIR_TEST_"sections/"
#define SIM_DIR_TEST_SNAPS_FILES_  SIM_DIR_TEST_"snaps_files/"

#define SIM_DIR_TEST_PX_         SIM_DIR_TEST_"interfaces_pxy/"
#define SIM_DIR_TEST_PX_EU_      SIM_DIR_TEST_PX_"eurotoll/"
#define SIM_DIR_TEST_PX_EU_PERF_ SIM_DIR_TEST_PX_"eurotoll_perf/"
#define SIM_DIR_TEST_PX_SIM_     SIM_DIR_TEST_PX_"simulator/"
#define SIM_DIR_TEST_PX_MST_     SIM_DIR_TEST_PX_"master/"

#define SIM_DIR_TEST_OSM_        SIM_DIR_TEST_"OSM_temp/"
#define SIM_DIR_TEST_OSM_RT_     SIM_DIR_TEST_OSM_"route/"
#define SIM_DIR_TEST_OSM_SNP_    SIM_DIR_TEST_OSM_"snap/"

#define SIM_DIR_TEST_CT_         SIM_DIR_TEST_"create_trips/"
#define SIM_DIR_TEST_CT_RT_      SIM_DIR_TEST_CT_"osm_routes/"
#define SIM_DIR_TEST_CT_WAY_     SIM_DIR_TEST_CT_"ways_trips/"
#define SIM_DIR_TEST_CT_PNT_     SIM_DIR_TEST_CT_"trips_points/"

#define SIM_DIR_TEST_WIN_           "\\Test\\"

#define SIM_DIR_TEST_SIMUL_WIN      SIM_DIR_TEST_WIN_"simulation"
#define SIM_DIR_TEST_DET_WIN        SIM_DIR_TEST_WIN_"detectors"
#define SIM_DIR_TEST_IN_OUT_WIN     SIM_DIR_TEST_WIN_"in-out"
#define SIM_DIR_TEST_SVTEST_WIN     SIM_DIR_TEST_WIN_"saved_test"
#define SIM_DIR_TEST_POLY_WIN       SIM_DIR_TEST_WIN_"polygons"
#define SIM_DIR_TEST_TRACE_WIN      SIM_DIR_TEST_WIN_"trace"
#define SIM_DIR_TEST_SOURCE_WIN     SIM_DIR_TEST_WIN_"sources"
#define SIM_DIR_TEST_SECTIONS_WIN   SIM_DIR_TEST_WIN_"sections"
#define SIM_DIR_TEST_SNAPFILES_WIN  SIM_DIR_TEST_WIN_"snaps_files"

//------------------------------------------------------------------------------
//----- MENU
#define SIM_BUTTON_SEP 0.39
#define SIM_BUTTON_XMV 0.3
#define SIM_BUTTON_YMV 0.3
#define SIM_BUTTON_DX 0.2
#define SIM_BUTTON_DY 0.2

//------------------------------------------------------------------------------
//----- PROXY
#define SIM_PROXY_CLOUD_PORT "5003"
#define SIM_PROXY_CLOUD_HOST "telematics.autopistashub.com"
#define SIM_PROXY_SIM_RECORDS_X_FILE 20
#define SIM_PROXY_SIM_RECORDS_TIME_ELAPSED 10
//#define SIM_CLOUD_HOST "4.tcp.ngrok.io"
//#define SIM_CLOUD_PORT "13438"
//#define SIM_CLOUD_PORT "5556"
//#define SIM_CLOUD_HOST "localhost"


//------------------------------------------------------------------------------
struct sItemColor
{
	Cadena caditem;
	int r;
	int g;
	int b;
};

//------------------------------------------------------------------------------
class Vector2d
{
	public:
		Vector2d(double x, double y){Set(x, y);};
		double GetX(void) const { return mX; };
		double GetY(void) const { return mY; };
		void  Set(double x, double y)	{mX = x;mY = y;};
	private:
		double mX;
		double mY;
};
//------------------------------------------------------------------------------
// Typedef an STL vector of vertices which are used to represent
// a polygon/contour and a series of triangles.
typedef std::vector< Vector2d > Vector2dVector;

//------------------------------------------------------------------------------
typedef struct 
{
	GLuint txt_id;
	GLuint txt_id_n;
	GLuint txt_id_o;

	Cadena name;
	Cadena name_n;
	Cadena name_o;

	Cadena tooltip;

	GLuint vbo_id;
	bool   flag;
	bool   over;
	double ymv;

	double x;
	double y;

	double x_screen;
	double y_screen;

	double w;
	double h;

} Button;


//------------------------------------------------------------------------------
typedef struct
{
	GLuint txt_id;
	GLuint txt_id_n;
	GLuint txt_id_o;

	Cadena name;
	Cadena name_n;
	Cadena name_o;

	GLuint vbo_id;
	bool   flag;
	bool   over;
	double ymv;

	double x;
	double y;

	double x_screen;
	double y_screen;

	double w;
	double h;

} Menu_Bar;

//------------------------------------------------------------------------------
struct Character
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to next glyph
};
//------------------------------------------------------------------------------
typedef struct 
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
	unsigned int f;
} Color;

typedef struct
{
	double r;
	double g;
	double b;
	double f;
} ColorF;

//------------------------------------------------------------------------------
typedef struct 
{
	int indiceVH[SIM_MAX_VH_X_NODE];
	int lastIndex;
	int first_time;
} sInfoOvlap;

//------------------------------------------------------------------------------
typedef struct 
{
	double X;
	double Y;
	double angle;
	char name[SIM_MAX_TEXTER_NAME_SIZE];
	//string name;
} Node_Text;

//------------------------------------------------------------------------------
typedef struct 
{
	Cadena file_name;
	Cadena name;
	double pos_z;
	int     size;
	double  size_txt;
	int quad_x_old;
	int quad_y_old;
	int quad_x;
	int quad_y;
	Node_Text array[SIM_MAX_TEXTER_PLACES];
	int num_letters;
	int count;
	int count_temp;
	glm::vec3 color;
} Type_Text;

//------------------------------------------------------------------------------
typedef struct 
{
	long long int wid;
	long long int nid;
	int       order;
	double    lon;
	double    lat;
	double    X;
	double    Y;
	int       lanes;

} Node;

//------------------------------------------------------------------------------
typedef struct
{
	long long int nid;
	long long int wid[10];
	int maxways;

} SnodeWay;

//------------------------------------------------------------------------------
typedef struct 
{
	long long nid_ini;
	long long nid_fin;
} WayExtremos;
//------------------------------------------------------------------------------
typedef struct 
{
	long long wid_ini;
	long long wid_fin;
} NodeExtremos;

//------------------------------------------------------------------------------
typedef vector<Node> vectNode;
typedef vector<long long int> vectWays;
typedef vector<long long> vectNodeId;
typedef map<long long, vectNode> MapWaysNodes;
typedef map<long long, int> MapWays;
typedef map<long long, int> MapRelations;
typedef map<long long, vectWays> MapRelWays;

//------------------------------------------------------------------------------

typedef enum
{
	SIM_MOTORWAY,
	SIM_MOTORWAY_LINK,
	SIM_TRUNK,
	SIM_TRUNK_LINK,
	SIM_PRIMARY,
	SIM_PRIMARY_LINK,
	SIM_SECONDARY,
	SIM_SECONDARY_LINK,
	SIM_TERTIARY,
	SIM_TERTIARY_LINK,
	SIM_UNCLASSIFIED,
	SIM_RESIDENTIAL,
	SIM_SERVICE,
	SIM_TRACK,
	SIM_LIVING_STREET,
	SIM_ROAD,
	SIM_ESCAPE,
	SIM_WAY_TYPE_END
} enumHWType;

//------------------------------------------------------------------------------
typedef enum
{
	ONEWAY,
	TWOWAY
} enumSense;

//------------------------------------------------------------------------------
typedef struct
{
	long long int wid;
	int           order;
	int           max_nodos;
	int           lanes;
	enumHWType    hwtype;
	enumSense     sense;
} NodeWayLink;

//------------------------------------------------------------------------------
typedef struct
{
	long long int    nid;
	long long int    wid;
	int              nid_index;

	double        angle;
	double        dist;
	unsigned int  lanes;

	double        deltamv;
	double        deltamv_x;
	double        deltamv_y;

	double        delta_lane_x;
	double        delta_lane_y;

	double        X;
	double        Y;

	double        lon;
	double        lat;

} NodoTrip;

//------------------------------------------------------------------------------
typedef struct
{

	long long int  nid;
	int           nid_index;
	int           order;

	double        angle;
	double        dist;

	double        deltamv;
	double        deltamv_x;
	double        deltamv_y;

	double        delta_lane_x;
	double        delta_lane_y;

	double        X;
	double        Y;

	double        lon;
	double        lat;

	int           node_vh_order;
	
	//---Way Stuff per Node----
	long long int  wid;
	NodeWayLink*   array_way_links;
	int            max_ways;

	enumHWType    hwtype;
	int           lanes;
	char          name[100];
	int           name_long;
	char          href[100];
	int           href_long;

} NodoInfo;

//------------------------------------------------------------------------------
typedef struct
{
	long long int wid;
	enumHWType    hwtype;
	int           lanes;

	int           max_nodos;
	char          name[100];
	int           name_long;
	char          href[100];
	int           href_long;

	NodoInfo      *array_nodo_info;
} WayInfo;

//------------------------------------------------------------------------------
typedef struct
{
	NodoInfo ntrip[5000];
	bool     sense[5000];
	int      max_nodos;

} TripInfo;

//------------------------------------------------------------------------------
struct GPoint
{
	double lat;
	double lon;

	double X;
	double Y;
};

//------------------------------------------------------------------------------
struct GPointLine
{
	double lat;
	double lon;

	double X;
	double Y;

	long long int seg_id;
};


//------------------------------------------------------------------------------
typedef struct
{
	char trip_name[20];
	int index;
} IndexTrip;


//------------------------------------------------------------------------------
typedef struct 
{
	Cadena name;
	Cadena tipo;
	Cadena key_tag;
	Cadena value;
	Cadena table;
	bool   relation;
	double size;
	double line_w;

} KeyValue;

//------------------------------------------------------------------------------
typedef struct
{
	long long wid;
	long long nfirst;
	long long nlast;
	bool used;
	Cadena order;
} Way;

//------------------------------------------------------------------------------
typedef struct
{
	long long rid;
	Way   ways[1000];
	int   n_polygon;
	int   way_number;
} Relation;

//------------------------------------------------------------------------------
typedef vector<KeyValue> vectorKeyValue;
//------------------------------------------------------------------------------
typedef struct 
{
	long long nid;
	Cadena name;
	double lat;
	double lon;
	double X;
	double Y;

} Node_Text_gen;

//------------------------------------------------------------------------------
typedef struct
{
	long long wid;
	Cadena name;
	double lat;
	double lon;
	double X;
	double Y;
	double angle;
	bool not_angle;

} Way_Text_gen;

//------------------------------------------------------------------------------
struct point3
{
	double x;
	double y;
	double z;
};
//------------------------------------------------------------------------------
typedef struct 
{
	Cadena source;
	Cadena type;
	Cadena file;
	float  clr_r;
	float  clr_g;
	float  clr_b;
	double height;
	double width;
	double quad_sz;
	double distance;
	int    texture;
	Cadena table;

} MapItem;


//------------------------------------------------------------------------------
struct Simveh
{
	Cadena cad_vehsource;

	long long int wid;
	Cadena cad_est_in;
	Cadena cad_est_out;
	double speed;
	double speedavrg;
	double speedavrgbck;
	double speed_pw;
	double size;
	Color color;
	int lane_type;
	char sentido;

	Cadena cadwayfile;
	Cadena cattabulacion;

	long long int gpsId;
	int    delta_time_fll;

	double gps_inaccuracy;
	double gps_point_timer;

	Cadena cad_lpn;

	Cadena cad_type;

	Cadena momento;

	int nodeinfo_index;
	double posx;
	double posy;
	int lane_now;

};

//------------------------------------------------------------------------------
static point3 MODEL_TXT[] =
{
	{-0.5,  0.5, 0.0},
	{-0.5, -0.5, 0.0},
	{ 0.5, -0.5, 0.0},

	{-0.5,  0.5, 0.0},
	{ 0.5, -0.5, 0.0},
	{ 0.5,  0.5, 0.0},
};

//------------------------------------------------------------------------------
static point3 MODEL_4P[] =
{
	{-0.5, -0.5, 0.0},
	{ 0.5, -0.5, 0.0},
	{-0.5,  0.5, 0.0},
    { 0.5,  0.5, 0.0}
};

//------------------------------------------------------------------------------
static point3 MODEL_BAR[] =
{
	{-0.02, -0.5, 0.0},
	{ 0.02, -0.5, 0.0},
	{-0.02,  0.5, 0.0},
	{ 0.02,  0.5, 0.0}
};

//------------------------------------------------------------------------------
static point3 MODEL_4P_SLENDER_W[] =
{
	{-1.0, -0.5, 0.0},
	{ 1.0, -0.5, 0.0},
	{-1.0,  0.5, 0.0},
	{ 1.0,  0.5, 0.0}
};

//------------------------------------------------------------------------------
static point3 MODEL_4P_3D_SLENDER_W[] =
{
	{-1.0, -0.5f, -0.5f},
	{ 1.0, -0.5f, -0.5f},
	{ 1.0,  0.5f, -0.5f},
	{ 1.0,  0.5f, -0.5f},
	{-1.0,  0.5f, -0.5f},
	{-1.0, -0.5f, -0.5f},

	{-1.0, -0.5f,  0.5f},
	{ 1.0, -0.5f,  0.5f},
	{ 1.0,  0.5f,  0.5f},
	{ 1.0,  0.5f,  0.5f},
	{-1.0,  0.5f,  0.5f},
	{-1.0, -0.5f,  0.5f},

	{-1.0,  0.5f,  0.5f},
	{-1.0,  0.5f, -0.5f},
	{-1.0, -0.5f, -0.5f},
	{-1.0, -0.5f, -0.5f},
	{-1.0, -0.5f,  0.5f},
	{-1.0,  0.5f,  0.5f},

	{1.0,  0.5f,  0.5f},
	{1.0,  0.5f, -0.5f},
	{1.0, -0.5f, -0.5f},
	{1.0, -0.5f, -0.5f},
	{1.0, -0.5f,  0.5f},
	{1.0,  0.5f,  0.5f},

	{-1.0, -0.5f, -0.5f},
	{ 1.0, -0.5f, -0.5f},
	{ 1.0, -0.5f,  0.5f},
	{ 1.0, -0.5f,  0.5f},
	{-1.0, -0.5f,  0.5f},
	{-1.0, -0.5f, -0.5f},

	{-1.0,  0.5f, -0.5f},
	{ 1.0,  0.5f, -0.5f},
	{ 1.0,  0.5f,  0.5f},
	{ 1.0,  0.5f,  0.5f},
	{-1.0,  0.5f,  0.5f},
	{-1.0,  0.5f, -0.5f}
};

//------------------------------------------------------------------------------
static point3 MODEL_SKY[] =
{
	{-1.0, -0.5f, -0.5f},
	{ 1.0, -0.5f, -0.5f},
	{ 1.0,  0.5f, -0.5f},
	{ 1.0,  0.5f, -0.5f},
	{-1.0,  0.5f, -0.5f},
	{-1.0, -0.5f, -0.5f},

	{-1.0, -0.5f,  0.5f},
	{ 1.0, -0.5f,  0.5f},
	{ 1.0,  0.5f,  0.5f},
	{ 1.0,  0.5f,  0.5f},
	{-1.0,  0.5f,  0.5f},
	{-1.0, -0.5f,  0.5f},

	{-1.0,  0.5f,  0.5f},
	{-1.0,  0.5f, -0.5f},
	{-1.0, -0.5f, -0.5f},
	{-1.0, -0.5f, -0.5f},
	{-1.0, -0.5f,  0.5f},
	{-1.0,  0.5f,  0.5f},

	{1.0,  0.5f,  0.5f},
	{1.0,  0.5f, -0.5f},
	{1.0, -0.5f, -0.5f},
	{1.0, -0.5f, -0.5f},
	{1.0, -0.5f,  0.5f},
	{1.0,  0.5f,  0.5f},

	{-1.0, -0.5f, -0.5f},
	{ 1.0, -0.5f, -0.5f},
	{ 1.0, -0.5f,  0.5f},
	{ 1.0, -0.5f,  0.5f},
	{-1.0, -0.5f,  0.5f},
	{-1.0, -0.5f, -0.5f},

	{-1.0,  0.5f, -0.5f},
	{ 1.0,  0.5f, -0.5f},
	{ 1.0,  0.5f,  0.5f},
	{ 1.0,  0.5f,  0.5f},
	{-1.0,  0.5f,  0.5f},
	{-1.0,  0.5f, -0.5f}
};


//------------------------------------------------------------------------------
#ifdef _MSC_VER
#define MIN __min
#define MAX __max
#else
#define MIN std::min
#define MAX std::max
#endif


// Define some fixed size types.

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


// Try to figure out what endian this machine is using. Note that the test
// below might fail for cross compilation; additionally, multi-byte
// characters are implementation-defined in C preprocessors.

#if (('1234' >> 24) == '1')
#elif (('4321' >> 24) == '1')
#define BIG_ENDIAN
#else
#error "Couldn't determine the endianness!"
#endif

//------------------------------------------------------------------------------
// A simple 32-bit pixel.

union Pixel32
{
	Pixel32()
		: integer(0) { }
	Pixel32(uint8 bi, uint8 gi, uint8 ri, uint8 ai = 255)
	{
		b = bi;
		g = gi;
		r = ri;
		a = ai;
	}

	uint32 integer;

	struct
	{
#ifdef BIG_ENDIAN
		uint8 a, r, g, b;
#else // BIG_ENDIAN
		uint8 b, g, r, a;
#endif // BIG_ENDIAN
	};
};

//------------------------------------------------------------------------------
struct Vec2
{
	Vec2() { }
	Vec2(float a, float b)
		: x(a), y(b) { }

	float x, y;
};

//------------------------------------------------------------------------------
struct Rect
{
	Rect() { }
	Rect(float left, float top, float right, float bottom)
		: xmin(left), xmax(right), ymin(top), ymax(bottom) { }

	void Include(const Vec2 &r)
	{
		xmin = MIN(xmin, r.x);
		ymin = MIN(ymin, r.y);
		xmax = MAX(xmax, r.x);
		ymax = MAX(ymax, r.y);
	}

	float Width() const { return xmax - xmin + 1; }
	float Height() const { return ymax - ymin + 1; }

	float xmin, xmax, ymin, ymax;
};

//------------------------------------------------------------------------------
// TGA Header struct to make it simple to dump a TGA to disc.

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, 1)
#pragma pack(1)               // Dont pad the following struct.
#endif
//------------------------------------------------------------------------------
struct TGAHeader
{
	uint8   idLength,           // Length of optional identification sequence.
		paletteType,        // Is a palette present? (1=yes)
		imageType;          // Image data type (0=none, 1=indexed, 2=rgb,
							// 3=grey, +8=rle packed).
	uint16  firstPaletteEntry,  // First palette index, if present.
		numPaletteEntries;  // Number of palette entries, if present.
	uint8   paletteBits;        // Number of bits per palette entry.
	uint16  x,                  // Horiz. pixel coord. of lower left of image.
		y,                  // Vert. pixel coord. of lower left of image.
		width,              // Image width in pixels.
		height;             // Image height in pixels.
	uint8   depth,              // Image color depth (bits per pixel).
		descriptor;         // Image attribute flags.
};

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop)
#endif

//------------------------------------------------------------------------------
// A horizontal pixel span generated by the FreeType renderer.

struct Span
{
	Span() { }
	Span(int _x, int _y, int _width, int _coverage)
		: x(_x), y(_y), width(_width), coverage(_coverage) { }

	int x, y, width, coverage;
};

typedef std::vector<Span> Spans;


typedef vector <Cadena> VectCad;


//------------------------------------------------------------------------------
class Simulator
{
	public:


		

	public:

		//----------General----------------------------------------

		static void init_fonts_plus(std::map<unsigned char, Character>& mfont);
		static enumHWType get_highway_type(Cadena cadtype);
		static double get_speed_factor_from_hwtype(enumHWType ehwtype);
		static Cadena get_filename_clean(Cadena cadfile);

		//----------ways render--------------
		static void build_LINE_SIMPLE(double* array, int ncoord, int max_in, double* array_out, int max_out, int* total_out);
		static void build_LINE_RECT(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_LINE_RENDER(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_LINE_RECT_TRG(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_LINE_RECT_TRG_LANES(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_LINE_RECT_DASHED(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_BUILDING(double height, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_LINE_RECT_POLY(double road_w, double* array_in, int total_in, double* array_out, int max_out, int* total_out);
		static void build_rectangle(double x, double y, double z, double  x1, double  y1, double z1, double delta_xlane, double delta_ylane, int& index_out, double * array_out);
		static void build_rectangle_vertical(double height, double x1, double y1, double z1, double x2, double y2, double z2, int& index_out, double* array_out);

		static int  get_new_index(int index, int max);
		static void set_triangle(double* ptr, int& index, int max_out, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3);
		//----------spin model--------------
		static void spin_model(double* array_points, int size, glm::mat4* modelMatrices);
		static void spin_model_vh(double* array_points, double* array_scale, double* array_angles, int size, glm::mat4* modelMatrices);

		//----------texture ---------
		static void texture_dds(GLuint& id, Cadena file);
		static void texture(GLuint& id, Cadena file);
		static void texture_RGBA(GLuint& id, Cadena file);
		static void texture_RGBA_repeat(GLuint& id, Cadena file);
		static void texture_customize(GLuint& id, Cadena file, GLuint typeWrap, GLuint typeColor, GLuint typeLoad);

		//----------Trigonometric----------------------------------------
		static int    pnpoly(int npol, double *xp, double *yp, double x, double y);
		static double distance(double x1, double x2, double y1, double y2);
		static double deg2rad(double deg);
		static double rad2deg(double rad);
		static double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);
		static double getAngle(double x1, double x2, double y1, double y2, bool& not_angle);
		static double getXfromLon(double lon);
		static double getYfromLat(double lat);
		static void   get_lat_lon(double lat_i, double lon_i, double distance, double angle, double& lat_o, double& lon_o);
		static double get_delta_lat(double lat_i, double deltay);
		static double get_delta_lon(double lon_i, double lat_i, double lat_i2, double deltax);
		static void   get_XY_from_node(BaseDatos* pdb, double& x, double& y, long long int ni);

		//---------------------------------------------------------------
		static Cadena get_timestamp_format_YYMMDDhhmmss();
		static Cadena get_timestamp_format_YYYYMMDD_T_hhmmss();
		static Cadena get_timestamp_GMT_format_YYYYMMDD_X_hhmmss();
		static time_t get_time_now_ms();
		static void   get_new_moment(int* mnt);
		static double get_segs(Cadena cad_e, Cadena cad_s);
		static int    getSegOut(tm& t);
		static double smoother_z(double position);

		//-----------Transits-------------------------------------------
		static int    get_overlap_avrg(double speed, Cadena category);
		static int    get_lanereturn_avrg(double speed, Cadena category);

		static int    get_delta_lane_return(int lane_now);
		static int    get_lane_default(double speed,  int lanes, int lane_type);
		static int    get_new_lane(int lane_now, int lanes);
		static int    get_delta_lane_ch(int lane_now, int new_lane);
		static double get_delta_lane(int lane, int maxLanes);

		static double diff_angle(double angle_1, double angle_2);
		static double get_speed(double posS, double posE, Cadena mnt_sal, Cadena mnt_ent);
		static void   get_category_stuff(Cadena category, double& size, double& speed_pw, int& lane_type, Color& color);
		static void   tranform_stations(int& estent, int& estsal);

		//-----------Data Base------------------------------------------
		static void get_nodes_from_way(BaseDatos* db, long long wid, Cadena order, vectNode* vn);

		static void RasterCallback(const int y, const int count, const FT_Span * const spans, void * const user);
		static void RenderSpans(FT_Library &library, FT_Outline * const outline, Spans *spans);
		static void WriteGlyphAsTGA(FT_Library &library, const std::string &fileName, wchar_t ch, FT_Face &face, int size, const Pixel32 &fontCol, const Pixel32 outlineCol, float outlineWidth);
		static bool WriteTGA(const std::string &filename, const Pixel32 *pxl, uint16 width, uint16 height);

		static void get_info_nodes_from_file(BaseDatos* db, WayInfo* way_info, Cadena file);
		static void get_info_nodes_from_vector(BaseDatos* db, WayInfo* way_info, vector<long long int>& vectnodes);



		
};



#endif






