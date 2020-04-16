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

Texture2D::Texture2D( const std::string& path, bool isGammaCorrected ) : m_Path( path ) {

    int width, height, channels;

    stbi_set_flip_vertically_on_load( 1 );

    stbi_uc* data = stbi_load( path.c_str(), &width, &height, &channels, 0 );

    m_Width = width;
    m_Height = height;
    
    if( !data )
        std::cout << "texture not found \n";

    glGenTextures( 1, &m_RendererID );
    glBindTexture( GL_TEXTURE_2D, m_RendererID );
    //glTexStorage2D( GL_TEXTURE_2D, 1, GL_SRGB, m_Width, m_Height );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    if( isGammaCorrected )
        glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    else
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
    

    //glGenerateMipmap( GL_TEXTURE_2D );

    stbi_image_free( data );
    glBindTexture( GL_TEXTURE_2D, 0 );

}

Texture2D::Texture2D( std::vector<std::string> faces ) {

    int width, height, channels;
    
    //stbi_set_flip_vertically_on_load(true);

    glGenTextures( 1, &m_RendererID );
    glBindTexture( GL_TEXTURE_CUBE_MAP, m_RendererID );
    /*
    for( unsigned int i = 0; i < faces.size(); i++ ) {

        stbi_uc* data = stbi_load( faces[i].c_str(), &width, &height, &channels, 0 );
        std::cout << "the face is: " << faces[i].c_str() << std::endl;
        if( data ) {
            // add i to the enum in each iteration
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                          0, GL_RGB, width, height, 0, GL_RGB, 
                          GL_UNSIGNED_BYTE, data );
            
            stbi_image_free( data );

        } else {

            std::cout << "Cubemap Texture not available: " << faces[i] << std::endl;
            stbi_image_free( data );

        }

    }
    */
    
    stbi_uc* data0 = stbi_load( faces[0].c_str(), &width, &height, &channels, 0 );
    stbi_uc* data1 = stbi_load( faces[1].c_str(), &width, &height, &channels, 0 );
    stbi_uc* data2 = stbi_load( faces[2].c_str(), &width, &height, &channels, 0 );
    stbi_uc* data3 = stbi_load( faces[3].c_str(), &width, &height, &channels, 0 );
    stbi_uc* data4 = stbi_load( faces[4].c_str(), &width, &height, &channels, 0 );
    stbi_uc* data5 = stbi_load( faces[5].c_str(), &width, &height, &channels, 0 );
    
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data0 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4 );
    glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data5 );
    
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // don't forget to set the wrapping to GL_CLAMP_TO_EDGE
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    
    stbi_image_free( data0 );
    stbi_image_free( data1 );
    stbi_image_free( data2 );
    stbi_image_free( data3 );
    stbi_image_free( data4 );
    stbi_image_free( data5 );
    
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

void Texture2D::UnBind() const {

    glBindTexture( GL_TEXTURE_2D, 0 );

}


std::shared_ptr<Texture2D> Texture2D::Create( const std::string& path ) {

    return std::make_shared<Texture2D>( path );

}

std::shared_ptr<Texture2D> Texture2D::Create( const std::string& path, bool isGamma ) {

    return std::make_shared<Texture2D>( path, isGamma );

}

std::shared_ptr<Texture2D> Texture2D::Create( std::vector<std::string> faces ) {

    return std::make_shared<Texture2D>( faces );

}
