#pragma once

#include "mylib.hpp"
#include "Util.hpp"
#include "Path.hpp"
#include <glad/glad.h>

using namespace ENGINE;

class PBRIBLTexturedLayer : public Layer {

public:
    PBRIBLTexturedLayer() : Layer(  "PBRIBLLayer" ),
    m_CameraController( 800.0f/600.0f ) {
       
    } 
    virtual ~PBRIBLTexturedLayer() = default;

    virtual void OnAttach() override {

        m_Shader = std::make_shared<CShader>( GetPath( "/shaders/ibl_textured.vert" ).c_str(), GetPath( "/shaders/ibl_textured.frag" ).c_str() );
        
        m_EquirectangularToCubemapShader = std::make_shared<CShader>( GetPath( "/shaders/cubemap.vert" ).c_str(),
                                                                            GetPath( "/shaders/equirectangular_to_cubemap.frag" ).c_str() );
        
        m_BackgroundShader = std::make_shared<CShader>( GetPath( "/shaders/background.vert" ).c_str(), GetPath( "/shaders/background.frag" ).c_str() );
        
        m_IrradianceShader = std::make_shared<CShader>( GetPath( "/shaders/cubemap.vert" ).c_str(), GetPath( "/shaders/irradiance_convolution.frag" ).c_str() );

        m_PreFilterShader = std::make_shared<CShader>( GetPath( "/shaders/cubemap.vert" ).c_str(), GetPath( "/shaders/prefilter.frag" ).c_str() );
        m_BrdfShader = std::make_shared<CShader>( GetPath( "/shaders/brdf.vert" ).c_str(), GetPath( "/shaders/brdf.frag" ).c_str() );

        m_HdrTexture = Texture2D::LoadTextureHDR( GetPath( "/textures/hdr/carpentry_shop_02_4k.hdr" ) );

        m_GoldTextureAlbedo = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/gold/albedo.png" ) );
        m_GoldTextureNormal = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/gold/normal.png" ) );;
        m_GoldTextureRoughness = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/gold/roughness.png" ) );;
        m_GoldTextureMetallic = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/gold/metallic.png" ) );;
        m_GoldTextureAO = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/gold/ao.png" ) );;

        m_IronTextureAlbedo = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/albedo.png" ) );
        m_IronTextureNormal = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/normal.png" ) );;
        m_IronTextureRoughness = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/roughness.png" ) );;
        m_IronTextureMetallic = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/metallic.png" ) );;
        m_IronTextureAO = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/rusted_iron/ao.png" ) );;

        m_GrassTextureAlbedo = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/grass/albedo.png" ) );
        m_GrassTextureNormal = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/grass/normal.png" ) );;
        m_GrassTextureRoughness = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/grass/roughness.png" ) );;
        m_GrassTextureMetallic = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/grass/metallic.png" ) );;
        m_GrassTextureAO = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/grass/ao.png" ) );;


        m_PlasticTextureAlbedo = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/plastic/albedo.png" ) );
        m_PlasticTextureNormal = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/plastic/normal.png" ) );;
        m_PlasticTextureRoughness = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/plastic/roughness.png" ) );;
        m_PlasticTextureMetallic = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/plastic/metallic.png" ) );;
        m_PlasticTextureAO = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/plastic/ao.png" ) );;


        m_WallTextureAlbedo = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/wall/albedo.png" ) );
        m_WallTextureNormal = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/wall/normal.png" ) );;
        m_WallTextureRoughness = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/wall/roughness.png" ) );;
        m_WallTextureMetallic = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/wall/metallic.png" ) );;
        m_WallTextureAO = Texture2D::LoadTexture2D( GetPath( "/textures/pbr/wall/ao.png" ) );;


        int width = Application::Get().GetWindow().GetWidth();
        int height = Application::Get().GetWindow().GetHeight();
        Renderer::SetViewport( 0, 0, width, height );

        m_Shader->Bind();
        m_Shader->SetUniformInt( "irradianceMap", 0 );
        m_Shader->SetUniformInt( "prefilterMap", 1 );
        m_Shader->SetUniformInt( "brdfLUT", 2 );

        m_Shader->SetUniformInt( "albedoMap", 3 );
        m_Shader->SetUniformInt( "normalMap", 4 );
        m_Shader->SetUniformInt( "metallicMap", 5 );
        m_Shader->SetUniformInt( "roughnessMap", 6 );
        m_Shader->SetUniformInt( "aoMap", 7 );



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

        Renderer::SetViewport( 0, 0, 512, 512 );
        //glViewport( 0, 0, 512, 512 ); // don’t forget to configure the viewport to the capture dimensions.
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

        // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_EnvCubemap );
        glGenerateMipmap( GL_TEXTURE_CUBE_MAP );

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
        
        Renderer::SetViewport( 0, 0, 32, 32 );
        //glViewport( 0, 0, 32, 32 ); // don’t forget to configure the viewport to the capturedimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO );
        // do the same as the EnvCubemap
        for( unsigned int i = 0; i < 6; ++i ) {

            m_IrradianceShader->SetUniformMat4( "u_View", captureViews[i] );
            glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0 );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        m_PrefilterMap = Texture2D::LoadPreFilteredMap();

        m_PreFilterShader->Bind();

        m_PreFilterShader->SetUniformInt( "environmentMap", 0 );
        m_PreFilterShader->SetUniformMat4( "u_Projection", captureProjection );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_EnvCubemap );
        
