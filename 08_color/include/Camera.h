#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

enum CameraMovement
{
    FORWORD,
    BACKWORD,
    LEFT,
    RIGHT
};

//camera的一些默认参数值
const float PITCH = 0.0f;
const float YAW = -90.0f;
const float ZOOM = 45.0f;
const float SPEED = 2.5f;     //上下左右方向的移动速度
const float SENSITIVE = 0.1f; //鼠标的默认灵敏度

class Camera
{
private:
    void updateCameraVectors();
    glm::mat4 myLookAt(glm::vec3 position, glm::vec3 center, glm::vec3 up);

public:
    glm::vec3 position;
    glm::vec3 front;   //z轴
    glm::vec3 up;      //y轴
    glm::vec3 right;   //x轴
    glm::vec3 worldUp; //世界坐标向上

    float pitch;
    float yaw;

    float movementSpeed;
    float mouseSensitive;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitive(SENSITIVE), zoom(ZOOM)
    {
        this->position = position;
        this->worldUp = worldUp;
        this->yaw = yaw;
        this->pitch = pitch;

        updateCameraVectors();
    }

    ~Camera();

    glm::mat4 getViewMatrix()
    {
        return myLookAt(position, position + front, up);
    }

    void processKeyboard(CameraMovement direction, float deltaTime)
    {
        float cameraSpeed = movementSpeed * deltaTime;
        if (direction == FORWORD)
        {
            position += cameraSpeed * front;
        }

        if (direction == BACKWORD)
        {
            position -= cameraSpeed * front;
        }

        if (direction == LEFT)
        {
            position -= right * cameraSpeed;
        }

        if (direction == RIGHT)
        {
            position += right * cameraSpeed;
        }

        position.y = 0.0f;
    }

    void processMouseMovement(float xOffset, float yOffset)
    {
        pitch += yOffset * mouseSensitive;
        // 限制俯仰角不能超过-89～89
        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }
        yaw += xOffset * mouseSensitive;

        std::cout << "pitch=" << pitch << "  yaw=" << yaw << std::endl;

        updateCameraVectors();
    }

    void processMouseScroll(float yOffset)
    {
        zoom -= yOffset;
        if (zoom < 1.0f)
        {
            zoom = 1.0f;
        }
        if (zoom > 45.0f)
        {
            zoom = 45.0f;
        }
    }
};

Camera::~Camera()
{
}

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::myLookAt(glm::vec3 position, glm::vec3 center, glm::vec3 up)
{
    glm::vec3 direction = glm::normalize(position - center);
    glm::vec3 cameraRight = glm::normalize(glm::cross(direction, up));
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, direction));

    glm::mat4 view;

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = cameraRight.x;
    rotation[1][0] = cameraRight.y;
    rotation[2][0] = cameraRight.z;

    rotation[0][1] = cameraUp.x;
    rotation[1][1] = cameraUp.y;
    rotation[2][1] = cameraUp.z;

    rotation[0][2] = direction.x;
    rotation[1][2] = direction.y;
    rotation[2][2] = direction.z;

    view = rotation * translation;

    return view;
}

#endif