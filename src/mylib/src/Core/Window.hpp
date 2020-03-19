#pragma once

#include "../../pch/pch.hpp"
#include "../Events/Event.hpp"
#include <GLFW/glfw3.h>

namespace ENGINE {

      struct WindowProps {

        // the title of our window
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps( const std::string& title = "Engine",
           unsigned int width = 800,
           unsigned int height = 600 )
            : Title( title ), Width( width ), Height( height ) {}


  };


    class Window {
    
    public:
        using EventCallbackFn = std::function<bool(Event&)>;
        
        Window( const WindowProps& props );
        ~Window();
        void Update();
        int GetWidth() const { return m_Data.Width; }
        int GetHeight() const { return m_Data.Height;  }

        GLFWwindow* GetNativeWindow() const { return m_Window; }
        void SetEventCallback( const EventCallbackFn& callback ) {

            m_Data.EventCallback = callback;

        }

        static Window* Create( const WindowProps& props = WindowProps() );
    
    private:
        void Init( const WindowProps& props );
        void Shutdown();
    private:
        GLFWwindow* m_Window;

        struct WindowData {

            std::string Title;
            unsigned int Width, Height;
            EventCallbackFn EventCallback;

        };        

        WindowData m_Data;

    };

}