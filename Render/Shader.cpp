#include "Shader.h"
#include <cerrno>
#include <iostream>
#include "utilities.h"


Shader::Shader(const char* vertFilepath, const char* fragFilepath, std::string const &name)
{
    std::string vertexShaderSource   = utilities::readGLSL(vertFilepath);
    std::string fragmentShaderSource = utilities::readGLSL(fragFilepath);

    const char* vertexSource   = vertexShaderSource.c_str();
    const char* fragmentSource = fragmentShaderSource.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    // Check Compile error
    getGLError(vertexShader, ShaderType::VERTEX);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    // Check Compile error
    getGLError(fragmentShader, ShaderType::FRAGMENT);

    _id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);
    // Check link error
    getGLError(this->_id, ShaderType::PROGRAM);

    glDetachShader(_id, vertexShader);
    glDetachShader(_id, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _name = name;
}

GLuint Shader::getId() const
{
    return _id;
}

std::string Shader::getName() const
{
    return _name;
}

void Shader::setName(std::string const& newName)
{
    _name = newName;
}

void Shader::Activate()
{
    glUseProgram(_id);
}

void Shader::Delete()
{
    glDeleteProgram(_id);
}

GLint Shader::getUniformLocation(const char* name)
{
    return glGetUniformLocation(_id, name);
}

void Shader::getGLError(const GLuint& id, ShaderType glType)
{
    GLint success;
    char  message[MAX_MESSAGE_LENGTH];

    if (glType == ShaderType::PROGRAM)
    {
        glGetShaderiv(id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(id, MAX_MESSAGE_LENGTH, nullptr, message);
            std::cout << "ERROR::SHADER::LINKING_FAILED: " << message << std::endl;
        }
    } else if (glType == ShaderType::FRAGMENT || glType == ShaderType::VERTEX)
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(id, MAX_MESSAGE_LENGTH, nullptr, message);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << message << std::endl;
        }
    }
}
