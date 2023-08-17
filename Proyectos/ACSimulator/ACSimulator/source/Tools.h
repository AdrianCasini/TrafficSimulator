#ifndef _TOOLS_H
#define _TOOLS_H

#include "Simulator.h"
#include "Controls.h"
#include "QuadMap.h"
#include <iomanip> 


class Tools
{

	public:


		enum eTypePolygon 
		{ 
			SIM_POLYGON_NULL, 
			SIM_POLYGON_TOLL, 
			SIM_POLYGON_SPEED, 
			SIM_POLYGON_AVOID_GPS, 
			SIM_POLYGON_CLEAN, 
			SIM_POLYGON_TYPE_MAX 
		};

		struct Polygon
		{
			double     buff_poly_x[SIM_MAX_POLYG_PNTS];
			double     buff_poly_y[SIM_MAX_POLYG_PNTS];
			double     buff_poly_points[SIM_MAX_POLYG_PNTS * 2];

			int        num_points;
			int        type;
			bool       bnew;
		};

		struct Polytype
		{
			int array_type[SIM_MAX_POLYGONS];
			int num;
		};


		struct MultiPolygon
		{
			Polygon   polygon[SIM_MAX_POLYGONS];
			int       num_polygon;

			Polytype  types[SIM_POLYGON_TYPE_MAX];

			double* mbuff_points_union;
			int     mpnts_points_union;

			double* mbuff_lines_in;
			int     mpnts_line_in;

			double* mbuff_lines_out;
			int 	mpnts_line_out;

			int     mpnts_points_union_now;
			int     mpnts_line_in_now;



		};

		MultiPolygon mMP;
		ofstream  mFilePolygons;
		Cadena    mCadFileName;

		GLuint    mVAO_point;
		GLuint    mVAO_line;
		GLuint    mVBO_points_model;

		GLuint    mVBO_lines;
		GLuint    mVBO_lines_surface;
		GLuint    mVBO_points_offset;


		GLuint    mVBO_Color;
		GLubyte*  mColorBuff;


		Controls* mCtrl;
		QuadMap*  mQuadMap;

		double    mXnext;
		double    mYnext;
		double    mZnext;

		double    mXini;
		double    mYini;
		double    mZini;

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

		Shader    mShader_line;
		Shader    mShader_point;

		int mScreenW;
		int mScreenH;

		bool mActionActive;

		double mTrip;
		Cadena mText_km;
		Cadena mText_eu;
		int    mSpeed;

		double mPointSize;
		double mLineSize;

		Cadena mcadgral;

	public:
		//----------------------------------------
		Tools();
		~Tools();

		void init(Controls* ctrl, QuadMap* mquadmap, Cadena cadgral);

		void add_line(double x, double y, double z);
		void follow_line(double x, double y, double z);
		void refresh();
		void tranform(double& x, double& y, double& z);
		void delete_actual_polygon();
		void reset_polygon(int indx);
		void reset_MP();
		void tool_selected(int tool_selected);
		bool exist_type_polygon(enum eTypePolygon etype);
		void calculate_payment(double x, double x_old, double y, double y_old);


		void activate();
		void deactivate();

		bool exist_polygon(int index);
		bool exist_any_polygon();
		bool is_active();

		bool inside_polygon(enum eTypePolygon etype, double x, double y);
		void clear();
		void end_polygon();
		void save_polygon(Cadena cadfile);

		void   refresh_draw();
		double get_tools_point_size();
		double get_tools_line_size();

		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_line_surface(glm::mat4& viewprojectionmatrix);
};

#endif

