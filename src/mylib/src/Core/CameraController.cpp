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
: m_AspectRatio( aspectRatio ), m_Camera( m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f ) {}

void CameraController::OnUpdate( ) {

	// the first way of handling CameraPosition is by using the Input System
		// Inside the OnUpdate
	if( Input::IsKeyPressed( KEY_A ) ) {

		m_CameraPosition -= m_Camera.Right * m_CameraTranslationSpeed;

	}
	else if( Input::IsKeyPressed( KEY_D ) ) {
		
        m_CameraPosition += m_Camera.Right * m_CameraTranslationSpeed;
	
	}
	if( Input::IsKeyPressed( KEY_W ) ) {
		
        m_CameraPosition += m_Camera.Front * m_CameraTranslationSpeed;

	}
	else if( Input::IsKeyPressed( KEY_S ) ) {
		
		m_CameraPosition -= m_Camera.Front * m_CameraTranslationSpeed;
	
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

    ProcessMouseMovement( offsetX, offsetY );

    return false;    

}

bool CameraController::OnMouseScrolled( MouseScrolledEvent& e ) {
    
    if( m_ZoomLevel >= 1.0f && m_ZoomLevel <= 45.0f )
        m_ZoomLevel -= e.GetYOffset();
    
    if( m_ZoomLevel <= 1.0f )
        m_ZoomLevel = 1.0f;
    
    if(m_ZoomLevel >= 45.0f)
        m_ZoomLevel -= e.GetYOffset();
    
	m_Camera.SetProjection( m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f );
    
    return false;
}

bool CameraController::OnWindowResized( WindowResizeEvent& e ) {
	
	m_AspectRatio = ( float )e.GetWidth() / ( float )e.GetHeight();
	m_Camera.SetProjection( m_ZoomLevel, m_AspectRatio, 0.1f, 100.0f );

	return false;
}

void CameraController::ProcessMouseMovement( float offsetX, float offsetY, bool constrainPitch ) {

    offsetX *= m_Camera.Sensetivity;
    offsetY *= m_Camera.Sensetivity;

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
