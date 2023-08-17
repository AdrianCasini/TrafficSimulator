#include "Texter.h"


//------------------------------------------------------------------------------
Texter::Texter()
{

}

//------------------------------------------------------------------------------
Texter::~Texter()
{

}

//------------------------------------------------------------------------------
void Texter::get_labels()
{
	Cadena cadfile;

	mLabels.clear();
	cadfile.formCadena(SIM_DIR_TEST_LABELS_"aucat.txt");
	cadfile = mcadgral + cadfile;

	if (!mLabels.get_data_from_txt(cadfile))
	{
		cout << "Error: There is an error obtaining information from file:[" << cadfile.getCadena() << "]" << endl;
	}
}

//------------------------------------------------------------------------------
void Texter::get_places_properties()
{
	Cadena cadfile;

	mPlaces.clear();

	cadfile.formCadena(SIM_DIR_ETC_CONFIG_"texter.txt");
	cadfile = mcadgral + cadfile;


	if (!mPlaces.get_data_from_txt(cadfile))
	{
		cout << "Error: There is an error obtaining information from file:[" << cadfile.getCadena() << "]" << endl;
	}
}

//------------------------------------------------------------------------------
void Texter::clean_temp()
{
	for (int i = 0; i < SIM_MAX_TEXTER_PLACES; i++)
	{
		mArrayTemp[i].X = 0.0;
		mArrayTemp[i].Y = 0.0;
		mArrayTemp[i].angle = 0.0;
		strcpy(mArrayTemp[i].name, "");
	}
}

//------------------------------------------------------------------------------
void Texter::init(Controls* ctrl, Tarif* trf, Tools* tls, Cadena cadgral)
{
	mCtrl = ctrl;
	mTRF  = trf;
	mTLS  = tls;
	mcadgral = cadgral;

	mActive = false;
	mNumSegmentsTexture32 = 5;
	mNumTypeOfPlace = 0;

	get_labels();
	get_places_properties();

	mArrayType = new Type_Text[SIM_MAX_TEXTER];
	mArrayTemp = new Node_Text[SIM_MAX_TEXTER_PLACES];

	for (int i = 0; i < mNumSegmentsTexture32; i++)
	{
		mMasterText[i].mModelMatrices = new glm::mat4[SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mMove    = new double[3 * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mScale   = new double[SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mScale_w = new double[SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mScale_h = new double[SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mAngle   = new double[SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mTexture = new int[SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
		mMasterText[i].mColor   = new double[3 * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE];
	}

	for (int i = 0; i < mPlaces.mMaxreg; i++)
	{
		mArrayType[i].file_name = mPlaces.mRegtable[i].reg["file"].str;
		mArrayType[i].name      = mPlaces.mRegtable[i].reg["name"].str;
		mArrayType[i].pos_z     = mPlaces.mRegtable[i].reg["pos_z"].num_d;
		mArrayType[i].size      = mPlaces.mRegtable[i].reg["quad_size"].num_i;
		mArrayType[i].size_txt  = mPlaces.mRegtable[i].reg["size_txt"].num_d;
		mArrayType[i].color.r   = mPlaces.mRegtable[i].reg["color_r"].num_d;
		mArrayType[i].color.g   = mPlaces.mRegtable[i].reg["color_g"].num_d;
		mArrayType[i].color.b   = mPlaces.mRegtable[i].reg["color_b"].num_d;

		mArrayType[i].quad_x_old  = -100000;
		mArrayType[i].quad_y_old  = -100000;
		mArrayType[i].quad_x      = 0;
		mArrayType[i].quad_y      = 0;
		mArrayType[i].num_letters = 0;
		mArrayType[i].count       = 0;
		mArrayType[i].count_temp  = 0;

		mNumTypeOfPlace++;
	}

	//----Labels---------------------------------
	for (int i = 0; i < mLabels.mMaxreg; i++)
	{
		mArrayType[mNumTypeOfPlace - 1].array[i].X = Simulator::getXfromLon(mLabels.mRegtable[i].reg["POSITION_X"].num_d);
		mArrayType[mNumTypeOfPlace - 1].array[i].Y = Simulator::getYfromLat(mLabels.mRegtable[i].reg["POSITION_Y"].num_d);
		mArrayType[mNumTypeOfPlace - 1].array[i].angle = 0.0;
		strcpy(mArrayType[mNumTypeOfPlace - 1].array[i].name, mLabels.mRegtable[i].reg["LABEL"].str.getCadena());
		mArrayType[mNumTypeOfPlace - 1].count_temp++;


	}
	mArrayType[mNumTypeOfPlace-1].count = mArrayType[mNumTypeOfPlace-1].count_temp;


	strcpy(mTitle, SIM_WINDOW_SUB_TITLE);
	mColorTitle = glm::vec3(0.4, 0.4f, 0.65f);
	mColorKM = glm::vec3(0.9, 0.6f, 0.55f);
	mColorEU = glm::vec3(0.9, 0.6f, 0.55f);

	mShader.inicializar(SIM_DIR_SHADERS_"text.vs", SIM_DIR_SHADERS_"text.frag");
	mShader_plus.inicializar(SIM_DIR_SHADERS_"text_plus.vs", SIM_DIR_SHADERS_"text_plus.frag");

	Simulator::init_fonts_plus(mCharacters_plus);
	init_render();
	
}


//------------------------------------------------------------------------------
void Texter::init_render()
{
	Cadena cad;

	for (int i = 0; i < mNumSegmentsTexture32; i++)
	{

		glGenVertexArrays(1, &mMasterText[i].mVAO_plus);
		glBindVertexArray(mMasterText[i].mVAO_plus);
		//---------
		glGenBuffers(1, &mMasterText[i].mVBO_model_plus);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_model_plus);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MODEL_TXT), MODEL_TXT, GL_STATIC_DRAW);

		glGenBuffers(1, &mMasterText[i].mVBO_Scale_w_plus);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Scale_w_plus);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mMasterText[i].mVBO_Scale_h_plus);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Scale_h_plus);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mMasterText[i].mVBO_Text_plus);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Text_plus);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLfloat) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mMasterText[i].mVBO_Color);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Color);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);


		glGenBuffers(1, &mMasterText[i].mVBO_Tranformation_plus);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Tranformation_plus);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);

		//---------
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_model_plus);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Scale_w_plus);
		glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Scale_h_plus);
		glVertexAttribPointer(2, 1, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Text_plus);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Color);
		glVertexAttribPointer(4, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);


		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[i].mVBO_Tranformation_plus);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
		//---------
		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		glBindVertexArray(0);

	}

	for (int i = 0; i < 32; i++)
	{
		cad.formCadena("%s%d", "text", i);
		mArrayLetter[i] = glGetUniformLocation(mShader_plus.mProgram, cad.getCadena());
	}
	
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

