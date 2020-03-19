#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ENGINE {

    class Camera {
    
    public:
    	Camera( float fov, float aspectRatio, float near, float far );

		void SetProjection( float fov, float aspectRatio, float near, float far );
		const glm::vec3& GetPosition() const { return m_Position; };
		void SetPosition( const glm::vec3& position ) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation( const float& rotation ) { m_Rotation = rotation; RecalculateViewMatrix(); }

		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		void UpdateCameraVectors();

    private:
        // this method will recalculate the positio and rotation of our camera after update
		void RecalculateViewMatrix();
        // View, Projection matrices
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		// the position, rotation of our camera
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

    };
    

}