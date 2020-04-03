#include "Mesh.hpp"
#include <glad/glad.h>

using namespace ENGINE;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
    
    Vertices = vertices;
    Indices = indices;
    Textures = textures;
    
    Init();
    
}

void Mesh::Init() {

    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glGenBuffers( 1, &m_EBO );
    
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, Vertices.size() * sizeof( Vertex ), &Vertices[0], GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof( unsigned int ), &Indices[0], GL_STATIC_DRAW );
    
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )0 );
    // normales
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )offsetof( Vertex, Normal ) );
    // coordonnées de texture
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( void* )offsetof( Vertex, TexCoords ) );
    
    //glBindVertexArray(0);
    
}

void Mesh::Draw( Shader shader ) {

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    
    for( unsigned int i = 0; i < Textures.size(); i++ ) {
        
        glActiveTexture( GL_TEXTURE0 + i );
        
        std::string number;
        std::string name = Textures[i].Type;
        
        if( name == "diffuse" ) {
            
            number = std::to_string( diffuseNr++ );
            
        } else if( name == "specular" ) {
            
            number = std::to_string( specularNr++ );
            
        }
        
        shader.UploadUniformInt( ("material." + name + number).c_str(), i );
        glBindTexture( GL_TEXTURE_2D, Textures[i].ID );
        
    }
    
    shader.UploadUniformFloat( "material.shininess", 32.0f );
    
    //glActiveTexture( GL_TEXTURE0 );
    glBindVertexArray( m_VAO );
    glDrawElements( GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0  );
    glBindVertexArray( 0 );

    // Always good practice to set everything back to defaults once configured.
    for( GLuint i = 0; i < Textures.size(); i++ ) {
        
        glActiveTexture( GL_TEXTURE0 + i );
        glBindTexture( GL_TEXTURE_2D, 0 );
    
    }
    
}
