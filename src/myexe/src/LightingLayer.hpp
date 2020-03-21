#pragma once

#include "mylib.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class LightingLayer : public Layer {

public:
    LightingLayer() : Layer(  "Lightinglayer" ), m_CameraController( 800.0f/600.0f ) {}
    virtual ~LightingLayer() = default;

    virtual void OnAttach() override {

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
            uniform mat4 u_Projection;
            uniform mat4 u_View;
            uniform mat4 u_Model;

			void main()
			{
				gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			

			void main()
			{
                color = vec4( 0.5, 0.6, 0.5, 1.0 );
			}
		)";


        float vertices[] = {

            -0.5f, -0.5f, -0.5f,  
            0.5f, -0.5f, -0.5f,   
            0.5f,  0.5f, -0.5f,   
            0.5f,  0.5f, -0.5f,   
            -0.5f,  0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  
            // Front Forward Face
            -0.5f, -0.5f,  0.5f,  
            0.5f, -0.5f,  0.5f,   
            0.5f,  0.5f,  0.5f,   
            0.5f,  0.5f,  0.5f,   
            -0.5f,  0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f,  
            // Left Face
            -0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  
            -0.5f, -0.5f, -0.5f,  
            -0.5f, -0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f,  
            // Right Face
            0.5f,  0.5f,  0.5f,   
            0.5f,  0.5f, -0.5f,   
            0.5f, -0.5f, -0.5f,   
            0.5f, -0.5f, -0.5f,   
            0.5f, -0.5f,  0.5f,   
            0.5f,  0.5f,  0.5f,   
            // Bottom Face
            -0.5f, -0.5f, -0.5f,  
            0.5f, -0.5f, -0.5f,   
            0.5f, -0.5f,  0.5f,   
            0.5f, -0.5f,  0.5f,   
            -0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f, -0.5f,  
            // Top Face
            -0.5f,  0.5f, -0.5f,  
            0.5f,  0.5f, -0.5f,   
            0.5f,  0.5f,  0.5f,   
            0.5f,  0.5f,  0.5f,   
            -0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f, -0.5f,  

        };
        // set up veertex data and buffers
        glGenVertexArrays( 1, &VAO );
        glGenBuffers( 1, &VBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( VAO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        // Position Attribute
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( GLvoid * )0 );
        glEnableVertexAttribArray( 0 );


        // glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), ( GLvoid * )( 3 * sizeof( float ) ) );
        // glEnableVertexAttribArray( 1 );
        // Color Attribute
        //m_Shader = Shader::Create("/home/syntax/Knowledge/learn_cpp/MyRepos/SimpleGraphicsEngine/src/myexe/src/assets/shaders/lighting.glsl" );
        
        //auto width = Application::Get().GetWindow().GetNativeWindow().GetWidth();
        //auto height = Application::Get().GetWindow().GetNativeWindow().GetHeight();
        
        // m_Camera = Camera( 45.0f, 800.0f/600.0f, 0.1f, 100.0f );
        m_Shader = Shader::Create( "lightingPart", vertexSrc, fragmentSrc );        
        //m_Texture = Texture::Create( "/home/syntax/Knowledge/learn_cpp/SimpleGraphicsEngine/src/myexe/src/assets/textures/Checkerboard.png" );

        // m_PespectiveCamera = PerspectiveCamera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
        // m_PespectiveCamera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
        // m_Shader->UploadUniformMat4( "u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix() );
        // m_Shader->UploadUniformMat4( "u_Transform ", glm::mat4(1.0f) );
        //m_Shader->UploadUniformInt("texture_sampler", 0);

    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        // m_PespectiveCamera.OnUpdate();
        m_CameraController.OnUpdate();
        // Render
        m_Shader->Bind();
        // m_Shader->UploadUniformMat4( "u_View", m_CameraController.GetCamera().GetViewMatrix() );
        // m_Shader->UploadUniformMat4( "u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix() );
        // m_Shader->UploadUniformMat4( "u_Transform ", glm::mat4(1.0f) );
        // Clear the colorbuffer
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        Renderer::Clear();

        glm::mat4 Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 View = m_CameraController.GetCamera().GetViewMatrix();
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::translate( Model, glm::vec3( 0.0f, 0.0f, -5.0f ) );

        m_Shader->UploadUniformMat4( "u_Projection", Projection );
        m_Shader->UploadUniformMat4( "u_View", View );
        m_Shader->UploadUniformMat4( "u_Model", Model );
        //m_Texture->Bind( 0 );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );

    }
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    // PerspectiveCamera m_PespectiveCamera;
    std::shared_ptr<Shader> m_Shader;
    // Camera m_Camera;
    //std::shared_ptr<Texture> m_Texture;
    unsigned int VAO, VBO;

};
