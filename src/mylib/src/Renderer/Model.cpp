#include "Model.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include "stb_image.h"

using namespace ENGINE;

Model::Model( std::string path ) {

    LoadModel( path );

}

void Model::LoadModel( std::string path ) {

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs );

    if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {

        std::cout << "ERROR::ASSIMP: " << importer.GetErrorString() << std::endl;
 
        return;

    }

    m_Directory = path.substr( 0, path.find_last_of( '/' ) );
    
    ProcessNode( scene->mRootNode, scene );

}


void Model::Draw( Shader shader ) {

    for( unsigned int i = 0; i < m_Meshes.size(); i++ )
        m_Meshes[i].Draw( shader );

}

std::shared_ptr<Model> Model::Create( const char* path ) {
    
    return std::make_shared<Model>( path );
    
}


void Model::ProcessNode( aiNode* node, const aiScene* scene ) {
    
    aiMesh* mesh = scene->mMeshes[0];

    for( unsigned int i = 0; i < scene->mNumMeshes; i++ ) {

        mesh = scene->mMeshes[i];
        //mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back( ProcessMesh( mesh, scene ) );

    }

    for( unsigned int i = 0; i < node->mNumChildren; i++ ) {

        ProcessNode( node->mChildren[i], scene );

    }
}

Mesh Model::ProcessMesh( aiMesh* mesh, const aiScene* scene ) {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for( unsigned int i = 0; i < mesh->mNumVertices; i++ ) {

        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if( mesh->mTextureCoords[0] ) {

            glm::vec2 texCoords;
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = texCoords;

        } else {

            vertex.TexCoords = glm::vec2( 0.0f );

        }


        vertices.push_back( vertex );

    }

    for( unsigned int i = 0; i < mesh->mNumFaces; i++ ) {

        aiFace face = mesh->mFaces[i];

        for( unsigned int j = 0; j < face.mNumIndices; j++ )
            indices.push_back(face.mIndices[j]);

    }

    if( mesh->mMaterialIndex >= 0 ) {

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    }

    return Mesh( vertices, indices, textures );

}

std::vector<Texture> Model::LoadMaterialTextures( aiMaterial* mat, aiTextureType type, std::string typeName ) {

    std::vector<Texture> textures;
    for( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ ) {

        aiString str;
        mat->GetTexture( type, i, &str );
        //Texture texture;
        //texture.ID = TextureFromFile( str.C_Str(), m_Directory );
        //texture.Type = type;
        //texture.Path = str;

        bool skip = false;
        // for every loaded texture
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {

          // if the texture path = to str then push it to textures
          if( textures_loaded[j].Path == str ) {

            textures.push_back( textures_loaded[j] );
            skip = true;

            break;
          }
        
            
        }

        if( !skip ) {

          // If texture hasn’t been loaded already, load it
          Texture texture;
          texture.ID = TextureFromFile( str.C_Str(), this->m_Directory );
          texture.Type = typeName;
          texture.Path = str;
          textures.push_back( texture );
          // add to loaded textures
          this->textures_loaded.push_back( texture );
        
        }
        
    }
    
    return textures;

}

unsigned int Model::TextureFromFile( const char *path, const std::string directory ) {

    //Generate texture ID and load texture data
    std::string filename = std::string( path );
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures( 1, &textureID );

    int width, height, channels;

    stbi_set_flip_vertically_on_load( 1 );

    stbi_uc* data = stbi_load( filename.c_str(), &width, &height, &channels, 0 );
    // Assign texture to ID
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    stbi_image_free( data );
    
    return textureID;

}
