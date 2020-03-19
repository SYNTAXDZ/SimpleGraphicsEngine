#include "CameraController.hpp"

#include "KeyCodes.hpp"
#include "MouseButtonCodes.hpp"
#include "Input.hpp"

#include "Core.hpp"

#include <iostream>

using namespace ENGINE;


bool firstMouse = true;

float lastX = 400;
float lastY = 300;

CameraController::CameraController( float aspectRatio )
: m_AspectRatio( aspectRatio ), m_Camera( m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f ),
  m_Rotation( 0.0f ) {
      m_Camera.SetPosition( m_CameraPosition );
      //m_CameraPosition = m_Camera.GetPosition();

  }

void CameraController::OnUpdate( ) {

	// the first way of handling CameraPosition is by using the Input System
		// Inside the OnUpdate
	if( Input::IsKeyPressed( KEY_LEFT ) ) {

		m_CameraPosition.x -= m_CameraTranslationSpeed;
		// m_CameraPosition.x -= cos( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;
		// m_CameraPosition.y -= sin( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;

	}
	else if( Input::IsKeyPressed( KEY_RIGHT ) ) {
		
		m_CameraPosition.x += m_CameraTranslationSpeed;
		// m_CameraPosition.x += cos( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;
		// m_CameraPosition.y += sin( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;
	
	}
	if( Input::IsKeyPressed( KEY_UP ) ) {
		
		m_CameraPosition.y += m_CameraTranslationSpeed;
		// m_CameraPosition.x += -sin( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;
		// m_CameraPosition.y += cos( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;

	}
	else if( Input::IsKeyPressed( KEY_DOWN ) ) {
		
		m_CameraPosition.y -= m_CameraTranslationSpeed;
		// m_CameraPosition.x -= -sin( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;
		// m_CameraPosition.y -= cos( glm::radians( m_CameraRotation ) ) * m_CameraTranslationSpeed;
	
	}
	
	m_Camera.SetPosition( m_CameraPosition );

}

void CameraController::OnEvent( Event& e ) {

    EventDispatcher dispatcher( e );
    dispatcher.Dispatch<MouseMovedEvent>( BIND_EVENT( CameraController::OnMouseMoved ) );
    dispatcher.Dispatch<MouseScrolledEvent>( BIND_EVENT( CameraController::OnMouseScrolled ) );
    dispatcher.Dispatch<WindowResizeEvent>( BIND_EVENT( CameraController::OnWindowResized ) );

}

bool CameraController::OnMouseMoved( MouseMovedEvent& e ) {

    if( firstMouse ) {

        lastX = e.GetX();
        lastY = e.GetY();

        firstMouse = false;

    }

    float offsetX = e.GetX() - lastX;
    float offsetY = lastY - e.GetY();
    
    lastX = e.GetX();
    lastY = e.GetY();

    //ProcessMouseMovement( offsetX, offsetY );

    return false;    

}

bool CameraController::OnMouseScrolled( MouseScrolledEvent& e ) {

    m_ZoomLevel -= e.GetYOffset() * 0.25f;
	m_ZoomLevel = std::max( m_ZoomLevel, 0.25f );
	m_Camera.SetProjection( m_ZoomLevel, m_AspectRatio * m_ZoomLevel, 0.1f, 100.0f );
    

}

bool CameraController::OnWindowResized( WindowResizeEvent& e ) {
	
	m_AspectRatio = ( float )e.GetWidth() / ( float )e.GetHeight();
	m_Camera.SetProjection( glm::radians(m_ZoomLevel), m_AspectRatio, 0.1f, 100.0f );

	return false;
}

/*
// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void CameraController::ProcessKeyboard( Camera_Movement direction ) {
    
    //float velocity = m_MovementSpeed * deltatime;
    float velocity = m_MovementSpeed;

    if( direction == FORWARD )

        m_Camera.Position += m_Camera.Front * velocity;
    
    if( direction == BACKWARD )

        m_Camera.Position -= m_Camera.Front * velocity;

    if( direction == RIGHT )

        m_Camera.Position += m_Camera.Right * velocity;
    
    if( direction == LEFT )

        m_Camera.Position -= m_Camera.Right * velocity;

}
*/
/*
// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void CameraController::ProcessMouseMovement( float offsetX, float offsetY, bool constrainPitch ) {

    offsetX *= 0.6f;
    offsetY *= 0.6f;

    m_Camera.Yaw   += offsetX;
    m_Camera.Pitch += offsetY;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if( constrainPitch ) {

        if( m_Camera.Pitch > 89.0f )
            m_Camera.Pitch = 89.0f;
        
        if( m_Camera.Pitch < -89.0f )
            m_Camera.Pitch = -89.0f;

    }

    // don't forget that when the pitch/yaw change you should update your Front, Right, Up vectors
    m_Camera.UpdateCameraVectors();

}
*/
