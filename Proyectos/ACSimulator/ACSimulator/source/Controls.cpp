#include "Controls.h"

//------------------------------------------------------------------------------
Controls::Controls()
{

}

//------------------------------------------------------------------------------
Controls::~Controls()
{

}

//------------------------------------------------------------------------------
void Controls::init(GLFWwindow* ptrw, Cadena cadgral)
{
	FILE* mFichero;
	GLFWimage image;
	const int size = 16 * 16 * 4;
	unsigned char pixels[size];
	memset(pixels, 0xff, sizeof(pixels));
	image.width = 16;
	image.height = 16;
	image.pixels = pixels;

	mcadgral = cadgral;


	mZZ = SIM_POSZ_INITIAL; 
	mXX = Simulator::getXfromLon(SIM_LON_MOVE);
	mYY = Simulator::getYfromLat(SIM_LAT_MOVE);

	mAngle     = SIM_PI / 2;

	mPosition  = glm::vec3(mXX, mYY, mZZ);
	mDirection = glm::vec3(mXX, mYY, 0);
	mUp        = glm::vec3(0, 1, 0);

	m_angle_xy   = 0;
	m_angle_z    = 0;

	m_delta_ang_xy = SIM_PI / 40;
	m_delta_ang_z  = SIM_PI / 40;

	mInitialFoV = 45.0;
	mSpeed      = 0.005;
	mSpeed_xy   = 0.00015;
	mDeep       = SIM_MAX_POSITION_Z+50;

	mTextI      = 0.00025;
	mText       = 0.00025;
	mDeltaTxt   = 0;

	mPtrWindow = ptrw;
	mFirstTime = 1;

	mMouseBttLeftPressed = false;
	mPositionSearch = false;

	mxpos_pressI=0;
	mypos_pressI=0;
	mxscreen_pressI=0;
	myscreen_pressI=0;

	//---------------------Cursors---------------------------------------
	mCURSOR_CROSS     = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	mCURSOR_HAND      = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	mCURSOR_BEAM      = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	   	  
	mFichero = fopen(SIM_DIR_IMAGES_"grab.rgba", "rb");
	if (mFichero != NULL) 
	{
		fread(image.pixels, sizeof(char), size, mFichero);
		mCURSOR_GRAB = glfwCreateCursor(&image, 0, 0);
	}

	mFichero = fopen(SIM_DIR_IMAGES_"wait.rgba", "rb");
	if (mFichero != NULL)
	{
		fread(image.pixels, sizeof(char), size, mFichero);
		mCURSOR_WAIT = glfwCreateCursor(&image, 0, 0);
	}
}

//------------------------------------------------------------------------------
glm::mat4 Controls::getViewMatrix() 
{
	return mViewMatrix;
}

//------------------------------------------------------------------------------
glm::mat4 Controls::getProjectionMatrix()
{
	return mProjectionMatrix;
}

//------------------------------------------------------------------------------
void Controls::computeMatricesFromInputs()
{
	double currentTime;

	if (mFirstTime)
	{
		mLastTime = glfwGetTime();
		mFirstTime = 0;
	}

	currentTime = glfwGetTime();
	mDeltaTime  = currentTime - mLastTime;
	mLastTime   = currentTime;

	mProjectionMatrix = glm::perspective(glm::radians(mInitialFoV), 16.0f / 9.0f, 0.0001f, mDeep);
	mViewMatrix       = glm::lookAt(mPosition,mDirection,mUp);

}