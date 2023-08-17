#include "Render.h"


//------------------------------------------------------------------------------
Render::Render()
{
	
}

//------------------------------------------------------------------------------
Render::~Render()
{
	
}

//------------------------------------------------------------------------------
void Render::init(Controls* ctrl, QuadMap* mquadmap)
{
	m_ctrl = ctrl;
	m_quad_map = mquadmap;

	// Point
	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_PTS; i++)
	{
		m_GRPPOINT[i].active = false;
		m_GRPPOINT[i].name = "";
		m_GRPPOINT[i].buff_pt_info = NULL;
		m_GRPPOINT[i].buff_pt_draw = NULL;
	}
	
	m_max_group_points = 0;


	// Line
	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_LINES; i++)
	{
		m_GRPLINE[i].active = false;
		m_GRPLINE[i].name = "";
		m_GRPLINE[i].buff_pt_info       = NULL;
		m_GRPLINE[i].buff_pt_draw       = NULL;
		m_GRPLINE[i].buff_pt_draw_color = NULL;
		m_GRPLINE[i].buff_in            = new double[3 * 5000];
		m_GRPLINE[i].buff_out           = new double[3 * 5000 * 30];
		m_GRPLINE[i].max_points_draw    = 0;
		m_GRPLINE[i].max_points         = 0;
		m_GRPLINE[i].size_refresh       = SIM_RENDER_LINE_SIZE;
		m_GRPLINE[i].size               = SIM_RENDER_LINE_SIZE;
		
	}

	m_max_group_lines = 0;
	render_setup();

}

//------------------------------------------------------------------------------
int Render::add_group_point(Cadena name, ColorF& color, GLuint texture, double size)
{
	bool added = false;
	int grp_index = 0;
	Cadena cad_img;

	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_PTS; i++)
	{
		if (m_GRPPOINT[i].active == false)
		{

			if (texture == 0)
			{
				cad_img = "route_point.png";
				Simulator::texture_RGBA(m_text_img_p, cad_img);
			}
			else
			{
				cad_img = "point_circle.png";
				Simulator::texture_RGBA(m_text_img_p, cad_img);
				//Simulator::texture_dds(m_text_img_p, "particle.DDS");
			}

			cout << "Grupo:[" << name.getCadena() << "] [" << i << "] color.r=[" << m_GRPPOINT[i].color.r << "]" << endl;

			m_GRPPOINT[i].active  = true;
			m_GRPPOINT[i].name    = name;
			m_GRPPOINT[i].color   = color;
			m_GRPPOINT[i].texture = m_text_img_p;
			m_GRPPOINT[i].size    = size;
			m_GRPPOINT[i].buff_pt_info = new GPoint[SIM_MAX_RENDER_PTS];
			m_GRPPOINT[i].buff_pt_draw = new double[3 * SIM_MAX_RENDER_PTS];
			grp_index = i;
			m_max_group_points++;
			added = true;
			break;
		}
	}

	if (added == false) { cout << "Warning: You have reached the maximun number of diferent group of points available" << SIM_MAX_RENDER_GROUPS_PTS << endl; }

	return grp_index;
}


//------------------------------------------------------------------------------
int Render::add_group_line(Cadena name, GLuint texture, double size)
{
	bool added = false;
	int grp_index = 0;


	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_LINES; i++)
	{
		if (m_GRPLINE[i].active == false)
		{

			m_GRPLINE[i].active = true;
			m_GRPLINE[i].name = name;
			m_GRPLINE[i].size = size;
			m_GRPLINE[i].buff_pt_info = new GPointLine[SIM_MAX_RENDER_PTS_x_LINE];
			grp_index = i;
			m_max_group_lines++;
			added = true;
			break;
		}
	}

	if (added == false) 
	{
		cout << "Warning: You have reached the maximun number of diferent group of lines available" << SIM_MAX_RENDER_GROUPS_LINES << endl; return 0;
	}

	return grp_index;
}

