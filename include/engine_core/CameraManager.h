/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

enum Camera_Movement {
    CAM_FORWARD,    // W
    CAM_BACKWARD,   // S
    CAM_LEFT,       // A
    CAM_RIGHT,      // D
    CAM_UP,         // Q
    CAM_DOWN        // E
};

class CameraManager
{
public:
    glm::vec3 position;

    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 world_up = up;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float zoom = 45.0f;

    float movement_speed = 100.0f;
    float mouse_sensitivity = 0.1f;

    CameraManager (glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f))
    {
        this->position = position;
        UpdateCameraVectors();
    }

    CameraManager(
        float posX,
        float posY,
        float posZ,
        float upX,
        float upY,
        float upZ
    )
    {
        position = glm::vec3(posX, posY, posZ);
        world_up = glm::vec3(upX, upY, upZ);

        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        yoffset *= mouse_sensitivity;
        xoffset *= mouse_sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        pitch = std::min(pitch, 89.0f);
        pitch = std::max(pitch, -89.0f);

        UpdateCameraVectors();
    }

    void UpdateCameraVectors()
    {
    	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    	front.y = sin(glm::radians(pitch));
    	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
    }

    void ProcessKeyboard(Camera_Movement direction, float delta_time)
    {
        switch (direction) {
            case CAM_FORWARD:
                position += front * movement_speed * delta_time;
                break;

            case CAM_BACKWARD:
                position -= front * movement_speed * delta_time;
                break;

            case CAM_LEFT:
                position -= glm::normalize(glm::cross(front, world_up)) * movement_speed * delta_time;
                break;

            case CAM_RIGHT:
                position += glm::normalize(glm::cross(front, world_up)) * movement_speed * delta_time;
                break;

            case CAM_UP:
                position += world_up * movement_speed * delta_time * 0.5f;
                break;

            case CAM_DOWN:
                position -= world_up * movement_speed * delta_time * 0.5f;
                break;
        }
    }
};

