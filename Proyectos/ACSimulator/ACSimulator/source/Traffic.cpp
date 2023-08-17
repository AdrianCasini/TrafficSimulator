#include "Traffic.h"

//------------------------------------------------------------------------------
Traffic::Traffic()
{

}

//------------------------------------------------------------------------------
Traffic::~Traffic()
{
	mTH.pPxy->format_info_pos_eurotoll_tailer(mTH.pVH);
}

//------------------------------------------------------------------------------
void Traffic::init(int fecha, Tools* tls, Menu* mn, PofI* poi, BaseDatos* db, Cadena cadgral)
{
	mpTLS = tls;
	mpMN  = mn;
	mpPOI = poi;

	mTH.db_time   = fecha;
	mTH.pDB       = db;

	mcadgral = cadgral;

	//----- VH & Pos -----
	mTH.pVH       = new(std::nothrow) Vehiculo[SIM_MAX_VEHICLES]; if (mTH.pVH == NULL)  { exit(1); }
	//--Trip------------
	mTH.pTrip     = new(std::nothrow) Trip;	                     if (mTH.pTrip == NULL)  { exit(1); }
	mTH.pTripI    = new(std::nothrow) TripInfo[SIM_MAX_TRIPS];   if (mTH.pTripI == NULL) { exit(1); }
	mTH.trip_index = 0;
	mTH.pTrip->init(mTH.pDB, mcadgral);

	//--Proxy------------
	mTH.pPxy      = new Proxy;
	mTH.pPxy->init(mTH.dir_init);

	//--NodeOrder------------
	mTH.pNO = new(std::nothrow) NodeOrder [SIM_MAX_NODE_ORDER];	  if (mTH.pNO == NULL) { exit(1); }
	mTH.norder_index = 0;
	mTH.map_norder.clear();

	//--Logic------------
	mpLG         = new Logic;
	mpLG->init(this);
	mpLG->mDebug = mDebug;

	//--Overlapper------------
	mOV.init();
	mOV.mDebug = mDebug;

	//--Test------------
	mTH.source_mutex = CreateMutex(NULL, FALSE, NULL);
	reset();
	mpTST = new Test;
	mpTST->init(&mTH, mcadgral);
	mpTST->reset_veh();

	//---3D Model-----------------
	mModelCount = 0;
	mOurModel.loadModel(SIM_DIR_OBJECTS_"car_turism.obj");

	for (int i = 0; i < mOurModel.meshes.size(); i++)
	{
		for (int j = 0; j < mOurModel.meshes[i].vertices.size(); j++)
		{
			if (mModelCount < SIM_MAX_3D_MODEL_POINTS)
			{
				mMODEL_3DP[mModelCount].x = mOurModel.meshes[i].vertices[j].Position.z;
				mMODEL_3DP[mModelCount].y = mOurModel.meshes[i].vertices[j].Position.x;
				mMODEL_3DP[mModelCount].z = mOurModel.meshes[i].vertices[j].Position.y;
				mModelCount++;
			}
		}
	}

	mTurbo = 0;
	mNightColor = false;

	setup_render();
}

//----------------------------------------------------------------------------------------------------
void Traffic::reset()
{
	mTH.state  = SIM_TR_STATE_STOP;
	mTH.type   = SIM_TR_MOVE_SIMPLE;
	mTH.render = SIM_TR_RENDER_SIMPLE;

	mTH.index_veh = 0;
	mTH.index_gral = 0;
	mTH.guard = 2;
	mTH.pw_speed = 0;

	mTH.vh_alive = 0;
	mTH.vh_alive_real_time = 0;

	mTH.norder_index = 0;
	mTH.map_norder.clear();

	mTH.db_time = 0;
	mTH.last_time     = glfwGetTime();
	mTH.current_time  = glfwGetTime();
	mTH.delta_time = 0.0;

	mTH.gps_counter = 830202000200000;
	mTH.delta_lane = 1.0;

	mpMN->mButtons[Menu::SIM_BTT_PAUSE].flag     = false;
	mpMN->mButtons[Menu::SIM_BTT_PLAY].flag      = false;
	mpMN->mButtons[Menu::SIM_BTT_ALGORITHM].flag = false;

}
//------------------------------------------------------------------------------
void Traffic::turbo()
{
	if (mTurbo == 0)
	{
		mpMN->mButtons[Menu::SIM_BTT_TURBO].flag = true;
		mTurbo = 1000;
	}
	else
	{
		mTurbo = 0;
		mpMN->mButtons[Menu::SIM_BTT_TURBO].flag = false;
	}

}

