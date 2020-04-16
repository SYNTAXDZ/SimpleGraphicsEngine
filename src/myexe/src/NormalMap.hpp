#pragma once

#include "mylib.hpp"

#include "Util.hpp"
#include "Path.hpp"

#include <glad/glad.h>

using namespace ENGINE;

static inline unsigned int quadVAO = 0;
static inline unsigned int quadVBO;
void RenderQuad() {

    // positions
    glm::vec3 pos1( -1.0, 1.0, 0.0 );
    glm::vec3 pos2( -1.0, -1.0, 0.0 );
    glm::vec3 pos3( 1.0, -1.0, 0.0 );
    glm::vec3 pos4( 1.0, 1.0, 0.0 );
    // texture coordinates
    glm::vec2 uv1( 0.0, 1.0 );
    glm::vec2 uv2( 0.0, 0.0 );
    glm::vec2 uv3( 1.0, 0.0 );
    glm::vec2 uv4( 1.0, 1.0 );
    // normal vector
    glm::vec3 nm( 0.0, 0.0, 1.0 );

    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;
    
    glm::vec3 tangent1, bitangent1;    
    glm::vec3 tangent2, bitangent2;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    tangent1.x = f * ( deltaUV2.y * edge1.x - deltaUV1.y * edge2.x );
    tangent1.y = f * ( deltaUV2.y * edge1.y - deltaUV1.y * edge2.y );
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    glm::normalize( tangent1 );
    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    glm::normalize( bitangent1 );

    // triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f/ ( deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y );

    tangent2.x = f * ( deltaUV2.y * edge1.x - deltaUV1.y * edge2.x );
    tangent2.y = f * ( deltaUV2.y * edge1.y - deltaUV1.y * edge2.y );
    tangent2.z = f * ( deltaUV2.y * edge1.z - deltaUV1.y * edge2.z );
    glm::normalize( tangent2 );
    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    glm::normalize( bitangent2 );

    float quadVertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    // configure plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


}

class NormalMappingingLayer : public Layer {

public:
    NormalMappingingLayer() : Layer(  "AdvencedLayer" ),
     m_CameraController( 800.0f/600.0f ) {}
    virtual ~NormalMappingingLayer() = default;

    virtual void OnAttach() override {

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
        
        std::string vertexPath = "/shaders/normal_mapping.vert";
        std::string fragmentPath = "/shaders/normal_mapping.frag";
        std::string texPath = "/textures/container2.png";
        
        std::string vertexShaderPath = GetPath( vertexPath );
        std::string fragmentShaderPath = GetPath( fragmentPath );
        std::string brickwall_diffuse = GetPath( "/textures/brickwall.jpg" );
        std::string brickwall_normal = GetPath( "/textures/brickwall_normal.jpg" );

        std::string vertexScreen = GetPath( "/shaders/screen.vert" );
        std::string fragmentScreen = GetPath( "/shaders/screen.frag" );
        
        m_Shader = std::make_shared<CShader>( vertexShaderPath.c_str(), fragmentShaderPath.c_str() );
        m_ScreenShader = std::make_shared<CShader>( vertexScreen.c_str(), fragmentScreen.c_str() );
       /* 
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
        */
        m_Texture = Texture2D::Create( brickwall_diffuse );
        m_TextureNormal = Texture2D::Create( brickwall_normal );
        
    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        m_CameraController.OnUpdate();

        glm::vec3 lightPos( 0.5f, 1.0f, 0.3f );

        Renderer::Clear();
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        
        glm::mat4 Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 View = m_CameraController.GetCamera().GetViewMatrix();
        glm::mat4 Transform = glm::mat4(1.0f);
        
        Transform = glm::rotate( Transform, glm::radians( (float)glfwGetTime() * -12.0f ), glm::vec3( 1.0f, 0.0f, 1.0f ) );
        
        m_Shader->Bind();
        m_Shader->SetUniformInt( "texture_diffuse1", 0 );
        m_Shader->SetUniformInt( "texture_normal1", 1 );
      
        m_Shader->SetUniformMat4( "u_Projection", Projection );
        m_Shader->SetUniformMat4( "u_View", View );
        m_Shader->SetUniformMat4( "u_Model", Transform );
        
        m_Shader->SetUniformVec3( "u_ViewPos", m_CameraController.GetCamera().GetPosition() );
        m_Shader->SetUniformVec3( "u_LightPos", lightPos );

        m_Texture->Bind( 0 );
        m_TextureNormal->Bind( 1 );
        RenderQuad();

        //m_Texture->UnBind();
        //m_TextureNormal->UnBind();

        /*
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        glDisable( GL_DEPTH_TEST );

        glClear( GL_COLOR_BUFFER_BIT );
        
        m_ScreenShader->Bind();
        m_ScreenShader->SetUniformInt( "screen_texture", 0 );
        glBindVertexArray( m_QuadVAO );
        glBindTexture( GL_TEXTURE_2D, m_TextureColorBuffer );
        glDrawArrays( GL_TRIANGLES, 0, 6 );
        */
    }
    
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    std::shared_ptr<CShader> m_Shader;
    std::shared_ptr<CShader> m_ScreenShader;
    std::shared_ptr<Texture2D> m_Texture;
    std::shared_ptr<Texture2D> m_TextureNormal;
    unsigned int m_VAO, m_VBO;
    int m_FBO;
    unsigned int m_QuadVAO, m_QuadVBO;
    unsigned int m_Framebuffer, m_TextureColorBuffer, m_RBO;
    unsigned int m_PlaneVAO, m_PlaneVBO;

};
