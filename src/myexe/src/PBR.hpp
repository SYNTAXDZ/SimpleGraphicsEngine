#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include "Path.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class PBRLayer : public Layer {

public:
    PBRLayer() : Layer(  "PBRLayer" ),
    m_CameraController( 800.0f/600.0f ) {} 
    virtual ~PBRLayer() = default;

    virtual void OnAttach() override {

        m_Shader = std::make_shared<CShader>( GetPath( "/shaders/pbr.vert" ).c_str(), GetPath( "/shaders/pbr.frag" ).c_str() );
        m_TexturedShader = std::make_shared<CShader>( GetPath( "/shaders/pbr_textured.vert" ).c_str(), GetPath( "/shaders/pbr_textured.frag" ).c_str() );
        
        m_TexturedShader->Bind();
        // set the sampler2D unfiroms
        m_TexturedShader->SetUniformInt( "albedoMap", 0 );
        m_TexturedShader->SetUniformInt( "normalMap", 1 );
        m_TexturedShader->SetUniformInt( "metallicMap", 2 );
        m_TexturedShader->SetUniformInt( "roughnessMap", 3 );
        m_TexturedShader->SetUniformInt( "aoMap", 4 );
        

        m_AlbedoTexture    = Texture2D::Create( GetPath( "/textures/pbr/rusted_iron/albedo.png" ) );
        m_NormalTexture    = Texture2D::Create( GetPath( "/textures/pbr/rusted_iron/normal.png" ) );
        m_MetallicTexture  = Texture2D::Create( GetPath( "/textures/pbr/rusted_iron/metallic.png" ) );
        m_RoughnessTexture = Texture2D::Create( GetPath( "/textures/pbr/rusted_iron/roughness.png" ) );
        m_AOTexture        = Texture2D::Create( GetPath( "/textures/pbr/rusted_iron/ao.png" ) );
        
        // we are using this way of loading textures due to some issues
        m_Texture = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/albedo.png" ) );
        m_Texture1 = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/normal.png" ) );
        m_Texture2 = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/metallic.png" ) );
        m_Texture3 = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/roughness.png" ) );
        m_Texture4 = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/ao.png" ) );


    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        m_CameraController.OnUpdate();
        
        Renderer::Clear();
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        
        glm::mat4 Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        glm::mat4 View = m_CameraController.GetCamera().GetViewMatrix();

        /*
        m_Shader->Bind();

        m_Shader->SetUniformVec3( "u_CamPos", m_CameraController.GetCamera().GetPosition() );
        
        m_Shader->SetUniformVec3( "albedo", glm::vec3( 0.5f, 0.0f, 0.0f ) );
        m_Shader->SetUniformFloat( "ao", 1.0f );
        
        m_Shader->SetUniformMat4( "u_Projection", Projection );
        m_Shader->SetUniformMat4( "u_View", View );
        */
        m_TexturedShader->Bind();
        m_TexturedShader->SetUniformVec3( "u_CamPos", m_CameraController.GetCamera().GetPosition() );
        m_TexturedShader->SetUniformMat4( "u_Projection", Projection );
        m_TexturedShader->SetUniformMat4( "u_View", View );
       
        /*
        m_AlbedoTexture->Bind( 0 );     
        m_NormalTexture->Bind( 1 );       
        m_MetallicTexture->Bind( 2 );    
        m_RoughnessTexture->Bind( 3 );    
        m_AOTexture->Bind( 4 );    
        */
       // bind the textures
       glActiveTexture( GL_TEXTURE0 );
       glBindTexture( GL_TEXTURE_2D, m_Texture );
       glActiveTexture( GL_TEXTURE1 );
       glBindTexture( GL_TEXTURE_2D, m_Texture1 );
       glActiveTexture( GL_TEXTURE2 );
       glBindTexture( GL_TEXTURE_2D, m_Texture2 );
       glActiveTexture( GL_TEXTURE3 );
       glBindTexture( GL_TEXTURE_2D, m_Texture3 );
       glActiveTexture( GL_TEXTURE4 );
       glBindTexture( GL_TEXTURE_2D, m_Texture4 );
        // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
        glm::mat4 model = glm::mat4(1.0f);
        for( int row = 0; row < nrRows; ++row ) {
            
            //m_Shader->SetUniformFloat( "metallic", (float)row / (float)nrRows );
            
            for( int col = 0; col < nrColumns; ++col ) {
                // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                //m_Shader->SetUniformFloat( "roughness", glm::clamp( ( float)col / ( float )nrColumns, 0.05f, 1.0f ) );
                
                model = glm::mat4( 1.0f );
                model = glm::translate(model, glm::vec3(
                    (col - (nrColumns / 2)) * spacing, 
                    (row - (nrRows / 2)) * spacing, 
                    0.0f
                ));
                
                //m_Shader->SetUniformMat4( "u_Model", model );
                m_TexturedShader->SetUniformMat4( "u_Model", model );
                renderSphere();
            }
        }

        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
        // keeps the codeprint small.
        for( unsigned int i = 0; i < sizeof( lightPositions2 ) / sizeof( lightPositions2[0] ); ++i ) {
            
            //m_TexturedShader->Bind();

            glm::vec3 newPos = lightPositions2[i] + glm::vec3( sin( glfwGetTime() * 5.0 ) * 5.0, 0.0, 0.0 );
            newPos = lightPositions2[i];
            //m_Shader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            //m_Shader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            m_TexturedShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            m_TexturedShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors2[i]);


            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            
            //m_Shader->SetUniformMat4("u_Model", model);

            m_TexturedShader->SetUniformMat4("u_Model", model);
            
            renderSphere();
        
        }
    
    }
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    std::shared_ptr<CShader> m_Shader;
    std::shared_ptr<CShader> m_TexturedShader;
    std::shared_ptr<Texture2D> m_AlbedoTexture;
    std::shared_ptr<Texture2D> m_NormalTexture;
    std::shared_ptr<Texture2D> m_MetallicTexture;
    std::shared_ptr<Texture2D> m_RoughnessTexture;
    std::shared_ptr<Texture2D> m_AOTexture;

    unsigned int m_Texture;
    unsigned int m_Texture1;
    unsigned int m_Texture2;
    unsigned int m_Texture3;
    unsigned int m_Texture4;

};
