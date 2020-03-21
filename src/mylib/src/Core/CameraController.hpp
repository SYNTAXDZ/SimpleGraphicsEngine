#pragma once

#include "../Renderer/Camera.hpp"
#include "../Events/Event.hpp"
#include "../Events/MouseEvent.hpp"
#include "../Events/KeyEvent.hpp"
#include "../Events/ApplicationEvent.hpp"

namespace ENGINE {

    class CameraController {

    public:
        CameraController( float aspectRatio );

        void OnUpdate();
        void OnEvent( Event& e );

        Camera& GetCamera() { return m_Camera; }
        void SetZoomLevel( float level ) { m_ZoomLevel = level; }
		float GetZoomLevel() const { return m_ZoomLevel; }
    private:
        void ProcessMouseMovement( float offsetX, float offsetY, bool constrainPitch = true ); 
        bool OnMouseMoved( MouseMovedEvent& e );
        bool OnMouseScrolled( MouseScrolledEvent& e );
        bool OnWindowResized( WindowResizeEvent& e );

    private:
        float m_AspectRatio;
		float m_ZoomLevel = 45.0f;
		
        Camera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 3.0f };
		float m_CameraTranslationSpeed = 0.1f;


    };

}
