#ifndef _DRAWING_H
#define _DRAWING_H

#include "Simulator.h"
#include "Controls.h"
#include "QuadMap.h"


class Drawing
{
	public:

		Controls* mCtrl;

		glm::vec4 mColor;
		glm::vec4 mColorRoute;

		int mPointIndex;
		int mPointIndex_route;

		int mPoints_line;
		int mPoints_line_out;

		GPoint* mArrayPoints;
		GPoint* mArrayPoints_route;
		QuadMap* mQuadMap;

		double* mbuff_points_draw;
		double* mbuff_points_route_draw;

		double* mbuff_lines_draw;
		double* mbuff_lines_draw_out;


		GLuint    mVAO_point;
		GLuint    mVAO_point_route;
		GLuint    mVAO_line;

		GLuint    mVBO_points_model;
		GLuint    mVBO_points_offset;
		GLuint    mVBO_points_offset_route;

		GLuint    mVBO_lines_surface;


		GLuint    mTextID_p;
		GLuint    mTextImg_p;
		GLuint    mCameraR_p;
		GLuint    mCameraU_p;
		GLuint    mVP_p;
		GLdouble  mSize_p;
		GLuint    mColor_p;

		GLuint    mVP_l;
		GLuint    mColor_l;

		Shader    mShader_line;
		Shader    mShader_point;



		int mScreenW;
		int mScreenH;

		Cadena mcadgral;

	public:
		//----------------------------------------
		Drawing();
		~Drawing();
		
		void init(Controls* ctrl, QuadMap* mquadmap, Cadena cadgral);
		void render_setup();

		void draw_route(WayInfo* wayinfo);
		void add_point(double x, double y, double z);
		void add_point(double x, double y, double z, double* buff, GPoint* buffP);

		void tranform(double& x, double& y, double& z);
		

		void refresh();
		void resize_line();

		bool button_active();
		bool button_not_active();

		void clear();

		double get_drawing_point_size();
		double get_drawing_line_size();

		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_line_surface(glm::mat4& viewprojectionmatrix);


};

#endif

