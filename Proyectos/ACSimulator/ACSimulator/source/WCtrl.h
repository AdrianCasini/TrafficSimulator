#ifndef _WCTRL_H
#define _WCTRL_H

#include "afxdialogex.h"
#include "resource.h"
#include "Simulator.h"
#include "Controls.h"
#include "Menu.h"
#include "MenuRight.h"
#include "MenuLeft.h"
#include "Traffic.h"
#include "Trip.h"
#include "Tools.h"
#include "Vehiculo.h"
#include "Trace.h"
#include "Tarif.h"
#include "Texter.h"
#include "MapMng.h"
#include "WayRenderMng.h"
#include "GPSPoint.h"
#include "Routing.h"
#include "Drawing.h"
#include "PofI.h"
#include "SectionMng.h"
#include "Inrix.h"

//-------------------
#define MAX_THREAD_HANDLES 2


class WCtrl
{
	public:

		struct sMainControl
		{
			GLFWwindow*   mWIN;
			Menu*         mMN;
			MenuLeft*     mMLFT;
			MenuRight*    mMRGT;
			Controls*     mCTRL;
			Traffic*      mTR;
			Tools*        mTLS;
			Vehiculo*     mVH;
			Trace*        mTC;
			Tarif*        mTRF;
			Texter*       mTXT;
			MapMng*       mMMG;
			WayRenderMng* mWRM;
			GPSPoint*     mGPT;
			Routing*      mRT;
			Drawing*      mDRW;
			PofI*         mPOI;
			SectionMng*   mSM;
			Inrix*        mIX;
			int mScreenWidth;
			int mScreenHeight;
		};

		sMainControl*  mMC;
		GLFWimage   mIcon[2];

		int mDateInitial;
		Cadena mDBName;
		bool mDebug;

		double mLastTime;
		double mCurrentTime;
		double mIUTime;
		int    mCountIUTime;

		HANDLE   mThreadHandles[MAX_THREAD_HANDLES];
		int      mHandleMax;

		bool   m_menu_locked;

		bool mWindowStarted;
		FileInfo mPositions;
		int mPosIndex;

		//Vehicle Follow variables
		NodoInfo* mVH_PtrNodeInfoNow_N;
		int mVH_TripIndex_N;
		int mVH_MaxNodesTrip_N;
		int mVH_Ordennode_N;
		double mVH_speed_N;
		double mVH_Nnd2nd_N;
		double mVH_Posx_N;
		double mVH_Posy_N;

		NodoInfo* mVH_PtrNodeInfoNow_S;
		int mVH_TripIndex_S;
		int mVH_MaxNodesTrip_S;
		int mVH_Ordennode_S;
		double mVH_speed_S;
		double mVH_Nnd2nd_S;
		double mVH_Posx_S;
		double mVH_Posy_S;

		int mVH_angle_follow_count;
		double mVH_angle_follow_now;
		double mVH_angle_follow_old;
		double mVH_delta_ang;

		bool mVH_follow;
		bool mNightBackground;

		enum eWCAction
		{
			SIM_WCACT_BUTTON_PRESS,
			SIM_WCACT_BUTTON_OK,
			SIM_WCACT_BUTTON_CANCEL,
			SIM_WCACT_BUTTON_ESC,
			SIM_WCACT_CURCALLBACK,
			SIM_WCACT_BUTTON_PRESSED_NO_MENU,
			SIM_WCACT_BUTTON_PRESSED_SELECT_COLOR,
		};

	
		int mButtonActiveException;

		struct sToolCtrl
		{
			//---tools---
			bool toll;
			bool speed;
			int  speed_value;
			bool avoid_gps;
			int  selected;
			int  selected_dlg;

			//--proxy picker---
			uint pp_master_trip;
			uint pp_sim_detector;
			uint pp_sim_gps;
			uint pp_sattol_gps_eurotoll;
			uint pp_sattol_gps_eurotoll_perf;

			//---clean---
			uint cl_ply;
			uint cl_tr;
			uint cl_poi;
			uint cl_rt;
			uint cl_sn;
			uint cl_trc;
			uint cl_color;
			uint cl_section;

			//---save----
			uint sv_traffic;
			uint sv_route;
			uint sv_polygons;

			//---color picker
			int red;
			int green;
			int blue;
			Cadena itemtype;


			//---tool tip
			bool tt_tooltip;
			bool tt_tool_tip_filedialog;
			HWND tt_hwnd_tool_tip;
			Cadena tt_text;
			int tt_x;
			int tt_y;

		};

		sToolCtrl m_tool_ctrl;

		Cadena mcadgral;
		Cadena mdirinit;


	public:
		//----------------------------------------
		WCtrl();
		~WCtrl();

		void init(sMainControl* mc, Cadena cadgral);
		void ctrl_clear();
		void set_icon();
		void start_db();

		void lock_menu();
		void unlock_menu();
		bool is_menu_locked(int index);
		bool get_file(Cadena caddirname, Cadena cadfilter, Cadena& cadfileget);
		bool save_file(Cadena caddirname, Cadena cadfilter, Cadena& cadfilesave);
		void refresh();
		void set_vehicle_size();
		void get_positions();
		void set_followvehicle();
		void follow_angle();

		void tools_action(enum eWCAction eaction,  double x = 0, double y = 0);
		void tools_finish();
		void route_action(enum eWCAction eaction, double x = 0, double y = 0);
		void snap_action(enum eWCAction eaction, double x = 0, double y = 0);
		void clean_action(enum eWCAction eaction);
		void save_action(enum eWCAction eaction);
		void picker_color_action(enum eWCAction eaction);

		void resume_proxy_picker();

		void key_callback(int key, int scancode, int action, int mode);
	    void refresh_callback();
		void scroll_callback(double xoffset, double yoffset);
		void cursor_position_callback(double xpos, double ypos);
		void mouse_button_callback(int button, int action, int mods);
		void window_size_callback(int width, int height);
		
		void button_top_callback(int button, double x, double y);
		void button_left_callback(int button, double x, double y);
		void button_right_callback(int button, double x, double y);

		void setup_thread_handles(HANDLE handle);
		void terminate();
		void process_arguments(int argc, const char* argv[], Cadena& dbname, Cadena& dbdir);
		void set_IU_time();

		bool is_not_menu(double ypos);
		bool is_menu_top(double ypos);
		bool is_menu_right(double xpos);
		bool is_menu_left(double xpos);

		void setup_camera_position();
		void setup_camera_direction();

		void move_left();
		void move_right();
		void move_up();
		void move_down();

		void move_turn_up();
		void move_turn_down();
		void move_turn_left();
		void move_turn_right();

		void move_zoom_in();
		void move_zoom_out();

		void move_to(double x, double y);

		//Dialog functions--------------------------------------------------------
		bool tooltip_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		bool proxy_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		bool color_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		bool tool_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		bool place_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		bool clean(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
		bool save(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

BOOL CALLBACK GoToProxyPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToColorPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToToolPicker (HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToPlacePicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToClean(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToSave(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GoToToolTipPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);


#endif