//------------------------------------------------------------------------------
void Texter::render(glm::mat4& viewprojectionmatrix, int screenW, int screenH)
{
	
	render_titles(screenW, screenH);

	if (mTLS->exist_type_polygon(Tools::SIM_POLYGON_TOLL))
	{
		render_toller_payment(screenW, screenH);
		render_toller_payment_sp(screenW, screenH);
	}
	
	if (is_active())
	{
		get_quads();
		render_text_massive(viewprojectionmatrix, screenW, screenH);
	}
}

//------------------------------------------------------------------------------
void Texter::refresh(int screenW, int screenH)
{
	mProjectionText = glm::ortho(0.0f, static_cast<GLfloat>(screenW), 0.0f, static_cast<GLfloat>(screenH));
	info_matrix_plus();
}

//------------------------------------------------------------------------------
void Texter::render_text_massive(glm::mat4& viewprojectionmatrix, int screenW, int screenH)
{
	Character ch;
	vector<unsigned char>::const_iterator it;
	int count;
	glm::vec3 color;

	color = glm::vec3(0.35f, 0.35f, 0.36f);
	int num_segment = 0;


	mShader_plus.use();
	glUniform3f(glGetUniformLocation(mShader_plus.mProgram, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(mShader_plus.mProgram, "VP"), 1, GL_FALSE, glm::value_ptr(viewprojectionmatrix));

	//-----------From [ ]  to [?]-------------------------
	count = 0;
	glBindVertexArray(mMasterText[num_segment].mVAO_plus);
	for (unsigned char i = 32; i < 32 + 32; i++)
	{
		ch = mCharacters_plus[i];
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(mArrayLetter[count], count);
		count++;
	}
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mMasterText[num_segment].mNumberText);
	glBindVertexArray(0);
	num_segment++;

	//-----------From [@]  to []]-------------------------
	count = 0;
	glBindVertexArray(mMasterText[num_segment].mVAO_plus);
	for (unsigned char i = 64; i < 64 + 32; i++)
	{
		ch = mCharacters_plus[i];
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(mArrayLetter[count], count);
		count++;
	}

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mMasterText[num_segment].mNumberText);
	glBindVertexArray(0);
	num_segment++;

	//-----------From [a]  to [~]-------------------------
	count = 0;

	glBindVertexArray(mMasterText[num_segment].mVAO_plus);
	for (unsigned char i = 97; i < 97 + 32; i++)
	{
		ch = mCharacters_plus[i];
		glActiveTexture(GL_TEXTURE0 + count);

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(mArrayLetter[count], count);
		count++;
	}

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mMasterText[num_segment].mNumberText);
	glBindVertexArray(0);
	num_segment++;


	//--------------------------------------------------
	count = 0;
	glBindVertexArray(mMasterText[num_segment].mVAO_plus);
	for (unsigned char i = 223; i < 223 + 32; i++)
	{
		ch = mCharacters_plus[i];
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(mArrayLetter[count], count);
		count++;
	}
	   
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mMasterText[num_segment].mNumberText);
	glBindVertexArray(0);
	num_segment++;
	
	//--------------------------------------------------
	count = 0;
	glBindVertexArray(mMasterText[num_segment].mVAO_plus);
	for (unsigned char i = 190; i < 190 + 32; i++)
	{
		ch = mCharacters_plus[i];
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(mArrayLetter[count], count);
		count++;
	}
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mMasterText[num_segment].mNumberText);
	glBindVertexArray(0);
	num_segment++;

}

