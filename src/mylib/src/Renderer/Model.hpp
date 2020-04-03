#pragma once

#include "Shader.hpp"
#include "Mesh.hpp"
#include <assimp/scene.h>
#include <string>
#include <vector>
#include <memory>

namespace ENGINE {

    class Model {

    public:
        Model( std::string path );
        void Draw( Shader shader );
        static std::shared_ptr<Model> Create( const char* path );

    private:
        void LoadModel( std::string path );
        void ProcessNode( aiNode* node, const aiScene* scene );
        Mesh ProcessMesh( aiMesh* mesh, const aiScene* scene );
        std::vector<Texture> LoadMaterialTextures( aiMaterial *mat, aiTextureType type, std::string typeName );
        unsigned int TextureFromFile( const char *path, const std::string directory );
    private:
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        std::vector<Texture> textures_loaded;
    };
}
