#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class PointLightLayer : public Layer {

public:
    PointLightLayer() : Layer(  "Lightinglayer" ), m_CameraController( 800.0f/600.0f ) {}
    virtual ~PointLightLayer() = default;

    virtual void OnAttach() override {
        
        glGenVertexArrays( 1, &VAO );
        glGenBuffers( 1, &VBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( VAO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )0 );
        glEnableVertexAttribArray( 0 );


        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )( 3 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );
        
        //glEnableVertexAttribArray( 2 );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )( 6 * sizeof( float ) ) );
        glEnableVertexAttribArray( 2 );
        
        // LAMP VAO
        
        glGenVertexArrays( 1, &m_LampVAO );
        glGenBuffers( 1, &VBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( m_LampVAO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        // Position Attribute
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( GLvoid * )0 );
        glEnableVertexAttribArray( 0 );
        
        m_Shader = Shader::Create( "lightingPart", vertexSrc, fragmentSrc );        
        m_LampShader = Shader::Create( "lampShader", vertexLampSrc, fragmentLampSrc );
        m_TextureDiffuse = Texture2D::Create( "/home/syntax/Knowledge/learn_cpp/MyRepos/SimpleGraphicsEngine/src/myexe/src/assets/textures/container2.png" );
        m_TextureSpecular = Texture2D::Create( "/home/syntax/Knowledge/learn_cpp/MyRepos/SimpleGraphicsEngine/src/myexe/src/assets/textures/container2_specular.png" );
                
        m_TextureDiffuse->Bind( 0 );
        m_TextureSpecular->Bind( 1 );
    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        // m_PespectiveCamera.OnUpdate();
        m_CameraController.OnUpdate();
        
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        Renderer::Clear();
        // Render
        m_Shader->Bind();
        glm::mat4 Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 View = m_CameraController.GetCamera().GetViewMatrix();
        glm::mat4 Model = glm::mat4(1.0f);
        
        // we set the uniforms for the object,light color's and also the Position of our light and view'
        m_Shader->UploadUniformFloat3( "light.ambient", glm::vec3( 0.2f ) );
        m_Shader->UploadUniformFloat3( "light.diffuse", glm::vec3( 0.5f ) );
        m_Shader->UploadUniformFloat3( "light.specular", glm::vec3( 1.0f ) );
        m_Shader->UploadUniformFloat3( "light.position", glm::vec3( 2.0f, 3.0f, 4.0f ) );
        m_Shader->UploadUniformFloat( "light.constant", 1.0f );
        m_Shader->UploadUniformFloat( "light.linear", 0.01f );
        m_Shader->UploadUniformFloat( "light.quadratic", 0.032f );
        m_Shader->UploadUniformFloat3( "viewPos", m_CameraController.GetCamera().GetPosition() );
            
        m_Shader->UploadUniformInt( "material.diffuse", 0 );
        m_Shader->UploadUniformInt( "material.specular", 1 );
        m_Shader->UploadUniformFloat("material.shininess", 32.0f); 
        
        m_Shader->UploadUniformMat4( "u_Projection", Projection );
        m_Shader->UploadUniformMat4( "u_View", View );
        //m_Shader->UploadUniformMat4( "u_Model", Model );
        
        glBindVertexArray( VAO );
        for( unsigned int i = 0; i < 10; i++ ) {

            Model = glm::mat4(1.0f);
            Model = glm::translate( Model, cubePositions[i] );
            float angle = 20.0f * i;
            Model = glm::rotate( Model, angle, glm::vec3( 1.0f, 0.3f, 0.5f ) );
            m_Shader->UploadUniformMat4( "u_Model", Model );
            glDrawArrays( GL_TRIANGLES, 0, 36 );

        }
        glBindVertexArray( 0 );

        m_LampShader->Bind();
        
        Model = glm::mat4(1.0f);
        Model = glm::translate( Model, glm::vec3( 1.0f, 1.5f, 2.0f ) );
        Model = glm::scale( Model, glm::vec3( 0.5f ) );
        
        m_Shader->UploadUniformMat4( "u_Projection", Projection );
        m_Shader->UploadUniformMat4( "u_View", View );
        m_Shader->UploadUniformMat4( "u_Model", Model );
        
        glBindVertexArray( m_LampVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

    }
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    // PerspectiveCamera m_PespectiveCamera;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Shader> m_LampShader;
    // Camera m_Camera;
    std::shared_ptr<Texture2D> m_TextureDiffuse;
    std::shared_ptr<Texture2D> m_TextureSpecular;
    unsigned int VAO, m_LampVAO, VBO;

};