//-----------------------------------------------------------------------
void Texter::info_matrix_plus()
{
	Character ch;
	Cadena cad;
	double X, Y, angle, scale, delta;
	int segment;
	bool bstore=false;

	for (int segment = 0; segment < mNumSegmentsTexture32; segment++)
	{
		mMasterText[segment].count = 0;
	}

	for (int types=0; types < mNumTypeOfPlace; types++)
	{
		scale = mArrayType[types].size_txt * mCtrl->mText;

		for (int word = 0; word < mArrayType[types].count; word++)
		{
			cad   = mArrayType[types].array[word].name;
			X     = mArrayType[types].array[word].X;
			Y     = mArrayType[types].array[word].Y;
			angle = mArrayType[types].array[word].angle;

			for (int i = 0; i < cad.getLong(); i++)
			{
				bstore = false;
				char c = *(cad.getCadena() + i);

				if ((0 < int(c)) && (int(c) < 127))
				{
					ch = mCharacters_plus[unsigned char(*(cad.getCadena() + i))];


					if ((32 <= int(c)) && (int(c) <= 63))
					{
						segment = 0;
						bstore = true;
					}
					else if ((64 <= int(c)) && (int(c) <= 95))
					{
						segment = 1;
						bstore = true;
					}
					else if ((96 <= int(c)) && (int(c) < 127))
					{
						segment = 2;
						bstore = true;
					}
				}
				else
				{
					i++;
					ch = mCharacters_plus[int(*(cad.getCadena() + i)) + 320];
							
					if ((223 <= int(*(cad.getCadena() + i)) + 320) && (int(c) < int(*(cad.getCadena() + i)) + 320))
					{
						segment = 3;
						bstore = true;
					}
					else if ((190 <= int(*(cad.getCadena() + i)) + 320) && (int(c) < int(*(cad.getCadena() + i)) + 320))
					{
						segment = 4;
						bstore = true;
					}
				}
				

				if (bstore == true)
				{

					mMasterText[segment].mColor[3 * mMasterText[segment].count + 0] = mArrayType[types].color.r;
					mMasterText[segment].mColor[3 * mMasterText[segment].count + 1] = mArrayType[types].color.g;
					mMasterText[segment].mColor[3 * mMasterText[segment].count + 2] = mArrayType[types].color.b;


					mMasterText[segment].mMove[3 * mMasterText[segment].count + 0] = X + (double(ch.Size.x) / 2.0 + double(ch.Bearing.x)) * scale * cos(angle);
					mMasterText[segment].mMove[3 * mMasterText[segment].count + 1] = Y - (double(ch.Size.y) / 2.0 - double(ch.Bearing.y)) * scale * cos(angle);
					mMasterText[segment].mMove[3 * mMasterText[segment].count + 2] = 0.005;

					mMasterText[segment].mScale_w[mMasterText[segment].count] = double(ch.Size.x) * scale;
					mMasterText[segment].mScale_h[mMasterText[segment].count] = double(ch.Size.y) * scale;

					mMasterText[segment].mScale[mMasterText[segment].count] = 1;
					mMasterText[segment].mAngle[mMasterText[segment].count] = SIM_PI + angle;

					if ((segment == 0) || (segment == 1) || (segment == 2))
					{
						mMasterText[segment].mTexture[mMasterText[segment].count] = unsigned char(*(cad.getCadena() + i));
					}
					else
					{
						mMasterText[segment].mTexture[mMasterText[segment].count] = int(*(cad.getCadena() + i)) + 320;
					}

					delta = double(ch.Advance >> 6) * scale;
					X = X + delta * cos(angle);
					Y = Y + delta * sin(angle);

					mMasterText[segment].count++;
					if (mMasterText[segment].count > SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE)
					{
						cout << "Error: mMasterText[ltype].count > SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE" << endl;
						mMasterText[segment].count--;
					}
				}
			}
		}
	}


	for (int segment = 0; segment < mNumSegmentsTexture32; segment++)
	{
		Simulator::spin_model_vh(mMasterText[segment].mMove, mMasterText[segment].mScale, mMasterText[segment].mAngle, mMasterText[segment].count, mMasterText[segment].mModelMatrices);
		mMasterText[segment].mNumberText = mMasterText[segment].count;

     	glBindBuffer(GL_ARRAY_BUFFER, mMasterText[segment].mVBO_Scale_w_plus);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 1 * sizeof(GLdouble) * mMasterText[segment].mNumberText, mMasterText[segment].mScale_w);

		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[segment].mVBO_Scale_h_plus);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 1 * sizeof(GLdouble) * mMasterText[segment].mNumberText, mMasterText[segment].mScale_h);

		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[segment].mVBO_Text_plus);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLfloat) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 1 * sizeof(GLfloat) * mMasterText[segment].mNumberText, mMasterText[segment].mTexture);

		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[segment].mVBO_Color);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(GLdouble) * mMasterText[segment].mNumberText, mMasterText[segment].mColor);


		glBindBuffer(GL_ARRAY_BUFFER, mMasterText[segment].mVBO_Tranformation_plus);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * SIM_MAX_TEXTER_PLACES * SIM_MAX_TEXTER_NAME_SIZE, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * mMasterText[segment].mNumberText, &mMasterText[segment].mModelMatrices[0]);

	}
}


