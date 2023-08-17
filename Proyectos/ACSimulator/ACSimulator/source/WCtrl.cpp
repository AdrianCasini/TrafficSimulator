#include "WCtrl.h"


//------------------------------------------------------------------------------
WCtrl::WCtrl()
{

}

//------------------------------------------------------------------------------
WCtrl::~WCtrl()
{
	
}

//------------------------------------------------------------------------------
void WCtrl::init(sMainControl* mc, Cadena cadgral)
{
	mMC = mc;

	mHandleMax = 0;

	set_icon();

	mLastTime                = glfwGetTime();
	mMC->mTR->mTH.delta_time = 0.02;
	mMC->mTR->mDebug         = mDebug;
	mMC->mTR->mTH.dir_init   = cadgral;

	mcadgral = cadgral;

	m_menu_locked = false;
	mButtonActiveException = Menu::SIM_NO_BUTTON;
	ctrl_clear();

	mWindowStarted = false; 
	mPosIndex = 0;
	get_positions();


	//Follow Route variables
	mVH_TripIndex_N =0;
	mVH_MaxNodesTrip_N =0;
	mVH_Ordennode_N =0;
	mVH_speed_N = 150.0;
	mVH_Nnd2nd_N =0.0;
	mVH_Posx_N = 0.0;
	mVH_Posy_N = 0.0;

	mVH_TripIndex_S = 0;
	mVH_MaxNodesTrip_S = 0;
	mVH_Ordennode_S = 0;
	mVH_speed_S = 150.0;
	mVH_Nnd2nd_S = 0.0;
	mVH_Posx_S = 0.0;
	mVH_Posy_S = 0.0;

	mVH_angle_follow_count = 0;
	mVH_angle_follow_old = 0.0;
	mVH_angle_follow_now = 0.0;
	mVH_delta_ang = 0.0;
	mVH_follow = false;
	mNightBackground=false;

	m_tool_ctrl.tt_tooltip = false;
	m_tool_ctrl.tt_tool_tip_filedialog = false;

}

//------------------------------------------------------------------------------
void WCtrl::set_followvehicle()
{
	Cadena cadwayfile;
	double deltaPwLane, deltax, deltay;

	cadwayfile.formCadena("fin_ini.txt");
	if (mMC->mTR->mpTST->get_trip_index(cadwayfile, "TRIP", mVH_TripIndex_N, mMC->mTR->mTH.trip_index))
	{
		mVH_MaxNodesTrip_N = mMC->mTR->mTH.pTripI[mVH_TripIndex_N].max_nodos;
		mVH_PtrNodeInfoNow_N = &(mMC->mTR->mTH.pTripI[mVH_TripIndex_N].ntrip[0]);

		deltaPwLane = mMC->mTR->mTH.delta_lane * Simulator::get_delta_lane(1, mVH_PtrNodeInfoNow_N->lanes);
		deltax = deltaPwLane * mVH_PtrNodeInfoNow_N->delta_lane_x;
		deltay = deltaPwLane * mVH_PtrNodeInfoNow_N->delta_lane_y;
		mVH_Posx_N = mVH_PtrNodeInfoNow_N->X - deltax;
		mVH_Posy_N = mVH_PtrNodeInfoNow_N->Y - deltay;

	}


	cadwayfile.formCadena("ini_fin.txt");
	if (mMC->mTR->mpTST->get_trip_index(cadwayfile, "TRIP", mVH_TripIndex_S, mMC->mTR->mTH.trip_index))
	{
		mVH_MaxNodesTrip_S = mMC->mTR->mTH.pTripI[mVH_TripIndex_S].max_nodos;
		mVH_PtrNodeInfoNow_S = &(mMC->mTR->mTH.pTripI[mVH_TripIndex_S].ntrip[0]);

		deltaPwLane = mMC->mTR->mTH.delta_lane * Simulator::get_delta_lane(1, mVH_PtrNodeInfoNow_S->lanes);
		deltax = deltaPwLane * mVH_PtrNodeInfoNow_S->delta_lane_x;
		deltay = deltaPwLane * mVH_PtrNodeInfoNow_S->delta_lane_y;
		mVH_Posx_S = mVH_PtrNodeInfoNow_S->X - deltax;
		mVH_Posy_S = mVH_PtrNodeInfoNow_S->Y - deltay;

	}
}
//------------------------------------------------------------------------------
void WCtrl::ctrl_clear()
{

	//--polygon----
	m_tool_ctrl.toll = false;
	m_tool_ctrl.speed = false;
	m_tool_ctrl.speed_value = 100;
	m_tool_ctrl.avoid_gps = false;
	m_tool_ctrl.selected = 0;

	//----proxy_picker----
	m_tool_ctrl.pp_master_trip = 0;
	m_tool_ctrl.pp_sim_detector = 0;
	m_tool_ctrl.pp_sim_gps = 0;
	m_tool_ctrl.pp_sattol_gps_eurotoll = 0;
	m_tool_ctrl.pp_sattol_gps_eurotoll_perf = 0;

	//--clean----
	m_tool_ctrl.cl_ply = 0;
	m_tool_ctrl.cl_tr = 0;
	m_tool_ctrl.cl_poi = 0;
	m_tool_ctrl.cl_rt = 0;
	m_tool_ctrl.cl_sn = 0;
	m_tool_ctrl.cl_trc = 0;
	m_tool_ctrl.cl_color = 0;

	//--save----
	m_tool_ctrl.sv_traffic=0;
	m_tool_ctrl.sv_route=0;
	m_tool_ctrl.sv_polygons = 0;

}
//------------------------------------------------------------------------------
void WCtrl::set_icon()
{
	unsigned char* ii;
	unsigned iw, ih;
	lodepng_decode32_file(&ii, &iw, &ih, SIM_DIR_IMAGES_"simulator.png");

	mIcon[0].pixels = ii;
	mIcon[0].width  = iw;
	mIcon[0].height = ih;
	mIcon[1].pixels = ii;
	mIcon[1].width  = iw;
	mIcon[1].height = ih;
}

//------------------------------------------------------------------------------
void WCtrl::process_arguments(int argc, const char* argv[], Cadena& dbname, Cadena& cadgral)
{
	mDateInitial = 1801000;
	dbname = "traffic_simulator";
	mDebug = false;
	mcadgral = cadgral;

	size_t found;
	string str = argv[0];
	found = str.find_first_of("/\\");
	mdirinit = (str.substr(0, found)).c_str();

	//cout << "Nr argumentos:" << argc << endl;
	if (argc == 1)
	{

	}
	else if (argc >= 2)
	{
		int i = 1;
		for (i = 1; i < argc; i++)
		{

			if (strcmp(argv[i], "-t") == 0)
			{
				i++;
				mDateInitial = atoi(argv[i]);
			}
			else if (strcmp(argv[i], "-n") == 0)
			{
				i++;
				dbname = Cadena(argv[i]);
			}
			else if (strcmp(argv[i], "-d") == 0)
			{
				i++;
				cadgral = Cadena(argv[i]);
				cadgral.formCadena("%s:", cadgral.getCadena());
				
			}
			else if (strcmp(argv[i], "-g") == 0)
			{
				mDebug = true;
			}
			else
			{
				cout << "Error en argumentos:" << endl;

				cout << "Ejemplo: " << argv[0] << " -n traffic_simulator -d e" << endl;
				cout << "Ejemplo: " << argv[0] << " -n traffic_simulator -d e -t 1801010 " << endl;
				exit(1);
			}
		}
	}
	else
	{
		cout << "Error en argumentos:" << endl;
		cout << "Ejemplo: " << argv[0] << " -t 1801010 -d traffic_simulator" << endl;
		exit(1);
	}


	mcadgral = cadgral;
}

//------------------------------------------------------------------------------
void WCtrl::start_db()
{
	Cadena cadexe;
	int status;

	cadexe.formCadena("%s%s%s -U %s -D %s/%s start 2>&1 >> log_db.txt", mdirinit.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PGCTL, SIM_DB_USER, mcadgral.getCadena(), SIM_DIR_DB_NAME);
	//cout << cadexe.getCadena() << endl;
	status = system(cadexe.getCadena());

	if (status == 3)
	{
		cadexe.formCadena("%s%s%s -U %s -D %s%s start 2>&1 >> log_db.txt", mdirinit.getCadena(), SIM_DIR_PQ_BIN_, SIM_PQ_PGCTL, SIM_DB_USER, mcadgral.getCadena(), SIM_DIR_DB_NAME);
		system(cadexe.getCadena());
	}
}

//------------------------------------------------------------------------------
void WCtrl::setup_thread_handles(HANDLE handle)
{
	mThreadHandles[mHandleMax] = handle;
	mHandleMax++;
}

//------------------------------------------------------------------------------
void WCtrl::terminate()
{
	for (int i = 0; i < MAX_THREAD_HANDLES; i++)
	{
		CloseHandle(mThreadHandles[i]);
	}
	glfwSetWindowShouldClose(mMC->mWIN, GL_TRUE);
	glfwTerminate();
}

//------------------------------------------------------------------------------
void WCtrl::set_IU_time()
{
	mMC->mTR->mTH.current_time = glfwGetTime();
	mMC->mTR->mTH.delta_time = mMC->mTR->mTH.current_time - mMC->mTR->mTH.last_time;
	mMC->mTR->mTH.last_time = mMC->mTR->mTH.current_time;
}

//------------------------------------------------------------------------------
void WCtrl::scroll_callback(double xoffset, double yoffset)
{
	if ((-yoffset / abs(yoffset)) > 0)
	{
		move_zoom_out();
	}
	else
	{
		move_zoom_in();
	}
}

