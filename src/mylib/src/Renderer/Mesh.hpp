#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include <assimp/scene.h>

namespace ENGINE {

    struct Vertex {

        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

    };

    struct Texture {

        unsigned int ID;
        std::string Type;
        aiString Path;

    };

    class Mesh {

    public:
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;
    public:
        Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures );
        void Draw( Shader shader );

    private:
        void Init();
        unsigned int m_VAO, m_VBO, m_EBO;


    };

}
