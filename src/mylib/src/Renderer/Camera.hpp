#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ENGINE {

    class Camera {
    
    public:
        float Yaw;
        float Pitch;
        float Sensetivity;
        glm::vec3 Front = glm::vec3( 0.0f, 0.0f, 1.0f );
        glm::vec3 Right = glm::vec3( 1.0f, 0.0f, 0.0f );
        glm::vec3 Up = glm::vec3( 0.0f, 1.0f, 0.0f );
        glm::vec3 WorldUp;
        
    public:
        Camera( float fov, float aspectRatio, float near, float far );

        void SetProjection( float fov, float aspectRatio, float near, float far );
        void SetViewMatrix();
        const glm::vec3& GetPosition() const { return m_Position; };
        void SetPosition( const glm::vec3& position ) { m_Position = position; SetViewMatrix(); }

        //float GetRotation() const { return m_Rotation; }
        //void SetRotation( const float& rotation ) { m_Rotation = rotation; RecalculateViewMatrix(); }

        glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
        glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
        //glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        void UpdateCameraVectors();

    private:
        // this method will recalculate the positio and rotation of our camera after update
        //void RecalculateViewMatrix();
            // View, Projection matrices
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        //glm::mat4 m_ViewProjectionMatrix;

        // the position, rotation of our camera
        glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
        //float m_Rotation = 0.0f;

    };


}