//------------------------------------------------------------------------------
void Render::del_group_point(int grp_pt_ind)
{
	if (m_GRPPOINT[grp_pt_ind].active == true)
	{
		m_GRPPOINT[grp_pt_ind].active = false;
		m_GRPPOINT[grp_pt_ind].max_points = 0;
		m_GRPPOINT[grp_pt_ind].name = "";

		if (m_GRPPOINT[grp_pt_ind].buff_pt_info != NULL) {delete (m_GRPPOINT[grp_pt_ind].buff_pt_info);}
		if (m_GRPPOINT[grp_pt_ind].buff_pt_draw != NULL) {delete (m_GRPPOINT[grp_pt_ind].buff_pt_draw);}

		m_max_group_points--;
	}
}

//------------------------------------------------------------------------------
void Render::del_group_line(int grp_line_ind)
{
	if (m_GRPLINE[grp_line_ind].active == true)
	{
		m_GRPLINE[grp_line_ind].active = false;
		m_GRPLINE[grp_line_ind].max_points = 0;
		m_GRPLINE[grp_line_ind].name = "";

		if (m_GRPLINE[grp_line_ind].buff_pt_info != NULL) { delete (m_GRPLINE[grp_line_ind].buff_pt_info); }
		if (m_GRPLINE[grp_line_ind].buff_pt_draw != NULL) { delete (m_GRPLINE[grp_line_ind].buff_pt_draw); }

		m_max_group_lines--;
	}
}

//------------------------------------------------------------------------------
void Render::add_gps_point(int grp_pt_ind, double lon, double lat )
{
	double x, y, z = 0.0;
	int max_points = m_GRPPOINT[grp_pt_ind].max_points;

	if (max_points < SIM_MAX_RENDER_PTS)
	{
		//----info----
		x = Simulator::getXfromLon(lon);
		y = Simulator::getYfromLat(lat);

		m_GRPPOINT[grp_pt_ind].buff_pt_info[max_points].lat = lat;
		m_GRPPOINT[grp_pt_ind].buff_pt_info[max_points].lon = lon;
		m_GRPPOINT[grp_pt_ind].buff_pt_info[max_points].X = x;
		m_GRPPOINT[grp_pt_ind].buff_pt_info[max_points].Y = y;

		//----draw----
		m_GRPPOINT[grp_pt_ind].buff_pt_draw[3 * max_points + 0] = x;
		m_GRPPOINT[grp_pt_ind].buff_pt_draw[3 * max_points + 1] = y;
		m_GRPPOINT[grp_pt_ind].buff_pt_draw[3 * max_points + 2] = z;

		m_GRPPOINT[grp_pt_ind].max_points++;

	}
	else
	{
		cout << "Warning: Maximun number of points reached:[" << SIM_MAX_RENDER_PTS << "]" << endl;
		return;
	}

	refresh_point(grp_pt_ind);

}

//------------------------------------------------------------------------------
void Render::add_gps_point_line(int grp_line_ind, double lon, double lat, long long int seg_id)
{
	double x,y,z = 0.0;
	int max_points = m_GRPLINE[grp_line_ind].max_points;

	if (max_points < SIM_MAX_RENDER_PTS_x_LINE)
	{
		//----info----
		x = Simulator::getXfromLon(lon);
		y = Simulator::getYfromLat(lat);

		m_GRPLINE[grp_line_ind].buff_pt_info[max_points].lat = lat;
		m_GRPLINE[grp_line_ind].buff_pt_info[max_points].lon = lon;
		m_GRPLINE[grp_line_ind].buff_pt_info[max_points].X = x;
		m_GRPLINE[grp_line_ind].buff_pt_info[max_points].Y = y;
		m_GRPLINE[grp_line_ind].buff_pt_info[max_points].seg_id = seg_id;

		m_GRPLINE[grp_line_ind].max_points++;
	}
	else
	{
		cout << "Warning: Maximun number of points reached:[" << SIM_MAX_RENDER_PTS_x_LINE << "]" << endl;
		return;
	}
}