//------------------------------------------------------------------------------
void Traffic::clear()
{
	mTH.state = SIM_TR_STATE_STOP;
	Sleep(200);
	mpTST->reset();
	reset();
}

//------------------------------------------------------------------------------
void Traffic::clear_polygon()
{
	Vehiculo* pVH = mTH.pVH;
	int i = SIM_MAX_VEHICLES;

	while (i--)
	{
		if (pVH->mAlive == true)
		{
			pVH->mSpeedPolygonInsideFactor = false;
			get_speed(pVH);
		}
		pVH++;
	}
}

//------------------------------------------------------------------------------
void Traffic::pause()
{
	if (mTH.state == SIM_TR_STATE_PLAY)
	{
		mTH.state = SIM_TR_STATE_PAUSE;
		mTH.last_time = glfwGetTime();
		mpMN->mButtons[Menu::SIM_BTT_PAUSE].flag = true;
		mpMN->mButtons[Menu::SIM_BTT_PLAY].flag = false;
	}
	else if (mTH.state == SIM_TR_STATE_PAUSE)
	{
		mTH.last_time = glfwGetTime();
		mpMN->mButtons[Menu::SIM_BTT_PAUSE].flag = false;
		mpMN->mButtons[Menu::SIM_BTT_PLAY].flag = true;
		mTH.state = SIM_TR_STATE_PLAY;
	}
}

//------------------------------------------------------------------------------
void Traffic::alg()
{
	if (mTH.type == SIM_TR_MOVE_SIMPLE)
	{
		mTH.type = SIM_TR_MOVE_ALG;
		mpMN->mButtons[Menu::SIM_BTT_ALGORITHM].flag = true;
		mpLG->set_alg(SIM_TR_MOVE_ALG);
	}
	else if (mTH.type == SIM_TR_MOVE_ALG)
	{
		mTH.type = SIM_TR_MOVE_SIMPLE;
		mpMN->mButtons[Menu::SIM_BTT_ALGORITHM].flag = false;
		mpLG->set_alg(SIM_TR_MOVE_SIMPLE);
	}
}

//------------------------------------------------------------------------------
void Traffic::render3d()
{
	if ((mTH.render == SIM_TR_RENDER_SIMPLE) || (mTH.render == SIM_TR_RENDER_3D_MAX))
	{
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model_Tri);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * 36, MODEL_4P_3D_SLENDER_W, GL_STATIC_DRAW);
		glBindVertexArray(0);

		mTH.render = SIM_TR_RENDER_3D;
		mpMN->mButtons[Menu::SIM_BTT_3D].flag = true;
		mpMN->mButtons[Menu::SIM_BTT_3D_MAX].flag = false;
	}
	else if (mTH.render == SIM_TR_RENDER_3D)
	{
		mTH.render = SIM_TR_RENDER_SIMPLE;
		mpMN->mButtons[Menu::SIM_BTT_3D].flag = false;
		mpMN->mButtons[Menu::SIM_BTT_3D_MAX].flag = false;
	}
}

//------------------------------------------------------------------------------
void Traffic::render3d_max()
{
	if ((mTH.render == SIM_TR_RENDER_SIMPLE) || (mTH.render == SIM_TR_RENDER_3D))
	{
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model_Tri);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * SIM_MAX_3D_MODEL_POINTS, mMODEL_3DP, GL_STATIC_DRAW);
		glBindVertexArray(0);

		mTH.render = SIM_TR_RENDER_3D_MAX;
		mpMN->mButtons[Menu::SIM_BTT_3D_MAX].flag = true;
		mpMN->mButtons[Menu::SIM_BTT_3D].flag = false;
	}
	else if (mTH.render == SIM_TR_RENDER_3D_MAX)
	{
		mTH.render = SIM_TR_RENDER_SIMPLE;
		mpMN->mButtons[Menu::SIM_BTT_3D].flag = false;
		mpMN->mButtons[Menu::SIM_BTT_3D_MAX].flag = false;
	}
}

