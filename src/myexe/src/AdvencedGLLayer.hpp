#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include "Path.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class AdvencedLayer : public Layer {

public:
    AdvencedLayer() : Layer(  "AdvencedLayer" ),
     m_CameraController( 800.0f/600.0f ) {}
    virtual ~AdvencedLayer() = default;

    virtual void OnAttach() override {

        // set up vertex data and buffers
        glGenVertexArrays( 1, &m_VAO );
        glGenBuffers( 1, &m_VBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( m_VAO );
        glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        // Position Attribute
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )0 );

        
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )( 3 * sizeof( float ) ) );
        
        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )( 6 * sizeof( float ) ) );
        
        glBindVertexArray( 0 );

        // set up vertex data and buffers
        glGenVertexArrays( 1, &m_QuadVAO );
        glGenBuffers( 1, &m_QuadVBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( m_QuadVAO );
        glBindBuffer( GL_ARRAY_BUFFER, m_QuadVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertices ), quadVertices, GL_STATIC_DRAW );
        // Position Attribute
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), ( GLvoid * )0 );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), ( GLvoid * )( 2 * sizeof( float ) ) );
        glBindVertexArray( 0 );
        
        glGenVertexArrays( 1, &m_PlaneVAO );
        glGenBuffers( 1, &m_PlaneVBO );
        glBindVertexArray( m_PlaneVAO );
        glBindBuffer( GL_ARRAY_BUFFER, m_PlaneVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( planeVertices ), planeVertices, GL_STATIC_DRAW );

        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )0 );

        glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )( 6 * sizeof( float ) ) );

        glBindVertexArray( 0 );
        
        std::string vertexPath = "/shaders/example.vert";
        std::string fragmentPath = "/shaders/example.frag";
        std::string texPath = "/textures/container2.png";
        
        std::string vertexShaderPath = GetPath( vertexPath );
        std::string fragmentShaderPath = GetPath( fragmentPath );
        std::string texturePath = GetPath( texPath );
        std::string marbleTexture = GetPath( "/textures/marble.jpg" );

        std::string vertexScreen = GetPath( "/shaders/screen.vert" );
        std::string fragmentScreen = GetPath( "/shaders/screen.frag" );
        
        m_Shader = std::make_shared<CShader>( vertexShaderPath.c_str(), fragmentShaderPath.c_str() );
        m_ScreenShader = std::make_shared<CShader>( vertexScreen.c_str(), fragmentScreen.c_str() );
        
        glGenFramebuffers( 1, &m_Framebuffer );
        glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer );

        glGenTextures( 1, &m_TextureColorBuffer );
        glBindTexture( GL_TEXTURE_2D, m_TextureColorBuffer );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorBuffer, 0 );
        
        glGenRenderbuffers( 1, &m_RBO );
        glBindRenderbuffer( GL_RENDERBUFFER, m_RBO );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600 );
        glBindRenderbuffer( GL_RENDERBUFFER, 0 );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO );
    
        if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! \n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        m_Texture = Texture2D::Create( texturePath );
        m_PlaneTexture = Texture2D::Create( marbleTexture );
        
    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        m_CameraController.OnUpdate();

        glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer );
        glEnable( GL_DEPTH_TEST );
        
        Renderer::Clear();
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        
        glm::mat4 Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 View = m_CameraController.GetCamera().GetViewMatrix();
        glm::mat4 Model = glm::mat4(1.0f);
        
        Model = glm::translate( Model, glm::vec3( 0.0f, 0.0f, -1.0f ) );
        
        m_Shader->Bind();
        m_Shader->SetUniformInt( "texture_sampler", 0 );
        m_Shader->SetUniformMat4( "u_Projection", Projection );
        m_Shader->SetUniformMat4( "u_View", View );
        m_Shader->SetUniformMat4( "u_Model", Model );
        
        glBindVertexArray( m_VAO );
        m_Texture->Bind( 0 );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        // floor
        glBindVertexArray( m_PlaneVAO );
        //m_Shader->SetUniformInt( "texture_sampler", 0 );
        m_Shader->SetUniformMat4( "u_Model", glm::mat4( 1.0f ) );
        m_PlaneTexture->Bind( 0 );
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        glBindVertexArray( 0 );
        
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glDisable( GL_DEPTH_TEST );

        //glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        //glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        
        m_ScreenShader->Bind();
        m_ScreenShader->SetUniformInt( "screen_texture", 0 );
        glBindVertexArray( m_QuadVAO );
        glBindTexture( GL_TEXTURE_2D, m_TextureColorBuffer );
        glDrawArrays( GL_TRIANGLES, 0, 6 );
    
    }
    
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    std::shared_ptr<CShader> m_Shader;
    std::shared_ptr<CShader> m_ScreenShader;
    std::shared_ptr<Texture2D> m_Texture;
    std::shared_ptr<Texture2D> m_PlaneTexture;
    unsigned int m_VAO, m_VBO;
    int m_FBO;
    unsigned int m_QuadVAO, m_QuadVBO;
    unsigned int m_Framebuffer, m_TextureColorBuffer, m_RBO;
    unsigned int m_PlaneVAO, m_PlaneVBO;

};
