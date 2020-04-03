#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "glad/glad.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

#include "CMesh.h"
#include "Shader.hpp"

namespace ENGINE {

unsigned int TextureFromFile( const char *path, const std::string directory );

class Model {

    private:

        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;
        
        void loadModel( std::string path ) {

            Assimp::Importer importer;
            /*
            By setting aiProcess_Triangulate we tell Assimp that if the model does not (entirely) consist of triangles
            it should transform all the model’s primitive shapes to triangles. The aiPorcess_FlipUVs flips the texture
            coordinates on the y-axis where necessary during processing.
            */
            const aiScene *scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
            // After we load the model, we check if the scene and the root node of the scene are not null and check one of its flags to see if the returned data is incomplete
            if( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
                
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;

                return;

            }
            // We also retrieve the directory path of the given file path.
            this->directory = path.substr( 0, path.find_last_of( '/' ) );
            /*
            we want to process al of the scene’s nodes so we pass the first node (root node) to the recursive processNode function. Because each node (possibly) contains a
            set of children we want to first process the node in question, and then continue processing all the node’s children and so on.
            */
            this->processNode( scene->mRootNode, scene );
            

        }

        void processNode( aiNode *node, const aiScene *scene ) {

            // Process all the node’s meshes (if any)
            for( unsigned int i = 0; i < node->mNumMeshes; i++ ) {

                // We first check each of the node’s mesh indices and retrieve the corresponding mesh by indexing the scene’s mMeshes array                
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                // he returned mesh is then passed to the processMesh function that returns a Mesh object that we can store in the meshes list/vector
                this->meshes.push_back( this->processMesh( mesh, scene ) );

            }

            /*
            Once all the meshes have been processed we iterate through all of the node’s children and call the same processNode function for each of
            the node’s children. Once a node no longer has any children the function stops executing.
            */
            for( unsigned int i = 0; i < node->mNumChildren; i++ ) {
                
                this->processNode( node->mChildren[i], scene );

            }
            
            

        }
        Mesh processMesh( aiMesh *mesh, const aiScene *scene ) {

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indecies;
            std::vector<Texture> textures;

            for( unsigned int i = 0; i < mesh->mNumVertices; i++ ) {
                
                Vertex vertex;
                glm::vec3 vector;
                /*
                Retrieving the vertex data is pretty simple: we define a Vertex struct that we add to the
                vertices array after each iteration. We loop for as much vertices there exist within the mesh
                (retrieved via mesh->mNumVertices). Within the iteration we then want to fill this struct with all the relevant data.
                */
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;

                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;

                if( mesh->mTextureCoords[0] ) {

                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;

                } else {

                    vertex.TexCoords = glm::vec2( 0.0f, 0.0f );

                }

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;

                /*
                The vertex struct is now completely filled with the required vertex attributes and we can push it to the back 
                of the vertices vector at the end of the iteration. This process is repeated for each of the mesh’s vertices
                */
                vertices.push_back( vertex );

            }

            for( unsigned int i = 0; i < mesh->mNumFaces; i++ ) {
                
                aiFace face = mesh->mFaces[i];

                for( unsigned int j = 0; j < face.mNumIndices; j++ ) {

                    indecies.push_back( face.mIndices[j] );

                }

            }
            

            if( mesh->mMaterialIndex >= 0 ) {
                /*
                We first retrieve the aiMaterial object from the scene’s mMaterials array. Then we
                want to load the mesh’s diffuse and/or specular textures. A material object internally stores an
                array of texture locations for each texture type. The different texture types are all prefixed with
                aiTextureType_. We use a helper function called loadMaterialTextures to retrieve
                the textures from the material. The function returns a vector of Texture structs that we then store
                at the end of the model’s textures vector.
                */
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                std::vector<Texture> diffuseMaps = this->loadMaterialTextures( material, aiTextureType_DIFFUSE, "texture_diffuse" );
                textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );

                std::vector<Texture> specularMaps = this->loadMaterialTextures( material, aiTextureType_SPECULAR, "texture_specular" );
                textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );
                
                /*
                 load normal maps from a textured model. The wavefront object format (.obj) exports normal maps slightly different 
                as Assimp’s aiTextureType_NORMAL doesn’t load its normal maps while aiTextureType_HEIGHT does
                */
                std::vector<Texture> normalMaps = this->loadMaterialTextures( material, aiTextureType_HEIGHT, "texture_normal" );
                textures.insert( textures.end(), normalMaps.begin(), normalMaps.end() );
                
                // 4. height maps
                std::vector<Texture> heightMaps = loadMaterialTextures( material, aiTextureType_AMBIENT, "texture_height" );
                textures.insert( textures.end(), heightMaps.begin(), heightMaps.end() );
        

            }

            return Mesh( vertices, indecies, textures );
            

        }
        /*
        the loadMaterialTextures function iterates over all the texture locations of the given
        texture type, retrieves the texture’s file location and then loads and generates the texture and stores
        the information in a Vertex struct
        */
        std::vector<Texture> loadMaterialTextures( aiMaterial *mat, aiTextureType type, std::string typeName ) {

            std::vector<Texture> textures;
            for( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ ) {
                
                aiString str;
                // get texture
                mat->GetTexture( type, i, &str );

                bool skip = false;

                // for every loaded texture
                for( unsigned int j = 0; j < textures_loaded.size(); j++ ) {
                    
                    // if the texture path = to str then push it to textures
                    if( textures_loaded[j].path == str ) {

                        textures.push_back( textures_loaded[j] );
                        skip = true;

                        break;

                    }

                }

                if( !skip ) {

                    // If texture hasn’t been loaded already, load it
                    Texture texture;
                    texture.id = TextureFromFile( str.C_Str(), this->directory );
                    texture.type = typeName;
                    texture.path = str;
                    textures.push_back( texture );
                    // add to loaded textures
                    this->textures_loaded.push_back( texture );

                }

            }
            
            return textures;

        }

    public:
        Model( const char *path );
        void Draw( Shader );
        std::vector<Mesh> GetMeshes();
        std::vector<Texture> GetLoadedTextures();
    
};

Model::Model( const char *path ) {

    this->loadModel( path );

}

void Model::Draw( Shader shader ) {

    for( unsigned int i = 0; i < meshes.size(); i++) {
        
        meshes[i].Draw( shader );

    }
    

}

std::vector<Mesh> Model::GetMeshes() {

    return this->meshes;

}

std::vector<Texture> Model::GetLoadedTextures() {

    return this->textures_loaded;

}

unsigned int TextureFromFile( const char *path, const std::string directory ) {

    //Generate texture ID and load texture data
    std::string filename = std::string( path );
    filename = directory + '/' + filename;
    
    unsigned int textureID;
    glGenTextures( 1, &textureID );
    
    int width, height, channels;
    
    unsigned char *image = stbi_load( filename.c_str( ), &width, &height, &channels, 0 );
    
    // Assign texture to ID
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap( GL_TEXTURE_2D );
    
    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture( GL_TEXTURE_2D, 0 );

    stbi_image_free( image );
    return textureID;

}

}
