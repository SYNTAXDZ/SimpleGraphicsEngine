#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include "Path.hpp"
#include <glad/glad.h>

using namespace ENGINE;

inline static unsigned int cubeVAO = 0;
inline static unsigned int cubeVBO = 0;
void renderCube() {

    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

class HDRLayer : public Layer {

public:
    HDRLayer() : Layer(  "AdvencedLayer" ),
     m_CameraController( 800.0f/600.0f ) {}
    virtual ~HDRLayer() = default;

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

        std::string vertexShaderPath = GetPath( "/shaders/hdr_lighting.vert" );
        std::string fragmentShaderPath = GetPath( "/shaders/hdr_lighting.frag" );
        std::string vertexScreen = GetPath( "/shaders/hdr.vert" );
        std::string fragmentScreen = GetPath( "/shaders/hdr.frag" );
        
        m_Shader = std::make_shared<CShader>( vertexShaderPath.c_str(), fragmentShaderPath.c_str() );
        m_HDRShader = std::make_shared<CShader>( vertexScreen.c_str(), fragmentScreen.c_str() );
        
        glGenFramebuffers( 1, &m_Framebuffer );
        glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer );

        glGenTextures( 1, &m_HdrColorBuffer );
        glBindTexture( GL_TEXTURE_2D, m_HdrColorBuffer );
        // now we use a floating point framebuffer
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_HdrColorBuffer, 0 );
        
        glGenRenderbuffers( 1, &m_RBO );
        glBindRenderbuffer( GL_RENDERBUFFER, m_RBO );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600 );
        glBindRenderbuffer( GL_RENDERBUFFER, 0 );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO );
    
        if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! \n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_WoodTexture = Texture2D::Create( GetPath( "/textures/wood.png" ), true );

       // positions
        std::vector<glm::vec3> lightPositions;
        lightPositions.push_back(glm::vec3( 0.0f,  0.0f, 49.5f)); // back light
        lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
        lightPositions.push_back(glm::vec3( 0.0f, -1.8f, 4.0f));
        lightPositions.push_back(glm::vec3( 0.8f, -1.7f, 6.0f));
        // colors
        std::vector<glm::vec3> lightColors;
        lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
        lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
        lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
        lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
        
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
        m_Shader->SetUniformInt( "diffuseTexture", 0 );
        m_Shader->SetUniformMat4( "u_Projection", Projection );
        m_Shader->SetUniformMat4( "u_View", View );
        //m_Shader->SetUniformMat4( "u_Model", Model );
        
        // set lighting uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++) {
                m_Shader->SetUniformVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
                m_Shader->SetUniformVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }
        m_Shader->SetUniformVec3("viewPos", m_CameraController.GetCamera().GetPosition() );
        // render tunnel
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
        m_Shader->SetUniformMat4( "u_Model", model );
        m_Shader->SetUniformInt( "inverse_normals", true );
        renderCube();
        glBindVertexArray( 0 );
        
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        //glDisable( GL_DEPTH_TEST );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        m_HDRShader->Bind();
        m_HDRShader->SetUniformInt( "hdrBuffer", 0 );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, m_HdrColorBuffer );
        renderQuad();
    
    }
    
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    std::shared_ptr<CShader> m_Shader;
    std::shared_ptr<CShader> m_HDRShader;
    //std::shared_ptr<Texture2D> m_Texture;
    std::shared_ptr<Texture2D> m_WoodTexture;
    unsigned int m_VAO, m_VBO;
    int m_FBO;
    unsigned int m_QuadVAO, m_QuadVBO;
    unsigned int m_Framebuffer, m_HdrColorBuffer, m_RBO;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

};