//------------------------------------------------------------------------------
void WCtrl::mouse_button_callback(int button, int action, int mods)
{

	int btt_MT, btt_RT, btt_LF;
	double xpos, ypos;

	glfwGetCursorPos(mMC->mWIN, &xpos, &ypos);

	if (m_tool_ctrl.tt_tooltip == true)
	{
		DestroyWindow(m_tool_ctrl.tt_hwnd_tool_tip);
		m_tool_ctrl.tt_tooltip = false;
		if (m_tool_ctrl.tt_tool_tip_filedialog != true)
		{
			mouse_button_callback(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, mods);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mMC->mCTRL->mMouseBttLeftPressed = true;
		mMC->mCTRL->mxscreen_pressI      = xpos;
		mMC->mCTRL->myscreen_pressI      = ypos;
		
		btt_MT    = mMC->mMN->get_button(mMC->mMN->mMENU_BTT_W, mMC->mMN->mMENU_BTT_H, mMC->mMN->mMENU_BTT_S, mMC->mMN->mMENU_BTT_DX, mMC->mMN->mMENU_BTT_DY, xpos, ypos);
		btt_RT    = mMC->mMRGT->get_button(mMC->mMRGT->mButtons, xpos, ypos);
		btt_LF    = mMC->mMLFT->get_button(mMC->mMLFT->mMENU_BTT_W, mMC->mMLFT->mMENU_BTT_H, mMC->mMLFT->mMENU_BTT_S, mMC->mMLFT->mMENU_BTT_DX, mMC->mMLFT->mMENU_BTT_DY, xpos, ypos);

		if      (btt_MT != -1) 	{if (!is_menu_locked(btt_MT)) {	button_top_callback(btt_MT, xpos, ypos);}}
		else if (btt_RT != -1)	{if (!is_menu_locked(btt_RT)) {	button_right_callback(btt_RT, xpos, ypos);}}
		else if (btt_LF != -1)  {if (!is_menu_locked(btt_LF)) {	button_left_callback(btt_LF, xpos, ypos);}}
		else // BUTTON LEFT PRESSED BUT NOT MENU
		{
			tools_action(SIM_WCACT_BUTTON_PRESSED_NO_MENU, xpos, ypos);

			if (mMC->mMN->mButtons[Menu::SIM_BTT_ROUTE].flag == true)
			{
				glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
				mMC->mRT->point_init(true);
			}
			else if (mMC->mMN->mButtons[Menu::SIM_BTT_SNAP].flag == true)
			{
				glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
				mMC->mRT->point_init(true);
			}
			else if (is_not_menu(ypos))
			{
				glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_GRAB);
			}
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mMC->mCTRL->mMouseBttLeftPressed = false;
		if (mMC->mMN->mButtons[Menu::SIM_BTT_ROUTE].flag == true)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
			if (mMC->mRT->mPoint_init == true)
			{
				mMC->mRT->add_point(xpos, ypos, 0);
			}
		}
		else if (mMC->mMN->mButtons[Menu::SIM_BTT_SNAP].flag == true)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
			if (mMC->mRT->mPoint_init == true)
			{
				mMC->mRT->add_point(xpos, ypos, 0);
			}
		}
	}
}

//------------------------------------------------------------------------------
void WCtrl::button_top_callback(int button, double x, double y)
{
	Cadena cadfile, caddir;
	HWND hwndGoto;

	switch (button)
	{

		//---------------------------------------------------------------
		//                        BUTTON PLAY  
		//---------------------------------------------------------------
	case Menu::SIM_BTT_PLAY:
		lock_menu();
		m_tool_ctrl.tt_tool_tip_filedialog = true;
		caddir = mcadgral + SIM_DIR_TEST_SIMUL_WIN;

		if (get_file(caddir, "*.*\0Text\0*.TXT\0", cadfile))
		{
			mMC->mTR->play_simulation(cadfile, Traffic::SIM_TR_METHOD_PLAY_FROM_SOURCETRIP);
		}
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON PAUSE TEST  
		//---------------------------------------------------------------
	case Menu::SIM_BTT_PAUSE:
		lock_menu();
		mMC->mTR->pause();
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON ALGORITHM  
		//---------------------------------------------------------------
	case Menu::SIM_BTT_ALGORITHM:
		lock_menu();
		mMC->mTR->alg();
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON SAVE TEST
		//---------------------------------------------------------------
	case Menu::SIM_BTT_SAVE_TEST:
		lock_menu();
		hwndGoto = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_SAVE), 0, (DLGPROC)GoToSave);
		ShowWindow(hwndGoto, SW_SHOW);
		SetFocus(hwndGoto);
		break;

		//---------------------------------------------------------------
		//                 BUTTON SET TEST
		//---------------------------------------------------------------
	case Menu::SIM_BTT_SET_TEST:
		lock_menu();
		m_tool_ctrl.tt_tool_tip_filedialog = true;
		caddir = mcadgral + SIM_DIR_TEST_SVTEST_WIN;
		if (get_file(caddir, "*.*\0Text\0*.TXT\0", cadfile))
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_WAIT);
			mMC->mTR->play_simulation(cadfile, Traffic::SIM_TR_METHOD_PLAY_FROMSAVEDFILE);
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_HAND);
		}
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON SEND INFO 
		//---------------------------------------------------------------
	case Menu::SIM_BTT_SEND_INFO:
		lock_menu();
		hwndGoto = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_PROXY_PICKER), 0, (DLGPROC)GoToProxyPicker);
		ShowWindow(hwndGoto, SW_SHOW);
		SetFocus(hwndGoto);
		break;

		//---------------------------------------------------------------
		//                 BUTTON TOOLS  
		//---------------------------------------------------------------

	case Menu::SIM_BTT_TOOLS:
		tools_action(SIM_WCACT_BUTTON_PRESS);
		break;

		//---------------------------------------------------------------
		//                 BUTTON ROUTE  
		//---------------------------------------------------------------
	case Menu::SIM_BTT_ROUTE:
		route_action(SIM_WCACT_BUTTON_PRESS);
		break;

		//---------------------------------------------------------------
		//                 BUTTON SNAP  
		//---------------------------------------------------------------
	case Menu::SIM_BTT_SNAP:
		snap_action(SIM_WCACT_BUTTON_PRESS);
		break;

		//---------------------------------------------------------------
		//                 BUTTON SNAP FROM FILE 
		//---------------------------------------------------------------
	case Menu::SIM_BTT_SNAPFILE:
		lock_menu();
		m_tool_ctrl.tt_tool_tip_filedialog = true;
		caddir = mcadgral + SIM_DIR_TEST_SNAPFILES_WIN;
		if (get_file(caddir, "*.*\0Text\0*.TXT\0", cadfile))
		{
			mMC->mRT->snap_from_file(cadfile);
		}
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON TRACE
		//---------------------------------------------------------------
	case Menu::SIM_BTT_TRACE:
		lock_menu();
		m_tool_ctrl.tt_tool_tip_filedialog = true;
		caddir = mcadgral + SIM_DIR_TEST_TRACE_WIN;
		get_file(caddir, "*.*\0Text\0*.TXT\0", cadfile);
		mMC->mTC->get_points_from_file(cadfile);
		mMC->mTC->refresh_size(mMC->mCTRL->mPosition.z);

		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON 3D
		//---------------------------------------------------------------
	case Menu::SIM_BTT_3D:
		lock_menu();
		mMC->mTR->render3d();
		mMC->mWRM->button_pressed_render3d();
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 BUTTON 3D MAX
		//---------------------------------------------------------------
	case Menu::SIM_BTT_3D_MAX:
		lock_menu();
		mMC->mTR->render3d_max();
		unlock_menu();
		break;


		//---------------------------------------------------------------
		//                 BUTTON TURBO
		//---------------------------------------------------------------
	case Menu::SIM_BTT_TURBO:
		lock_menu();
		mMC->mTR->turbo();
		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                        BUTTON FIND  
		//---------------------------------------------------------------
	case Menu::SIM_BTT_FIND:
		lock_menu();

		hwndGoto = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_PLACE_PICKER), 0, (DLGPROC)GoToPlacePicker);
		ShowWindow(hwndGoto, SW_SHOW);
		SetFocus(hwndGoto);

		unlock_menu();
		break;

		//---------------------------------------------------------------
		//                 CLEAN
		//---------------------------------------------------------------
	case Menu::SIM_BTT_CLEAN:

		lock_menu();
		hwndGoto = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_CLEAN), 0, (DLGPROC)GoToClean);
		ShowWindow(hwndGoto, SW_SHOW);
		SetFocus(hwndGoto);

		break;

		//---------------------------------------------------------------
		//                 SECTION MANAGER
		//---------------------------------------------------------------
	case Menu::SIM_BTT_SECMNGR:

		lock_menu();
		m_tool_ctrl.tt_tool_tip_filedialog = true;
		caddir = mcadgral + SIM_DIR_TEST_SECTIONS_WIN;
		get_file(caddir, "*.*\0Text\0*.TXT\0", cadfile);
		mMC->mSM->get_roadsection_info(cadfile);
		unlock_menu();

		break;

		//---------------------------------------------------------------
		//                 SECTION INRIX SPEED BUCKET
		//---------------------------------------------------------------
	case Menu::SIM_BTT_INRIX_SPEED_BUCKET :

		lock_menu();
		mMC->mIX->set_reset_speed_bucket(Inrix::SIM_INRIX_SPEED_BUCKET);
		unlock_menu();

		break;

		//---------------------------------------------------------------
		//                 SECTION INRIX SPEED
		//---------------------------------------------------------------
	case Menu::SIM_BTT_INRIX_SPEED:

		lock_menu();
		mMC->mIX->set_reset_speed(Inrix::SIM_INRIX_SPEED);
		unlock_menu();

		break;

		//---------------------------------------------------------------
		//      DEFAULT: 
		//---------------------------------------------------------------
	default: 
		break;
	}

}