//------------------------------------------------------------------------------
void Traffic::send_cloud(bool flag)
{
	mTH.pPxy->mOutputCloud = flag;
}

//------------------------------------------------------------------------------
void Traffic::send_file(bool flag)
{
	if ((flag == false) && (mTH.pPxy->mOutputFile == true))
	{
		//closing Eurotoll Files
		mTH.pPxy->format_info_pos_eurotoll_tailer(mTH.pVH);
	}

	mTH.pPxy->mOutputFile = flag;
}

//------------------------------------------------------------------------------
void Traffic::send_info(bool active)
{
	mTH.pPxy->mSendInfo.active = active;
	mTH.pPxy->send_simulator_info((mTH.pPxy->mSendInfo.sim_detector || mTH.pPxy->mSendInfo.sim_gps));
	mTH.pPxy->send_master_trip_info(mTH.pPxy->mSendInfo.master_trip);
}

//------------------------------------------------------------------------------
void Traffic::save_test(Cadena cadfile)
{
	mpTST->save_test(cadfile);
	mTH.last_time = glfwGetTime();
}

//------------------------------------------------------------------------------
void Traffic::play_simulation(Cadena cadfile, enum sim_tr_method_play emethod)
{
	int count = 0;
	if (emethod == SIM_TR_METHOD_PLAY_FROM_SOURCETRIP)
	{
		mpTST->get_simulation_info(cadfile);
		mTH.state = Traffic::SIM_TR_STATE_PLAY;
	}
	else if (emethod == SIM_TR_METHOD_PLAY_FROMSAVEDFILE)
	{
		mpTST->set_test(cadfile);
		mTH.last_time = glfwGetTime();

		for (int i = 0; i < SIM_MAX_VEHICLES; i++)
		{
			if (mTH.pVH[i].mAlive)
			{
				setup_matrix(i, &mTH.pVH[i]);
				count++;
			}
		}
	
		mTH.vh_alive = count;
		mpMN->mButtons[Menu::SIM_BTT_PAUSE].flag = true;
		Simulator::spin_model_vh(mVhMove, mVhScale, mVhAngle, mTH.vh_alive, mModelMatrices);
		mTH.state = Traffic::SIM_TR_STATE_PAUSE;
	}
	else
	{
		cout << "Error: Thereis no method" << endl;
	}
}

//-------------------   THREAD OBTAINING VEHICLE INFORMATION --------------------
void Traffic::get_info_traffic()
{
	while (true)
	{
		switch (mTH.state)
		{
			case SIM_TR_STATE_STOP:
				Sleep(SIM_TR_STOP_TIMEOUT);
				break;

			case SIM_TR_STATE_PAUSE:
				Sleep(SIM_TR_PAUSE_TIMEOUT);
				break;

			case SIM_TR_STATE_PLAY:
				mpTST->get_new_vehicle_info();
				Sleep(200);
				break;
		}
	}//Thread
}
//------------------------------------------------------------------------------
void Traffic::render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{

	switch (mTH.state)
	{
		case SIM_TR_STATE_STOP:
			break;

		case SIM_TR_STATE_PAUSE:
			render_mv(viewmatrix, viewprojectionmatrix);
			break;

		case SIM_TR_STATE_PLAY:
			do_move();
			render_mv(viewmatrix, viewprojectionmatrix);
			break;

		default:
			break;
	}
}

//------------------------------------------------------------------------------
void Traffic::do_move()
{
	int index = 0;
	Vehiculo* pVH = mTH.pVH;
	int i = SIM_MAX_VEHICLES;
	Logic::do_move pf = mpLG->mpFunc_domove;

	while (i--)
	{
		if (pVH->mAlive == true)
		{
			//(mpLG->mpFunc_domove)(pVH, index); // NO FUNCIONA
			(mpLG->*pf)(pVH, index);
			finish_move(index, pVH);
		}
		pVH++;
	}

	mTH.vh_alive = index;
	mpMN->mButtons[Menu::SIM_BTT_PLAY].flag = (index > 0);
	Simulator::spin_model_vh(mVhMove, mVhScale, mVhAngle, mTH.vh_alive, mModelMatrices);
}


