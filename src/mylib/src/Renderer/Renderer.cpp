#include "Renderer.hpp"
#include <glad/glad.h>

using namespace ENGINE;

void Renderer::Init() {

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_DEPTH_TEST );

}
void Renderer::SetViewport( unsigned int x, unsigned int y,unsigned int width,unsigned int height ) {

    glViewport( x, y, width, height );

}
void Renderer::SetClearColor( const glm::vec4& color ) {

    glClearColor( color.x, color.y, color.z, color.w );

}
void Renderer::Clear() {

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}
/*void Renderer::DrawIndexed() {

    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    glBindTexture( GL_TEXTURE_2D, 0 );

}
*/