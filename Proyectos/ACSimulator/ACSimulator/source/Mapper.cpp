#include "Mapper.h"


//------------------------------------------------------------------------------
Mapper::Mapper()
{

}

//------------------------------------------------------------------------------
Mapper::~Mapper()
{
	
}

//------------------------------------------------------------------------------
void Mapper::init(Controls* ctrl, MapItem* mp, Cadena caddirgral)
{
	mCtrl = ctrl;
	mDirGral = caddirgral;

	if (mp->type == "line")
	{
		mType = GL_LINES;
	}
	else
	{
		mType = GL_TRIANGLES;
	}
	mFilename    = mp->file;
	mVectColor.r = mp->clr_r;
	mVectColor.g = mp->clr_g;
	mVectColor.b = mp->clr_b;
	mSQsize      = mp->quad_sz;
	mPosZ        = mp->distance;
	mTexture     = mp->texture;

	mPosZ_BTT    = mCtrl->mPosition.z;


	mQuad_oldX = -100000;
	mQuad_oldY = -100000;

	mMAX_POINTS = 200000;

	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		mArray_point[i] = 0;
		mArray_buff[i] = NULL;
		mArray_size[i] = 0;
	}

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		glGenBuffers(1, &mArray_VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
	}
	glBindVertexArray(0);

	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		mQuads[i] = "";
	}
	mMapQuads_file_new.clear();
    mMapQuads_file_old.clear();


	//cout << "inicio mapper" << endl;
	mMutex = CreateMutex(NULL, FALSE, NULL);
	if (mMutex == NULL) { cout << "CreateMutex error" << endl; }

}

//------------------------------------------------------------------------------
void Mapper::render(glm::mat4& viewprojectionmatrix, bool flag)
{
	mPosZ_BTT = mCtrl->mPosition.z;
	if (flag)
	{
		mPosZ_BTT = 200;
	}

	if (mPosZ_BTT < mPosZ)
	{
		glBindVertexArray(mVAO);
		for (int i = 0; i < SIM_MAX_MAPPER; i++)
		{

			if ((mArray_point[i] != 0) && (mArray_buff[i] != NULL))
			{

				//if (mArray_point[i] < mMAX_POINTS)
				//{

					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
					glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
					glVertexAttribDivisor(0, 0);
					glDrawArrays(mType, 0, mArray_point[i]);
				//}

	
			}

		}
		glBindVertexArray(0);
	}
}

//------------------------------------------------------------------------------
void Mapper::need_new_quad()
{
	if (mCtrl->mPosition.z < mPosZ)
	{
		int quad_center_x = int(mCtrl->mPosition.x / (mSQsize)); if (mCtrl->mPosition.x >= 0) { quad_center_x = quad_center_x + 1; }
		int quad_center_y = int(mCtrl->mPosition.y / (mSQsize)); if (mCtrl->mPosition.y <= 0) { quad_center_y = quad_center_y - 1; }


		if (mQuad_oldX != quad_center_x)
		{
			getInfo(quad_center_x, quad_center_y);
			mQuad_oldX = quad_center_x;
		}
		else if (mQuad_oldY != quad_center_y)
		{
			getInfo(quad_center_x, quad_center_y);
			mQuad_oldY = quad_center_y;
		}
	}
}

//------------------------------------------------------------------------------
void Mapper::getInfo(int quadCX, int quadCY)
{
	Cadena cadpath;
	int quadx, quady, count = 0;
	Cadena new_files[SIM_MAX_MAPPER];

	for (int i = 0; i < SIM_MAX_MAPPER_X; i++)
	{
		int x = 1 + i - int(SIM_MAX_MAPPER_X / 2);

		for (int j = 0; j < SIM_MAX_MAPPER_Y; j++)
		{
			int y =   1 + j - int(SIM_MAX_MAPPER_Y / 2);
			quadx = quadCX * mSQsize - x * mSQsize;
			quady = quadCY * mSQsize + y * mSQsize;

			cadpath.formCadena(SIM_DIR_ETC_BINES_"%s/%s_%d_%d.bin", mFilename.getCadena(), mFilename.getCadena(), quadx, quady);
			cadpath = mDirGral + cadpath;

			mMapQuads_file_new[cadpath] = 9999;
			if (mMapQuads_file_old[cadpath] != 9999)
			{
				new_files[count] = cadpath;
				count++;
			}
		}
	}

	mMapQuads_file_old.clear();
	mMapQuads_file_old = mMapQuads_file_new;

	count = 0;
	for (int i = 0; i < SIM_MAX_MAPPER; i++)
	{
		if (mMapQuads_file_new[mQuads[i]] != 9999 )
		{
			//Pick one file brand new
			mQuads[i] = new_files[count];
			count++;
			getInfo_n_array(i, mQuads[i]);

			if ((mArray_point[i] != 0) && (mArray_buff[i] != NULL))
			{
				try
				{
					glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
					glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(double) * mArray_point[i], mArray_buff[i], GL_STREAM_DRAW);
				}
				catch (const std::exception& ex)
				{
					cout << "Error de Mapper::getInfo" << endl;
				}
			}
		}
	}

	mMapQuads_file_new.clear();

}

//------------------------------------------------------------------------------
void Mapper::getInfo_n_array(int indx, Cadena cadpath)
{
	int lSize;
	int cantidad;
	int result;
	int resultparcial = 0;
	cantidad = 3 * 10000;
	DWORD dwWaitResult;

	delete[] mArray_buff[indx];
	mArray_buff[indx] = NULL;
	mArray_point[indx] = 0;
	mArray_size[indx] = 0;

	mFichero = fopen(cadpath.getCadena(), "rb");
	if (mFichero == NULL)
	{
		//std::cout << "Error Fichero: " << cadpath.getCadena() <<  endl;
		return;
	}
	else
	{
		fseek(mFichero, 0, SEEK_END);
		lSize = ftell(mFichero);
		rewind(mFichero);

		if (lSize != 0)
		{
			try
			{
				mArray_size[indx] = lSize;
				mArray_buff[indx] = new double[(mArray_size[indx] / sizeof(double))];
			}
			catch (...)
			{
				std::cout << "Error obteniendo memoria 1" << endl;
				delete[] mArray_buff[indx];
				mArray_buff[indx] = NULL;
			}

			if (mArray_buff[indx] == NULL)
			{
				std::cout << "Error obteniendo memoria 2" << endl;
				mArray_point[indx] = 0;
			}
			else
			{

				resultparcial = fread(mArray_buff[indx], sizeof(double), cantidad, mFichero);
				result = resultparcial;
				fseek(mFichero, result * sizeof(double), SEEK_SET);

				do
				{
					if (((lSize / sizeof(double)) - result) <= cantidad)
					{
						cantidad = ((lSize / sizeof(double)) - result);
					}

					resultparcial = fread(mArray_buff[indx] + result, sizeof(double), cantidad, mFichero);

					result = resultparcial + result;
					fseek(mFichero, result * sizeof(double), SEEK_SET);

				} while ((result * sizeof(double)) < lSize);

				fclose(mFichero);

				if ((result * sizeof(double)) == lSize)
				{
					mArray_point[indx] = int(result / 3);
				}
				else
				{
					mArray_point[indx] = 0;
				}
			}
		}
		else
		{
			fclose(mFichero);
			return;
		}
	}
}

