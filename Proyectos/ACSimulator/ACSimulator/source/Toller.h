#ifndef _TOLLER_H
#define _TOLLER_H

#include "Simulator.h"
#include "Controls.h"



class Toller
{
	public:


		Controls* mCtrl;
		double    mbuff_lines_draw[SIM_MAX_POLYG_PNTS];
		double    mbuff_lines_draw_out[SIM_MAX_POLYG_PNTS * 6];
		double    mbuff_points_draw_union[SIM_MAX_POLYG_PNTS];
		double    mbuff_poly_points[SIM_MAX_POLYG_PNTS];

		double    mbuff_poly_x[SIM_MAX_POLYG_PNTS];
		double    mbuff_poly_y[SIM_MAX_POLYG_PNTS];

		int       mPoints_line;
		int 	  mPoints_line_out;
		int       mPoly_points;
		int       mPoints_point_draw;


		double    mX;
		double    mY;
		double    mZ;

		double    mIX;
		double    mIY;
		double    mIZ;

		GLuint    mVAO_point;
		GLuint    mVAO_line;
		GLuint    mVBO_lines;
		GLuint    mVBO_lines_surface;

		GLuint    mVBO_points_model;
		GLuint    mVBO_points_offset;


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

		bool mFlagBtt;
		bool mFirstPolygon_Point;

		double mTrip;
		Cadena mText_km;
		Cadena mText_eu;




	public:
		//----------------------------------------
		Toller();
		~Toller();

		void init(Controls* ctrl);

		void add_line(double x, double y, double z);
		void follow_line(double x, double y, double z);
		void refresh();
		void tranform(double& x, double& y, double& z);

		void button_pressed(bool flag);
		bool button_active();
		bool button_not_active();
		bool is_first_point();
		bool exist_polygon();
		bool is_active();
		bool is_payment_active();
		void calculate_payment(double x, double x_old, double y, double y_old);
		bool inside_polygon(double x, double y);
		void clear();
		void end_polygon();


		void   resize_line();
		double get_toller_point_size();
		double get_toller_line_size();

		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_line_surface(glm::mat4& viewprojectionmatrix);


};

#endif

