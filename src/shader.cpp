//
// Created by cyphics on 11/3/22.
//

#include "shader.h"
#include "helpers.h"

std::string OpenFile(const std::string& path) {
    std::string code;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream stream;
        stream << shaderFile.rdbuf();
        shaderFile.close();
        code = stream.str();
    } catch (std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ " << path << " : " << strerror(errno) <<  std::endl;
    }
    return code;
}

std::string getShaderTypeString(GLenum shaderType) {
    switch (shaderType) {
        case GL_VERTEX_SHADER:
            return "VERTEX_SHADER";
        case GL_FRAGMENT_SHADER:
            return "FRAGMENT_SHADER";
        case GL_GEOMETRY_SHADER:
            return "GEOMETRY_SHADER";
        default:
            return "UNKNOWN";
    }
}

void Shader::setShader(GLenum shaderType, const std::string &filePath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string shaderCodeRaw = OpenFile("../shaders/" + filePath);
    const char* shaderCode = shaderCodeRaw.c_str();
    int success;
    char infoLog[512];
    unsigned int *shaderID;
    switch (shaderType) {
        case GL_VERTEX_SHADER:
            shaderID = &vertexID;
            break;
        case GL_FRAGMENT_SHADER:
            shaderID = &fragmentID;
            break;
        case GL_GEOMETRY_SHADER:
            shaderID = &geometryID;
            break;
    }
    // 2. Compile shaders
    *shaderID = glCreateShader(shaderType);
    glShaderSource(*shaderID, 1, &shaderCode, NULL);
    glCompileShader(*shaderID);
    glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED for " << filePath << "\n" << infoLog << std::endl;
        exit(1);
    }
    std::cout << "Shader " << getShaderTypeString(shaderType) << " set with value " << *shaderID << std::endl;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath, const char* identifier) {
    setShader(GL_VERTEX_SHADER, vertexPath);
    setShader(GL_FRAGMENT_SHADER, fragmentPath);
}

void Shader::linkShaders() {
    int success;
    char infoLog[512];

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    if (geometryID > 0) {
        glAttachShader(ID, geometryID);
    }
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);
    // print linking errors if any
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        exit(1);
    }

    // delete shaders; the're linked into our program and are no longer necessary
    glDeleteShader(vertexID);
    if (geometryID > -1) {
        glDeleteShader(geometryID);
    }
    glDeleteShader(fragmentID);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool (const std::string &name, bool  value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt  (const std::string &name, int   value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cout << "WARNING::SHADER::LOCATION Location " << name << " not found." << std::endl;
    }
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string &name, float d, float d1, float d2) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), d, d1, d2);
}

void Shader::setVec3(const std::string &name, glm::vec3 vector) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

