#include "WCtrl.h"
#include "Simulator.h"
#include "Controls.h"
#include "Vehiculo.h"
#include "Traffic.h"
#include "Trip.h"
#include "Menu.h"
#include "MenuLeft.h"
#include "MenuRight.h"
#include "Trace.h"
#include "Texter.h"
#include "Tools.h"
#include "GPSPoint.h"
#include "Routing.h"
#include "Drawing.h"
#include "Mapper.h"
#include "MapMng.h"
#include "WayRenderMng.h"
#include "Model.h"
#include "PofI.h"
#include "QuadMap.h"
#include "SectionMng.h"
#include "Inrix.h"

//-----------------------------------------------------------------------
// Widows Callbacks - Function prototypes
//-----------------------------------------------------------------------
void   key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void   mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void   cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void   scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void   window_refresh_callback(GLFWwindow* window);
void   window_size_callback(GLFWwindow* window, int width, int height);

//-----------------------------------------------------------------------
GLFWwindow* window = NULL;
GLFWcursor* cursor = NULL;
int screenWidth  = SIM_SCREEN_W;
int screenHeight = SIM_SCREEN_H;

//-----------------------------------------------------------------------
Controls     CT;
Menu         MN;
MenuRight    MR;
MenuLeft     ML;
Traffic      TR;
Vehiculo     VH;
Trace        TC;
Texter       TX;
Tools        TS;
Tarif        TF;
MapMng       MM;
WayRenderMng WRM;
WCtrl        WC;
GPSPoint     PT;
Routing      RT;
Drawing      DRW;
BaseDatos    DB1, DB2, DB3, DB4;
PofI         PI;
QuadMap      QM;
SectionMng   SM;
Inrix        IX;