//------------------------------------------------------------------------------
void Render::create_segments(int grp_line_ind)
{
	int total_out;
	int seg_id_old = m_GRPLINE[grp_line_ind].buff_pt_info[0].seg_id;
	int i_idx = 0;
	SSegmentLine sl;

	m_GRPLINE[grp_line_ind].max_points_draw = 0;
	m_GRPLINE[grp_line_ind].size_refresh = get_drawing_line_size(m_GRPLINE[grp_line_ind].size);

	for (int i = 0; i < m_GRPLINE[grp_line_ind].max_points; i++)
	{
		if (m_GRPLINE[grp_line_ind].buff_pt_info[i].seg_id != seg_id_old)
		{
			
			Simulator::build_LINE_RENDER(m_GRPLINE[grp_line_ind].size_refresh, m_GRPLINE[grp_line_ind].buff_in, i_idx, m_GRPLINE[grp_line_ind].buff_out, i_idx * 30, &total_out);
			memcpy(&m_GRPLINE[grp_line_ind].buff_pt_draw[m_GRPLINE[grp_line_ind].max_points_draw * 3], m_GRPLINE[grp_line_ind].buff_out, total_out * 3 * sizeof(double));
			m_GRPLINE[grp_line_ind].max_points_draw = total_out + m_GRPLINE[grp_line_ind].max_points_draw;
			seg_id_old = m_GRPLINE[grp_line_ind].buff_pt_info[i].seg_id;
			i_idx = 0;
			sl.max_points = total_out;
			m_GRPLINE[grp_line_ind].map_seg_render_inf[m_GRPLINE[grp_line_ind].buff_pt_info[i].seg_id] = sl;

		}

		m_GRPLINE[grp_line_ind].buff_in[3 * i_idx + 0] = m_GRPLINE[grp_line_ind].buff_pt_info[i].X;
		m_GRPLINE[grp_line_ind].buff_in[3 * i_idx + 1] = m_GRPLINE[grp_line_ind].buff_pt_info[i].Y;
		m_GRPLINE[grp_line_ind].buff_in[3 * i_idx + 2] = 0.0;
		i_idx++;
	}

	Simulator::build_LINE_RENDER(m_GRPLINE[grp_line_ind].size_refresh, m_GRPLINE[grp_line_ind].buff_in, i_idx, m_GRPLINE[grp_line_ind].buff_out, i_idx * 30, &total_out);
	memcpy(&m_GRPLINE[grp_line_ind].buff_pt_draw[m_GRPLINE[grp_line_ind].max_points_draw * 3], m_GRPLINE[grp_line_ind].buff_out, total_out * 3 * sizeof(double));
	m_GRPLINE[grp_line_ind].max_points_draw = total_out + m_GRPLINE[grp_line_ind].max_points_draw;

}

//------------------------------------------------------------------------------
void Render::finish_line(int grp_line_ind)
{
	double z = 0.0;
	int total_out = 0;

	m_GRPLINE[grp_line_ind].buff_pt_draw       = new double[3 * m_GRPLINE[grp_line_ind].max_points * 30 * 10];
	m_GRPLINE[grp_line_ind].buff_pt_draw_color = new GLubyte[4 * m_GRPLINE[grp_line_ind].max_points * 40];

	refresh_line(grp_line_ind);
}

