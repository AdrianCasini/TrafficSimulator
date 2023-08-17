#include "SectionMng.h"


//------------------------------------------------------------------------------
SectionMng::SectionMng()
{

}

//------------------------------------------------------------------------------
SectionMng::~SectionMng()
{
	
}

//------------------------------------------------------------------------------
void SectionMng::init(Controls* ctl, Traffic* tr, Menu* mn, Cadena cadgral)
{
	mpTR = tr;
	mpCT = ctl;
	mpMN = mn;
	mcadgral = cadgral;

	for (int i = 0; i < SIM_MAX_ROADSECTIONS; i++)
	{
		m_roadsections[i].array_data       = NULL;
		m_roadsections[i].array_data_color = NULL;
		m_roadsections[i].nodes            = NULL;
	}

	m_roadindex = 0;
	m_array_data_in  = new double[4 * 3000];
	m_shader.inicializar(SIM_DIR_SHADERS_"section_mngr.vs", SIM_DIR_SHADERS_"section_mngr.frag");
	m_VP      = glGetUniformLocation(m_shader.mProgram, "mVP");
	m_hw      = get_size();
	m_active  = false;
	m_refresh = false;

}

//------------------------------------------------------------------------------
void SectionMng::get_roadsection_info(Cadena cadfile)
{
	cadfile = Simulator::get_filename_clean(cadfile);
	NodoInfo* ptrnode;

	if(m_roadindex < SIM_MAX_ROADSECTIONS)
	{
		m_roadsections[m_roadindex].cadroadfile = cadfile;
		mpTR->mpTST->get_trip_index(m_roadsections[m_roadindex].cadroadfile, "SECTION", m_roadsections[m_roadindex].index_trip, mpTR->mTH.trip_index);
	
		m_roadsections[m_roadindex].max_nodes        = mpTR->mTH.pTripI[m_roadsections[m_roadindex].index_trip].max_nodos;
		m_roadsections[m_roadindex].pnodesectioninfo = &(mpTR->mTH.pTripI[m_roadsections[m_roadindex].index_trip].ntrip[0]);
		m_roadsections[m_roadindex].array_data       = new double [3 * m_roadsections[m_roadindex].max_nodes * 40];
		m_roadsections[m_roadindex].array_data_color = new GLubyte[4 * m_roadsections[m_roadindex].max_nodes * 40];
		m_roadsections[m_roadindex].nodes            = new int[m_roadsections[m_roadindex].max_nodes];
		m_roadsections[m_roadindex].total_points     = generate_surface(m_roadindex);

		ptrnode = m_roadsections[m_roadindex].pnodesectioninfo;
		for (int i = 0; i < m_roadsections[m_roadindex].max_nodes; i++) { mpTR->mpTST->set_nodeorder(ptrnode); ptrnode++; }

		get_color(m_roadindex);
		setup_color(m_roadindex);
		render_setup(m_roadindex);

		m_roadindex++;
		mpMN->mButtons[Menu::SIM_BTT_SECMNGR].flag = true;
		m_active = true;

	}
}

//------------------------------------------------------------------------------
int SectionMng::generate_surface(int road_index)
{
	int total_out = 0;

	for (int i = 0; i < m_roadsections[road_index].max_nodes; i++)
	{
		m_array_data_in[4 * i + 0] = m_roadsections[road_index].pnodesectioninfo[i].X;
		m_array_data_in[4 * i + 1] = m_roadsections[road_index].pnodesectioninfo[i].Y;
		m_array_data_in[4 * i + 2] = 0.0;
		m_array_data_in[4 * i + 3] = m_roadsections[road_index].pnodesectioninfo[i].lanes;
	}
	Simulator::build_LINE_RECT_TRG_LANES(m_hw, m_array_data_in, m_roadsections[road_index].max_nodes, m_roadsections[road_index].array_data, m_roadsections[road_index].max_nodes * 40, &total_out);
	return total_out;
}

//------------------------------------------------------------------------------
void SectionMng::get_color(int road_index)
{
	double dist = 0.0;
	int count_veh = 0;
	int section_index = 0;

	for (int nd = 0; nd < m_roadsections[road_index].max_nodes - 1; nd++)
	{
		dist = m_roadsections[road_index].pnodesectioninfo[nd].dist + dist;

		for (int j = 0; j < 10; j++)
		{
			count_veh = count_veh + mpTR->mpTST->mpTH->pNO[m_roadsections[road_index].pnodesectioninfo[nd].node_vh_order].laneorder[j].m_cant;
		}

		m_roadsections[road_index].nodes[nd] = section_index;
	
		if (dist > SIM_SECTION_KM)
		{
			set_color(road_index, section_index, count_veh);
			//----New Section------
			section_index++;
			dist = 0.0;
			count_veh = 0;
		}
	}
}

