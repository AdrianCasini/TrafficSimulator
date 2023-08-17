#ifndef _OBJMNG_H
#define _OBJMNG_H

#include "Simulator.h"
#include "Controls.h"



class ObjMng
{

	public:

		GLuint    mVAO;
		GLuint    mVBO;
		Controls* mCtrl;

		double    mX;
		double    mY;
		double    mZ;

		double    mIX;
		double    mIY;
		double    mIZ;

		GLuint    mTextID_p;
		GLuint    mTextImg_p;
		GLuint    mCameraR_p;
		GLuint    mCameraU_p;
		GLuint    mVP;
		GLdouble  mSize;
		GLuint    mColor;

		glm::vec4 mColorV4;

		Shader    mShader;

		int mScreenW;
		int mScreenH;
		bool mFlagBtt;


	public:

		//----------------------------------------
		ObjMng();
		~ObjMng();

		void init(Controls* ctrl);
		void refresh();
		void button_pressed(bool flag);
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);

};

#endif

