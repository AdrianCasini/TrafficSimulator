#ifndef _TEXTER_H
#define _TEXTER_H

#include "Simulator.h"
#include "Controls.h"
#include "Tarif.h"
#include "Tools.h"
#include "FileInfo.h"

#include <clocale>
#include <cctype>
#include <io.h>
#include <fcntl.h>
class Texter
{
	public:
		
		struct MasterText
		{
			double* mMove;
			double* mScale;
			double* mScale_w;
			double* mScale_h;
			double* mAngle;
			int*    mTexture;
			double* mColor;
			glm::mat4* mModelMatrices;
			int count;
			int mNumberText;

			GLuint mVAO_plus;
			GLuint mVBO_plus;
			GLuint mVBO_model_plus;
			GLuint mVBO_Scale_plus;
			GLuint mVBO_Scale_w_plus;
			GLuint mVBO_Scale_h_plus;
			GLuint mVBO_Text_plus;
			GLuint mVBO_Color;
			GLuint mVBO_Tranformation_plus;

		};

		MasterText mMasterText[5];
		GLuint mText;
		GLuint mArrayLetter[32];
		double mModel[6 * 100];
		int mNumSegmentsTexture32;
		int mNumTypeOfPlace;

		Shader mShader_plus;
		std::map<unsigned char, Character> mCharacters_plus;
		
		//---------------------------
		GLuint    mArray_VBO[SIM_MAX_TEXTER];
		Type_Text* mArrayType; 
		Node_Text* mArrayTemp; 
		map<int, Cadena> mMapCuads;

		Controls*   mCtrl;
		Tarif*      mTRF;
		Tools*     mTLS;

		GLuint mVAO;
		GLuint mVBO;
		Shader mShader;
		glm::mat4 mProjectionText;

		FT_Outline ss;
		bool mActive;

		char mTitle[50];
		glm::vec3 mColorTitle;
		glm::vec3 mColorEU;
		glm::vec3 mColorKM;

		FileInfo mLabels;
		FileInfo mPlaces;

		Cadena mcadgral;

	
	public:
		//----------------------------------------
		Texter();
		~Texter();

		void init(Controls*, Tarif *, Tools* tls, Cadena cadgral);
		void init_render();
		void get_quads();
		void get_info(int , int , int i);
		void get_each_file(int, int);

		void get_labels();
		void get_places_properties();
		void clean_temp();

		void active();
		bool is_active();

		void render(glm::mat4& viewprojectionmatrix, int screenW, int screenH);
		void render_titles(int screenW, int screenH);
		void render_toller_payment(int screenW, int screenH);
		void render_toller_payment_sp(int screenW, int screenH);
		void render_text_massive(glm::mat4& viewprojectionmatrix, int screenW, int screenH);
		void render_text(string text, GLfloat x, GLfloat y, GLfloat scale);

		void refresh(int screenW, int screenH);


		void info_matrix_plus();
		void get_text_plus();
		

};

#endif

