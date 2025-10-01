#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

unsigned int compileShader(const char* source, GLenum type);
unsigned int createShaderProgram();
unsigned int createSimpleShaderProgram(); 

extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;

#endif 