//------------------------------------------------------------------------------
void Render::set_color(int grp_line_ind, map<long long int, Color>& map_color_seg)
{
	map <long long int, Color>::const_iterator itmap;
	long long int seg_id;
	int ix = 0;

	for (itmap = map_color_seg.begin(); itmap != map_color_seg.end(); ++itmap)
	{
		seg_id = itmap->first;
		for (int j = 0; j < m_GRPLINE[grp_line_ind].map_seg_render_inf[seg_id].max_points; j++)
		{
			m_GRPLINE[grp_line_ind].buff_pt_draw_color[4 * ix + 0] = map_color_seg[seg_id].r;
			m_GRPLINE[grp_line_ind].buff_pt_draw_color[4 * ix + 1] = map_color_seg[seg_id].g;
			m_GRPLINE[grp_line_ind].buff_pt_draw_color[4 * ix + 2] = map_color_seg[seg_id].b;
			m_GRPLINE[grp_line_ind].buff_pt_draw_color[4 * ix + 3] = map_color_seg[seg_id].f;
			ix++;

		}
	}
}

//------------------------------------------------------------------------------
void Render::render_setup()
{
	//---VAO---VBO--POINT----
	glGenVertexArrays(1, &m_VAO_point);
	glBindVertexArray(m_VAO_point);

	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_PTS; i++)
	{
		glGenBuffers(1, &m_GRPPOINT[i].vbo_model);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPPOINT[i].vbo_model);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);

		glGenBuffers(1, &m_GRPPOINT[i].vbo_offset);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPPOINT[i].vbo_offset);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_RENDER_PTS, NULL, GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPPOINT[i].vbo_model);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPPOINT[i].vbo_offset);
		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);

	}

	glBindVertexArray(0);

	//---VAO---VBO--LINE----
	glGenVertexArrays(1, &m_VAO_line);
	glBindVertexArray(m_VAO_line);

	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_LINES; i++)
	{

		glGenBuffers(1, &m_GRPLINE[i].vbo_offset);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[i].vbo_offset);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_RENDER_PTS_x_LINE, NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_GRPLINE[i].vbo_color);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[i].vbo_color);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * SIM_MAX_RENDER_PTS_x_LINE, NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[i].vbo_offset);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[i].vbo_color);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 0);

	}

	glBindVertexArray(0);


	//---shader----------------------------------
	m_shader_point.inicializar(SIM_DIR_SHADERS_"render_point.vs", SIM_DIR_SHADERS_"render_point.frag");
	m_shader_line.inicializar(SIM_DIR_SHADERS_"render_line.vs", SIM_DIR_SHADERS_"render_line.frag");

	//--shader variables POINT-------------------
	m_text_id_p  = glGetUniformLocation(m_shader_point.mProgram, "texture_smp");
	m_camera_r_p = glGetUniformLocation(m_shader_point.mProgram, "camera_r");
	m_camera_u_p = glGetUniformLocation(m_shader_point.mProgram, "camera_u");
	m_vp_p       = glGetUniformLocation(m_shader_point.mProgram, "vp");
	m_size_p     = glGetUniformLocation(m_shader_point.mProgram, "point_size");
	m_color_p    = glGetUniformLocation(m_shader_point.mProgram, "color_in");

	//--shader variables LINE-------------------
	m_vp_l = glGetUniformLocation(m_shader_line.mProgram, "vp");

}

//------------------------------------------------------------------------------
void Render::refresh_point(int grp_pt_ind)
{
	//--Point--
	glBindVertexArray(m_VAO_point);
	if (m_GRPPOINT[grp_pt_ind].active == true)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPPOINT[grp_pt_ind].vbo_offset);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * m_GRPPOINT[grp_pt_ind].max_points, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * m_GRPPOINT[grp_pt_ind].max_points, m_GRPPOINT[grp_pt_ind].buff_pt_draw);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	glBindVertexArray(0);

}

