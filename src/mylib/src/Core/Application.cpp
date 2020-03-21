#include "Application.hpp"
#include "KeyCodes.hpp"
#include "MouseButtonCodes.hpp"
#include "Input.hpp"

#include "../Renderer/Renderer.hpp"

#include <iostream>

using namespace ENGINE;

#define BIND_EVENT_FN( x ) std::bind( &Application::x, this, std::placeholders::_1 )

Application* Application::s_Instance = nullptr;

Application::Application() {
    
    s_Instance = this;
    m_Window = std::unique_ptr<Window>( Window::Create() );
    auto eventCallback = [this]( Event& e ) -> bool {
		
			this->OnEvent( e );

			return true;
		
	};
    m_Window->SetEventCallback( eventCallback );

    Renderer::Init();
    Renderer::SetViewport( 0, 0, m_Window->GetWidth(), m_Window->GetHeight() );

}

void Application::OnEvent( Event& e ) {

    EventDispatcher dispatcher( e );
    dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FN( OnWindowClose ) );
    dispatcher.Dispatch<WindowResizeEvent>( BIND_EVENT_FN( OnWindowResize ) );
    
    std::cout << "The Event: " << e << "\n";
    
    for( auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {

        (*--it )->OnEvent( e );

        if( e.Handeld )
            break;

    }

}

void Application::PushLayer( Layer* layer ) {

    m_LayerStack.PushLayer( layer );

}

void Application::PushOverlay( Layer* overlay ) {

    m_LayerStack.PushOverlay( overlay );

}

void Application::Run() {
        
    while( m_Running ) {
        
        if( Input::IsKeyPressed( KEY_ESCAPE ) )
            m_Running = false;
        
        for( Layer* layer : m_LayerStack )
            layer->OnUpdate();

        m_Window->Update();        

    }
    

}

bool Application::OnWindowClose( WindowCloseEvent& e ) {

    m_Running = false;

    return true;

}

bool Application::OnWindowResize( WindowResizeEvent& e ) {

    Renderer::SetViewport( 0, 0, (unsigned int)e.GetWidth(), (unsigned int)e.GetHeight() );

    return true;

}