//------------------------------------------------------------------------------
void WCtrl::resume_proxy_picker()
{
	bool active;

	mMC->mTR->mTH.pPxy->mSendInfo.master_trip                 = m_tool_ctrl.pp_master_trip;
	mMC->mTR->mTH.pPxy->mSendInfo.sim_detector                = m_tool_ctrl.pp_sim_detector;
	mMC->mTR->mTH.pPxy->mSendInfo.sim_gps                     = m_tool_ctrl.pp_sim_gps;
	mMC->mTR->mTH.pPxy->mSendInfo.sattol_gps_eurotoll         = m_tool_ctrl.pp_sattol_gps_eurotoll;
	mMC->mTR->mTH.pPxy->mSendInfo.sattol_gps_eurotoll_perf    = m_tool_ctrl.pp_sattol_gps_eurotoll_perf;

	active = (m_tool_ctrl.pp_sim_detector        ||
		m_tool_ctrl.pp_sim_gps                   ||
		m_tool_ctrl.pp_sattol_gps_eurotoll       ||
		m_tool_ctrl.pp_sattol_gps_eurotoll_perf  ||
		m_tool_ctrl.pp_master_trip);

	mMC->mTR->send_info(active);

	if (active)
	{
		mMC->mMN->mButtons[Menu::SIM_BTT_SEND_INFO].flag = true;
	}
	else
	{
		mMC->mMN->mButtons[Menu::SIM_BTT_SEND_INFO].flag = false;
	}

	unlock_menu();
}


//------------------------------------------------------------------------------
void WCtrl::clean_action(enum eWCAction eaction)
{
	switch (eaction)
	{

		//-----------------------------
	case SIM_WCACT_BUTTON_OK:

		if (m_tool_ctrl.cl_ply)
		{
			mMC->mTLS->clear();
			mMC->mTR->clear_polygon();
			mMC->mMN->mButtons[Menu::SIM_BTT_TOOLS].flag = false;

		}
		if (m_tool_ctrl.cl_tr)
		{
			mMC->mTR->clear();
			mMC->mMN->mButtons[Menu::SIM_BTT_TOOLS].flag = false;
			mMC->mMN->mButtons[Menu::SIM_BTT_PLAY].flag = false;
			mMC->mMN->mButtons[Menu::SIM_BTT_PAUSE].flag = false;
			mMC->mMN->mButtons[Menu::SIM_BTT_ALGORITHM].flag = false;
		}
		if (m_tool_ctrl.cl_poi)
		{
			mMC->mPOI->clear();
			mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_POINTOFINTEREST].flag = false;

		}
		if (m_tool_ctrl.cl_rt)
		{
			mMC->mRT->clear();
			mMC->mMN->mButtons[Menu::SIM_BTT_ROUTE].flag = false;

		}
		if (m_tool_ctrl.cl_sn)
		{
			mMC->mRT->clear();
			mMC->mMN->mButtons[Menu::SIM_BTT_SNAP].flag = false;
		}
		if (m_tool_ctrl.cl_trc)
		{
			mMC->mTC->clear();
			mMC->mMN->mButtons[Menu::SIM_BTT_TRACE].flag = false;

		}
		if (m_tool_ctrl.cl_color)
		{
			mMC->mMMG->clearcolor();
			mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_CPICKER].flag = false;
		}
		if (m_tool_ctrl.cl_section)
		{
			mMC->mMN->mButtons[Menu::SIM_BTT_SECMNGR].flag = false;
			mMC->mSM->clear();
		}


		mMC->mTRF->clear();
		mMC->mGPT->clear();
		
		unlock_menu();
		break;


		//-----------------------------
	default:
		break;

	}
}


//------------------------------------------------------------------------------
void WCtrl::tools_action(enum eWCAction eaction, double x, double y)
{
	HWND hwnd;
	switch (eaction)
	{
		//-----------------------------
		case SIM_WCACT_BUTTON_PRESS:
			lock_menu();
			if (mMC->mTLS->mActionActive == false)
			{
				mMC->mMN->mButtons[Menu::SIM_BTT_TOOLS].flag = true;
				hwnd = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_TOOL_PICKER), 0, (DLGPROC)GoToToolPicker);
				ShowWindow(hwnd, SW_SHOW);
			}
			else
			{
				tools_finish();
			}

			break;
		
		//-----------------------------
		case SIM_WCACT_BUTTON_PRESSED_NO_MENU:
			if (mMC->mTLS->mActionActive == true)
			{
				glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
				mMC->mTLS->add_line(x, y, 0);
			}

			break;

       //-----------------------------
		case SIM_WCACT_BUTTON_OK:

			mMC->mTLS->mSpeed = m_tool_ctrl.speed_value;

			if (m_tool_ctrl.selected > 0)
			{
				mMC->mTLS->tool_selected(m_tool_ctrl.selected);
				mMC->mTLS->activate();
				mMC->mMN->mButtons[Menu::SIM_BTT_TOOLS].flag = true;
				mButtonActiveException = Menu::SIM_BTT_TOOLS;
			}
			else
			{
				tools_finish();
			}

			break;

		//-----------------------------
		case SIM_WCACT_BUTTON_ESC:
			if (mMC->mTLS->mActionActive == true)
			{
				tools_finish();
			}

			break;

		//-----------------------------
		case SIM_WCACT_CURCALLBACK:
			if (mMC->mTLS->mActionActive == true)
			{
				glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
				mMC->mTLS->follow_line(x, y, 0.0);
			}
			break;

		//-----------------------------
		case SIM_WCACT_BUTTON_CANCEL:
			tools_finish();
			break;

		//-----------------------------
		default:
			break;

	}
}

//------------------------------------------------------------------------------
void WCtrl::tools_finish()
{
	mMC->mMN->mButtons[Menu::SIM_BTT_TOOLS].flag = false;
	mButtonActiveException = Menu::SIM_NO_BUTTON;
	mMC->mTLS->deactivate();
	unlock_menu();
}

//------------------------------------------------------------------------------
void WCtrl::save_action(enum eWCAction eaction)
{
	Cadena cadfile_simulation, cad_file_source, cad_file_traffic, cad_file_polygons, caddir;

	switch (eaction)
	{

		//-----------------------------
	case SIM_WCACT_BUTTON_OK:

		if (m_tool_ctrl.sv_route)
		{
			if (mMC->mRT->is_active())
			{
				caddir = mcadgral + SIM_DIR_TEST_SOURCE_WIN;
				MessageBox(NULL, "Choose the SOURCE file for the simulation", "", MB_OK);
				if (get_file(caddir, "*.*\0Text\0*.TXT\0", cad_file_source))
				{
					MessageBox(NULL, "Choose the SIMULATION file for storing the new simulation", "", MB_OK);
					caddir = mcadgral + SIM_DIR_TEST_SIMUL_WIN;
					if (save_file(caddir, "*.*\0Text\0*.BIN\0", cadfile_simulation))
					{
						mMC->mRT->create_simulation_file(cad_file_source, cadfile_simulation);
					}
				}
			}
			else
			{
				MessageBox(NULL, "Thereis no Route active", "", MB_OK);
			}

		}
		else if (m_tool_ctrl.sv_traffic)
		{
			caddir = mcadgral + SIM_DIR_TEST_SVTEST_WIN;
			if (save_file(caddir, "*.*\0Text\0*.BIN\0", cad_file_traffic))
			{
				mMC->mTR->save_test(cad_file_traffic);
			}
		}
		else if (m_tool_ctrl.sv_polygons)
		{
			caddir = mcadgral + SIM_DIR_TEST_POLY_WIN;
			if (save_file(caddir, "*.*\0Text\0*.BIN\0", cad_file_polygons))
			{
				mMC->mTLS->save_polygon(cad_file_polygons);
			}
		}

		unlock_menu();


	case SIM_WCACT_BUTTON_CANCEL:
	default:
		unlock_menu();
		break;
	}
}


//------------------------------------------------------------------------------
void WCtrl::route_action(enum eWCAction eaction, double x, double y)
{
	Cadena cadfile_simulation, cad_file_source;
	switch (eaction)
	{
		//-----------------------------
	case SIM_WCACT_BUTTON_PRESS:
		lock_menu();
		if (mMC->mRT->mActionActiveRoute == false)
		{
			mMC->mRT->activate_route();
			mMC->mMN->mButtons[Menu::SIM_BTT_ROUTE].flag = true;
			mButtonActiveException = Menu::SIM_BTT_ROUTE;
		}
		else
		{
			mMC->mMN->mButtons[Menu::SIM_BTT_ROUTE].flag = false;
			mButtonActiveException = Menu::SIM_NO_BUTTON;
			mMC->mRT->deactivate_route();
			unlock_menu();
		}

		break;

		//-----------------------------
	case SIM_WCACT_BUTTON_PRESSED_NO_MENU:
		if (mMC->mRT->mActionActiveRoute == true)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
			mMC->mRT->add_point(x, y, 0);
		}

		break;

		//-----------------------------
	default:
		break;

	}
}

