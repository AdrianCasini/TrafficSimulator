//================================================================
// ACSimulator: Controls.h
//================================================================
//  FILE: Controls.h
//  DESCRIPTION :
//  CREATION DATE : 15 - 10 - 2019
//  DESIGN AUTHOR : Adrian Casini
//  DEVELOPER : Adrian Casini
//  COMPANY : Adrian Casini
//================================================================

#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#define GLM_FORCE_RADIANS


#include "Simulator.h"

using namespace glm;

class Controls
{

	public:

	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec3 mUp;

	double m_angle_xy;
	double m_angle_z;

	double m_delta;
	double m_delta_ang_xy;
	double m_delta_ang_z;


	double   mZZ;
	double   mYY;
	double   mXX;

	double   mAngle;

	double  mSpeed;
	double  mSpeed_xy;
	double  mLastTime;
	double  mDeltaZ;
	unsigned int    mFirstTime;
	double  mDeltaTime;

	float   mDeep;
	float   mInitialFoV;

	double  mText;
	double  mTextI;
	double  mDeltaTxt;

	bool mMouseBttLeftPressed;
	bool mPositionSearch;

	double mxpos_pressI;
	double mypos_pressI;
	double mxscreen_pressI;
	double myscreen_pressI;


	GLFWwindow* mPtrWindow;

	GLFWcursor* mCURSOR_CROSS;
	GLFWcursor* mCURSOR_HAND;
	GLFWcursor* mCURSOR_BEAM;
	GLFWcursor* mCURSOR_GRAB;
	GLFWcursor* mCURSOR_WAIT;
	GLFWcursor* mCURSOR_ESP2;
	GLFWcursor* mCURSOR_ESP3;


	Cadena mcadgral;

	public:
		Controls();
		 ~Controls();

	public:

		void      computeMatricesFromInputs();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
		void      init(GLFWwindow*, Cadena cadgral);

};

#endif