//------------------------------------------------------------------------------
void SectionMng::set_color(int road_index, int section_index, int count_veh)
{
	//dark red
	if (count_veh > 100) {set_sect_color(road_index, section_index, 148, 3, 24, 100);}
    //red
	else if ((count_veh < 100) && (count_veh > 60)) {set_sect_color(road_index, section_index, 253, 75, 66, 100);}
	//orange
	else if ((count_veh < 60) && (count_veh > 30))  {set_sect_color(road_index, section_index, 244, 122, 0, 100);}
	//light yelow
	else if ((count_veh < 30) && (count_veh > 10))  {set_sect_color(road_index, section_index, 255, 255, 127, 100);}
	//light gray
	//else {set_sect_color(road_index, section_index, 173, 216, 230, 220);}
    else {set_sect_color(road_index, section_index, 150, 240, 150, 220);}
}

//------------------------------------------------------------------------------
void SectionMng::set_sect_color(int road_index, int section_index, int r, int g, int b, int f)
{
	m_roadsections[road_index].color[section_index].r = r;
	m_roadsections[road_index].color[section_index].g = g;
	m_roadsections[road_index].color[section_index].b = b;
	m_roadsections[road_index].color[section_index].f = f;
}

//------------------------------------------------------------------------------
void SectionMng::setup_color(int road_index)
{
	int pix = 0;
	int section_index = 0;

	for (int nd = 0; nd < m_roadsections[road_index].max_nodes - 1; nd++)
	{
		section_index = m_roadsections[road_index].nodes[nd];

		for (int j = 0; j < 30; j++)
		{
			m_roadsections[road_index].array_data_color[4 * pix + 0] = m_roadsections[road_index].color[section_index].r;
			m_roadsections[road_index].array_data_color[4 * pix + 1] = m_roadsections[road_index].color[section_index].g;
			m_roadsections[road_index].array_data_color[4 * pix + 2] = m_roadsections[road_index].color[section_index].b;
			m_roadsections[road_index].array_data_color[4 * pix + 3] = m_roadsections[road_index].color[section_index].f;
			pix++;
		}
	}
}

//------------------------------------------------------------------------------
void SectionMng::render_setup(int road_index)
{
	glGenVertexArrays(1, &m_roadsections[road_index].VAO);
	glBindVertexArray(m_roadsections[road_index].VAO);

	glGenBuffers(1, &m_roadsections[road_index].VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_roadsections[road_index].VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * m_roadsections[road_index].total_points, m_roadsections[road_index].array_data, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_roadsections[road_index].VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, m_roadsections[road_index].VBO_color);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * m_roadsections[road_index].total_points, m_roadsections[road_index].array_data_color, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_roadsections[road_index].VBO);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_roadsections[road_index].VBO_color);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);

	glBindVertexArray(0);
}

//------------------------------------------------------------------------------
void SectionMng::render_refresh(int road_index)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_roadsections[road_index].VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * m_roadsections[road_index].total_points, m_roadsections[road_index].array_data, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_roadsections[road_index].VBO_color);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * m_roadsections[road_index].total_points, m_roadsections[road_index].array_data_color, GL_STREAM_DRAW);
}

//------------------------------------------------------------------------------
void SectionMng::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{

	if (m_refresh)
	{
		for (int i = 0; i < m_roadindex; i++)
		{
			render_refresh(i);
		}
		m_refresh = false;
	}


	m_shader.use();
	glUniformMatrix4fv(m_VP, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	glEnable(GL_POLYGON_SMOOTH);
	for (int i = 0; i < m_roadindex; i++)
	{
		glBindVertexArray(m_roadsections[i].VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_roadsections[i].total_points);
		glBindVertexArray(0);
	}
}

//------------------------------------------------------------------------------
bool SectionMng::is_active()
{
	return (m_roadindex > 0);
}

//------------------------------------------------------------------------------
void SectionMng::clear()
{
	for (int i = 0; i < SIM_MAX_ROADSECTIONS; i++)
	{
		if (m_roadsections[i].array_data != NULL)
		{
			delete (m_roadsections[i].array_data);
			delete (m_roadsections[i].array_data_color);
			delete (m_roadsections[i].nodes);
		}
		m_roadsections[i].array_data = NULL;
		m_roadsections[i].array_data_color = NULL;
		m_roadsections[i].nodes = NULL;
	}

	m_roadindex = 0;
	mpMN->mButtons[Menu::SIM_BTT_SECMNGR].flag = false;
	m_active = false;
}

//------------------------------------------------------------------------------
void SectionMng::refresh_thread()
{
	while (true)
	{
		while (m_active)
		{
			m_hw = get_size();

			for (int i = 0; i < m_roadindex; i++)
			{
				get_color(i);
				setup_color(i);
				render_refresh(i);
			}
			m_refresh = true;
			Sleep(5000);
 		}
		Sleep(10000);
	}
}

//------------------------------------------------------------------------------
void SectionMng::refresh()
{
	m_hw = get_size();

	for (int i = 0; i < m_roadindex; i++)
	{
		m_roadsections[i].total_points = generate_surface(i);
		render_refresh(i);
	}
}

//------------------------------------------------------------------------------
double SectionMng::get_size()
{
	return (0.0015 * mpCT->mPosition.z);
}
