#include "Skybox.h"

Skybox::Skybox(std::vector<std::string> imagesPaths, const char* vertPath, const char* fragPath)
{
    TextureParams params = {
        false,
        false,
        TextureFiltering::BILINEAR,
        TextureWrapMode::CLAMP_TO_EDGE,
        glm::vec4(1.0,1.0,1.0,1.0),
        MipGeneration::ON_LOAD,
        -1 // With any value, Cube Texture's mipmap level will alway be set to 1
    };
    _texture = Texture::loadCubemap(imagesPaths, params);
    _shader = new Shader(vertPath, fragPath);
    _shader->Activate();
    _shader->setInt("_Skybox", 0);

    _vao.Bind();
    _vbo.setBuffer(vertexPos);
    _ebo.setBuffer(indices);
    _vao.linkAttrib(_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
    _vao.Unbind();
    _vbo.Unbind();
    _ebo.Unbind();
}

Skybox::Skybox(std::string const& imagesPaths, const char* vertPath, const char* fragPath)
{
    TextureParams params = {
        false,
        false,
        TextureFiltering::BILINEAR,
        TextureWrapMode::CLAMP_TO_EDGE,
        glm::vec4(1.0,1.0,1.0,1.0),
        MipGeneration::ON_LOAD,
        -1 // With any value, Cube Texture's mipmap level will alway be set to 1
    };
    _texture = Texture::loadHdr(imagesPaths.c_str(), params);
    _shader = new Shader(vertPath, fragPath);
    _shader->Activate();
    _shader->setInt("_Skybox", 0);
}

void Skybox::draw(Camera& camera)
{
    glDepthFunc(GL_LEQUAL);

    //_shader->Activate();
    // call direct for a bit better performance :D
    // i know what i do so i 'll make a render pipeline in the future, DoNT
    glUseProgram(_shader->_id);
    
    glUniformMatrix4fv(glGetUniformLocation(_shader->_id, "_Projection"),
        1, GL_FALSE, glm::value_ptr(camera.getPerspectiveProjectionMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(_shader->_id, "_View"),
        1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glm::mat4 mtxTransform = glm::translate(glm::mat4(1.0), camera.transform.position);
    glUniformMatrix4fv(glGetUniformLocation(_shader->_id, "_Transform"),
            1, GL_FALSE, glm::value_ptr(mtxTransform));

    glBindVertexArray(_vao._id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture._id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
}

void Skybox::clean()
{
    _vao.Delete();
    _vbo.Delete();
    _ebo.Delete();
}