//------------------------------------------------------------------------------
//     Main Thread to Obtain text
//------------------------------------------------------------------------------
void Texter::get_quads()
{

	if (is_active())
	{
		bool activity = false;
		for (int itx = 0; itx < mPlaces.mMaxreg-1; itx++)
		{
			mArrayType[itx].count_temp = 0;
			if (mCtrl->mPosition.z < mArrayType[itx].pos_z) //&& (abs (mCtrl->mPosition.z - mArrayType[itx].pos_z) < 50))
			{
				mArrayType[itx].quad_x = int(mCtrl->mPosition.x / (mArrayType[itx].size));
				if (mCtrl->mPosition.x >= 0) { mArrayType[itx].quad_x = mArrayType[itx].quad_x + 1; }
				mArrayType[itx].quad_y = int(mCtrl->mPosition.y / (mArrayType[itx].size));
				if (mCtrl->mPosition.y <= 0) { mArrayType[itx].quad_y = mArrayType[itx].quad_y - 1; }

				if (mArrayType[itx].quad_x_old != mArrayType[itx].quad_x)
				{
					mArrayType[itx].quad_x_old = mArrayType[itx].quad_x;
					mArrayType[itx].count_temp = 0;
					
					get_info(mArrayType[itx].quad_x, mArrayType[itx].quad_y, itx);

					mArrayType[itx].count = 0;
					memcpy(mArrayType[itx].array, mArrayTemp, mArrayType[itx].count_temp * sizeof(Node_Text));
					mArrayType[itx].count = mArrayType[itx].count_temp;
					mArrayType[itx].count_temp = 0;
					activity = true;

				}
				
				if (mArrayType[itx].quad_y_old != mArrayType[itx].quad_y)
				{
					mArrayType[itx].quad_y_old = mArrayType[itx].quad_y;
					mArrayType[itx].count_temp = 0;

					get_info(mArrayType[itx].quad_x, mArrayType[itx].quad_y, itx);

					mArrayType[itx].count = 0;
					memcpy(mArrayType[itx].array, mArrayTemp, mArrayType[itx].count_temp * sizeof(Node_Text));
					mArrayType[itx].count = mArrayType[itx].count_temp;
					mArrayType[itx].count_temp = 0;
					activity = true;
				}
			}
			else
			{
				mArrayType[itx].count = 0;
				mArrayType[itx].count_temp = 0;
				mArrayType[itx].quad_x = 10000;
				mArrayType[itx].quad_y = 10000;
				if (mArrayType[itx].quad_x_old != mArrayType[itx].quad_x)
				{
					mArrayType[itx].quad_x_old = 10000;
					mArrayType[itx].quad_y_old = 10000;
					activity = true;
				}
			}
		}//end of each type text


		if (activity == true)
		{
			info_matrix_plus();
			activity = false;
		}
	}
}

