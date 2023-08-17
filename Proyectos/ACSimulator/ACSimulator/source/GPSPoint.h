#ifndef _GPSPOINT_H
#define _GPSPOINT_H

#include "Simulator.h"
#include "Controls.h"
#include "QuadMap.h"


class GPSPoint
{

	public:

		ofstream mFile;
		Cadena trip_name;

		int mPointIndex;
	
		int mScreenW;
		int mScreenH;
		bool mFlagBtt;

		GPoint* mArrayPoints;		
		QuadMap*   mQuadMap;
		
		double corner_NW_lat;
		double corner_NW_lon;
		double corner_SE_lat;
		double corner_SE_lon;


		double* mbuff_points_draw;

		GLuint    mVAO_point;
		GLuint    mVBO_points_model;
		GLuint    mVBO_points_offset;

		Controls* mCtrl;

		GLuint    mTextID_p;
		GLuint    mTextImg_p;
		GLuint    mCameraR_p;
		GLuint    mCameraU_p;
		GLuint    mVP_p;
		GLdouble  mSize_p;
		GLuint    mColor_p;

		GLuint    mVP_l;
		GLuint    mColor_l;

		glm::vec4 mColor;

		Shader    mShader_point;

		Cadena mcadgral;


	public:
		//----------------------------------------
		GPSPoint();
		~GPSPoint();

		void init(Controls* ctrl, QuadMap* mquadmap, Cadena cadgral);

		void add_point(double x, double y, double z);
		void end_points();
		void refresh();
		void tranform(double& x, double& y, double& z);
		bool open_file();

		void button_pressed(bool flag);
		bool button_active();
		bool button_not_active();
		bool is_active();

		void clear();
		double get_point_size();
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_setup();


};

#endif

