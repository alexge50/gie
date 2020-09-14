//
// Created by alex on 9/13/20.
//

#ifndef NODE_EDITOR_SHADER_H
#define NODE_EDITOR_SHADER_H

#include <glad/glad.h>
#include <optional>
#include <string>
#include <vector>
#include <iostream>

struct Shader
{
    unsigned int programId = 0;

    Shader(unsigned int programId = 0): programId{programId} {}

    Shader(const Shader&) = delete;
    Shader(Shader&& other)
    {
        programId = other.programId;
        other.programId = 0;
    }

    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other) noexcept
    {
        glDeleteProgram(programId);
        programId = other.programId;
        other.programId = 0;

        return *this;
    }

    ~Shader()
    {
        glDeleteProgram(programId);
    }

    void use()
    {
        glUseProgram(programId);
    }

    auto getUniformLocation(const char* name)
    {
        return glGetUniformLocation(programId, name);
    }
};

std::optional<Shader> createShader(
        const std::string& vertexShaderSource,
        const std::string& fragmentShaderSource
)
{
    const char* text;

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &(text = vertexShaderSource.c_str()), nullptr);
    glCompileShader(vertexShader);

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &(text = fragmentShaderSource.c_str()), nullptr);
    glCompileShader(fragmentShader);

    auto shaders = {
            vertexShader,
            fragmentShader
    };

    for(auto shader: shaders)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

            std::cout << &errorLog[0] << std::endl;

            glDeleteShader(shader); // Don't leak the shader.
            return std::nullopt;
        }
    }


    Shader shader{glCreateProgram()};
    glAttachShader(shader.programId, vertexShader);
    glAttachShader(shader.programId, fragmentShader);
    glLinkProgram(shader.programId);

    return shader;
}

#endif //NODE_EDITOR_SHADER_H
