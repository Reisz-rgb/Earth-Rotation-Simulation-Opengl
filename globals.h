#ifndef GLOBALS_H
#define GLOBALS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Window settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// Camera variables 
extern float cameraDistance;
extern float cameraYaw;
extern float cameraPitch;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern bool mousePressed;

// Timing 
extern float deltaTime;
extern float lastFrame;
extern float timeElapsed;

// Sphere data
struct Sphere {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    int indexCount;
};

extern Sphere sphere;

// Orbit data
struct Orbit {
    std::vector<float> vertices;
    unsigned int VAO, VBO;
    int vertexCount;
};

extern Orbit earthOrbitRing;
extern Orbit moonOrbitRing;

#endif 