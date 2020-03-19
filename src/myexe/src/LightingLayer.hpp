#include "mylib.hpp"
#include <glad/glad.h>

class LightingLayer : public ENGINE::Layer {

public:
    LightingLayer() : Layer(  "Lightinglayer" ) {}
    virtual ~LightingLayer() = default;

    virtual void OnAttach() override {

         float vertices[] = {
        
            // Front Backward Face  // Normals: are perpendicular vectors on each vertex
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 
            0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 
            0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 
            0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 
            -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f, 
            // Front Forward Face
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f, 
            0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f, 
            0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f, 
            0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f, 
            -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,  1.0f, 
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,  1.0f, 
            // Left Face
            -0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f, 
            -0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f, 
            -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f, 
            -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f, 
            -0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f, 
            -0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f, 
            // Right Face
            0.5f,  0.5f,  0.5f,    1.0f, 0.0f,  0.0f, 
            0.5f,  0.5f, -0.5f,    1.0f, 0.0f,  0.0f, 
            0.5f, -0.5f, -0.5f,    1.0f, 0.0f,  0.0f, 
            0.5f, -0.5f, -0.5f,    1.0f, 0.0f,  0.0f, 
            0.5f, -0.5f,  0.5f,    1.0f, 0.0f,  0.0f, 
            0.5f,  0.5f,  0.5f,    1.0f, 0.0f,  0.0f, 
            // Bottom Face
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, 
            0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, 
            0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f, 
            0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f, 
            -0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f, 
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f, 
            // Top Face
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 
            0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 
            0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 
            0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 
            -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f
    
        };

        // set up veertex data and buffers
        glGenVertexArrays( 1, &VAO );
        glGenBuffers( 1, &VBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( VAO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        // Position Attribute
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( GLvoid * )0 );
        glEnableVertexAttribArray( 0 );


        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( GLvoid * )( 3 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );
        // Color Attribute
        m_Shader = ENGINE::Shader::Create("/home/syntax/Temp/Experiment/Engine/src/myexe/src/assets/shaders/lighting.glsl" );
        //m_Texture = ENGINE::Texture::Create( "/home/syntax/Temp/Experiment/Engine/src/myexe/src/assets/textures/Checkerboard.png" );

        // m_PespectiveCamera = ENGINE::PerspectiveCamera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
        // m_PespectiveCamera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
        // m_Shader->UploadUniformMat4( "u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix() );
        // m_Shader->UploadUniformMat4( "u_Transform ", glm::mat4(1.0f) );
        //m_Shader->UploadUniformInt("texture_sampler", 0);

    }
    
    virtual void OnDettach() override {}
    
    virtual void OnUpdate() override {

        // m_PespectiveCamera.OnUpdate();
        // m_CameraController.OnUpdate();
        // Render
        m_Shader->Bind();
        // m_Shader->UploadUniformMat4( "u_View", m_CameraController.GetCamera().GetViewMatrix() );
        // m_Shader->UploadUniformMat4( "u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix() );
        // m_Shader->UploadUniformMat4( "u_Transform ", glm::mat4(1.0f) );
        // Clear the colorbuffer
        ENGINE::Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        ENGINE::Renderer::Clear();

        // glm::mat4 ProjectionMatrix = m_PespectiveCamera.GetProjection();
        // glm::mat4 ViewMatrix = m_PespectiveCamera.GetViewMatrix();
        glm::mat4 ProjectionMatrix = glm::perspective( glm::radians(100.0f), 800.0f/600.0f, 0.1f, 100.0f );
        glm::mat4 ViewMatrix(1.0f);
        glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
        
        m_Shader->UploadUniformMat4( "u_ViewProjection", ViewProjectionMatrix );
        m_Shader->UploadUniformMat4( "u_Transform ", glm::mat4(1.0f) );
        //m_Texture->Bind( 0 );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

    }
    virtual void OnEvent( ENGINE::Event& e ) override {

        // m_CameraController.OnEvent( e );

    }

private:
    // ENGINE::CameraController m_CameraController;
    // ENGINE::PerspectiveCamera m_PespectiveCamera;
    std::shared_ptr<ENGINE::Shader> m_Shader;
    //std::shared_ptr<ENGINE::Texture> m_Texture;
    unsigned int VAO, VBO;

};