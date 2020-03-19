#include "Texture.hpp"
#include "stb_image.h"

#include <glad/glad.h>

using namespace ENGINE;

Texture::Texture( const std::string& path ) : m_Path( path ) {

    int width, height, channels;

    stbi_set_flip_vertically_on_load( 1 );

    stbi_uc* data = stbi_load( path.c_str(), &width, &height, &channels, 0 );

    m_Width = width;
    m_Height = height;

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

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data );

    stbi_image_free( data );  

}

Texture::~Texture() {

    glDeleteTextures( 1, &m_RendererID );

}

void Texture::Bind( unsigned int slot ) const {

    glActiveTexture( GL_TEXTURE0 + slot );
    glBindTexture( GL_TEXTURE_2D, m_RendererID );

}

std::shared_ptr<Texture> Texture::Create( const std::string& path ) {

    return std::make_shared<Texture>( path );

}