//------------------------------------------------------------------------------
void WCtrl::snap_action(enum eWCAction eaction, double x, double y)
{
	Cadena cadfile_simulation, cad_file_source;
	switch (eaction)
	{
		//-----------------------------
	case SIM_WCACT_BUTTON_PRESS:
		lock_menu();
		if (mMC->mRT->mActionActiveSnap == false)
		{
			mMC->mRT->activate_snap();
			mMC->mMN->mButtons[Menu::SIM_BTT_SNAP].flag = true;
			mButtonActiveException = Menu::SIM_BTT_SNAP;
		}
		else
		{
			mMC->mMN->mButtons[Menu::SIM_BTT_SNAP].flag = false;
			mButtonActiveException = Menu::SIM_NO_BUTTON;
			mMC->mRT->deactivate_snap();
			unlock_menu();
		}

		break;

		//-----------------------------
	case SIM_WCACT_BUTTON_PRESSED_NO_MENU:
		if (mMC->mRT->mActionActiveSnap == true)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
			mMC->mRT->add_point(x, y, 0);
		}

		break;

		//-----------------------------
	default:
		break;

	}
}

//------------------------------------------------------------------------------
void WCtrl::picker_color_action(enum eWCAction eaction)
{

	switch (eaction)
	{
		//-----------------------------
	case SIM_WCACT_BUTTON_PRESSED_SELECT_COLOR:
		for (int i = 0; i < mMC->mMMG->mCountM_L; i++)
		{
			if (strcmp(mMC->mMMG->MPP_L[i]->mFilename.getCadena(), m_tool_ctrl.itemtype.getCadena()) == 0)
			{
				mMC->mMMG->MPP_L[i]->mVectColor.r = (float)(float(m_tool_ctrl.red) / 255.0);
				mMC->mMMG->MPP_L[i]->mVectColor.g = (float)(float(m_tool_ctrl.green) / 255.0);
				mMC->mMMG->MPP_L[i]->mVectColor.b = (float)(float(m_tool_ctrl.blue) / 255.0);
				m_tool_ctrl.itemtype = "";
			}
		}
		break;

		//-----------------------------
	case SIM_WCACT_BUTTON_OK:
		unlock_menu();
		break;

		//-----------------------------
	default:
		break;

	}
}

//------------------------------------------------------------------------------
void WCtrl::get_positions()
{
	Cadena cadfile;

	mPositions.clear();
	cadfile.formCadena(SIM_DIR_TEST_POSITIONS_"c32_sur.txt");
	cadfile = mcadgral + cadfile;

	if (!mPositions.get_data_from_txt(cadfile))
	{
		cout << "Error: There is an error obtaining information from file:[" << cadfile.getCadena() << "]" << endl;
	}
}

//------------------------------------------------------------------------------
bool WCtrl::get_file(Cadena caddirname, Cadena cadfilter, Cadena& cadfile)
{
	OPENFILENAME of_ofn;
	char dir_name[100];
	char filter[100];
	char title[100];
	char file_name[100];
	bool resp = false;

	strcpy(dir_name, caddirname.getCadena());
	strcpy(filter, cadfilter.getCadena());
	strcpy(title, "Choose a File");
	dir_name[caddirname.getLong()]= '\0';
	filter[cadfilter.getLong()] = '\0';

	// open a file name
	ZeroMemory(&of_ofn, sizeof(of_ofn));
	of_ofn.lStructSize = sizeof(of_ofn);
	of_ofn.hwndOwner = NULL;
	of_ofn.lpstrFile = file_name;
	of_ofn.lpstrFile[0] = '\0';
	of_ofn.nMaxFile = sizeof(file_name);
	of_ofn.lpstrFilter = "*.*\0Text\0*.txt\0";
	of_ofn.nFilterIndex = 2;
	of_ofn.lpstrFileTitle = title;
	of_ofn.nMaxFileTitle = 0;
	of_ofn.lpstrInitialDir = dir_name;
	of_ofn.Flags = OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&of_ofn))
	{
		cadfile = file_name;
		resp = true;
	}

	m_tool_ctrl.tt_tool_tip_filedialog = false;
	return resp;
}

//------------------------------------------------------------------------------
bool WCtrl::save_file(Cadena caddirname, Cadena cadfilter, Cadena& cadfile)
{
	OPENFILENAME of_ofn;
	char dir_name[100];
	char filter[100];
	char title[100];
	char file_name[100];
	bool resp=false;

	cadfile = "";

	strcpy(dir_name, caddirname.getCadena());
	strcpy(filter, cadfilter.getCadena());
	strcpy(title, "Save a File");
	dir_name[caddirname.getLong()] = '\0';
	filter[cadfilter.getLong()] = '\0';

	// open a file name
	ZeroMemory(&of_ofn, sizeof(of_ofn));
	of_ofn.lStructSize = sizeof(of_ofn);
	of_ofn.hwndOwner = NULL;
	of_ofn.lpstrFile = file_name;
	of_ofn.lpstrFile[0] = '\0';
	of_ofn.nMaxFile = sizeof(file_name);
	of_ofn.lpstrFilter = "*.*\0Text\0*.txt\0";
	of_ofn.nFilterIndex = 2;
	of_ofn.lpstrFileTitle = title;
	of_ofn.nMaxFileTitle = 0;
	of_ofn.lpstrInitialDir = dir_name;
	of_ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileNameA(&of_ofn))
	{
		cadfile = file_name;
		cadfile.formCadena("%s%s", cadfile.getCadena(), ".txt");
		resp = true;
	}

	m_tool_ctrl.tt_tool_tip_filedialog = false;

	return resp;
}

//------------------------------------------------------------------------------
void WCtrl::button_left_callback(int button, double x, double y)
{
	HWND hwndGoto;
	Cadena cadfile, caddir;

	switch (button)
	{
		//---------------------------------------------------------------
		//                 BUTTON CREATE ROUTES  
		//---------------------------------------------------------------
		case MenuLeft::SIM_BTT_ML_CREATE_ROUTES:
			lock_menu();
			mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_CREATE_ROUTES].flag = true;
			caddir = mcadgral + SIM_DIR_TEST_IN_OUT_WIN;
			get_file(caddir, "*.*\0Text\0*.txt\0", cadfile);
			mMC->mRT->routes_from_file(cadfile);
			unlock_menu();
			break;

		//---------------------------------------------------------------
		//                 BUTTON POINT OF INTEREST  
		//---------------------------------------------------------------
		case MenuLeft::SIM_BTT_ML_POINTOFINTEREST:
			lock_menu();
			mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_POINTOFINTEREST].flag = true;
			caddir = mcadgral + SIM_DIR_TEST_DET_WIN;
			get_file(caddir, "*.*\0Text\0*.TXT\0", cadfile);
			mMC->mPOI->generate_draw(cadfile);
			unlock_menu();
			break;

		//---------------------------------------------------------------
		//                 BUTTON COLOR PICKER  
		//---------------------------------------------------------------
		case MenuLeft::SIM_BTT_ML_CPICKER:
			lock_menu();
			mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_CPICKER].flag = true;
			hwndGoto = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_COLOR_PICKER), 0, (DLGPROC)GoToColorPicker);
			ShowWindow(hwndGoto, SW_SHOW);
			break;

		//---------------------------------------------------------------
		//                 BUTTON TEXT  
		//---------------------------------------------------------------
		case MenuLeft::SIM_BTT_ML_TEXT:
			lock_menu();
			mMC->mTXT->active();
		    mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_TEXT].flag = mMC->mTXT->is_active();
			unlock_menu();
			break;


		//---------------------------------------------------------------
		//                 BUTTON NIGHT  
		//---------------------------------------------------------------
		case MenuLeft::SIM_BTT_ML_NIGHT:
			lock_menu();
			if (mNightBackground == true)
			{
				glClearColor(SIM_COLOUR_SEA_DAY);
				mNightBackground = false;
				mMC->mTR->mNightColor = false;
				mMC->mTR->set_day();
				mMC->mMMG->set_color_day();
				mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_NIGHT].flag = false;
			}
			else
			{
				glClearColor(SIM_COLOUR_SEA_NIGHT);
				mNightBackground = true;
				mMC->mTR->mNightColor = true;
				mMC->mTR->set_night();
				mMC->mMMG->set_color_night();
				mMC->mMLFT->mButtons[MenuLeft::SIM_BTT_ML_NIGHT].flag = true;
			}

			unlock_menu();
			break;


		default:
			break;

	}
}

//------------------------------------------------------------------------------
void WCtrl::button_right_callback(int button, double x, double y)
{

	switch (button)
	{

		//===============================================================================
		//       Pitch & Yawn
		//===============================================================================
		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_UP_R:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_turn_up();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_DOWN_R:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_turn_down();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_RIGHT_R:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_turn_right();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_LEFT_R:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_turn_left();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_ROTATE_R:
		//-------------------------------------------------------------------------------
		lock_menu();
		mMC->mCTRL->m_angle_xy = 0;
		mMC->mCTRL->m_angle_z = 0;
		setup_camera_position();
		unlock_menu();
		break;

		//===============================================================================
		//       UP-DOWN LEFT-RIGHT
		//===============================================================================

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_UP_MV:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_up();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_DOWN_MV:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_down();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_RIGHT_MV:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_right();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_LEFT_MV:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_left();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_MOVE_MV:
		//-------------------------------------------------------------------------------
		lock_menu();
		mMC->mCTRL->mZZ = SIM_POSZ_INITIAL;
		mMC->mCTRL->mXX = SIM_POSX_INITIAL_BCN;
		mMC->mCTRL->mYY = SIM_POSY_INITIAL_BCN;
		setup_camera_direction();
		setup_camera_position();

		unlock_menu();
		break;

		//===============================================================================
		//                      ZOOM
		//===============================================================================

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_ZOOM_IN:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_zoom_in();
		unlock_menu();
		break;

		//-------------------------------------------------------------------------------
	case MenuRight::SIM_BTT_ZOOM_OUT:
		//-------------------------------------------------------------------------------
		lock_menu();
		move_zoom_out();
		unlock_menu();
		break;

	default:
		break;

	}
}

