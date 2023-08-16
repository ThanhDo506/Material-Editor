#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader
{
#ifndef MAX_MESSAGE_LENGTH
#define MAX_MESSAGE_LENGTH 1024
#endif
public:
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        PROGRAM
    };

    Shader(const char* vertFilepath, const char* fragFilepath);

    GLuint getId() const;

    void Activate();
    void Delete();

    GLint getUniformLocation(const char* name);

#pragma region Modify uniform
    void setBool(const char* uniform, bool value) const
    {
        glUniform1i(glGetUniformLocation(this->_id, uniform), static_cast<int>(value));
    }
    void setBool(std::string const &uniform, bool value) const
    {
        setBool(uniform.c_str(), value);
    }

    void setInt(const char* uniform, int value) const
    {
        glUniform1i(glGetUniformLocation(this->_id, uniform), value);
    }
    void setInt(std::string const &uniform, int value) const
    {
        setInt(uniform.c_str(), value);
    }

    void setFloat(const char* uniform, float value) const
    {
        glUniform1f(glGetUniformLocation(this->_id, uniform), value);
    }
    void setFloat(std::string const &uniform, float value) const
    {
        setFloat(uniform.c_str(), value);
    }

    void setVec2(const char* uniform, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(this->_id, uniform), 1, &value[0]);
    }
    void setVec2(std::string const &uniform, const glm::vec2& value) const
    {
        setVec2(uniform.c_str(), value);
    }

    void setVec2(const char* uniform, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(this->_id, uniform), x, y);
    }
    void setVec2(std::string const &uniform, float x, float y) const
    {
        setVec2(uniform.c_str(), x, y);
    }

    void setVec3(const char* uniform, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(this->_id, uniform), 1, &value[0]);
    }
    void setVec3(std::string const &uniform, const glm::vec3& value) const
    {
        setVec3(uniform.c_str(), value);
    }

    void setVec3(const char* uniform, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(this->_id, uniform), x, y, z);
    }
    void setVec3(std::string const &uniform, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(this->_id, uniform.c_str()), x, y, z);
    }

    void setVec4(const char* uniform, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(this->_id, uniform), 1, &value[0]);
    }
    void setVec4(std::string const &uniform, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(this->_id, uniform.c_str()), 1, &value[0]);
    }

    void setVec4(const char* uniform, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(this->_id, uniform), x, y, z, w);
    }
    void setVec4(std::string const &uniform, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(this->_id, uniform.c_str()), x, y, z, w);
    }

    void setMat2(const char* uniform, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(this->_id, uniform), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat2(std::string const &uniform, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(this->_id, uniform.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const char* uniform, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(this->_id, uniform), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat3(std::string const &uniform, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(this->_id, uniform.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const char* uniform, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(this->_id, uniform), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(std::string const &uniform, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(this->_id, uniform.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

#pragma endregion

private:
    static void getGLError(const GLuint& id, ShaderType glType);
    GLuint      _id;

    friend class Mesh;
    friend class Model;
    friend class Skybox;
};

#endif // !SHADER_H
