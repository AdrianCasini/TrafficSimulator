#ifndef _SECTIONMNG_H
#define _SECTIONMNG_H


#include "Simulator.h"
#include "Traffic.h"
#include "Controls.h"
#include "Menu.h"

#define SIM_MAX_ROADSECTIONS 10
#define SIM_SECTION_KM 1.0


class SectionMng
{
	public:

		Traffic*  mpTR;
		Controls* mpCT;
		Menu*     mpMN;
		Cadena mcadgral;

		struct RoadSection
		{
			Cadena cadroadfile;
			int subsection_number[500];
			int* nodes;
			Color  color[500];

			NodoInfo* pnodesectioninfo;
			int max_nodes;
			int index_trip;

			GLuint  VAO;
			GLuint  VBO;
			GLuint  VBO_color;
			double* array_data;
			GLubyte* array_data_color;
			int total_points;
			
		};

		RoadSection  m_roadsections[SIM_MAX_ROADSECTIONS];
		int          m_roadindex;

		double*      m_array_data_in;
		
		Shader       m_shader;
		GLuint       m_VP;
		double       m_hw;
		bool         m_active;
		bool         m_refresh;

	public:

		SectionMng();
		~SectionMng();
		void init(Controls*, Traffic*, Menu* mn, Cadena cadgral);
		bool is_active();
		void clear();
		int generate_surface(int index);
		double get_size();

		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void refresh();
		void refresh_thread();

		void get_roadsection_info(Cadena cadfile);
		void render_setup(int index);
		void render_refresh(int road_index);
		void setup_color(int sec_index);
		void set_sect_color(int road_index, int section_index, int r, int g, int b, int f);
		void set_color(int sec_index, int index_section, int count_veh);
		void get_color(int sec_index);


};

#endif

