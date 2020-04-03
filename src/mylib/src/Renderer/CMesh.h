#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.hpp"

namespace ENGINE {

struct Vertex {

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    glm::vec2 TexCoords;

};

struct Texture {
    
    unsigned int id;
    std::string type;
    aiString path;

};

class Mesh {

    private:
    
        unsigned int VAO, EBO, VBO;

        void setupMesh() {
            
            glGenVertexArrays( 1, &this->VAO );
            glGenBuffers( 1, &this->VBO );
            glGenBuffers( 1, &this->EBO );

            glBindVertexArray( this->VAO );
            glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
            glBufferData( GL_ARRAY_BUFFER, this->vertices.size() * sizeof( Vertex ), &this->vertices[0], GL_STATIC_DRAW );

            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->EBO );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, this->vertices.size() * sizeof( unsigned int ), &this->indices[0], GL_STATIC_DRAW );

            // vertex positions
            glEnableVertexAttribArray( 0 );
            glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * )0 );

            // vertex normals
            glEnableVertexAttribArray( 1 );
            glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * )offsetof( Vertex, Normal ) );

            // vertex texcoords
            glEnableVertexAttribArray( 2 );
            glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void * )offsetof( Vertex, TexCoords ) );

            // vertex tangent
            glEnableVertexAttribArray( 3 );
            glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )offsetof( Vertex, Tangent ) );
            // vertex bitangent
            glEnableVertexAttribArray( 4 );
            glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )offsetof( Vertex, Bitangent ) );


            glBindVertexArray( 0 );

        }

    public:

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures );

        void Draw( Shader shader );

        unsigned int GetVAO();
        
};

Mesh::Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures ) {

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();

}

void Mesh::Draw( Shader shader ) {

    unsigned int diffuseN = 1;
    unsigned int specularN = 1;
    unsigned int normalN = 1;
    unsigned int heightN = 1;

    for( unsigned int i = 0; i < this->textures.size(); i++ ) {
        
        glActiveTexture( GL_TEXTURE0 + i );

        std::stringstream ss;
        std::string number;
        std::string name = this->textures[i].type;

        if( name == "texture_diffuse" ) {

            ss << diffuseN++;   // Transfer unsigned int to stream

        } else if( name == "texture_specular" ) {

            ss << specularN++;

        } else if( name == "texture_normal" ) {

            ss << normalN++;

        } else if( name == "texture_height" ) {

            ss << heightN++;

        }

        number = ss.str();

        //glUniform1i( glGetUniformLocation( shader.Program, ( "material." + name + number ).c_str() ), i );
        shader.UploadUniformInt( ( "material." + name + number ).c_str(), i );
        glBindTexture( GL_TEXTURE_2D, this->textures[i].id );

    }

    // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
    shader.UploadUniformFloat( "material.shininess", 32.0f );
    
    glActiveTexture( GL_TEXTURE0 );
    // Draw mesh
    glBindVertexArray( this->VAO );
    glDrawElements( GL_TRIANGLES, this->indices.size( ), GL_UNSIGNED_INT, 0 );
    glBindVertexArray( 0 );
    
    // Always good practice to set everything back to defaults once configured.
    for( GLuint i = 0; i < this->textures.size( ); i++ ) {
        
        glActiveTexture( GL_TEXTURE0 + i );
        glBindTexture( GL_TEXTURE_2D, 0 );
    
    }
    

}

unsigned int Mesh::GetVAO() {

    return this->VAO;

}

}
