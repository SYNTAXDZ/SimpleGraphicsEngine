#include "Texture.hpp"
#include "stb_image.h"

//#include <glad/glad.h>

#include <iostream>

using namespace ENGINE;

Texture2D::Texture2D( const std::string& path ) : m_Path( path ) {

    int width, height, channels;

    //stbi_set_flip_vertically_on_load( 1 );

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

    } else if( channels == 1 ) {

        internalFormat = GL_RED;
        dataFormat = GL_RED;

    }

    if( data ) {

        glGenTextures( 1, &m_RendererID );
        glBindTexture( GL_TEXTURE_2D, m_RendererID );
        glTexStorage2D( GL_TEXTURE_2D, 1, internalFormat, m_Width, m_Height );
        //glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, data );
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );  
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data );
        
        stbi_image_free( data );
    }
    //glGenerateMipmap( GL_TEXTURE_2D );

    
    //glBindTexture( GL_TEXTURE_2D, 0 );

}

unsigned int Texture2D::LoadTexture2D( const std::string& path ) {
    
    unsigned int textureID;
    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_2D, textureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    int width, height, channels;

    stbi_set_flip_vertically_on_load( 1 );

    stbi_uc* data = stbi_load( path.c_str(), &width, &height, &channels, 0 );
    
    if( !data )
        std::cout << "texture not found \n";
    
    GLenum internalFormat = 0, dataFormat = 0;

    if( channels == 4 ) {

        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;

    } else if( channels == 3 ) {

        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;

    } else if( channels == 1 ) {

        internalFormat = GL_RED;
        dataFormat = GL_RED;

    }

    glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data );
    //glGenerateMipmap( GL_TEXTURE_2D );
    
    stbi_image_free( data );
    
    return textureID;

} 


unsigned int Texture2D::LoadTextureHDR( const std::string& path ) {

    stbi_set_flip_vertically_on_load( true );
    int width, height, nrComponents;
    // maps the HDR values to a list of floating point values: 32 bits per channel
    // and 3 channels per color by default.
    float *data = stbi_loadf( path.c_str(), &width, &height, &nrComponents, 0 );

    unsigned int hdrTexture;

    if( data ) {
    
        glGenTextures( 1, &hdrTexture );
        glBindTexture( GL_TEXTURE_2D, hdrTexture );
        // Load The Texture Of Type FLOAT
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        stbi_image_free(data);

    } else {

        std::cout << "Failed to load HDR image." << std::endl;
    
    }

    return hdrTexture;

} 

unsigned int Texture2D::LoadEmptyCubemap() {
    
    unsigned int envCubemap;
    glGenTextures( 1, &envCubemap );
    glBindTexture( GL_TEXTURE_CUBE_MAP, envCubemap );

    for( unsigned int i = 0; i < 6; ++i ) {

        // note that we store each face with 16 bit floating point values
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                      512, 512, 0, GL_RGB, GL_FLOAT, nullptr );
        
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return envCubemap;
    
}

/*
 As the irradiance map averages all surrounding radiance uniformly it doesn’t have a lot of high
 frequency details so we can store the map at a low resolution (32x32) and let OpenGL’s linear
 filtering do most of the work.
*/
unsigned int Texture2D::LoadIrradinaceMap() {

    unsigned int irradianceMap;

    glGenTextures( 1, &irradianceMap );
    glBindTexture( GL_TEXTURE_CUBE_MAP, irradianceMap );

    for( unsigned int i = 0; i < 6; ++i ) {

        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr );

    }

    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    return irradianceMap;

}

unsigned int Texture2D::LoadPreFilteredMap() {

    unsigned int prefilterMap = 0;
    
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    
    for( unsigned int i = 0; i < 6; ++i ) {
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minifcation filter to mip_linear 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return prefilterMap;

}


unsigned int ENGINE::Texture2D::LoadLutTexture() {
    
    unsigned int brdfLUTTexture = 0;
    glGenTextures( 1, &brdfLUTTexture );
    // pre-allocate enough memory for the LUT texture.
    glBindTexture( GL_TEXTURE_2D, brdfLUTTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, 0 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   
    return brdfLUTTexture;
    
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

void Texture2D::Release() {

    glDeleteTextures( 1, &m_RendererID );
    m_RendererID = 0;

}

Texture2D::~Texture2D() {

    Release();

}

void Texture2D::TextureParameter( GLenum type, GLenum pname, GLint param ) {

    GLuint boundTexture = 0;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, (GLint*) &boundTexture);
    //glGetIntegerv( GL_TEXTURE_BINDING_CUBE_MAP, (GLint*) &boundTexture);
    glBindTexture( type, m_RendererID );
    glTexParameteri( type, pname, param );
    glBindTexture( type, boundTexture );

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
