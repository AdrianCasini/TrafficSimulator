
#ifndef _MESH_H
#define _MESH_H

#include "Simulator.h"

#include <clocale>
#include <cctype>
#include <io.h>
#include <fcntl.h>

#pragma once

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture 
{
    GLuint id;
    string type;
    string path;
};

class Mesh 
{
    public:
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;

        GLuint VAO, VBO, EBO;


        Mesh();
        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
        ~Mesh();

        void Draw(Shader shader);
        void setupMesh();

};

#endif