        glBindFramebuffer( GL_FRAMEBUFFER, m_CaptureFBO );
        
        unsigned int maxMipLevels = 5;
        for( unsigned int mip = 0; mip < maxMipLevels; ++mip ) {

            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = 128 * std::pow( 0.5, mip );
            unsigned int mipHeight = 128 * std::pow( 0.5, mip );
            
            glBindRenderbuffer( GL_RENDERBUFFER, m_CaptureRBO );
            glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight );
            glViewport( 0, 0, mipWidth, mipHeight );

            //m_PreFilterShader->Bind();

            float roughness = (float)mip / (float)( maxMipLevels - 1 );
            m_PreFilterShader->SetUniformFloat( "roughness", roughness );
            for( unsigned int i = 0; i < 6; ++i ) {
        
                m_PreFilterShader->SetUniformMat4( "u_View", captureViews[i] );
                glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
                renderCube();
        
            }
        
        }
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        
        m_BrdfLutTexture = Texture2D::LoadLutTexture();
       
        glBindFramebuffer( GL_FRAMEBUFFER, m_CaptureFBO );
        glBindRenderbuffer( GL_RENDERBUFFER, m_CaptureRBO );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512 );
        glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BrdfLutTexture, 0 );
        
        glViewport( 0, 0, 512, 512 );
        m_BrdfShader->Bind();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        RenderQuad();
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
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_EnvCubemap );
        glGenerateMipmap( GL_TEXTURE_CUBE_MAP );


        m_Shader->Bind();

        m_Shader->SetUniformVec3( "u_CamPos", m_CameraController.GetCamera().GetPosition() );
        
        //m_Shader->SetUniformVec3( "albedo", glm::vec3( 0.5f, 0.0f, 0.0f ) );
        //m_Shader->SetUniformFloat( "ao", 1.0f );
        
        m_Shader->SetUniformMat4( "u_Projection", m_Projection );
        m_Shader->SetUniformMat4( "u_View", m_View );
        

        // then before rendering, configure the viewport to the original framebuffer's screen dimensions
        int srcWidth, srcHeight;
        
        glfwGetFramebufferSize( Application::Get().GetWindow().GetNativeWindow(), &srcWidth, &srcHeight );
        Renderer::SetViewport( 0, 0, srcWidth, srcHeight );

        //m_Shader->Bind();
        // bind pre-computed IBL data
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap );
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture(GL_TEXTURE_2D, m_BrdfLutTexture );
      
          // rusted iron
        glActiveTexture( GL_TEXTURE3);
        glBindTexture( GL_TEXTURE_2D, m_IronTextureAlbedo);
        glActiveTexture( GL_TEXTURE4);
        glBindTexture( GL_TEXTURE_2D, m_IronTextureNormal);
        glActiveTexture( GL_TEXTURE5);
        glBindTexture( GL_TEXTURE_2D, m_IronTextureMetallic);
        glActiveTexture( GL_TEXTURE6);
        glBindTexture( GL_TEXTURE_2D, m_IronTextureRoughness);
        glActiveTexture( GL_TEXTURE7);
        glBindTexture( GL_TEXTURE_2D, m_IronTextureAO);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
        m_Shader->SetUniformMat4( "u_Model", model );
        renderSphere();

        // gold
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_GoldTextureAlbedo);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_GoldTextureNormal);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_GoldTextureMetallic );
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_GoldTextureRoughness);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, m_GoldTextureAO);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
        m_Shader->SetUniformMat4("u_Model", model);
        renderSphere();

        // grass
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_GrassTextureAlbedo);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_GrassTextureNormal);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_GrassTextureMetallic);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_GrassTextureRoughness);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, m_GrassTextureAO);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
        m_Shader->SetUniformMat4( "u_Model", model );
        renderSphere();

        // plastic
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_PlasticTextureAlbedo);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_PlasticTextureNormal);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_PlasticTextureMetallic);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_PlasticTextureRoughness);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, m_PlasticTextureAO);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
        m_Shader->SetUniformMat4( "u_Model", model );
        renderSphere();

        // wall
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_WallTextureAlbedo);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_WallTextureNormal);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_WallTextureMetallic);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_WallTextureRoughness);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, m_WallTextureAO);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
        m_Shader->SetUniformMat4( "u_Model", model );
        renderSphere();


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
        //glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap); // display prefilter map
        renderCube();

        //m_BrdfShader->Bind();
        //RenderQuad();
        
    
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
    std::shared_ptr<CShader> m_PreFilterShader;
    std::shared_ptr<CShader> m_BrdfShader;

    unsigned int m_HdrTexture = 0;
    unsigned int m_CaptureFBO, m_CaptureRBO;
    unsigned int m_EnvCubemap = 0;
    unsigned int m_IrradianceMap = 0;
    unsigned int m_PrefilterMap = 0;
    unsigned int m_BrdfLutTexture = 0;

    // pbr textures
    unsigned int m_GoldTextureAlbedo = 0;
    unsigned int m_GoldTextureNormal = 0;
    unsigned int m_GoldTextureRoughness = 0;
    unsigned int m_GoldTextureMetallic = 0;
    unsigned int m_GoldTextureAO = 0;

    unsigned int m_GrassTextureAlbedo = 0;
    unsigned int m_GrassTextureNormal = 0;
    unsigned int m_GrassTextureRoughness = 0;
    unsigned int m_GrassTextureMetallic = 0;
    unsigned int m_GrassTextureAO = 0;

    unsigned int m_PlasticTextureAlbedo = 0;
    unsigned int m_PlasticTextureNormal = 0;
    unsigned int m_PlasticTextureRoughness = 0;
    unsigned int m_PlasticTextureMetallic = 0;
    unsigned int m_PlasticTextureAO = 0;

    unsigned int m_IronTextureAlbedo = 0;
    unsigned int m_IronTextureNormal = 0;
    unsigned int m_IronTextureRoughness = 0;
    unsigned int m_IronTextureMetallic = 0;
    unsigned int m_IronTextureAO = 0;

    unsigned int m_WallTextureAlbedo = 0;
    unsigned int m_WallTextureNormal = 0;
    unsigned int m_WallTextureRoughness = 0;
    unsigned int m_WallTextureMetallic = 0;
    unsigned int m_WallTextureAO = 0;


    glm::mat4 m_Projection, m_View;

};
