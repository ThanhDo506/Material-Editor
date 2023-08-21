#include "Mesh.h"
#include <string>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<TextureMap>& textures)
{
    this->_vertices = vertices;
    this->_indices  = indices;
    this->_textures = textures;

    _vao.Bind();
    _vbo.setBuffer(vertices);
    _ebo.setBuffer(indices);
    
    // Position 3 float
    _vao.linkAttrib(_vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
    // Normal 3 float
    _vao.linkAttrib(_vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    // Color 3 float change to 4 float later if need
    _vao.linkAttrib(_vbo, 2, 4, GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
    // Texture coordinate 2 float
    _vao.linkAttrib(_vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));
    // Tangent 3 float
    _vao.linkAttrib(_vbo, 4, 3, GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));
    
    _vao.Unbind();
    _vbo.Unbind();
    _ebo.Unbind();

}

Mesh::~Mesh()
{
}

void Mesh::draw(Shader& shader,
    Camera& camera,
    std::string const &uniformPrefix,
    bool isSameShaderProgram)
{
    if(!isSameShaderProgram)
    {
        glUseProgram(shader._id);
        // or can use below line instead
        //shader.Activate();
        shader.setMat4("_View", camera.getViewMatrix());
        shader.setMat4("_Projection", camera.getPerspectiveProjectionMatrix());
        for (unsigned int i = 0; i < _textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string type = TextureMap::to_string(_textures[i].getType());
            if(!uniformPrefix.empty())
            {
                type = uniformPrefix + type;
            }
            // call directly
            glUniform1i(glGetUniformLocation(shader._id, type.c_str()), i);
            glBindTexture(GL_TEXTURE_2D, _textures[i]._texture._id);
        }
    }
    glBindVertexArray(_vao._id);
    // Draw the mesh
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::clean()
{
    _vao.Delete();
    _ebo.Delete();
    _vbo.Delete();
}