//---------------------------------------------------------------------------------------------------------
DWORD THREAD_TR_ID, THREAD_TX_ID, THREAD_MM_ID, THREAD_WRM_ID, THREAD_SM_ID, THREAD_IX_ID;
HANDLE THREAD_TR_HANDLE, THREAD_TX_HANDLE, THREAD_MM_HANDLE, THREAD_WRM_HANDLE, THREAD_SM_HANDLE, THREAD_IX_HANDLE;
//---------------------------------------------------------------------------------------------------------
DWORD WINAPI THREAD_TR_Obtain_VH(LPVOID lpParameter) {((Traffic*)lpParameter)->get_info_traffic();  return 0;}
DWORD WINAPI THREAD_WRM(LPVOID lpParameter)          {((WayRenderMng*)lpParameter)->get_info();     return 0;}
DWORD WINAPI THREAD_SM(LPVOID lpParameter)           {((SectionMng*)lpParameter)->refresh_thread(); return 0; }
DWORD WINAPI THREAD_IX(LPVOID lpParameter)           {((Inrix*)lpParameter)->refresh_thread();      return 0; }
//========================================================================================================
//                                  MAIN
//========================================================================================================
int main(int argc, const char* argv[])
{
	WSADATA wsaData;
	Cadena dbname;
	Cadena dirgral;
	int result = 0;

	//--- Process Command Line Arguments ---
	WC.process_arguments(argc, argv, dbname, dirgral);
	WC.start_db();

	//--- Conecting with DataBase ---
	try 
	{
		if (DB1.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) {return 1;}
		if (DB2.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) {return 1;}
		if (DB3.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) {return 1;}
		if (DB4.conectar(SIM_DB_HOST, SIM_DB_PORT, dbname, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) {return 1;}
	}
	catch (...) { printf("There is a problem connecting with the database!\n"); return 1; }

	//--- Initialising Windows stuff ---
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) { printf("WSAStartup failed: %d\n", result);return result;}

	//--- Initialising GLFW ------------
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glEnable(GL_MULTISAMPLE);

	//-- Initialising Windows & Configure-----
	window = glfwCreateWindow(screenWidth, screenHeight, SIM_WINDOW_TITLE, NULL, NULL);
	
	WCtrl::sMainControl mc;
	mc.mWIN  = window;
	mc.mMN   = &MN;
	mc.mMLFT = &ML;
	mc.mMRGT = &MR;
	mc.mCTRL = &CT;
	mc.mTR   = &TR;
	mc.mTLS  = &TS;
	mc.mVH   = &VH;
	mc.mTC   = &TC;
	mc.mTRF  = &TF;
	mc.mTXT  = &TX;
	mc.mWRM  = &WRM;
	mc.mMMG  = &MM;
	mc.mGPT  = &PT;
	mc.mRT   = &RT; 
	mc.mDRW  = &DRW;
	mc.mPOI  = &PI;
	mc.mSM   = &SM;
	mc.mIX   = &IX;
	mc.mScreenHeight = screenHeight;
	mc.mScreenWidth  = screenWidth; 
	WC.init(&mc, dirgral);

	glfwMaximizeWindow(window);
	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	glfwSetWindowSizeLimits(window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwSetWindowIcon(window, 2, WC.mIcon);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetWindowAspectRatio(window, 16, 9);
	//---------------------------------------
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetKeyCallback(window          , key_callback);
	glfwSetCursorPosCallback(window    , cursor_position_callback);
	glfwSetMouseButtonCallback(window  , mouse_button_callback);
	glfwSetScrollCallback(window       , scroll_callback);
	glfwSetWindowSizeCallback(window   , window_size_callback);
	//glfwSetWindowCloseCallback(window, window_close_callback);
	//---------------------------------------
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursor(window, cursor);
	glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);
	glfwPollEvents();
	//---------------------------------------
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//---------------------------------------
	glewExperimental = true; 
	glewInit();
	//---------------------------------------
	glClearColor(SIM_COLOUR_SEA_DAY);
	//glfwSwapInterval(5);

	//-------------------------------------------------------------------------
	//           Initialize Gral Clases & Threads
	//-------------------------------------------------------------------------
	CT.init(window, dirgral);
	TC.init(&MN, dirgral);
	QM.init();
	DRW.init(&CT, &QM, dirgral);
	TS.init(&CT, &QM, dirgral);
	PT.init(&CT, &QM, dirgral);
	MN.init(screenWidth, screenHeight, dirgral);
	ML.init(screenWidth, screenHeight, dirgral);
	MR.init(screenWidth, screenHeight, dirgral);
	PI.init(&CT, &DB1, dirgral);
	TR.init( WC.mDateInitial, &TS, &MN, &PI,  &DB2, dirgral);
	MM.init(&CT, &DB3, dirgral);
	WRM.init(&CT, &DB4, dirgral);
	TX.init(&CT, &TF, &TS, dirgral);
	RT.init(&CT, &DRW, dirgral);
	SM.init(&CT, &TR, &MN, dirgral);
	IX.init(&CT, &MN, &QM, dirgral);

	THREAD_TR_HANDLE    = CreateThread(0, 0, THREAD_TR_Obtain_VH, &TR,  0, &THREAD_TR_ID);
	THREAD_WRM_HANDLE   = CreateThread(0, 0, THREAD_WRM,          &WRM, 0, &THREAD_WRM_ID);
	THREAD_SM_HANDLE    = CreateThread(0, 0, THREAD_SM,           &SM,  0, &THREAD_SM_ID);
	THREAD_IX_HANDLE    = CreateThread(0, 0, THREAD_IX,           &IX,  0, &THREAD_IX_ID);
	
	WC.setup_thread_handles(THREAD_TR_HANDLE);
	WC.setup_thread_handles(THREAD_WRM_HANDLE);
	WC.setup_thread_handles(THREAD_SM_HANDLE);
	WC.mWindowStarted = true;
	WC.refresh_callback();
	WC.set_followvehicle();
	CT.mText = CT.mTextI * CT.mPosition.z;


	//===========================================================================================================
	//----------------------------------|         MAIN Loop        |---------------------------------------------
	//===========================================================================================================
	while (!glfwWindowShouldClose(window))
	{
		if (!TR.mTurbo)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
		
			// Little Time - IU Stuff
			WC.set_IU_time();
			// Camera / Projection / View
			CT.computeMatricesFromInputs();
			glm::mat4 ProjectionMatrix     = CT.getProjectionMatrix();
			glm::mat4 ViewMatrix           = CT.getViewMatrix();
			glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

			// Map Manager
			MM.render(ViewProjectionMatrix, false);
			// Way Render
			WRM.render(ViewProjectionMatrix, false);

			// Section Manager
			if (SM.is_active()) { SM.render(ViewMatrix, ViewProjectionMatrix); }
			// Inrix
			if (IX.is_active()) { IX.render(ViewMatrix, ViewProjectionMatrix); }
			// Vehicles Simulation
			TR.render(ViewMatrix, ViewProjectionMatrix);
			// Text
			TX.render(ViewProjectionMatrix, WC.mMC->mScreenWidth, WC.mMC->mScreenHeight);
			// Point Of Interest
			if (PI.is_active()) { PI.render(ViewMatrix, ViewProjectionMatrix); }
			// Route
			if (RT.is_active()) { RT.render(ViewMatrix, ViewProjectionMatrix); }
			// Points
			if (PT.is_active()) { PT.render(ViewMatrix, ViewProjectionMatrix); }
			// Tools
			if (TS.is_active()) { TS.render(ViewMatrix, ViewProjectionMatrix); }
			// Trace
			if (TC.is_active()) { TC.render(ViewMatrix, ViewProjectionMatrix);}


			// Menu
			MN.render();
			ML.render();
			MR.render();

			glDepthMask(GL_TRUE);
			glfwSwapBuffers(window);

			//WC.follow_angle();
			glfwPollEvents();
		}
		else
		{
			TR.mTH.delta_time = 0.5;
			TR.do_move();
			TR.mTurbo--;
			if (TR.mTurbo == 0) {TR.mpMN->mButtons[Menu::SIM_BTT_TURBO].flag = false;}
			glfwPollEvents();
		}
	}

	WC.terminate();
	DB1.desconectar();
	DB2.desconectar();
	DB3.desconectar();
	DB4.desconectar();
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)           { WC.scroll_callback(xoffset, yoffset);}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)        { WC.cursor_position_callback(xpos, ypos);}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)   { WC.mouse_button_callback(button, action, mods);}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) { WC.key_callback(key, scancode, action, mode);}
void window_refresh_callback(GLFWwindow* window)                                   { WC.refresh_callback();}
void window_size_callback(GLFWwindow* window, int width, int height)               { WC.window_size_callback(width, height); }

//---------------------------------------------------------------------------------------------------------------------------------------
BOOL CALLBACK  GoToPlacePicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)   {return (WC.place_picker(hwndDlg, message, wParam, lParam));}
BOOL CALLBACK  GoToToolPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)    {return (WC.tool_picker(hwndDlg, message, wParam, lParam));}
BOOL CALLBACK  GoToToolTipPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) { return (WC.tooltip_picker(hwndDlg, message, wParam, lParam)); }
BOOL CALLBACK  GoToColorPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {return (WC.color_picker(hwndDlg, message, wParam, lParam));}
BOOL CALLBACK  GoToProxyPicker(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {return (WC.proxy_picker(hwndDlg, message, wParam, lParam));}
BOOL CALLBACK  GoToClean(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)       {return (WC.clean(hwndDlg, message, wParam, lParam)); }
BOOL CALLBACK  GoToSave(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)        {return (WC.save(hwndDlg, message, wParam, lParam)); }

