#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "globals.h"

void createSphere(float radius, int sectors, int stacks);
void drawPlanet(unsigned int shaderProgram, glm::mat4 model, unsigned int textureID, float emission);
void createOrbitRing(Orbit& orbit, float radius, int segments);
void drawOrbit(unsigned int shaderProgram, const Orbit& orbit, glm::mat4 model, glm::vec3 color);
unsigned int loadTexture(const char* path);

#endif 