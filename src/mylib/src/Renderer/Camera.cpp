#include "Camera.hpp"

using namespace ENGINE;

Camera::Camera( float fov, float aspectRatio, float near, float far )
    : m_ProjectionMatrix( glm::perspective( glm::radians( fov ), aspectRatio, near, far ) ),
      m_ViewMatrix( 1.0f ) {

        Yaw = -90.0f;
        Pitch = 0.0f;
        Sensetivity = 0.1f;
        WorldUp = Up;
        UpdateCameraVectors();

      }

void Camera::SetProjection( float fov, float aspectRatio, float near, float far ) {

     m_ProjectionMatrix = glm::perspective( glm::radians( fov ), aspectRatio, near, far );

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


void Camera::SetViewMatrix() {

    m_ViewMatrix = glm::lookAt( m_Position, m_Position + Front, Up );

}
