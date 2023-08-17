#ifndef _RENDER_H
#define _RENDER_H

#include "Simulator.h"
#include "Controls.h"
#include "QuadMap.h"


#define SIM_MAX_RENDER_GROUPS_PTS 50
#define SIM_MAX_RENDER_GROUPS_LINES 50

#define SIM_MAX_RENDER_PTS 5000
#define SIM_MAX_RENDER_PTS_x_LINE 80000
#define SIM_RENDER_POINT_SIZE 0.0065
#define SIM_RENDER_LINE_SIZE 0.009


struct SPointGroup
{
	bool active;
	int max_points;

	Cadena name;
	ColorF color;
	GLuint texture;
	double size;

	GLuint vbo_model;
	GLuint vbo_offset;

	GPoint* buff_pt_info;
	double* buff_pt_draw;
};

struct SSegmentLine
{
	Color color;
	int max_points;
};


struct SLineGroup
{
	bool active;
	int  max_points;
	int  max_points_draw;

	Cadena name;
	GLuint texture;
	double size;
	double size_refresh;

	GLuint vbo_offset;
	GLuint vbo_color;

	GPointLine* buff_pt_info;
	double*  buff_in;
	double*  buff_out;
	double*  buff_pt_draw;
	GLubyte* buff_pt_draw_color;


	map<long long int, SSegmentLine> map_seg_render_inf;
};


class Render
{
	public:

		Controls*  m_ctrl;
		QuadMap*   m_quad_map;

		GLuint     m_VAO_point;
		GLuint     m_VAO_line;
		int        m_max_group_points;
		int        m_max_group_lines;

		SPointGroup m_GRPPOINT[SIM_MAX_RENDER_GROUPS_PTS];
		SLineGroup  m_GRPLINE[SIM_MAX_RENDER_GROUPS_LINES];
		Shader      m_shader_point;
		Shader      m_shader_line;

		//Point---------------
		GLuint    m_text_id_p;
		GLuint    m_text_img_p;
		GLuint    m_camera_r_p;
		GLuint    m_camera_u_p;
		GLuint    m_vp_p;
		GLdouble  m_size_p;
		GLuint    m_color_p;

		//Line---------------
		GLuint    m_vp_l;

		
	public:
		
		Render();
		~Render();
		
		void init(Controls* ctrl, QuadMap* mquadmap);

		//main
		void add_gps_point(int grp_pt_ind, double lon, double lat);
		void add_gps_point_line(int grp_pt_ind, double lon, double lat, long long int point_id);

		void del_group_point(int grp_pt_ind);
		void del_group_line(int grp_line_ind);

		int add_group_line(Cadena name, GLuint texture, double size);
		int add_group_point(Cadena name, ColorF& color, GLuint texture, double size);

		void create_segments(int grp_line_ind);
		void finish_line(int grp_line_ind);

		void set_color(int grp_line_ind, map<long long int, Color>& map_color_seg);

	
		//general
		void clear();
		double get_drawing_point_size(double size);
		double get_drawing_line_size(double size);

		//render
		void refresh();
		void refresh_point(int grp_pt_ind);
		void refresh_line(int grp_line_ind);

		void render_setup();
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);

		void render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_line(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);

};

#endif

