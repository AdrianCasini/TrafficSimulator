
#ifndef _MODEL_H
#define _MODEL_H

#include "Simulator.h"
#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

GLint TextureFromFile(const char* path, string directory);

class Model
{

    public:

        Model();
        Model(const GLchar* path);
        ~Model();

        void loadModel(const string path);
        void Draw(Shader shader);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        void processNode(aiNode* node, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
        GLint TextureFromFile(const char* path, string directory);

    //private:
    public:
        /*  Model Data  */
        vector<Mesh> meshes;
        string directory;
        vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

};





#endif