#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
static constexpr float SPEED = 2.5f;
static constexpr float SENSITIVITY = 0.1f;
static constexpr float ZOOM = 45.0f;

using namespace glm;

class Camera {
  public:
    // camera Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float WindowWidth;
    float WindowHeight;
    float lastY;
    float lastX;
    bool firstMouse = true;

    Camera() = default;

    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch, float Width, float Height)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f))
        , MovementSpeed(SPEED)
        , MouseSensitivity(SENSITIVITY)
        , Zoom(ZOOM)
        , WindowWidth(Width)
        , WindowHeight(Height)
        , lastY(Height / 2)
        , lastX(Width / 2) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void processKeyboard(GLFWwindow* window, float delta_time) {
        float velocity = MovementSpeed * delta_time;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            velocity *= 4;
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            velocity = 0.001;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Position += Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Position -= Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Position -= Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Position += Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            Position -= Up * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            Position += Up * velocity;
    }

    void processMouseMovement(float xposIn, float yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;  // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

  private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
