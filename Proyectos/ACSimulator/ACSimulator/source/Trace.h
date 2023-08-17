#ifndef _TRACE_H
#define _TRACE_H

#include "Simulator.h"
#include "Menu.h"


class Trace
{
	public:


		Menu*      mpMenu;

		int        mPoints;
		bool       mActive;
		int        mIndice;
		double     mSize;
		double     mSizeNow;



		Color     mColor_in;
		Color     mColor_out;

		double*   m_buffer_points;
		double*   mMap_buffer_model;
		double*   mMap_buffer_trip;
		GLubyte*  mMap_buffer_color;

		glm::mat4* modelMatrices;


		GLuint    mVAO;
		GLuint    mVBO_Tranformation;
		GLuint    mVBO_Model;
		GLuint    mVBO_Color;

		Shader    mShader;

		GLuint   mVP;
		GLdouble mPW;
		GLuint   mTextureID;
		GLuint   mTextureImg;

		GLuint   mBufferIndex;

		Cadena mcadgral;


	public:
		//----------------------------------------
		Trace();
		~Trace();

		void init(Menu* menu, Cadena cadgral);
		bool is_active();
		void refresh_size(double position_z);
		void get_points_from_file(Cadena cadfile);
		void get_points_from_file_format_master(Cadena cadfile);
		void get_points_from_file_format_2(Cadena cadfile);
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void clear();


};

#endif

