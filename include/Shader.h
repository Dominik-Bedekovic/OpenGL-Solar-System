#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

class Shader {
    static std::string readSource(const char* path) {
        std::ifstream input(path);
        if (!input) throw std::runtime_error(std::string("Cannot read shader: ") + path);
        std::ostringstream text;
        text << input.rdbuf();
        return text.str();
    }
    static GLuint compile(GLenum kind, const std::string& source, const char* path) {
        GLuint shader = glCreateShader(kind);
        const char* text = source.c_str();
        glShaderSource(shader, 1, &text, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[4096] = {};
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            glDeleteShader(shader);
            throw std::runtime_error(std::string("Shader compilation failed: ") + path + "\n" + log);
        }
        return shader;
    }
public:
    GLuint ID = 0;
    Shader(const char* vertexPath, const char* fragmentPath) {
        const auto vertexSource = readSource(vertexPath);
        const auto fragmentSource = readSource(fragmentPath);
        GLuint vertex = 0, fragment = 0;
        try {
            vertex = compile(GL_VERTEX_SHADER, vertexSource, vertexPath);
            fragment = compile(GL_FRAGMENT_SHADER, fragmentSource, fragmentPath);
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            GLint success;
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                char log[4096] = {};
                glGetProgramInfoLog(ID, sizeof(log), nullptr, log);
                throw std::runtime_error(std::string("Shader linking failed:\n") + log);
            }
        } catch (...) {
            if (vertex) glDeleteShader(vertex);
            if (fragment) glDeleteShader(fragment);
            Delete();
            throw;
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    ~Shader() { Delete(); }
    void Activate() const { glUseProgram(ID); }
    void Delete() { if (ID) { glDeleteProgram(ID); ID = 0; } }
    void setMat4(const std::string& name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }
};
#endif
