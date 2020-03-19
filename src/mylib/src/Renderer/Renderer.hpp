#pragma once

#include <glm/glm.hpp>

namespace ENGINE {

    class Renderer {

    public:
        static void Init();
        static void SetViewport( unsigned int x, unsigned int y,unsigned int width,unsigned int height );
        static void SetClearColor( const glm::vec4& color );
        static void Clear();
        // static void DrawIndexed();

    };

}