//------------------------------------------------------------------------------
void Texter::get_info(int cuadCX, int cuadCY, int itx)
{
	Cadena cadpath;
	int cuadx, cuady;
	int size = mArrayType[itx].size;
	Cadena cad_file = mArrayType[itx].file_name;


	clean_temp();

	cuadx = cuadCX * size - size;
	cuady = cuadCY * size + size;
	mMapCuads[0] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size - 2 * size;
	cuady = cuadCY * size + size;
	mMapCuads[1] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size - 2 * size;
	cuady = cuadCY * size + 2 * size;
	mMapCuads[2] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size - size;
	cuady = cuadCY * size + 2 * size;
	mMapCuads[3] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size + 0;
	cuady = cuadCY * size + 2 * size;
	mMapCuads[4] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size + 0;
	cuady = cuadCY * size + size;
	mMapCuads[5] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size + 0;
	cuady = cuadCY * size + 0;
	mMapCuads[6] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size - size;
	cuady = cuadCY * size - 0;
	mMapCuads[7] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	cuadx = cuadCX * size - 2 * size;
	cuady = cuadCY * size - 0;
	mMapCuads[8] = cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", cad_file.getCadena(), cad_file.getCadena(), cuadx, cuady);

	for (int ifile = 0; ifile < SIM_MAX_TEXTER_QUADS; ifile++)
	{
		get_each_file(itx, ifile);
	}
}


//------------------------------------------------------------------------------
void Texter::get_each_file(int itx, int ifile)
{
	Cadena cadpath;
	
	cadpath.formCadena("%s/%s", mcadgral.getCadena(), mMapCuads[ifile].getCadena());

	ifstream  infile(cadpath.getCadena());
	map<string, int> mrep;

	if (!infile.is_open())
	{
		return;
	}
	else
	{
		string line_str;

		while ((getline(infile, line_str)) && (mArrayType[itx].count_temp < SIM_MAX_TEXTER_PLACES))
		{
			if (line_str != "")
			{

				if (std::regex_match(line_str, regex(".*(&apos;).*")))
				{
					string str = line_str;
					line_str = regex_replace(str, regex("(&apos;)"), "'");
				}

				stringstream ss(line_str);
				vector<string> result;

				while (ss.good())
				{
					string substr;
					getline(ss, substr, ';');
					result.push_back(substr);
				}

				mArrayTemp[mArrayType[itx].count_temp].X = atof(result[0].c_str());
				mArrayTemp[mArrayType[itx].count_temp].Y = atof(result[1].c_str());

				if (mArrayType[itx].name == "residential")
				{
					if (result[3].size() < SIM_MAX_TEXTER_NAME_SIZE)
					{
						mArrayTemp[mArrayType[itx].count_temp].angle = atof(result[2].c_str());

						result[3].copy(mArrayTemp[mArrayType[itx].count_temp].name, result[3].size(), 0);
						mArrayTemp[mArrayType[itx].count_temp].name[result[3].size()] = '\0';
						if (mrep.find(result[3]) == mrep.end())
						{
							mrep[result[3]] = 1;
							mArrayType[itx].count_temp++;
							mArrayType[itx].num_letters = mArrayType[itx].num_letters + result[3].size();
						}
					}
				}
				else
				{
					if (result[2].size() < SIM_MAX_TEXTER_NAME_SIZE)
					{
						result[2].copy(mArrayTemp[mArrayType[itx].count_temp].name, result[2].size(), 0);
						mArrayTemp[mArrayType[itx].count_temp].name[result[2].size()] = '\0';
						mArrayTemp[mArrayType[itx].count_temp].angle = 0.0;
						mArrayType[itx].count_temp++;
						mArrayType[itx].num_letters = mArrayType[itx].num_letters + result[2].size();
					}
				}
			}
		}
		infile.close();
	}
}


