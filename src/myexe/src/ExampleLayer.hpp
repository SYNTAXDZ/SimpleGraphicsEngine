#include "mylib.hpp"
#include <glad/glad.h>


class ExampleLayer : public ENGINE::Layer {

public:
    ExampleLayer() : Layer(  "Examplelayer" ) {}
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override {

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoords;
			
            out vec2 v_TexCoords;
			//out vec4 v_Color;
			//uniform mat4 u_ViewProjection;
			void main()
			{
				v_TexCoords = a_TexCoords;
				//v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoords;
			
            uniform sampler2D texSampler;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = texture( texSampler, v_TexCoords );
			}
		)";

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        // glEnable( GL_DEPTH_TEST );

        float vertices[5 * 3] = {
		    -0.5f, -0.5f, 0.0f, -1.0f, -1.0f,
		     0.5f, -0.5f, 0.0f, 1.0f, -1.0f,
	    	 0.0f,  0.5f, 0.0f, 0.5f, 1.0f,
	    };

        // set up veertex data and buffers
        glGenVertexArrays( 1, &VAO );
        glGenBuffers( 1, &VBO );
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray( VAO );
        glBindBuffer( GL_ARRAY_BUFFER, VBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

        // Position Attribute
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( GLvoid * )0 );
        glEnableVertexAttribArray( 0 );


        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), ( GLvoid * )( 3 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );
        // Color Attribute
        m_Shader = ENGINE::Shader::Create("example", vertexSrc ,fragmentSrc );
        //m_Shader = ENGINE::Shader::Create( "/home/syntax/Knowledge/learn_cpp/MyRepos/SimpleGraphicsEngine/src/myexe/src/assets/shaders/lighting.glsl"  );
        m_Texture = ENGINE::Texture2D::Create( "/home/syntax/Knowledge/learn_cpp/MyRepos/SimpleGraphicsEngine/src/myexe/src/assets/textures/Checkerboard.png" );

        m_Shader->UploadUniformInt("texture_sampler", 0);

    }
    
    virtual void OnDettach() override {}
    
    virtual void OnUpdate() override {


        if( ENGINE::Input::IsKeyPressed( KEY_SPACE ) )
            std::cout << "SPACE key pressed \n";

        // Render
        m_Shader->Bind();
        // Clear the colorbuffer
        ENGINE::Renderer::SetClearColor( { 0.4f, 0.3f, 0.5f, 1.0f } );
        ENGINE::Renderer::Clear();
        // glClearColor( 0.f, 0.3f, 0.8f, 0.9f );
        // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


        m_Texture->Bind( 0 );
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLES, 0, 3 );

    }
    virtual void OnEvent( ENGINE::Event& e ) override {}

private:
    std::shared_ptr<ENGINE::Shader> m_Shader;
    std::shared_ptr<ENGINE::Texture2D> m_Texture;
    unsigned int VAO, VBO;

};
