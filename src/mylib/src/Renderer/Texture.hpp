#pragma once

#include <memory>
#include <string>
#include <vector>

namespace ENGINE {

    class Texture2D {

    public:
        Texture2D( const std::string& path );
        Texture2D( std::vector<std::string> faces );
        ~Texture2D();

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        void Bind( unsigned int slot ) const;
        void Bind() const;

        static std::shared_ptr<Texture2D> Create( const std::string& path );
        static std::shared_ptr<Texture2D> Create( std::vector<std::string> faces );

    private:
        unsigned int m_RendererID;
        unsigned int m_Width, m_Height;
        std::string m_Path;

    };

}
