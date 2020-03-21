#include <glad/glad.h>

#include "../Events/ApplicationEvent.hpp"
#include "../Events/KeyEvent.hpp"
#include "../Events/MouseEvent.hpp"
#include "KeyCodes.hpp"
#include "MouseButtonCodes.hpp"
#include "Window.hpp"

#include <iostream>

using namespace ENGINE;

Window* Window::Create( const WindowProps& props ) {

    return new Window( props );

}

Window::Window( const WindowProps& props ) {

    Init( props );

}

Window::~Window() {

    Shutdown();

}

void Window::Init( const WindowProps& props ) {

    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 4 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    m_Window = glfwCreateWindow( m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr );

    glfwMakeContextCurrent( m_Window );

    //int status = gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress );
    //if( status == 0 ) std::cout << "glad not init \n";
    if(!gladLoadGL()) {

        std::cout << "failed to init glad \n";

    }

    glViewport( 0, 0, m_Data.Width, m_Data.Height );

    glfwSetWindowUserPointer( m_Window, &m_Data );
    glfwSwapInterval( 1 );
    
    glfwSetInputMode( m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
      // Set GLFW callbacks
    glfwSetWindowSizeCallback( m_Window, []( GLFWwindow* window, int width, int height ) {
      // get the userPointer and cast it to WindowData, and Set the EventCallback
      WindowData& data = *( WindowData* )glfwGetWindowUserPointer( window );
      data.Width = width;
      data.Height = height;

      WindowResizeEvent event( width, height );
      data.EventCallback( event );
    });

    glfwSetWindowCloseCallback( m_Window, []( GLFWwindow* window ) {
      WindowData& data = *( WindowData* )glfwGetWindowUserPointer( window );
      WindowCloseEvent event;
      data.EventCallback( event );
    });

    glfwSetKeyCallback( m_Window, []( GLFWwindow* window, int key, int scancode, int action, int mods ) {
      WindowData& data = *( WindowData* )glfwGetWindowUserPointer( window );

      switch ( action ) {
        case GLFW_PRESS:
        {
          //KeyPressedEvent event(static_cast<KeyCode>(key), 0);
          KeyPressedEvent event(key, 0);
          data.EventCallback(event);
          break;
        }
        case GLFW_RELEASE:
        {
          KeyReleasedEvent event(key);
          data.EventCallback(event);
          break;
        }
        case GLFW_REPEAT:
        {
          KeyPressedEvent event(key, 1);
          data.EventCallback(event);
          break;
        }
      }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      KeyTypedEvent event(keycode);
      data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch( action ) {
        case GLFW_PRESS:
        {
          MouseButtonPressedEvent event(button);
          data.EventCallback(event);
          break;
        }
        case GLFW_RELEASE:
        {
          MouseButtonReleasedEvent event(button);
          data.EventCallback(event);
          break;
        }
      }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      MouseMovedEvent event((float)xPos, (float)yPos);
      data.EventCallback(event);
    });

}

void Window::Update() {

    glfwPollEvents();
    glfwSwapBuffers( m_Window );

}

void Window::Shutdown() {

    glfwDestroyWindow( m_Window );

}