//------------------------------------------------------------------------------
void Traffic::finish_move(int& index, Vehiculo* pVH)
{
	setup_matrix(index, pVH);
	setup_tools(index, pVH);

	pVH->mDistance = pVH->mDistance + std::sqrt((pVH->mPosicion_x - pVH->mPosicion_x_old) *
		(pVH->mPosicion_x - pVH->mPosicion_x_old) +
		(pVH->mPosicion_y - pVH->mPosicion_y_old) *
		(pVH->mPosicion_y - pVH->mPosicion_y_old)
	);

	pVH->mPosicion_x_old = pVH->mPosicion_x;
	pVH->mPosicion_y_old = pVH->mPosicion_y;

	if (mpPOI->mb_node_detection)
	{
		if ((mpPOI->mNodes[pVH->mPtrNodeInfoNow->nid].num == -1) && (pVH->mNode_passed != pVH->mPtrNodeInfoNow->nid))
		{
			pVH->mNode_passed = pVH->mPtrNodeInfoNow->nid;
			pVH->mSendDetectorInfo = true;
			strcpy(pVH->mSendDetectorName, mpPOI->mNodes[pVH->mPtrNodeInfoNow->nid].name.getCadena());

			SIM_TXO_RESET;
			std::string COLOR_TX("\033[0;30;46m");
			cout << COLOR_TX <<  "==>Detection: Vehicle:[" << pVH->mIndexGral << "] ==> Detector:[" << mpPOI->mNodes[pVH->mPtrNodeInfoNow->nid].name.getCadena() << "]" << SIM_RESET << endl;

		}
	}

	if (mTH.pPxy->mSendInfo.active) mTH.pPxy->send_info(pVH);
	pVH->mSendDetectorInfo = false;

	//The following vehicle
	index++;

}

//------------------------------------------------------------------------------
void Traffic::vehicle_out(Vehiculo* pVH)
{
	double time_diff;

	mpTST->log_veh_out(pVH);

	pVH->mTimeFin = glfwGetTime();
	time_diff = pVH->mTimeFin - pVH->mTimeIni;
	if (mTH.pPxy->mOutputFile == true) { mTH.pPxy->finalize_file(pVH); }

	//-----Alg overpassing stuff-------------------------------------------------
	if (mTH.type == SIM_TR_MOVE_ALG)
	{
		if (pVH->mVhFollowing != -1) mTH.pVH[pVH->mVhFollowing].mVhBehind = -1;
		if (pVH->mVhBehind != -1)    mTH.pVH[pVH->mVhBehind].mVhFollowing = -1;
		mTH.pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_cant--;
		if (mTH.pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_cant == 0)
		{
			mTH.pNO[(pVH->mPtrNodeInfoNow)->node_vh_order].laneorder[pVH->mLaneNow].m_vh_first = -1;
		}
	}

	//---------------------------------------------------------------------------
	pVH->mState = SIM_TR_ALG_NOT_ALIVE;
	pVH->mAlive = false;
	mTH.vh_alive_real_time--;

}

//------------------------------------------------------------------------------
void Traffic::setup_matrix(int index, Vehiculo* pVH)
{
	int i = 3 * index;
	int j = 4 * index;
	//------------------MOVE Matrix---------------------------------
	mVhMove[i + 0] = pVH->mPosicion_x;
	mVhMove[i + 1] = pVH->mPosicion_y;
	mVhMove[i + 2] = pVH->mPosicion_z;

	//------------------COLOUR Matrix-------------------------------
	
	if (!mNightColor)
	{
		mVhColor[j + 0] = pVH->mColor.r;
		mVhColor[j + 1] = pVH->mColor.g;
		mVhColor[j + 2] = pVH->mColor.b;
		mVhColor[j + 3] = pVH->mColor.f;
	}
	else
	{
		mVhColor[j + 0] = 245 + rand() % 10;
		mVhColor[j + 1] = 245 + rand() % 10;
		mVhColor[j + 2] = 235;
		mVhColor[j + 3] = pVH->mColor.f;
	}


	//------------------SCALE ----------------------------------------
	mVhScale[index] = pVH->mSizeCatg;

	//------------------ANGLE ----------------------------------------
	mVhAngle[index] = float(pVH->mPtrNodeInfoNow->angle);
}

