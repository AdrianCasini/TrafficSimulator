#include "Trace.h"

//------------------------------------------------------------------------------
Trace::Trace()
{

}

//------------------------------------------------------------------------------
Trace::~Trace()
{

}



//------------------------------------------------------------------------------
void Trace::init(Menu* menu, Cadena cadgral)
{

	mpMenu = menu;
	mcadgral = cadgral;

	mSize = 0.20;
	mSizeNow = 0.20;
	mPoints = 0;
	mActive = false;
	
	m_buffer_points   = new GLdouble[SIM_MAX_TRACE_POINTS * 3];
	mMap_buffer_model = new GLdouble[SIM_MAX_TRACE_POINTS * 12];
	mMap_buffer_color = new GLubyte[SIM_MAX_TRACE_POINTS * 4];
	mMap_buffer_trip  = new GLdouble[SIM_MAX_TRACE_POINTS * 3];

	if (m_buffer_points == NULL) return;
	if (mMap_buffer_model == NULL) return;
	if (mMap_buffer_color == NULL) return;
	if (mMap_buffer_trip == NULL) return;


	modelMatrices = new glm::mat4[SIM_MAX_TRACE_POINTS];


	mColor_out.r = 62;
	mColor_out.g = 62;
	mColor_out.b = 255;
	mColor_out.f = 255;

	mColor_in.r = 30;
	mColor_in.g = 255;
	mColor_in.b = 22;
	mColor_in.f = 255;

	for (int i = 0; i < SIM_MAX_TRACE_POINTS; i++)
	{
		m_buffer_points[3 * i + 0] = 0.0f;
		m_buffer_points[3 * i + 1] = 0.0f;
		m_buffer_points[3 * i + 2] = 0.0f;

		mMap_buffer_color[4 * i + 0] = mColor_out.r;
		mMap_buffer_color[4 * i + 1] = mColor_out.g;
		mMap_buffer_color[4 * i + 2] = mColor_out.b;
		mMap_buffer_color[4 * i + 3] = mColor_out.f;
	}


	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);


	glGenBuffers(1, &mVBO_Model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLdouble), MODEL_4P, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mVBO_Color);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * SIM_MAX_TRACE_POINTS, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mVBO_Tranformation);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Tranformation);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * SIM_MAX_TRACE_POINTS, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Tranformation);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));


	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);


	glBindVertexArray(0);


	mShader.inicializar(SIM_DIR_SHADERS_"trace_punto.vs", SIM_DIR_SHADERS_"trace_punto.frag");

	mVP        = glGetUniformLocation(mShader.mProgram, "VP");
	mPW        = glGetUniformLocation(mShader.mProgram, "PW");
	mTextureID = glGetUniformLocation(mShader.mProgram, "Texture");


	glGenTextures(1, &mTextureImg);
	glBindTexture(GL_TEXTURE_2D, mTextureImg);
	/*image = SOIL_load_image(SIM_DIR_IMAGES_"images/arrow_short.png", &width, &height, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);*/

	//mTextureImg = loadDDS(SIM_DIR_IMAGES_"images/particle.DDS");
	Simulator::texture_dds(mTextureImg, "particle.DDS");

	glBindTexture(GL_TEXTURE_2D, 0);
	
}
//------------------------------------------------------------------------------
bool Trace::is_active()
{
	return (mActive);
}

//------------------------------------------------------------------------------
void Trace::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * SIM_MAX_TRACE_POINTS, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(GLubyte) * mPoints, mMap_buffer_color);
	
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Tranformation);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * SIM_MAX_TRACE_POINTS, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * mPoints, &modelMatrices[0]);


	mShader.use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureImg);
	glUniform1i(mTextureID, 0);

	glUniformMatrix4fv(mVP, 1, GL_FALSE, &viewprojectionmatrix[0][0]);
	glUniform1f(mPW,mSizeNow);


	glBindVertexArray(mVAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mPoints);
	glBindVertexArray(0);
}

//------------------------------------------------------------------------------
void Trace::clear()
{

	mPoints = 0;
	for (int i = 0; i < SIM_MAX_TRACE_POINTS; i++)
	{
		mMap_buffer_color[4 * i + 0] = mColor_out.r;
		mMap_buffer_color[4 * i + 1] = mColor_out.g;
		mMap_buffer_color[4 * i + 2] = mColor_out.b;
		mMap_buffer_color[4 * i + 3] = mColor_out.f;
	}
	mActive = false;
	mpMenu->mButtons[Menu::SIM_BTT_TRACE].flag = false;
}

//------------------------------------------------------------------------------
void Trace::refresh_size(double position_z)
{
	mSizeNow = mSize * position_z;
	if (mSizeNow < 0.1) {mSizeNow = 0.1;}
}

