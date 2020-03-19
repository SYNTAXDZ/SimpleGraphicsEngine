#pragma once
#include "../../pch/pch.hpp"

#include "Window.hpp"

#include "../Events/Event.hpp"
#include "../Events/ApplicationEvent.hpp"

#include "LayerStack.hpp"

namespace ENGINE {

    class Application {

    public:
        Application();
        virtual ~Application() = default;        

        void Run();
        void OnEvent( Event& e );

        void PushLayer( Layer* layer );
        void PushOverlay( Layer* overlay );

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }
    
    private:
        bool OnWindowClose( WindowCloseEvent& e );
    private:
        bool m_Running = true;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
        static Application* s_Instance;

    };

    // Application* CreateApplication();

}