#pragma once

#include "../Renderer/Camera.hpp"
#include "../Events/Event.hpp"
#include "../Events/MouseEvent.hpp"
#include "../Events/KeyEvent.hpp"
#include "../Events/ApplicationEvent.hpp"

/*
enum Camera_Movement {

    FORWARD,
    BACKWARD,
    RIGHT, 
    LEFT

};
*/
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
        //void ProcessKeyboard( Camera_Movement direction );
        void ProcessMouseMovement( float offsetX, float offsetY, bool constrainPitch = true ); 
        bool OnMouseMoved( MouseMovedEvent& e );
        bool OnMouseScrolled( MouseScrolledEvent& e );
        bool OnWindowResized( WindowResizeEvent& e );

    private:
        float m_AspectRatio;
		float m_ZoomLevel = 45.0f;
		Camera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;


    };

}
