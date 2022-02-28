#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};


class CameraHandler
{
public:

    //Projection Variables
    float m_aspectRatio;
    float m_dimension;
    float m_fov;

    //View Variables
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    glm::vec3 m_up;

    //Camera Variables
    float m_speed;
    float m_sensitivity;
    float m_yaw;
    float m_pitch;

    int m_mode;
    

    CameraHandler(float aspectRatio = float(8/6), float dimension = 100.0f , float fov = 45.0f, float speed = 1.0f, float sensitivity = 0.1f) : m_aspectRatio(aspectRatio), m_dimension(dimension), m_fov(fov), m_speed(speed), m_sensitivity(sensitivity)
    {
        m_position = glm::vec3(0.0f, 0.4f, 1.5f);
        m_target = glm::vec3(0.0f, -0.4f, -1.0f);
        m_up = m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_yaw = -90.0f;
        m_pitch = 0.0f;
        m_sensitivity = 0.8f;
        m_right = glm::normalize(glm::cross(m_target, m_up));
        m_mode = 0;
    }

    ~CameraHandler();

    glm::mat4 getViewMatrix(){
        return glm::lookAt(m_position, m_position + m_target, m_up);
    }

    glm::mat4 getProjectionMatrix(){
        
        if(m_mode == 0)
            return glm::perspective(m_fov, m_aspectRatio, 0.1f, m_dimension);
        else
            return glm::ortho(-5.0f/2,5.0f/2, -5.0f/2, 5.0f/2, 0.01f, 2 * m_dimension);
    }

    void processKeyboard(Camera_Movement direction, float deltaTime){
        float velocity = m_speed * deltaTime;

        if (direction == FORWARD)
            m_position += m_target * velocity;
        if (direction == RIGHT)
            m_position += m_right * velocity;
        if (direction == BACKWARD)
            m_position -= m_target * velocity;
        if (direction == LEFT)
            m_position -= m_right * velocity;
        if (direction == UP)
            m_position += m_up * velocity;
        if (direction == DOWN)
            m_position -= m_up * velocity;

    }

    void ProcessMouseMovement(float xoffset, float yoffset, float deltaTime, bool constrainPitch = true)
    {
        xoffset *= m_sensitivity * deltaTime;
        yoffset *= m_sensitivity * deltaTime;

        m_yaw   += xoffset;
        m_pitch -= yoffset;

        if (constrainPitch)
        {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        updateVectors();
    }

    CameraHandler * updateVectors(){
        m_target = glm::vec3(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)), sin(glm::radians(m_pitch)), sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
        m_target = glm::normalize(m_target);

        m_right = glm::normalize(glm::cross(m_target, m_world_up));
        m_up = glm::normalize(glm::cross(m_right, m_target));

        return this;
    }

};
