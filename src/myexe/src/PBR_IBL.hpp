#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include "Path.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class PBRIBLLayer : public Layer {

public:
    PBRIBLLayer() : Layer(  "PBRIBLLayer" ),
    m_CameraController( 800.0f/600.0f ) {
       
    } 
    virtual ~PBRIBLLayer() = default;

    virtual void OnAttach() override {

        m_Shader = std::make_shared<CShader>( GetPath( "/shaders/pbr_ibl.vert" ).c_str(), GetPath( "/shaders/pbr_ibl.frag" ).c_str() );
        
        m_EquirectangularToCubemapShader = std::make_shared<CShader>( GetPath( "/shaders/cubemap.vert" ).c_str(),
                                                                            GetPath( "/shaders/equirectangular_to_cubemap.frag" ).c_str() );
        
        m_BackgroundShader = std::make_shared<CShader>( GetPath( "/shaders/background.vert" ).c_str(), GetPath( "/shaders/background.frag" ).c_str() );
        
        m_IrradianceShader = std::make_shared<CShader>( GetPath( "/shaders/cubemap.vert" ).c_str(), GetPath( "/shaders/irradiance_convolution.frag" ).c_str() );


        m_HdrTexture = Texture2D::LoadTextureHDR( GetPath( "/textures/hdr/carpentry_shop_02_4k.hdr" ) );
    
        int width = Application::Get().GetWindow().GetWidth();
        int height = Application::Get().GetWindow().GetHeight();
        Renderer::SetViewport( 0, 0, width, height );

        /*
         to convert the source HDR image onto a cubemap texture, we have to render the same
         cube 6 times looking at each individual face of the cube while recording its visual
         result with a framebuffer object
        */
        glGenFramebuffers( 1, &m_CaptureFBO );
        glGenRenderbuffers( 1, &m_CaptureRBO );
        glBindFramebuffer( GL_FRAMEBUFFER, m_CaptureFBO );
        glBindRenderbuffer( GL_RENDERBUFFER, m_CaptureRBO );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512 );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CaptureRBO );
        
        /*
         also generate the corresponding cubemap, pre-allocating memory for each of its 6 faces
         by loading an empty Cubemap
        */
        m_EnvCubemap = Texture2D::LoadEmptyCubemap();
        
         // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
        /*
         we need to capture the equirectangular 2D texture onto the cubemap faces. but it effectively
         boils down to setting up 6 different view matrices facing each side of the cube, given a projection
         matrix with a fov of 90 degrees to capture the entire face, and render a cube 6 times
         storing the results in a floating point framebuffer
        */
        glm::mat4 captureProjection = glm::perspective( glm::radians(90.0f), 1.0f, 0.1f, 10.0f );
        glm::mat4 captureViews[] = {
            // we are looking from the center to the Cube's Face
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
    
        // Convert The Equirectangular Map to a Cubemap
        m_EquirectangularToCubemapShader->Bind();
        m_EquirectangularToCubemapShader->SetUniformInt( "equirectangularMap", 0 );
        m_EquirectangularToCubemapShader->SetUniformMat4( "u_Projection", captureProjection );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, m_HdrTexture );

        glViewport( 0, 0, 512, 512 ); // don’t forget to configure the viewport to the capture dimensions.
        // Use The Coresponding  Framebuffer
        glBindFramebuffer( GL_FRAMEBUFFER, m_CaptureFBO );
        /*
         We take the color attachment of the framebuffer and switch its texture target around
         for every face of the cubemap, directly rendering the scene into one of the cubemap’s
         faces. Once this routine has finished (which we only have to do once) the cubemap
         envCubemap should be the cubemapped environment version of our original HDR image
        */
        for( unsigned int i = 0; i < 6; ++i ) {
            // Set The View
            m_EquirectangularToCubemapShader->SetUniformMat4( "u_View", captureViews[i] );
            // Attach The Texture to The Framebuffer
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubemap, 0 );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            renderCube(); // renders a 1x1 cube
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        /*
         we can convolute the earlier captured envCubemap. First we create the irradiance cubemap
         (again, we only have to do this once before the render loop )
        */
        m_IrradianceMap = Texture2D::LoadIrradinaceMap();
        // we re-scale the capture framebuffer to the new resolution
        glBindFramebuffer( GL_FRAMEBUFFER, m_CaptureFBO );
        glBindRenderbuffer( GL_RENDERBUFFER, m_CaptureRBO );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32 );

        /*
         Using the convolution shader we convolute the environment map in a similar way we captured
         the environment cubemap
        */
        m_IrradianceShader->Bind();
        m_IrradianceShader->SetUniformInt( "environmentMap", 0);
        m_IrradianceShader->SetUniformMat4( "u_Projection", captureProjection );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_EnvCubemap );
        
        glViewport( 0, 0, 32, 32 ); // don’t forget to configure the viewport to the capturedimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO );
        // do the same as the EnvCubemap
        for( unsigned int i = 0; i < 6; ++i ) {

            m_IrradianceShader->SetUniformMat4( "u_View", captureViews[i] );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0 );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        
    }

    virtual void OnDettach() override {}

    virtual void OnUpdate() override {

        m_CameraController.OnUpdate();
        
        Renderer::Clear();
        Renderer::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f } );
        
        m_Projection = m_CameraController.GetCamera().GetProjectionMatrix();
        m_View = m_CameraController.GetCamera().GetViewMatrix();

         m_BackgroundShader->Bind();
        m_BackgroundShader->SetUniformMat4( "u_Projection", m_Projection );
        
        m_Shader->Bind();

        m_Shader->SetUniformVec3( "u_CamPos", m_CameraController.GetCamera().GetPosition() );
        
        m_Shader->SetUniformVec3( "albedo", glm::vec3( 0.5f, 0.0f, 0.0f ) );
        m_Shader->SetUniformFloat( "ao", 1.0f );
        
        m_Shader->SetUniformMat4( "u_Projection", m_Projection );
        m_Shader->SetUniformMat4( "u_View", m_View );
        
        // then before rendering, configure the viewport to the original framebuffer's screen dimensions
        int srcWidth, srcHeight;
        
        glfwGetFramebufferSize( Application::Get().GetWindow().GetNativeWindow(), &srcWidth, &srcHeight );
        Renderer::SetViewport( 0, 0, srcWidth, srcHeight );

        m_Shader->Bind();

        // bind pre-computed IBL data
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap );
        // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
        glm::mat4 model = glm::mat4(1.0f);
        for( int row = 0; row < nrRows; ++row ) {
            
            m_Shader->SetUniformFloat( "metallic", (float)row / (float)nrRows );
            //m_Shader->SetUniformFloat( "metallic", 0.9f );
            
            for( int col = 0; col < nrColumns; ++col ) {
                // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                m_Shader->SetUniformFloat( "roughness", glm::clamp( ( float)col / ( float )nrColumns, 0.05f, 1.0f ) );
                //m_Shader->SetUniformFloat( "roughness", 0.2f );
                
                model = glm::mat4( 1.0f );
                model = glm::translate(model, glm::vec3(
                    (col - (nrColumns / 2)) * spacing, 
                    (row - (nrRows / 2)) * spacing, 
                    0.0f
                ));
                
                m_Shader->SetUniformMat4( "u_Model", model );
                renderSphere();
            }
        }


        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
        // keeps the codeprint small.
        for( unsigned int i = 0; i < sizeof( lightPositions ) / sizeof( lightPositions[0] ); ++i ) {

            glm::vec3 newPos = lightPositions[i] + glm::vec3( sin( glfwGetTime() * 5.0 ) * 5.0, 0.0, 0.0 );
            newPos = lightPositions[i];
            m_Shader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            m_Shader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            
            m_Shader->SetUniformMat4("u_Model", model);
            
            renderSphere();
        
        }
        
        m_BackgroundShader->Bind();
        m_BackgroundShader->SetUniformMat4( "u_View", m_View );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_EnvCubemap );
        //glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap ); // display irradiance map
        renderCube();
    
    }
    virtual void OnEvent( Event& e ) override {

        m_CameraController.OnEvent( e );

    }

private:
    CameraController m_CameraController;
    std::shared_ptr<CShader> m_Shader;
    std::shared_ptr<CShader> m_EquirectangularToCubemapShader;
    std::shared_ptr<CShader> m_BackgroundShader;
    std::shared_ptr<CShader> m_IrradianceShader;

    unsigned int m_HdrTexture;
    unsigned int m_CaptureFBO, m_CaptureRBO;
    unsigned int m_EnvCubemap;
    unsigned int m_IrradianceMap;

    glm::mat4 m_Projection, m_View;

};
