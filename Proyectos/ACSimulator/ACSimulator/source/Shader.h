#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
	public:
	GLuint mProgram;

	public:

		Shader();
		~Shader();
		
		void inicializar(const GLchar* vertexPath, const GLchar* fragmentPath);
		void use();

};

#endif