//------------------------------------------------------------------------------
void Traffic::setup_tools(int index, Vehiculo* pVH)
{
	//---- Tools simulation ----
	if (mpTLS->exist_any_polygon())
	{
		if (mpTLS->exist_type_polygon(Tools::SIM_POLYGON_TOLL))
		{
			if (mpTLS->inside_polygon(Tools::SIM_POLYGON_TOLL, pVH->mPosicion_x, pVH->mPosicion_y))
			{
				mVhColor[4 * index + 0] = 100;
				mVhColor[4 * index + 1] = 88;
				mVhColor[4 * index + 2] = 88;
				mpTLS->calculate_payment(pVH->mPosicion_x, pVH->mPosicion_x_old, pVH->mPosicion_y, pVH->mPosicion_y_old);
			}
		}
		if (mpTLS->exist_type_polygon(Tools::SIM_POLYGON_SPEED))
		{
			if (mpTLS->inside_polygon(Tools::SIM_POLYGON_SPEED, pVH->mPosicion_x, pVH->mPosicion_y))
			{
				mVhColor[4 * index + 0] = 88;
				mVhColor[4 * index + 1] = 100;
				mVhColor[4 * index + 2] = 88;

				pVH->mSpeedPolygonInsideFactor = true;
			}
			else
			{
				pVH->mSpeedPolygonInsideFactor = false;
			}
		}
		if (mpTLS->exist_type_polygon(Tools::SIM_POLYGON_AVOID_GPS))
		{
			if (mpTLS->inside_polygon(Tools::SIM_POLYGON_AVOID_GPS, pVH->mPosicion_x, pVH->mPosicion_y))
			{
				mVhColor[4 * index + 0] = 88;
				mVhColor[4 * index + 1] = 22;
				mVhColor[4 * index + 2] = 88;
			}
		}
		if (mpTLS->exist_type_polygon(Tools::SIM_POLYGON_CLEAN))
		{
			if (mpTLS->inside_polygon(Tools::SIM_POLYGON_CLEAN, pVH->mPosicion_x, pVH->mPosicion_y))
			{
				pVH->mState = SIM_TR_ALG_EXIT;
			}
		}
	}
}

//------------------------------------------------------------------------------
void Traffic::get_speed(Vehiculo* pVH)
{
	pVH->mSpeedHwTypeFactor = Simulator::get_speed_factor_from_hwtype(pVH->mPtrNodeInfoNow->hwtype);
	pVH->mSpeedNow = pVH->mSpeedAvrg * pVH->mSpeedHwTypeFactor;

	if (pVH->mSpeedPolygonInsideFactor)
	{
		pVH->mSpeedNow = pVH->mSpeedNow * double(mpTLS->mSpeed) / 100.0;
	}
	if (pVH->mSpeedBrakeFactor)
	{
		pVH->mSpeedNow = pVH->mSpeedBrakeObstacle;//(pVH->mSpeedNow + pVH->mSpeedBrakeObstacle) / 2.0;
	}

}