//------------------------------------------------------------------------------
bool WCtrl::is_menu_locked(int index)
{
	if ((index == mButtonActiveException) || (index == -1))
	{
		return false;
	}
	return m_menu_locked;
}

//------------------------------------------------------------------------------
void WCtrl::lock_menu()
{
	m_menu_locked = true;
}

//------------------------------------------------------------------------------
void WCtrl::unlock_menu()
{
	m_menu_locked = false;
	mButtonActiveException = Menu::SIM_NO_BUTTON;
}

//------------------------------------------------------------------------------
void WCtrl::setup_camera_direction()
{
	mMC->mCTRL->mDirection = glm::vec3(mMC->mCTRL->mXX, mMC->mCTRL->mYY, 0);
}

//------------------------------------------------------------------------------
void WCtrl::setup_camera_position()
{
	mMC->mCTRL->mPosition.x = (float)mMC->mCTRL->mXX - (float)((mMC->mCTRL->mZZ * sin(mMC->mCTRL->m_angle_z) * sin(mMC->mCTRL->m_angle_xy)));
	mMC->mCTRL->mPosition.y = (float)mMC->mCTRL->mYY - (float)((mMC->mCTRL->mZZ * sin(mMC->mCTRL->m_angle_z) * cos(mMC->mCTRL->m_angle_xy)));
	mMC->mCTRL->mPosition.z = (float)mMC->mCTRL->mZZ * (float)cos(mMC->mCTRL->m_angle_z);

	mMC->mCTRL->mUp.x = (float) (sin(mMC->mCTRL->m_angle_xy) * cos(mMC->mCTRL->m_angle_z));
	mMC->mCTRL->mUp.y = (float) (cos(mMC->mCTRL->m_angle_xy) * cos(mMC->mCTRL->m_angle_z));
	mMC->mCTRL->mUp.z = (float) (cos(mMC->mCTRL->m_angle_z));
}

//------------------------------------------------------------------------------
void WCtrl::move_left()
{
	double delta_x, delta_y;

	delta_x = (mMC->mCTRL->mZZ / 50) * cos(mMC->mCTRL->m_angle_xy);
	delta_y = (mMC->mCTRL->mZZ / 50) * sin(mMC->mCTRL->m_angle_xy);

	mMC->mCTRL->mDirection.x = mMC->mCTRL->mDirection.x - (float)delta_x;
	mMC->mCTRL->mDirection.y = mMC->mCTRL->mDirection.y + (float)delta_y;

	mMC->mCTRL->mPosition.x = mMC->mCTRL->mPosition.x - (float)delta_x;
	mMC->mCTRL->mPosition.y = mMC->mCTRL->mPosition.y + (float)delta_y;

	mMC->mCTRL->mXX = mMC->mCTRL->mXX - (float)delta_x;
	mMC->mCTRL->mYY = mMC->mCTRL->mYY + (float)delta_y;

	set_vehicle_size();
}

//------------------------------------------------------------------------------
void WCtrl::move_right()
{
	double delta_x, delta_y;
	delta_x = (mMC->mCTRL->mZZ / 50) * cos(mMC->mCTRL->m_angle_xy);
	delta_y = (mMC->mCTRL->mZZ / 50) * sin(mMC->mCTRL->m_angle_xy);

	mMC->mCTRL->mDirection.x = mMC->mCTRL->mDirection.x + (float)delta_x;
	mMC->mCTRL->mDirection.y = mMC->mCTRL->mDirection.y - (float)delta_y;


	mMC->mCTRL->mPosition.x = mMC->mCTRL->mPosition.x + (float)delta_x;
	mMC->mCTRL->mPosition.y = mMC->mCTRL->mPosition.y - (float)delta_y;

	mMC->mCTRL->mXX = mMC->mCTRL->mXX + (float)delta_x;
	mMC->mCTRL->mYY = mMC->mCTRL->mYY - (float)delta_y;

	set_vehicle_size();
}

//------------------------------------------------------------------------------
void WCtrl::move_up()
{
	double delta_x, delta_y;
	delta_x = (mMC->mCTRL->mZZ / 50) * cos(mMC->mCTRL->m_angle_xy + SIM_PI / 2);
	delta_y = (mMC->mCTRL->mZZ / 50) * sin(mMC->mCTRL->m_angle_xy + SIM_PI / 2);

	mMC->mCTRL->mDirection.x = mMC->mCTRL->mDirection.x - (float)delta_x;
	mMC->mCTRL->mDirection.y = mMC->mCTRL->mDirection.y + (float)delta_y;

	mMC->mCTRL->mPosition.x = mMC->mCTRL->mPosition.x - (float)delta_x;
	mMC->mCTRL->mPosition.y = mMC->mCTRL->mPosition.y + (float)delta_y;

	mMC->mCTRL->mXX = mMC->mCTRL->mXX - (float)delta_x;
	mMC->mCTRL->mYY = mMC->mCTRL->mYY + (float)delta_y;

	set_vehicle_size();

}

//------------------------------------------------------------------------------
void WCtrl::move_down()
{
	double delta_x, delta_y;

	delta_x = (mMC->mCTRL->mZZ / 50) * cos(mMC->mCTRL->m_angle_xy + SIM_PI / 2.0);
	delta_y = (mMC->mCTRL->mZZ / 50) * sin(mMC->mCTRL->m_angle_xy + SIM_PI / 2.0);

	mMC->mCTRL->mDirection.x = mMC->mCTRL->mDirection.x + (float)delta_x;
	mMC->mCTRL->mDirection.y = mMC->mCTRL->mDirection.y - (float)delta_y;

	mMC->mCTRL->mPosition.x = mMC->mCTRL->mPosition.x + (float)delta_x;
	mMC->mCTRL->mPosition.y = mMC->mCTRL->mPosition.y - (float)delta_y;

	mMC->mCTRL->mXX = mMC->mCTRL->mXX + (float)delta_x;
	mMC->mCTRL->mYY = mMC->mCTRL->mYY - (float)delta_y;

	set_vehicle_size();

}

//------------------------------------------------------------------------------
void WCtrl::move_to(double x, double y)
{
	double delta_x, delta_y;

	delta_x = (mMC->mCTRL->mZZ / 50) * cos(mMC->mCTRL->m_angle_xy + SIM_PI / 2.0);
	delta_y = (mMC->mCTRL->mZZ / 50) * sin(mMC->mCTRL->m_angle_xy + SIM_PI / 2.0);

	mMC->mCTRL->mDirection.x = (float)x;
	mMC->mCTRL->mDirection.y = (float)y;

	mMC->mCTRL->mPosition.x = (float)x;
	mMC->mCTRL->mPosition.y = (float)y;

	mMC->mCTRL->mXX = (float)x;
	mMC->mCTRL->mYY = (float)y;
}

//------------------------------------------------------------------------------
void WCtrl::move_turn_up()
{
	double ang_dum = mMC->mCTRL->m_angle_z - mMC->mCTRL->m_delta_ang_z;

	if (ang_dum > 0)
	{
		mMC->mCTRL->m_angle_z = ang_dum;
	}

	set_vehicle_size();
	setup_camera_position();
}

//------------------------------------------------------------------------------
void WCtrl::move_turn_down()
{
	double ang_dum = mMC->mCTRL->m_angle_z + mMC->mCTRL->m_delta_ang_z;

	if (ang_dum < SIM_PI / 2)
	{
		mMC->mCTRL->m_angle_z = ang_dum;
	}

	set_vehicle_size();
	setup_camera_position();
}

//------------------------------------------------------------------------------
void WCtrl::move_turn_left()
{
	mMC->mCTRL->m_angle_xy = mMC->mCTRL->m_angle_xy + mMC->mCTRL->m_delta_ang_xy;
	setup_camera_position();
}

//------------------------------------------------------------------------------
void WCtrl::move_turn_right()
{
	mMC->mCTRL->m_angle_xy = mMC->mCTRL->m_angle_xy - mMC->mCTRL->m_delta_ang_xy;
	setup_camera_position();
}

//------------------------------------------------------------------------------
void WCtrl::move_zoom_in()
{
	mMC->mCTRL->mZZ = mMC->mCTRL->mZZ / 1.1;
	refresh();
	setup_camera_position();
}

//------------------------------------------------------------------------------
void WCtrl::move_zoom_out()
{
	mMC->mCTRL->mZZ = mMC->mCTRL->mZZ * 1.1;
	refresh();
	setup_camera_position();
}

//------------------------------------------------------------------------------
bool WCtrl::is_menu_top(double ypos)
{
	return (ypos < mMC->mMN->mMENU_BTT_H);
}

//------------------------------------------------------------------------------
bool WCtrl::is_menu_right(double xpos)
{
	return (xpos > mMC->mMRGT->mMENU_X_POS);
}

//------------------------------------------------------------------------------
bool WCtrl::is_menu_left(double xpos)
{
	return (xpos < mMC->mMLFT->mMENU_X_POS);
}

//------------------------------------------------------------------------------
bool WCtrl::is_not_menu(double ypos)
{
	return (!is_menu_top( ypos));
}

