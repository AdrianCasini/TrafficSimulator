#ifndef _ROUTING_H
#define _ROUTING_H

#include "Simulator.h"
#include "Controls.h"
#include "Drawing.h"
#include "FileInfo.h"
#include "SimCurl.h"

#define SIM_MAX_NODE_REP_WINDOW 7
#define SIM_BLOCK_POINTS_SNAP 30
class Routing
{
	public:

		ofstream  mFile;
		Controls* mCtrl;
		Drawing*  mDRW;
		Cadena    mcadgral;
		BaseDatos mDB;
		WayInfo   mWayInfo;
		bool      mPoint_init;
		FileInfo  mRoutes;

		long long int mcheckRep[SIM_MAX_NODE_REP_WINDOW];
		int           mindex_rep;
		bool          mActionActiveRoute;
		bool          mActionActiveSnap;


		Cadena    mCadTimeStamp;
		Cadena    mCadFileNodes;
		Cadena    mCadFileGpsPoints;
		Cadena    mCadFileOSMResult;
		Cadena    mCadFileOSMNodes;
		Cadena    mCadFileOut;
		Cadena    mCadFileOutSim;


		SimCurl   mCurl;
		SimCurl::SCurl_Buffer msCurlBuff;
		SJsonHandler msJSONhandler;


	public:

		Routing();
		~Routing();

		void init(Controls* ctrl, Drawing* drw, Cadena cadgral);
		void reset_check_rep();
		bool check_rep(long long int nid);
		void clear();
		void resize_line();
		bool is_active();
		void point_init(bool);
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void activate_route();
		void deactivate_route();
		void add_point(double x, double y, double z);

		void create_gpspoints_file(bool bfrom_file, Cadena cadgpspoints);




		//--OSM Route Service--------------------------------------------
		void route_from_draw();
		void routes_from_file(Cadena cad_in_out);
		void routes_from_segment(Cadena cadgpslist, vector<long long int>& vectnodes);
		void get_list_gps_points_from_draw(Cadena& cadgpslist);
		bool execute_OSM_route_service(Cadena cadgpslist);
		void get_nodes_from_osm_route_response(char*, int, vector<long long int>& vectnodes);
		void write_route(Cadena cadfileout);
		void draw_route();
		//---------------------------------------------------------------

		void create_simulation_file(Cadena cadfilesource, Cadena cadfilesimulation);
		//void set_route_files_names();

		//--------Snap to route-----------
		void activate_snap();
		void deactivate_snap();
		void snap_from_file(Cadena cadfile);
		void snap_from_draw();
		void draw_snap();
		void write_snap(Cadena cadfileout);
		void set_snap_files_names();
		void execute_OSM_snap_service(char* gpspoints, int gps_number);
		//void obtaing_OSM_snap_nodes();
		void get_gps_points(Cadena cadfile, char* ptrgps,int max, int& gps_number);

		Cadena get_gps_block(int charindx_ini, int& charindx_last, char* ptr);
		Cadena get_rad_block(int block, char* ptr);
};

#endif

