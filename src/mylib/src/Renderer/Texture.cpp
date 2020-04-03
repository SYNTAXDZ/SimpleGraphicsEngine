#include "Texture.hpp"
#include "stb_image.h"

#include <glad/glad.h>

#include <iostream>

using namespace ENGINE;

Texture2D::Texture2D( const std::string& path ) : m_Path( path ) {

    int width, height, channels;

    stbi_set_flip_vertically_on_load( 1 );

    stbi_uc* data = stbi_load( path.c_str(), &width, &height, &channels, 0 );

    m_Width = width;
    m_Height = height;
    
    if( !data )
        std::cout << "texture not found \n";
    
    GLenum internalFormat = 0, dataFormat = 0;

    if( channels == 4 ) {

        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;

    } else if( channels == 3 ) {

        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;

    }

    glGenTextures( 1, &m_RendererID );
    glBindTexture( GL_TEXTURE_2D, m_RendererID );
    glTexStorage2D( GL_TEXTURE_2D, 1, internalFormat, m_Width, m_Height );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data );
    
    stbi_image_free( data );
    glBindTexture( GL_TEXTURE_2D, 0 );

}

Texture2D::Texture2D( std::vector<std::string> faces ) {

    int width, height, channels;

    stbi_set_flip_vertically_on_load( 1 );

    m_Width = width;
    m_Height = height;
    
    glGenTextures( 1, &m_RendererID );
    glBindTexture( GL_TEXTURE_2D, m_RendererID );
    //glTexStorage2D( GL_TEXTURE_2D, 1, internalFormat, m_Width, m_Height );
    
    for( unsigned int i = 0; i < faces.size(); i++ ) {

        stbi_uc* data = stbi_load( faces[i].c_str(), &width, &height, &channels, 0 );

        if( data ) {

            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                          0, GL_RGB, width, height, 0, GL_RGB, 
                          GL_UNSIGNED_BYTE, data );
            
            stbi_image_free( data );

        } else {

            std::cout << "Cubemap Texture not available: " << faces[i] << std::endl;
            stbi_image_free( data );

        }

    }

    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}

Texture2D::~Texture2D() {

    glDeleteTextures( 1, &m_RendererID );

}

void Texture2D::Bind( unsigned int slot ) const {

    glActiveTexture( GL_TEXTURE0 + slot );
    glBindTexture( GL_TEXTURE_2D, m_RendererID );

}

void Texture2D::Bind() const {

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_CUBE_MAP, m_RendererID );

}

std::shared_ptr<Texture2D> Texture2D::Create( const std::string& path ) {

    return std::make_shared<Texture2D>( path );

}

std::shared_ptr<Texture2D> Texture2D::Create( std::vector<std::string> faces ) {

    return std::make_shared<Texture2D>( faces );

}