//------------------------------------------------------------------------------
void WCtrl::cursor_position_callback(double xpos, double ypos)
{
	int btt_index;
	HWND hwndGoto, parentW;

	mMC->mCTRL->mText = mMC->mCTRL->mTextI * mMC->mCTRL->mPosition.z;

	//===============================================
	//             Main Menu
	//===============================================
	if (is_menu_top(ypos))
	{

		btt_index = mMC->mMN->get_button(mMC->mMN->mMENU_BTT_W, mMC->mMN->mMENU_BTT_H, mMC->mMN->mMENU_BTT_S, mMC->mMN->mMENU_BTT_DX, mMC->mMN->mMENU_BTT_DY, xpos, ypos);
		if (btt_index != -1)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_HAND);

			mMC->mMN->mButtons[btt_index].over = true;
			mMC->mMN->mButtons[btt_index].ymv  = SIM_BUTTON_YMV;

			m_tool_ctrl.tt_text = mMC->mMN->mButtons[btt_index].tooltip;
			m_tool_ctrl.tt_x = xpos + 15;
			m_tool_ctrl.tt_y = ypos + 25;


			if (m_tool_ctrl.tt_tooltip == false)
			{
				m_tool_ctrl.tt_hwnd_tool_tip = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG_TOOLTIP), 0, (DLGPROC)GoToToolTipPicker);
				ShowWindow(m_tool_ctrl.tt_hwnd_tool_tip, SW_SHOWNA);
				parentW = GetParent(m_tool_ctrl.tt_hwnd_tool_tip);
				SetFocus(parentW);
				m_tool_ctrl.tt_tooltip = true;
			}
		}
		else
		{
			if (m_tool_ctrl.tt_tooltip == true)
			{
				DestroyWindow(m_tool_ctrl.tt_hwnd_tool_tip);
				m_tool_ctrl.tt_tooltip = false;
			}

			mMC->mMLFT->not_over();
			mMC->mMRGT->not_over();
			mMC->mMN->not_over();
		}
	}
	//===============================================
	//             Right Menu
	//===============================================
	else if (is_menu_right(xpos))
	{
		btt_index = mMC->mMRGT->get_button(mMC->mMRGT->mButtons, xpos, ypos);

		if (btt_index != -1)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_HAND);

			mMC->mMRGT->mButtons[btt_index].over = true;
			for (int i = 0; i < MenuRight::SIM_MAX_BUTTON_MR; i++)
			{
				if (i == btt_index)
				{
					mMC->mMRGT->mButtons[i].over = true;
				}
				else
				{
					mMC->mMRGT->mButtons[i].over = false;
				}
			}
		}
		else
		{
			mMC->mMLFT->not_over();
			mMC->mMRGT->not_over();
			mMC->mMN->not_over();
		}


		if (m_tool_ctrl.tt_tooltip == true)
		{
			DestroyWindow(m_tool_ctrl.tt_hwnd_tool_tip);
			m_tool_ctrl.tt_tooltip = false;
		}
	}
	//===============================================
	//             Left Menu
	//===============================================
	else if (is_menu_left(xpos))
	{
		btt_index = mMC->mMLFT->get_button(mMC->mMLFT->mMENU_BTT_W, mMC->mMLFT->mMENU_BTT_H, mMC->mMLFT->mMENU_BTT_S, mMC->mMLFT->mMENU_BTT_DX, mMC->mMLFT->mMENU_BTT_DY, xpos, ypos);

		if (btt_index != -1)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_HAND);

			mMC->mMLFT->mButtons[btt_index].over = true;
			for (int i = 0; i < MenuLeft::SIM_MAX_BUTTON_ML; i++)
			{
				if (i == btt_index)
				{
					mMC->mMLFT->mButtons[i].over = true;
				}
				else
				{
					mMC->mMLFT->mButtons[i].over = false;
				}
			}
		}
		else
		{
			mMC->mMLFT->not_over();
			mMC->mMRGT->not_over();
			mMC->mMN->not_over();
		}

		if (m_tool_ctrl.tt_tooltip == true)
		{
			DestroyWindow(m_tool_ctrl.tt_hwnd_tool_tip);
			m_tool_ctrl.tt_tooltip = false;
		}
	}
	else
	{

		if (m_tool_ctrl.tt_tooltip == true)
		{
			DestroyWindow(m_tool_ctrl.tt_hwnd_tool_tip);
			m_tool_ctrl.tt_tooltip = false;
		}


		mMC->mMN->not_over();
		mMC->mMRGT->not_over();
		mMC->mMLFT->not_over();

		if (mMC->mCTRL->mMouseBttLeftPressed)
		{
			double power = 0.0085;
			double deltax = xpos - mMC->mCTRL->mxscreen_pressI;
			double deltay = ypos - mMC->mCTRL->myscreen_pressI;
			double angle_delta = atan2(deltay, deltax);
			mMC->mCTRL->mxscreen_pressI = xpos;
			mMC->mCTRL->myscreen_pressI = ypos;

			deltax =  power * mMC->mCTRL->mZZ * cos(mMC->mCTRL->m_angle_xy + angle_delta);
			deltay =  power * mMC->mCTRL->mZZ * sin(mMC->mCTRL->m_angle_xy + angle_delta);

			mMC->mCTRL->mDirection.x = mMC->mCTRL->mDirection.x - (float)deltax;
			mMC->mCTRL->mDirection.y = mMC->mCTRL->mDirection.y + (float)deltay;


			mMC->mCTRL->mPosition.x = mMC->mCTRL->mPosition.x - (float)deltax;
			mMC->mCTRL->mPosition.y = mMC->mCTRL->mPosition.y + (float)deltay;

			mMC->mCTRL->mXX = mMC->mCTRL->mXX - (float)deltax;
			mMC->mCTRL->mYY = mMC->mCTRL->mYY + (float)deltay;

		}

		if (mMC->mTLS->mActionActive == true)
		{
			
			tools_action(SIM_WCACT_CURCALLBACK,xpos, ypos);
		}
		else if (mMC->mMN->mButtons[Menu::SIM_BTT_ROUTE].flag == true)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
			mMC->mRT->point_init(false);
		}
		else if (mMC->mMN->mButtons[Menu::SIM_BTT_SNAP].flag == true)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
			mMC->mRT->point_init(false);
		}
		else if (mMC->mCTRL->mMouseBttLeftPressed)
		{
			glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_GRAB);
		}
		else
		{
			glfwSetCursor(mMC->mWIN, NULL);
		}
	}
}

//------------------------------------------------------------------------------
void WCtrl::key_callback(int key, int scancode, int action, int mode)
{
	mMC->mRT->point_init(false);
	mMC->mCTRL->mSpeed_xy = 0.00015f * mMC->mCTRL->mPosition.z / 0.01;
	mMC->mCTRL->mDeltaZ   = Simulator::smoother_z(mMC->mCTRL->mPosition.z);
	mMC->mCTRL->mText     = mMC->mCTRL->mTextI * mMC->mCTRL->mPosition.z;

	switch (key)
	{
		//---------------------------------------------------------------------------------
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) {tools_action(SIM_WCACT_BUTTON_ESC);}
			break;

		//---------------------------------------------------------------------------------
		//---------------------------------------------------
		// Move Zoom In
		case GLFW_KEY_F:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {move_zoom_in();}
			break;
		//---------------------------------------------------
		// Move Zoom out
		case GLFW_KEY_G:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {move_zoom_out();}
			break;

		//---------------------------------------------------------------------------------
		//---------------------------------------------------
		//Turn up 
		case GLFW_KEY_I:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) { move_turn_up(); }
			break;
		//---------------------------------------------------
		//Turn down 
		case GLFW_KEY_K:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) { move_turn_down(); }
			break;
		//---------------------------------------------------
		//Turn Left 
		case GLFW_KEY_J:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) { move_turn_left(); }
			break;
		//---------------------------------------------------
		//Turn Right 
		case GLFW_KEY_L:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) { move_turn_right(); }
			break;

		//---------------------------------------------------------------------------------
		//---------------------------------------------------
		// Move left
		case GLFW_KEY_LEFT:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {move_left();}
			break;
		//---------------------------------------------------
		// Move Right
		case GLFW_KEY_RIGHT:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) { move_right(); }
			break;
		//---------------------------------------------------
		// Move Up
		case GLFW_KEY_UP:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {move_up();}
			break;
		//---------------------------------------------------
		// Move Down
		case GLFW_KEY_DOWN:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT)) {move_down();}
			break;

		//--------------- Position-----------------------
		// Position Search Upward
		case GLFW_KEY_X:
			if (action == GLFW_PRESS)
			{
				if (!mMC->mCTRL->mPositionSearch)
				{
					mMC->mCTRL->mPositionSearch = true;
					mMC->mCTRL->mXX = Simulator::getXfromLon(mPositions.mRegtable[mPosIndex].reg["POSITION_X"].num_d);
					mMC->mCTRL->mYY = Simulator::getYfromLat(mPositions.mRegtable[mPosIndex].reg["POSITION_Y"].num_d);
					setup_camera_direction();
					setup_camera_position();
					mPosIndex++;
					if (mPosIndex >= mPositions.mMaxreg) mPosIndex = 0;
				}
			}
			else if (action == GLFW_RELEASE)
			{
				mMC->mCTRL->mPositionSearch = false;
			}

			break;
		//---------------------------------------------------
		// Position Search Downward
		case GLFW_KEY_C:
			if (action == GLFW_PRESS)
			{
				if (!mMC->mCTRL->mPositionSearch)
				{
					mMC->mCTRL->mPositionSearch = true;
					mMC->mCTRL->mXX = Simulator::getXfromLon(mPositions.mRegtable[mPosIndex].reg["POSITION_X"].num_d);
					mMC->mCTRL->mYY = Simulator::getYfromLat(mPositions.mRegtable[mPosIndex].reg["POSITION_Y"].num_d);
					setup_camera_direction();
					setup_camera_position();
					mPosIndex--;
					if (mPosIndex < 0) mPosIndex = mPositions.mMaxreg-1;
				}
			}
			else if (action == GLFW_RELEASE)
			{
				mMC->mCTRL->mPositionSearch = false;
			}

			break;

		//--------------- Follower Movement Control -----------------------
		//---------------------------------------------------
	    // To the North
		case GLFW_KEY_N:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
			{
				mMC->mTR->mpLG->move_outsider(mVH_PtrNodeInfoNow_N, mVH_MaxNodesTrip_N, mVH_speed_N, mVH_Ordennode_N,mVH_Nnd2nd_N, mVH_Posx_N, mVH_Posy_N);
				mMC->mCTRL->mXX = mVH_Posx_N;
				mMC->mCTRL->mYY = mVH_Posy_N;
				mMC->mCTRL->m_angle_xy = -mVH_PtrNodeInfoNow_N->angle + (SIM_PI / 2);

				setup_camera_direction();
				setup_camera_position();
				mVH_follow = true;

			}
			//else if ()
			//{
			//	mVH_follow = false;
			//}

			break;
		//---------------------------------------------------
		// To the South
		case GLFW_KEY_B:
			if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
			{
				mMC->mTR->mpLG->move_outsider(mVH_PtrNodeInfoNow_S, mVH_MaxNodesTrip_S, mVH_speed_S, mVH_Ordennode_S, mVH_Nnd2nd_S, mVH_Posx_S, mVH_Posy_S);
				mMC->mCTRL->mXX = mVH_Posx_S;
				mMC->mCTRL->mYY = mVH_Posy_S;
				mMC->mCTRL->m_angle_xy = -mVH_PtrNodeInfoNow_S->angle + (SIM_PI/2);

				setup_camera_direction();
				setup_camera_position();

			}

			break;

		//---------------------------------------------------
		default:
			break;
	}
}
//------------------------------------------------------------------------------
void WCtrl::follow_angle()
{

	mVH_angle_follow_now = -mVH_PtrNodeInfoNow_N->angle + (SIM_PI / 2);
	if (mVH_angle_follow_now != mVH_angle_follow_old)
	{
		cout << "-----------------------------------------------------------------" << endl;
		cout << "mVH_angle_follow_now  " << mVH_angle_follow_now << endl;
		cout << "mVH_angle_follow_old  " << mVH_angle_follow_old << endl;
		cout << "-----------------------------------------------------------------" << endl;
		mVH_delta_ang = (mVH_angle_follow_now - mVH_angle_follow_old) / 20.0;
		mVH_angle_follow_old = mVH_angle_follow_now;
		mVH_angle_follow_count = 0;

	}

	mVH_angle_follow_count++;

	if (mVH_angle_follow_count > 20)
	{
		mVH_delta_ang = 0.0;
	}

	mMC->mCTRL->m_angle_xy = mMC->mCTRL->m_angle_xy + mVH_delta_ang;
	cout << "mVH_angle_follow_count  " << mVH_angle_follow_count << endl;
	cout << "mVH_delta_ang  " << mVH_delta_ang << endl;
	cout << "mMC->mCTRL->m_angle_xy  " << mMC->mCTRL->m_angle_xy << endl;
	cout << "-----------------------------------------------------------------" << endl;

}

