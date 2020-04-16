#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include "Path.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class ModelLayer : public Layer {

public:
    ModelLayer() : Layer(  "Modellayer" ),
     m_CameraController( 800.0f/600.0f ),
     m_Model( GetPath( "/models/cyborg/cyborg.obj" ).c_str() ) {} 
    virtual ~ModelLayer() = default;

    virtual void OnAttach() override {

        //m_Shader = Shader::Create( "modelPart", modelVertexSrc, modelFragmentSrc );
        m_Shader = std::make_shared<CShader>( GetPath( "/shaders/normal_mapping.vert" ).c_str(), GetPath( "/shaders/normal_mapping.frag" ).c_str() );

    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        // m_PespectiveCamera.OnUpdate();
        m_CameraController.OnUpdate();
        Renderer::Clear();
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        // Render
        m_Shader->Bind();
        glm::mat4 Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 View = m_CameraController.GetCamera().GetViewMatrix();
        glm::mat4 Transform = glm::mat4(1.0f);
        Transform = glm::translate( Transform, glm::vec3( 0.0f, 0.0f, -2.0f ) );
        Transform = glm::scale( Transform, glm::vec3( 0.5f ) );
        
        // we set the uniforms for the object,light color's and also the Position of our light and view'
        //m_Shader->UploadUniformFloat3( "light.ambient", glm::vec3( 0.2f ) );
        //m_Shader->UploadUniformFloat3( "light.diffuse", glm::vec3( 0.5f ) );
        //m_Shader->UploadUniformFloat3( "light.specular", glm::vec3( 1.0f ) );
        //m_Shader->UploadUniformFloat3( "light.position", glm::vec3( 2.0f, 3.0f, 4.0f ) );
        //m_Shader->UploadUniformFloat( "light.constant", 1.0f );
        //m_Shader->UploadUniformFloat( "light.linear", 0.01f );
        //m_Shader->UploadUniformFloat( "light.quadratic", 0.032f );
        //m_Shader->UploadUniformFloat3( "viewPos", m_CameraController.GetCamera().GetPosition() );
            
        m_Shader->SetUniformInt( "texture_diffuse1", 0 );
        m_Shader->SetUniformInt( "texture_normal1", 1 );

        m_Shader->SetUniformVec3( "u_LightPos", glm::vec3( 0.0f, 0.5f, -1.0f ) );
        m_Shader->SetUniformVec3( "u_ViewPos", m_CameraController.GetCamera().GetPosition() ); 
        
        m_Shader->SetUniformMat4( "u_Projection", Projection );
        m_Shader->SetUniformMat4( "u_View", View );
        m_Shader->SetUniformMat4( "u_Model", Transform );
       //m_Model->Draw( *m_Shader );
        auto shader = *m_Shader;
        m_Model.Draw( shader );
        //m_Shader->UnBind();
        /*
        m_LampShader->Bind();
        
        Model = glm::mat4(1.0f);
        Model = glm::translate( Model, glm::vec3( 1.0f, 1.5f, 2.0f ) );
        Model = glm::scale( Model, glm::vec3( 0.5f ) );
        
        m_LampShader->UploadUniformMat4( "u_Projection", Projection );
        m_LampShader->UploadUniformMat4( "u_View", View );
        m_LampShader->UploadUniformMat4( "u_Model", Model );
        
        glBindVertexArray( m_LampVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        */
    
    }
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    // PerspectiveCamera m_PespectiveCamera;
    std::shared_ptr<CShader> m_Shader;
    Model m_Model;
    //std::shared_ptr<Model> m_Model;

};
