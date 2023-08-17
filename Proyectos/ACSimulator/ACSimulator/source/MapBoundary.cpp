#include "MapBoundary.h"


//------------------------------------------------------------------------------
MapBoundary::MapBoundary()
{

}

//------------------------------------------------------------------------------
MapBoundary::~MapBoundary()
{
	
}

//------------------------------------------------------------------------------
void MapBoundary::init(Controls* ctrl, MapItem* mp, Cadena caddirgral)
{
	mCtrl = ctrl;
	mDirGral = caddirgral;
	mType = GL_TRIANGLES;
	
	mFilename    = mp->file;
	mVectColorDay.r = mp->clr_r;
	mVectColorDay.g = mp->clr_g;
	mVectColorDay.b = mp->clr_b;
	set_color_day();

	mSQsize      = mp->quad_sz;
	mPosZ        = mp->distance;
	mTexture     = mp->texture;

	mPosZ_BTT    = mCtrl->mPosition.z;

	for (int i = 0; i < SIM_MAX_BOUNDARY_MAPPER; i++)
	{
		mArray_point[i] = 0;
		mArray_buff[i] = NULL;
		mArray_size[i] = 0;
	}

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	for (int i = 0; i < SIM_MAX_BOUNDARY_MAPPER; i++)
	{
		glGenBuffers(1, &mArray_VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
	}
	glBindVertexArray(0);

	mMutex = CreateMutex(NULL, FALSE, NULL);
	if (mMutex == NULL) { cout << "CreateMutex error" << endl; }

	mb_only_once = true;
	mmax_files = 0;


}

//------------------------------------------------------------------------------
void MapBoundary::set_color_day()
{
	mVectColor.r = mVectColorDay.r;
	mVectColor.g = mVectColorDay.g;
	mVectColor.b = mVectColorDay.b;
}


//------------------------------------------------------------------------------
void MapBoundary::set_color_night()
{
	mVectColor.r = mVectColorNight.r;
	mVectColor.g = mVectColorNight.g;
	mVectColor.b = mVectColorNight.b;
}

//------------------------------------------------------------------------------
void MapBoundary::render(glm::mat4& viewprojectionmatrix, bool flag)
{

	glBindVertexArray(mVAO);
	for (int i = 0; i < mmax_files; i++)
	{
		if ((mArray_point[i] != 0) && (mArray_buff[i] != NULL))
		{

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, mArray_VBO[i]);
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
			glVertexAttribDivisor(0, 0);
			glDrawArrays(mType, 0, mArray_point[i]);
		}
	}
	glBindVertexArray(0);
	
}

//------------------------------------------------------------------------------
void MapBoundary::need_new_quad()
{
	if (mb_only_once)
	{
		getInfo();
		mb_only_once = false;

		for (int i = 0; i < mmax_files; i++)
		{
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
}

//------------------------------------------------------------------------------
void MapBoundary::getInfo()
{
	Cadena cadpath;

	for (int i = 0; i < SIM_MAX_BOUNDARY_MAPPER; i++)
	{
		cadpath.formCadena("%s%sboundary/boundary_%d.bin", mDirGral.getCadena(), SIM_DIR_ETC_BINES_, i);
		mmax_files++;
		if (getInfo_n_array(i, cadpath) == -1) 	{break;}
	}
}

//------------------------------------------------------------------------------
int MapBoundary::getInfo_n_array(int indx, Cadena cadpath)
{
	int lSize;
	int cantidad;
	int result;
	int resultparcial = 0;
	cantidad = 3 * 10000;
	DWORD dwWaitResult;

	delete mArray_buff[indx];
	mArray_buff[indx] = NULL;
	mArray_point[indx] = 0;
	mArray_size[indx] = 0;

	mFichero = fopen(cadpath.getCadena(), "rb");
	if (mFichero == NULL)
	{
		//std::cout << "Error Fichero: " << cadpath.getCadena() <<  endl;
		return -1;
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
				delete mArray_buff[indx];
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
			return 1;
		}
	}

	return 0;
}