//=============================================================================================================================
//                                                   RENDER
//=============================================================================================================================
//------------------------------------------------------------------------------
void Traffic::render_mv(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix)
{
	try
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(GLubyte) * mTH.vh_alive, mVhColor);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Scale);
		glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLdouble) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 1 * sizeof(GLdouble) * mTH.vh_alive, mVhScale);

		glBindBuffer(GL_ARRAY_BUFFER, mVBO_Tranformation);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * mTH.vh_alive, &mModelMatrices[0]);

		mShader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTextureImg_car);
		glUniform1i(mTextureID_car, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mTextureImg_truck);
		glUniform1i(mTextureID_truck, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mTextureImg_motorcycle);
		glUniform1i(mTextureID_motorcycle, 2);

		glUniformMatrix4fv(mVP, 1, GL_FALSE, &viewprojectionmatrix[0][0]);

		glBindVertexArray(mVAO);

		if (mTH.render == SIM_TR_RENDER_SIMPLE) //(QUITAR solo una vez)
		{
			glUniform1f(mPW, 0.6 * mVHSize);
			glUniform1i(mMODEL_3D, false);
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mTH.vh_alive);
		}
		else
		{

			if (mTH.render == SIM_TR_RENDER_3D)
			{
				glUniform1f(mPW, 0.4 * mVHSize);
				glUniform1i(mMODEL_3D, true);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 36, mTH.vh_alive);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
			{
				glUniform1f(mPW, 0.2 * mVHSize);
				glUniform1i(mMODEL_3D, true);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, mModelCount, mTH.vh_alive);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

		}


		glBindVertexArray(0);
	}
	catch (...)
	{
		cout << "Error de Traffic::render " << endl;
	}
}

//------------------------------------------------------------------------------
void Traffic::setup_render()
{

	mVhMove  = new GLdouble[SIM_MAX_VEHICLES * 3];
	mVhColor = new GLubyte[SIM_MAX_VEHICLES * 4];
	mVhScale = new GLdouble[SIM_MAX_VEHICLES * 1];
	mVhAngle = new GLdouble[SIM_MAX_VEHICLES * 1];

	memset(mVhMove,  0, SIM_MAX_VEHICLES * 3 * sizeof(double));
	memset(mVhColor, 0, SIM_MAX_VEHICLES * 4 * sizeof(GLubyte));
	memset(mVhScale, 0, SIM_MAX_VEHICLES * 1 * sizeof(double));
	memset(mVhAngle, 0, SIM_MAX_VEHICLES * 1 * sizeof(double));

	mShader.inicializar(SIM_DIR_SHADERS_"traffic.vs", SIM_DIR_SHADERS_"traffic.frag");
	//mTextureImg = loadDDS(DIR_SHADERS_"particle.DDS");

	mTextureID_car = glGetUniformLocation(mShader.mProgram, "Texture_car");
	mTextureID_truck = glGetUniformLocation(mShader.mProgram, "Texture_truck");
	mTextureID_motorcycle = glGetUniformLocation(mShader.mProgram, "Texture_motorcycle");

	Simulator::texture_RGBA(mTextureImg_car, "vehicles/car_day.png");
	Simulator::texture_RGBA(mTextureImg_truck, "vehicles/truck.png");
	Simulator::texture_RGBA(mTextureImg_motorcycle, "vehicles/motorcycle.png");

	mVP = glGetUniformLocation(mShader.mProgram, "VP");
	mPW = glGetUniformLocation(mShader.mProgram, "PW");
	mMODEL_3D = glGetUniformLocation(mShader.mProgram, "MODEL_3D");
	mTxIndex = glGetUniformLocation(mShader.mProgram, "txt_index");

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO_Model);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MODEL_4P_SLENDER_W), MODEL_4P_SLENDER_W, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO_Model_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model_Tri);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3) * 36, MODEL_4P_3D_SLENDER_W, GL_STATIC_DRAW);
	

	glGenBuffers(1, &mVBO_Color);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLubyte) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &mVBO_Scale);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Scale);
	glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(GLdouble) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &mVBO_Tranformation);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Tranformation);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLdouble) * SIM_MAX_VEHICLES, NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Model_Tri);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Color);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLvoid*)0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Scale);
	glVertexAttribPointer(3, 1, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO_Tranformation);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glBindVertexArray(0);

	mModelMatrices = new glm::mat4[SIM_MAX_VEHICLES];

}

//------------------------------------------------------------------------------
void Traffic::set_vh_size(double size)
{
	mVHSize = size;
}

//------------------------------------------------------------------------------
void Traffic::set_night()
{
	Simulator::texture_RGBA(mTextureImg_car, "vehicles/car_night.png");
}
//------------------------------------------------------------------------------
void Traffic::set_day()
{
	Simulator::texture_RGBA(mTextureImg_car, "vehicles/car_day.png");
}

