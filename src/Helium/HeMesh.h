#pragma once

#include "HeliumCommon.h"
#include "HeShader.h"

namespace ArtificialNature {

    struct HeVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct HeTexture
    {
        unsigned int id;
        string type;
        string path;
    };

    class HeMesh
    {
    public:
        vector<HeVertex> vertices;
        vector<unsigned int> indices;
        vector<HeTexture> textures;
        unsigned int VAO;

        HeMesh(vector<HeVertex> vertices, vector<unsigned int> indices, vector<HeTexture> textures);

        void Draw(HeShader& shader);

    private:
        unsigned int VBO, EBO;

        void setupMesh();
    };

}
