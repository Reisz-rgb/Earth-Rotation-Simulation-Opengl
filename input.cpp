#include "globals.h"
#include "input.h"

// Define global variables declared in globals.h
float cameraDistance = 50.0f;
float cameraYaw = 45.0f;
float cameraPitch = 30.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mousePressed = false;

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    if(mousePressed) {
        float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        cameraYaw += xoffset;
        cameraPitch += yoffset;
        
        // Prevent camera flipping
        if(cameraPitch > 89.0f) cameraPitch = 89.0f;
        if(cameraPitch < -89.0f) cameraPitch = -89.0f;
    }
}

// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS) mousePressed = true;
        else if(action == GLFW_RELEASE) mousePressed = false;
    }
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= (float)yoffset * 2.0f;
    if(cameraDistance < 10.0f) cameraDistance = 10.0f;
    if(cameraDistance > 100.0f) cameraDistance = 100.0f;
}

// Process input
void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}