//------------------------------------------------------------------------------
void WCtrl::refresh()
{

	glfwGetWindowSize(mMC->mWIN, &mMC->mScreenWidth, &mMC->mScreenHeight);
	glViewport(0, 0, mMC->mScreenWidth, mMC->mScreenHeight);

	mMC->mCTRL->mText = mMC->mCTRL->mTextI * mMC->mCTRL->mPosition.z;

	mMC->mMN->refresh(mMC->mScreenWidth, mMC->mScreenHeight);
	mMC->mMRGT->refresh(mMC->mScreenWidth, mMC->mScreenHeight);
	mMC->mMLFT->refresh(mMC->mScreenWidth, mMC->mScreenHeight);
	mMC->mTXT->refresh(mMC->mScreenWidth, mMC->mScreenHeight);

	mMC->mRT->resize_line();
	mMC->mTLS->refresh_draw();
	mMC->mTC->refresh_size(mMC->mCTRL->mPosition.z);

	mMC->mSM->refresh();
	mMC->mIX->refresh();

	set_vehicle_size();
}

//------------------------------------------------------------------------------
void WCtrl::refresh_callback()
{
	if (mWindowStarted) refresh();
}

//------------------------------------------------------------------------------
void WCtrl::window_size_callback(int width, int height)
{
	if (mWindowStarted) refresh();
}

//------------------------------------------------------------------------------
void WCtrl::set_vehicle_size()
{

	double vhSize = mMC->mVH->mSize * mMC->mCTRL->mPosition.z * 3;
	if (vhSize < mMC->mVH->mSize)
	{
		vhSize = mMC->mVH->mSize;
	}
	mMC->mTR->set_vh_size(vhSize);
}

//------------------------------------------------------------------------------
bool WCtrl::tooltip_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND parentW;
	HWND text_control;
	HDC hDC;


	switch (message)
	{
		case WM_INITDIALOG:

			MoveWindow(hwndDlg, (int)m_tool_ctrl.tt_x, (int)m_tool_ctrl.tt_y, m_tool_ctrl.tt_text.getLong() * 10, (int)200, true);
			parentW = GetParent(m_tool_ctrl.tt_hwnd_tool_tip);
			SetFocus(parentW);

			SetDlgItemTextA(hwndDlg, IDC_TOOLTIP_TEXT1, m_tool_ctrl.tt_text.getCadena());

			/* set text window font */
			text_control = GetDlgItem(hwndDlg, IDC_TOOLTIP_TEXT1);
			hDC = GetDC(text_control);
			SendMessage(text_control,WM_SETFONT,(WPARAM)33,	(LPARAM)0);
			//SendMessage(text_control, WM_CTLCOLORDLG, (WPARAM)text_control, (LPARAM)hwndDlg);
			ReleaseDC(text_control, hDC);

			return TRUE;
	}

	return true;
}

//------------------------------------------------------------------------------
bool WCtrl::color_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	HWND hwndList;
	CHOOSECOLOR cc;                 // common dialog box structure
	static COLORREF acrCustClr[16]; // array of custom colors
	HBRUSH hbrush;                  // brush handle
	static DWORD rgbCurrent;        // initial color selection

	// Initialize CHOOSECOLOR
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwndDlg;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	
	typedef struct
	{
		TCHAR achName[MAX_PATH];
	} WayItems;

	WayItems ListWayTypes[] =
	{
		{TEXT("way_motorway")},
		{TEXT("way_motorway_link")},
		{TEXT("way_trunk")},
		{TEXT("way_trunk_link")},
		{TEXT("way_primary")},
		{TEXT("way_primary_link")},
		{TEXT("way_secondary")},
		{TEXT("way_secondary_link")},
		{TEXT("way_tertiary")},
		{TEXT("way_tertiary_link")},
		{TEXT("way_residential")},
		{TEXT("way_unclassified")},
		{TEXT("way_service")},
		{TEXT("way_track")},
		{TEXT("way_path")},
		{TEXT("way_footway")},
		{TEXT("way_living_street")},
		{TEXT("way_road")},
		{TEXT("way_escape")},
		{TEXT("way_pedestrian")},
		{TEXT("building_any")}
	};


	switch (message)
	{
	case WM_INITDIALOG:

		hwndList = GetDlgItem(hwndDlg, IDC_CP_LIST);
		for (int i = 0; i < ARRAYSIZE(ListWayTypes); i++)
		{
			int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)ListWayTypes[i].achName);
			// Set the array index of the player as item data.
			// This enables us to retrieve the item from the array
			// even after the items are sorted by the list box.
			SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
		}

		return TRUE;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDOK:
				picker_color_action(SIM_WCACT_BUTTON_OK);
				DestroyWindow(hwndDlg);
				return TRUE;

			case IDC_CP_LIST:
			{
				switch (HIWORD(wParam))
				{
					case LBN_SELCHANGE:
					{
						hwndList = GetDlgItem(hwndDlg, IDC_CP_LIST);
						int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
						int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

						if (ChooseColor(&cc) == TRUE)
						{
							hbrush = CreateSolidBrush(cc.rgbResult);
							rgbCurrent = cc.rgbResult;

							m_tool_ctrl.red      = GetRValue(rgbCurrent);
							m_tool_ctrl.green    = GetGValue(rgbCurrent);
							m_tool_ctrl.blue     = GetBValue(rgbCurrent);
							m_tool_ctrl.itemtype = ListWayTypes[i].achName;
							picker_color_action(SIM_WCACT_BUTTON_PRESSED_SELECT_COLOR);

						}
						return TRUE;
					}
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

//------------------------------------------------------------------------------
bool WCtrl::tool_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int check = 0;
	HWND hwndSlider;
	int pos;

	switch (message)
	{
	case WM_INITDIALOG:

		hwndSlider = GetDlgItem(hwndDlg, IDC_TP_SPEED_SLIDER);

		SendMessage(hwndSlider, TBM_SETTIC, 0,  0);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 10);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 20);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 30);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 40);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 50);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 60);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 70);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 80);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 90);
		SendMessage(hwndSlider, TBM_SETTIC, 0, 100);

		SendMessage(hwndSlider, TBM_SETRANGEMIN, true, 0);
		SendMessage(hwndSlider, TBM_SETRANGEMAX, true, 100);
		SendMessage(hwndSlider, TBM_SETTIPSIDE, TBTS_TOP, 0);
		SendMessage(hwndSlider, TBM_SETPOS, m_tool_ctrl.speed_value, m_tool_ctrl.speed_value);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			if (IsDlgButtonChecked(hwndDlg, IDC_TP_TOLL))           { m_tool_ctrl.selected = 1; }
			else if (IsDlgButtonChecked(hwndDlg, IDC_TP_SPEED))     { m_tool_ctrl.selected = 2; }
			else if (IsDlgButtonChecked(hwndDlg, IDC_TP_AVOID_GPS)) { m_tool_ctrl.selected = 3; }
			else if (IsDlgButtonChecked(hwndDlg, IDC_TP_CLEAN))     { m_tool_ctrl.selected = 4; }
			else { m_tool_ctrl.selected = 0; }

			hwndSlider = GetDlgItem(hwndDlg, IDC_TP_SPEED_SLIDER);
			pos = (int)SendMessage(hwndSlider, TBM_GETPOS, 0, 0);
			m_tool_ctrl.speed_value = pos;

			//cout << m_tool_ctrl.speed_value << endl;

			DestroyWindow(hwndDlg);
			tools_action(WCtrl::SIM_WCACT_BUTTON_OK);
			return TRUE;

		case IDCANCEL:

			m_tool_ctrl.selected = 0;
			DestroyWindow(hwndDlg);
			tools_action(WCtrl::SIM_WCACT_BUTTON_CANCEL);
			return TRUE;

			return TRUE;
		}
	}
	return FALSE;

}