//------------------------------------------------------------------------------
void Texter::active()
{
	if (mActive == false) {mActive = true;}
	else { mActive = false; }
}

//------------------------------------------------------------------------------
bool Texter::is_active()
{
	return mActive;
}

//------------------------------------------------------------------------------
void Texter::render_titles( int screenW, int screenH)
{
	double delta_refresh = (double)screenW / (double)SIM_SCREEN_W;

	mShader.use();
	glUniformMatrix4fv(glGetUniformLocation(mShader.mProgram, "projection"), 1, GL_FALSE, glm::value_ptr(mProjectionText));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);
	glUniform3f(glGetUniformLocation(mShader.mProgram, "textColor"), mColorTitle.x, mColorTitle.y, mColorTitle.z);
	render_text(mTitle, screenW-(145.0 * delta_refresh), 10.0 * delta_refresh, 0.1 * delta_refresh);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------
void Texter::render_text(string text, GLfloat x, GLfloat y, GLfloat scale)
{
	Character ch;
	Cadena cad = text.c_str();

	for (int i = 0; i < text.size(); i++)
	{
		char c = *(cad.getCadena() + i);

		if ((0 < int(c)) && (int(c) < 127))
		{
			ch = mCharacters_plus[unsigned char(*(cad.getCadena() + i))];
		}
		else
		{
			i++;
			ch = mCharacters_plus[int(*(cad.getCadena() + i)) + 320];
		}


		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;


		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};


		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
}

//------------------------------------------------------------------------------
void Texter::render_toller_payment(int screenW, int screenH)
{
	char array_char[200];

	mShader.use();
	glUniformMatrix4fv(glGetUniformLocation(mShader.mProgram, "projection"), 1, GL_FALSE, glm::value_ptr(mProjectionText));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);

	glUniform3f(glGetUniformLocation(mShader.mProgram, "textColor"), mColorKM.x, mColorKM.y, mColorKM.z);
	strcpy(array_char, mTLS->mText_km.getCadena());
	render_text(array_char, screenW * 0.7, screenH *0.5, 0.6);

	glUniform3f(glGetUniformLocation(mShader.mProgram, "textColor"), mColorEU.x, mColorEU.y, mColorEU.z);
	strcpy(array_char, mTLS->mText_eu.getCadena());
	render_text(array_char, screenW * 0.7, screenH * 0.6, 0.6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


//------------------------------------------------------------------------------
void Texter::render_toller_payment_sp(int screenW, int screenH)
{
	char array_char[200];

	mShader.use();
	glUniformMatrix4fv(glGetUniformLocation(mShader.mProgram, "projection"), 1, GL_FALSE, glm::value_ptr(mProjectionText));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);

	glUniform3f(glGetUniformLocation(mShader.mProgram, "textColor"), mColorKM.x, mColorKM.y, mColorKM.z);
	strcpy(array_char, mTRF->mText_km.getCadena());
	render_text(array_char, (float)screenW - (float)550, screenH - 430, 1.1);

	glUniform3f(glGetUniformLocation(mShader.mProgram, "textColor"), mColorEU.x, mColorEU.y, mColorEU.z);
	strcpy(array_char, mTRF->mText_eu.getCadena());
	render_text(array_char, (float)screenW - (float)550, screenH - 490, 1.1);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
