#ifndef _POFI_H
#define _POFI_H

#include "Simulator.h"
#include "Controls.h"
#include "FileInfo.h"


class PofI
{
	public:

		BaseDatos* mpDB;
		Controls* mCtrl;

		glm::vec4 mColor;

		int       mItemIndex;
		double*   mbuff_item_draw;
		GLuint    mVAO_item;
		GLuint    mVBO_item_model;
		GLuint    mVBO_item_offset;

		GLuint    mTextID_p;
		GLuint    mTextImg_p;
		GLuint    mCameraR_p;
		GLuint    mCameraU_p;
		GLuint    mVP_p;
		GLdouble  mSize_p;
		GLuint    mColor_p;

		Shader    mShader_point;

		int mScreenW;
		int mScreenH;

		Cadena mcadgral;

		FileInfo   mItems;

		bool mb_node_detection;

		struct Nodedetector
		{
			int num;
			Cadena name;
		};

		map<long long int, Nodedetector> mNodes;

	public:
		//----------------------------------------
		PofI();
		~PofI();
		
		void init(Controls* ctrl, BaseDatos* db, Cadena cadgral);
		void render_setup();
		void generate_draw(Cadena file_name);
		void refresh();
		bool button_active();
		bool button_not_active();
		void clear();
		bool is_active();
		double get_point_size();
		void render(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);
		void render_point(glm::mat4& viewmatrix, glm::mat4& viewprojectionmatrix);


};

#endif

