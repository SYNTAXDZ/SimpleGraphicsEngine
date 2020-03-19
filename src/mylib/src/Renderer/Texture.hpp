#pragma once

#include <memory>
#include <string>

namespace ENGINE {

    class Texture {

    public:
        Texture( const std::string& path );
        ~Texture();

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        void Bind( unsigned int slot ) const;

        static std::shared_ptr<Texture> Create( const std::string& path );

    private:
        unsigned int m_RendererID;
        unsigned int m_Width, m_Height;
        std::string m_Path;

    };

}