//------------------------------------------------------------------------------
void Trace::get_points_from_file(Cadena cadfile)
{
	get_points_from_file_format_master(cadfile);
}
//------------------------------------------------------------------------------
void Trace::get_points_from_file_format_master(Cadena cadfile)
{
	string line;
	Color color_trip;
	string substr;
	vector<Cadena> result;
	Cadena cad;
	double lon;
	double lat;
	int factor = 120;
	int delta = 40;
	int pos=0;
	char sep_char;

	ifstream f(cadfile.getCadena());
	if (!f.is_open()) 
	{ 
		MessageBox(NULL, cadfile.getCadena(), "You havn't picked up any file", MB_OK);
		perror("get_points_from_file_format_master: error while opening file");	
		return; 
	}

	color_trip.r = rand() % factor + delta;
	color_trip.g = rand() % factor + delta;
	color_trip.b = rand() % factor + delta;


	while (getline(f, line))
	{
		if (line.find_first_of('#', pos) != string::npos) 	{sep_char = '#';}
		if (line.find_first_of(',', pos) != string::npos)	{sep_char = ',';}
		if (line.find_first_of(';', pos) != string::npos)   { sep_char = ';'; }

		if (mPoints >= SIM_MAX_TRACE_POINTS)
		{
			std::cout << "You have reached the maximun number of GPS points:[" << mPoints << "]" << endl;
			MessageBox(NULL, "You have reached the maximun number of GPS points", "Limit Reached", MB_OK);;
			break;
		}
		else
		{

			if ((line[0] != '#') && (line[0] != ' '))
			{
				stringstream ss(line);
				result.clear();
				while (ss.good())
				{
					//41.123456,2.123456
					//41.123456#2.123456
					getline(ss, substr, sep_char);
					cad = substr.c_str();
					cad.rTrim();
					cad.lTrim();
					result.push_back(cad);
				}

				lat = atof(result[0].c_str());
				lon = atof(result[1].c_str());

				m_buffer_points[3 * mPoints + 0] = Simulator::getXfromLon(lon);
				m_buffer_points[3 * mPoints + 1] = Simulator::getYfromLat(lat);
				m_buffer_points[3 * mPoints + 2] = 0.0;


				mMap_buffer_color[4 * mPoints + 0] = color_trip.r;
				mMap_buffer_color[4 * mPoints + 1] = color_trip.g;
				mMap_buffer_color[4 * mPoints + 2] = color_trip.b;
				mMap_buffer_color[4 * mPoints + 3] = 255;

				mPoints++;
				mActive = true;
				mpMenu->mButtons[Menu::SIM_BTT_TRACE].flag = true;

			}
		}
	}

	std::cout << "Trace GPS Points:[" << mPoints << "]" << endl;
	Simulator::spin_model(m_buffer_points, mPoints, modelMatrices);
}

//------------------------------------------------------------------------------
void Trace::get_points_from_file_format_2(Cadena cadfile)
{
	string line;
	string substr;
	vector<Cadena> result;
	Cadena cad;
	Color color_trip;
	double lon, lon_grades, lon_min, lon_fractpart, lon_intpart;
	double lat, lat_grades, lat_min, lat_fractpart, lat_intpart;
	int factor = 220;
	int delta = 35;
	int indexline = 1;


	color_trip.r = (rand() % factor + delta);
	color_trip.g = (rand() % factor + delta);
	color_trip.b = (rand() % factor + delta);

	ifstream f(cadfile.getCadena());
	if (!f.is_open())
	{
		perror("get_points_from_file_format_2: error while opening file");
		MessageBox(NULL, cadfile.getCadena(), "You have NOT picked up any file", MB_OK);
		return;

	}


	while (getline(f, line))
	{

		if (mPoints >= SIM_MAX_TRACE_POINTS)
		{
			std::cout << "You have reached the maximun number of GPS points:[" << mPoints << "]" << endl;
			MessageBox(NULL, "You have reached the maximun number of GPS points", "Limit Reached", MB_OK);;
			break;
		}
		else
		{
			stringstream ss(line);
			result.clear();
			while (ss.good())
			{
				//*HQ,830202000200000,V1,095244,A,4135.7295,N,00218.9802,E,217.5,000,050221,FFFFFFFF#
				getline(ss, substr, ',');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();
				result.push_back(cad);
			}

			lat = atof(result[5].c_str());
			lat_fractpart = modf(lat, &lat_intpart);
			lat_grades = double(int(lat_intpart / 100.0) % 100);
			lat_min = lat - lat_grades * 100.0;
			lat_min = lat_min / 60.0;
			lat = lat_grades + lat_min;

			lon = atof(result[7].c_str());
			lon_fractpart = modf(lon, &lon_intpart);
			lon_grades = double(int(lon_intpart / 100.0) % 100);
			lon_min = lon - lon_grades * 100.0;
			lon_min = lon_min / 60.0;
			lon = lon_grades + lon_min;

			m_buffer_points[3 * mPoints + 0] = Simulator::getXfromLon(lon);
			m_buffer_points[3 * mPoints + 1] = Simulator::getYfromLat(lat);
			m_buffer_points[3 * mPoints + 2] = 0.0;


			mMap_buffer_color[4 * mPoints + 0] = color_trip.r;
			mMap_buffer_color[4 * mPoints + 1] = color_trip.g;
			mMap_buffer_color[4 * mPoints + 2] = color_trip.b;
			mMap_buffer_color[4 * mPoints + 3] = 255;


			mPoints++;
			mActive = true;
			mpMenu->mButtons[Menu::SIM_BTT_TRACE].flag = true;
		}
	}

	cout << "mPoints GPS:[" << mPoints << "]" << endl;
	Simulator::spin_model(m_buffer_points, mPoints, modelMatrices);
}