//------------------------------------------------------------------------------
void Render::refresh()
{
	for (int i = 0; i < SIM_MAX_RENDER_GROUPS_LINES; i++)
	{
		if (m_GRPLINE[i].active == true)
		{
			refresh_line(i);
		}
	}

}
//------------------------------------------------------------------------------
void Render::refresh_line(int grp_line_ind)
{
	create_segments(grp_line_ind);

	//--Line--
	glBindVertexArray(m_VAO_line);
	if (m_GRPLINE[grp_line_ind].active == true)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[grp_line_ind].vbo_offset);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * m_GRPLINE[grp_line_ind].max_points_draw, m_GRPLINE[grp_line_ind].buff_pt_draw, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * m_GRPLINE[grp_line_ind].max_points_draw, m_GRPLINE[grp_line_ind].buff_pt_draw);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[grp_line_ind].vbo_color);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * m_GRPLINE[grp_line_ind].max_points_draw, m_GRPLINE[grp_line_ind].buff_pt_draw_color, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,1, 4 * sizeof(GLubyte) * m_GRPLINE[grp_line_ind].max_points_draw, m_GRPLINE[grp_line_ind].buff_pt_draw_color);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}
	glBindVertexArray(0);
}

//------------------------------------------------------------------------------
void Render::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	render_line(viewmatrix, viewprojectionmatrix);
	render_point(viewmatrix, viewprojectionmatrix);
}

//------------------------------------------------------------------------------
void Render::render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	m_shader_point.use();

	glUniform3f(m_camera_r_p, viewmatrix[0][0], viewmatrix[1][0], viewmatrix[2][0]);
	glUniform3f(m_camera_u_p, viewmatrix[0][1], viewmatrix[1][1], viewmatrix[2][1]);
	glUniformMatrix4fv(m_vp_p, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	//=>VAO
	glBindVertexArray(m_VAO_point);

	for (int i = 0; i < m_max_group_points; i++)
	{
		if (m_GRPPOINT[i].active == true)
		{
			//texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_GRPPOINT[i].texture);
			glUniform1i(m_text_id_p, 0);
			//size
			glUniform1f(m_size_p, get_drawing_point_size(m_GRPPOINT[i].size));
			//color
			glUniform4f(m_color_p, m_GRPPOINT[i].color.r, m_GRPPOINT[i].color.g, m_GRPPOINT[i].color.b, m_GRPPOINT[i].color.f);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, m_GRPPOINT[i].vbo_offset);
			glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
			glVertexAttribDivisor(1, 1);
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_GRPPOINT[i].max_points);

			glBindTexture(GL_TEXTURE_2D, 0);

		}
	}

	//<=VAO
	glBindVertexArray(0);	

}


//------------------------------------------------------------------------------
void Render::render_line(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	m_shader_line.use();

	glUniformMatrix4fv(m_vp_l, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

	//=>VAO
	glBindVertexArray(m_VAO_line);

	for (int i = 0; i < m_max_group_lines; i++)
	{
		if (m_GRPLINE[i].active == true)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[i].vbo_offset);
			glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * m_GRPLINE[i].max_points_draw, m_GRPLINE[i].buff_pt_draw, GL_STREAM_DRAW);
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
			//glVertexAttribDivisor(0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_GRPLINE[i].vbo_color);
			glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * m_GRPLINE[i].max_points_draw, m_GRPLINE[i].buff_pt_draw_color, GL_STREAM_DRAW);
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);
			//glVertexAttribDivisor(1, 0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_SMOOTH);
			glDrawArrays(GL_TRIANGLES, 0, m_GRPLINE[i].max_points_draw);
		}
	}



	//<=VAO
	glBindVertexArray(0);
}


//------------------------------------------------------------------------------
double Render::get_drawing_point_size(double size)
{
	double element_size = size * SIM_RENDER_POINT_SIZE * m_ctrl->mPosition.z;

	if (element_size < SIM_RENDER_POINT_SIZE)
	{
		element_size = SIM_RENDER_POINT_SIZE;
	}

	return element_size;
}

//------------------------------------------------------------------------------
double Render::get_drawing_line_size(double size)
{
	return (size * SIM_RENDER_LINE_SIZE * m_ctrl->mPosition.z);
}