//------------------------------------------------------------------------------
bool WCtrl::place_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int check = 0;
	HWND hwndList;
	typedef struct
	{
		TCHAR achName[MAX_PATH];
	} PlaceItems;

	PlaceItems ListPlaceTypes[] =
	{
		{TEXT("Vigo")},
		{TEXT("Barcelona")},
		{TEXT("Zaragosa")},
		{TEXT("Vallcarca")},
		{TEXT("Madrid")}
	};


	switch (message)
	{
	case WM_INITDIALOG:


		hwndList = GetDlgItem(hwndDlg, IDC_PP_COMBO_PLACES);

		SendMessage(hwndList, CB_SETMINVISIBLE, 10, 0);


		for (int i = 0; i < ARRAYSIZE(ListPlaceTypes); i++)
		{
			int pos = (int)SendMessage(hwndList, CB_ADDSTRING, 0, (LPARAM)ListPlaceTypes[i].achName);
			//SendMessage(hwndList, CB_SETITEMDATA, pos, (LPARAM)i);
			
			cout << ListPlaceTypes[i].achName << "   " << pos << endl;
		}
		
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			DestroyWindow(hwndDlg);
			return TRUE;

		}
	}
	return FALSE;
}

//------------------------------------------------------------------------------
bool WCtrl::proxy_picker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd;
	bool active = false;

	switch (message)
	{
	case WM_INITDIALOG:

		hwnd = GetDlgItem(hwndDlg, IDC_PP_MASTER_TRIP);              SendMessage(hwnd, BM_SETCHECK, m_tool_ctrl.pp_master_trip, 0);
		hwnd = GetDlgItem(hwndDlg, IDC_PP_SIM_DETECTOR);             SendMessage(hwnd, BM_SETCHECK, m_tool_ctrl.pp_sim_detector, 0);
		hwnd = GetDlgItem(hwndDlg, IDC_PP_SIM_GPS);                  SendMessage(hwnd, BM_SETCHECK, m_tool_ctrl.pp_sim_gps, 0);
		hwnd = GetDlgItem(hwndDlg, IDC_PP_SATTOL_GPS_EUROTOLL);		 SendMessage(hwnd, BM_SETCHECK, m_tool_ctrl.pp_sattol_gps_eurotoll, 0);
		hwnd = GetDlgItem(hwndDlg, IDC_PP_SATTOL_GPS_EUROTOLL_PERF); SendMessage(hwnd, BM_SETCHECK, m_tool_ctrl.pp_sattol_gps_eurotoll_perf, 0);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:

			m_tool_ctrl.pp_master_trip              = IsDlgButtonChecked(hwndDlg, IDC_PP_MASTER_TRIP);
			m_tool_ctrl.pp_sim_detector             = IsDlgButtonChecked(hwndDlg, IDC_PP_SIM_DETECTOR);
			m_tool_ctrl.pp_sim_gps                  = IsDlgButtonChecked(hwndDlg, IDC_PP_SIM_GPS);
			m_tool_ctrl.pp_sattol_gps_eurotoll      = IsDlgButtonChecked(hwndDlg, IDC_PP_SATTOL_GPS_EUROTOLL);
			m_tool_ctrl.pp_sattol_gps_eurotoll_perf = IsDlgButtonChecked(hwndDlg, IDC_PP_SATTOL_GPS_EUROTOLL_PERF);
			resume_proxy_picker();

			DestroyWindow(hwndDlg);
			return TRUE;

		case IDCANCEL:

			resume_proxy_picker();

			DestroyWindow(hwndDlg);
			hwndDlg = NULL;
			return TRUE;
		}
	}
	return FALSE;

}

//------------------------------------------------------------------------------
bool WCtrl::clean(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool active = false;

	switch (message)
	{
	case WM_INITDIALOG:


		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			m_tool_ctrl.cl_ply = IsDlgButtonChecked(hwndDlg, IDC_CL_PLY);
			m_tool_ctrl.cl_tr  = IsDlgButtonChecked(hwndDlg, IDC_CL_TR);
			m_tool_ctrl.cl_poi = IsDlgButtonChecked(hwndDlg, IDC_CL_POI);
			m_tool_ctrl.cl_rt  = IsDlgButtonChecked(hwndDlg, IDC_CL_RT);
			m_tool_ctrl.cl_sn  = IsDlgButtonChecked(hwndDlg, IDC_CL_SN);
			m_tool_ctrl.cl_trc = IsDlgButtonChecked(hwndDlg, IDC_CL_TRC);
			m_tool_ctrl.cl_color = IsDlgButtonChecked(hwndDlg, IDC_CL_COLOR);
			m_tool_ctrl.cl_section = IsDlgButtonChecked(hwndDlg, IDC_CL_SECTIONS);

			clean_action(WCtrl::SIM_WCACT_BUTTON_OK);
			DestroyWindow(hwndDlg);
			return TRUE;

		case IDCANCEL:

			clean_action(WCtrl::SIM_WCACT_BUTTON_CANCEL);

			DestroyWindow(hwndDlg);
			hwndDlg = NULL;
			return TRUE;
		}
	}
	return FALSE;

}

//------------------------------------------------------------------------------
bool WCtrl::save(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool active = false;

	switch (message)
	{
	case WM_INITDIALOG:

		m_tool_ctrl.sv_route = 0;
		m_tool_ctrl.sv_traffic = 0;
		m_tool_ctrl.sv_polygons = 0;

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			m_tool_ctrl.sv_route    = IsDlgButtonChecked(hwndDlg, IDC_SAVE_ROUTE);
			m_tool_ctrl.sv_traffic  = IsDlgButtonChecked(hwndDlg, IDC_SAVE_TRAFFIC);
			m_tool_ctrl.sv_polygons = IsDlgButtonChecked(hwndDlg, IDC_SAVE_POLYGON);
			DestroyWindow(hwndDlg);
			save_action(WCtrl::SIM_WCACT_BUTTON_OK);
			
			hwndDlg = NULL;
			return TRUE;

		case IDCANCEL:

			DestroyWindow(hwndDlg);
			save_action(WCtrl::SIM_WCACT_BUTTON_CANCEL);
			hwndDlg = NULL;
			return TRUE;
		}
	}
	return FALSE;

}



















/*else if (mMC->mMN->mButtons[SIM_BTT_POINTS].flag == true)
{
	glfwSetCursor(mMC->mWIN, mMC->mCTRL->mCURSOR_CROSS);
	mMC->mGPT->add_point(xpos, ypos, 0);
}*/


//Setup and compile our shaders
//Shader shader; ("../../../Path/To/Shaders/model_loading.vs", "../../../Path/To/Shaders/model_loading.frag");
//Shader shader;
//shader.inicializar(SIM_DIR_SHADERS_"object.vs", SIM_DIR_SHADERS_"object.frag");
// Load models

/*

glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
shader.use();
glm::mat4 projection = CT.getProjectionMatrix();
glm::mat4 view = CT.getViewMatrix();
glUniformMatrix4fv(glGetUniformLocation(shader.mProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
glUniformMatrix4fv(glGetUniformLocation(shader.mProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

glm::mat4 model;
model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); // Translate it down a bit so it's at the center of the scene
//model = glm::rotate(model, float(SIM_PI/2.0), glm::vec3(1.0, 1.0, 0.0));
model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));	// It's a bit too big for our scene, so scale it down
glUniformMatrix4fv(glGetUniformLocation(shader.mProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
ourModel.Draw(shader);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

*/



/*if (ML.mButtons[SIM_BTT_ML_CPICKER].flag)
{
	cout << "The place is:" << cadText.getCadena() << endl;
	DB.conectar(SIM_DB_HOST, SIM_DB_PORT, SIM_DB_NAME, SIM_DB_USER, "", SIM_DB_TIMEOUT);

	cadConsulta.formCadena("SELECT * FROM Text_nodes_place_city WHERE name LIKE '%%%s%%'", cadText.getCadena());
	DB.ejecutarConsulta(cadConsulta);
	//std::cout << "cadConsulta: [" << cadConsulta.getCadena() << "]" << endl;

	if (DB.getNumRegistros() > 0)
	{
		xx = long long(DB.getDouble("xlon", 0));
		yy = long long(DB.getDouble("ylat", 0));

		move_to(xx, yy);

	}
	DB.desconectar();
}*/