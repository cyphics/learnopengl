//
// Created by cyphics on 11/3/22.
//

#ifndef OPENGL_HELPERS_H
#define OPENGL_HELPERS_H

#include <string>
#include <iostream>
#include "glad/glad.h"

#define glCheckError() glCheckError_(__FILE__, __LINE__)

inline bool glCheckError_(const char *file, int line) {
    bool gotError;
    GLenum errorCode;
    while((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                   error = "INVALID_ENUM"; break;
            case GL_INVALID_INDEX:                  error = "INVALID_INDEX"; break;
            case GL_INVALID_OPERATION:              error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                 error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:                error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                  error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                                error = "UNKNOWN_ERROR"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return gotError;
}
#endif //OPENGL_HELPERS_H
