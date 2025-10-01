#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

// GLFW Callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Input processing
void processInput(GLFWwindow* window);

#endif