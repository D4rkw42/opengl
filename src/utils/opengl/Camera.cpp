// Definições para câmera

#include "Camera.hpp"

Camera::Camera(const glm::vec3& position, int width, int height) {
    this->position = position;
    this->width = width;
    this->height = height;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, const Shader &shader, const char *uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(this->position, this->position + this->orientation, this->up);
    proj = glm::perspective(glm::radians(FOVdeg), static_cast<float>(this->width) / this->height, nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(proj * view));
}

void Camera::Inputs(GLFWwindow *window, float delta) {
    // keyboard

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->position += this->speed * this->orientation * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->position += this->speed * -glm::normalize(glm::cross(this->orientation, this->up)) * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->position += this->speed * -this->orientation * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->position += this->speed * glm::normalize(glm::cross(this->orientation, this->up)) * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->position += this->speed * this->up * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        this->position += this->speed * -this->up * delta;
    }

    // mouse
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (this->firstClick) {
            glfwSetCursorPos(window, (this->width / 2), (this->height / 2));
            this->firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = this->sensivity * (float)(mouseY - (height / 2)) / height;
		float rotY = this->sensivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		//glm::vec3 newOrientation = glm::rotate(this->orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->orientation, this->up)));

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotX), glm::normalize(glm::cross(this->orientation, this->up)));
        glm::vec3 newOrientation = glm::vec3(rotationMatrix * glm::vec4(this->orientation, 0.0f));
        this->orientation = newOrientation;

		// Decides whether or not the next vertical Orientation is legal or not
		// if (abs(glm::oriented_angle(newOrientation, this->up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		// {
		// 	this->orientation = newOrientation;
		// }

		// Rotates the Orientation left and right
		//this->orientation = glm::rotate(this->orientation, glm::radians(-rotY), this->up);

        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-rotY), this->up);
        newOrientation = glm::vec3(rotationMatrix * glm::vec4(this->orientation, 0.0f));
        this->orientation = newOrientation;

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        this->firstClick = true;
    }
}
