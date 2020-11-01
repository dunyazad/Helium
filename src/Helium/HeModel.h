#pragma once

#include "HeliumCommon.h"

#include "HeMesh.h"
#include "HeShader.h"

#include <assimp/scene.h>

namespace ArtificialNature {

    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

    class HeModel
    {
    public:
        vector<HeTexture> textures_loaded;
        vector<HeMesh>    meshes;
        string directory;
        bool gammaCorrection;

        HeModel(string const& path, bool gamma = false);

        void HeDraw(HeShader& shader);

    private:
        void loadModel(string const& path);
        void processNode(aiNode* node, const aiScene* scene);
        HeMesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<HeTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };

}
