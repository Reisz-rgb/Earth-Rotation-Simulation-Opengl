#include "geometry.h"
#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>

Sphere sphere;

// Create sphere geometry
void createSphere(float radius, int sectors, int stacks) {
    sphere.vertices.clear();
    sphere.indices.clear();
    
    float x, y, z, xy;
    float nx, ny, nz;
    float sectorStep = 2 * M_PI / sectors;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;
    
    for(int i = 0; i <= stacks; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);
        
        for(int j = 0; j <= sectors; ++j) {
            sectorAngle = j * sectorStep;
            
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            sphere.vertices.push_back(x);
            sphere.vertices.push_back(y);
            sphere.vertices.push_back(z);
            
            // Normals are the same as positions for a unit sphere centered at origin
            nx = x / radius;
            ny = y / radius;
            nz = z / radius;
            sphere.vertices.push_back(nx);
            sphere.vertices.push_back(ny);
            sphere.vertices.push_back(nz);
        }
    }
    
    // Index generation (triangles)
    for(int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;
        
        for(int j = 0; j < sectors; ++j, ++k1, ++k2) {
            // 2 triangles per quad
            if(i != 0) { // first triangle of quad
                sphere.indices.push_back(k1);
                sphere.indices.push_back(k2);
                sphere.indices.push_back(k1 + 1);
            }
            
            if(i != (stacks-1)) { // second triangle of quad
                sphere.indices.push_back(k1 + 1);
                sphere.indices.push_back(k2);
                sphere.indices.push_back(k2 + 1);
            }
        }
    }
    
    sphere.indexCount = sphere.indices.size();
    
    // OpenGL buffer setup
    glGenVertexArrays(1, &sphere.VAO);
    glGenBuffers(1, &sphere.VBO);
    glGenBuffers(1, &sphere.EBO);
    
    glBindVertexArray(sphere.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, sphere.VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(float), 
                 sphere.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indices.size() * sizeof(unsigned int),
                 sphere.indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void drawPlanet(unsigned int shaderProgram, glm::mat4 model, glm::vec3 color, float emission) {
    // Note: glBindVertexArray(sphere.VAO) should be called once per frame in the render loop before calling this function for the first time.
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(color));
    glUniform1f(glGetUniformLocation(shaderProgram, "emissionStrength"), emission);
    glDrawElements(GL_TRIANGLES, sphere.indexCount, GL_UNSIGNED_INT, 0);
}

Orbit earthOrbitRing;
Orbit moonOrbitRing;

// Create orbit ring geometry
void createOrbitRing(Orbit& orbit, float radius, int segments) {
    orbit.vertices.clear();
    
    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / (float)segments * 2.0f * M_PI;
        float x = radius * cosf(angle);
        float z = radius * sinf(angle);
        
        orbit.vertices.push_back(x);
        orbit.vertices.push_back(0.0f); // Orbits in the XZ plane (y=0)
        orbit.vertices.push_back(z);
    }
    
    orbit.vertexCount = orbit.vertices.size() / 3;
    
    // OpenGL buffer setup
    glGenVertexArrays(1, &orbit.VAO);
    glGenBuffers(1, &orbit.VBO);
    
    glBindVertexArray(orbit.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, orbit.VBO);
    glBufferData(GL_ARRAY_BUFFER, orbit.vertices.size() * sizeof(float), 
                 orbit.vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0); // Unbind VAO
}

// Add a draw function for the orbits
void drawOrbit(unsigned int shaderProgram, const Orbit& orbit, glm::mat4 model, glm::vec3 color) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lineColor"), 1, glm::value_ptr(color));
    
    glBindVertexArray(orbit.VAO);
    glDrawArrays(GL_LINE_LOOP, 0, orbit.vertexCount);
    glBindVertexArray(0);
}