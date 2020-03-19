#include "Camera.hpp"

using namespace ENGINE;

Camera::Camera( float fov, float aspectRatio, float near, float far )
    : m_ProjectionMatrix( glm::perspective( glm::radians( fov ), aspectRatio, near, far ) ),
      m_ViewMatrix( 1.0f ) {

          m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

      }

void Camera::SetProjection( float fov, float aspectRatio, float near, float far ) {

     m_ProjectionMatrix = glm::perspective( glm::radians( fov ), aspectRatio, near, far );
     m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

}

void Camera::RecalculateViewMatrix() {

	// calculate the transformation matrix by multiplying the translation of the position
	// by the rotation in the 'z' axis
	// glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), m_Position ) *
		// glm::rotate( glm::mat4( 1.0f ), glm::radians( m_Rotation ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

    // glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), m_Position );

	// actually the View Matrix is the inverse of our Transformation (to get the illusion of movemment)
	// from camera view perspective
	// m_ViewMatrix = glm::inverse( transform );

    m_ViewMatrix = glm::lookAt( m_Position, m_Position + glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );

	// recalculate the ViewProjection multiplication
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

}
void Camera::UpdateCameraVectors() {
            
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos( glm::radians( Yaw ) ) * cos( glm::radians( Pitch ) );
    front.y = sin( glm::radians( Pitch ) );
    front.z = sin( glm::radians( Yaw ) ) * cos( glm::radians( Pitch ) );
    Front = glm::normalize( front );
    // Also re-calculate the Right and Up vector
    Right = glm::normalize( glm::cross( Front, WorldUp ) );
    // Normalize the vectors, because their length gets closer to 0 the more 
    // you look up or down which results in slower movement.
    Up = glm::normalize( glm::cross( Right, Front ) );

}
/*
void Camera::SetViewMatrix() {

    ViewMatrix = glm::lookAt( Position, Position + Front, Up );

}
*/