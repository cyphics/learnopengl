//
// Created by cyphics on 10/25/22.
//

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

std::string OpenFile(const char* path);

class Shader {
public:
    // the program ID
    unsigned int ID;
    unsigned int shaderIDs[3];
    const char* identifier;

    // constructor reads and builds the shader
//    Shader(const char* vertexPath, const char* fragmentPath, const char* identifier = "");
    Shader(const std::string &vertexPath, const std::string &fragmentPath, const char* identifier = "");
    // use/activate the shader
    void setShader(GLenum shaderType, const std::string &filePath);
    void linkShaders();
    void use();
    // utility uniform functions
    void setBool (const std::string &name, bool  value) const;
    void setInt  (const std::string &name, int   value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;
    void setVec3(const std::string &name, float d, float d1, float d2) const;
    void setVec3(const std::string &name, glm::vec3 vector) const;

};
#endif //OPENGL_SHADER_H
