#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

#include "globals.h"
#include "shader.h"
#include "geometry.h"
#include "input.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float timeElapsed = 0.0f;


int main() {
    if(!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System - Modern OpenGL", NULL, NULL);
    if(!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    unsigned int shaderProgram = createShaderProgram(); 
    createSphere(1.0f, 36, 18);
    
    unsigned int planetShaderProgram = createShaderProgram(); 
    unsigned int orbitShaderProgram = createSimpleShaderProgram(); 

    stbi_set_flip_vertically_on_load(true);
    unsigned int sunTexture = loadTexture("textures/sun.jpg");     
    unsigned int earthTexture = loadTexture("textures/earth.jpg"); 
    unsigned int moonTexture = loadTexture("textures/moon.jpg");   
    
    // Create sphere geometry
    createSphere(1.0f, 36, 18);
    
    // Create orbit rings 
    const int ORBIT_SEGMENTS = 100;
    createOrbitRing(earthOrbitRing, 16.0f, ORBIT_SEGMENTS); 
    createOrbitRing(moonOrbitRing, 2.5f, ORBIT_SEGMENTS);  

    
    std::cout << "=== Simulator ===" << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "  Left Mouse + Drag: Rotate camera" << std::endl;
    std::cout << "  Scroll Wheel: Zoom in/out" << std::endl;
    std::cout << "  ESC: Exit" << std::endl << std::endl;
    
    while(!glfwWindowShouldClose(window)) {
        // Timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timeElapsed += deltaTime * 0.5f; 
        
        processInput(window); 
        
        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(planetShaderProgram);
        glUseProgram(shaderProgram);
        
        // Camera setup
        glm::vec3 cameraPos;
        cameraPos.x = cameraDistance * cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw));
        cameraPos.y = cameraDistance * sin(glm::radians(cameraPitch));
        cameraPos.z = cameraDistance * cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw));
        
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
        
        // Set camera and light uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 0.0f, 0.0f, 0.0f); 
        
        // Bind Sphere VAO once for all draw calls
        glBindVertexArray(sphere.VAO);
        
        // Sun
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, timeElapsed * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f));
        drawPlanet(shaderProgram, model, sunTexture, 1.5f); // High emission
        
        // Earth Orbit
        glm::mat4 earthOrbit = glm::mat4(1.0f);
        earthOrbit = glm::rotate(earthOrbit, timeElapsed * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        earthOrbit = glm::translate(earthOrbit, glm::vec3(16.0f, 0.0f, 0.0f));

        // Earth
        model = earthOrbit;
        model = glm::rotate(model, glm::radians(23.5f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, timeElapsed * 5.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        drawPlanet(shaderProgram, model, earthTexture, 0.0f); // No emission
        
        // Moon
        model = earthOrbit;
        model = glm::rotate(model, timeElapsed * 7.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        drawPlanet(shaderProgram, model, moonTexture, 0.0f); // No emission

        glUseProgram(orbitShaderProgram);

         // Set camera uniforms for the orbit shader
        glUniformMatrix4fv(glGetUniformLocation(orbitShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(orbitShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        // Earth Orbit Ring (centered at 0,0,0)
        glm::mat4 orbitModel = glm::mat4(1.0f);
        drawOrbit(orbitShaderProgram, earthOrbitRing, orbitModel, glm::vec3(0.5f, 0.5f, 0.5f)); // Light gray
        drawOrbit(orbitShaderProgram, moonOrbitRing, earthOrbit, glm::vec3(0.3f, 0.3f, 0.3f)); // Darker gray

 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &sphere.VAO);
    glDeleteBuffers(1, &sphere.VBO);
    glDeleteBuffers(1, &sphere.EBO);
    
    // Clean up orbit resources
    glDeleteVertexArrays(1, &earthOrbitRing.VAO);
    glDeleteBuffers(1, &earthOrbitRing.VBO);
    glDeleteVertexArrays(1, &moonOrbitRing.VAO);
    glDeleteBuffers(1, &moonOrbitRing.VBO);
    
    glDeleteProgram(planetShaderProgram);
    glDeleteProgram(orbitShaderProgram); 
    glfwTerminate();
    return 0;
}