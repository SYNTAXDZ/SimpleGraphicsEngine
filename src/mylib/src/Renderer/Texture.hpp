#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

namespace ENGINE {

    class Texture2D {

    public:
        Texture2D( const std::string& path );
        Texture2D( const std::string& path, bool isGammaCorrected );
        Texture2D( std::vector<std::string> faces );
        ~Texture2D();

        Texture2D( const Texture2D& ) = delete;
        Texture2D &operator=( const Texture2D& ) = delete;

        Texture2D( Texture2D &&other ) : m_RendererID( other.m_RendererID ) {

            this->m_RendererID =  other.m_RendererID;

            //return *this;

        } 

        Texture2D &operator=( Texture2D &&other ) {

            if( this != &other ) {

                Release();
                std::swap( m_RendererID, other.m_RendererID );

            }

        }

        // Load An Image Texture
        static unsigned int LoadTexture2D( const std::string& path );
        // Load An Radiance HDR textrue 
        static unsigned int LoadTextureHDR( const std::string& path );
        // Load A Cubemap With No Data 
        static unsigned int LoadEmptyCubemap();
        // Load An Irradiance Map
        static unsigned int LoadIrradinaceMap();

        static unsigned int LoadPreFilteredMap();
        
        static unsigned int LoadLutTexture();
        
        inline unsigned int GetID() const { return m_RendererID; }

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        void TextureParameter( GLenum type, GLenum pname, GLint param );
        void Bind( unsigned int slot ) const;
        void Bind() const;
        //void Bind( GLenum type, unsigned int id );
        void UnBind() const;

        void Release();


        static std::shared_ptr<Texture2D> Create( const std::string& path );
        static std::shared_ptr<Texture2D> Create( const std::string& path, bool isGamma );
        static std::shared_ptr<Texture2D> Create( std::vector<std::string> faces );

    private:
        unsigned int m_RendererID = 0;
        unsigned int m_Width, m_Height;
        std::string m_Path;

    };

}
