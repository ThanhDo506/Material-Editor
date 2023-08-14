#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>
#include "../BufferObject/VertexAttributeObject.h"
#include "../BufferObject/ElementBufferObject.h"
#include "../BufferObject/VertexBufferObject.h"
#include "../Render/TextureMap.h"
#include "Shader.h"
#include "../World Entities/Camera.h"

class Mesh
{
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<TextureMap>& textures);
    ~Mesh();
    void draw(Shader& shader, Camera& camera, bool isSameShaderProgram = false);

    void clean();

private:
    VertexAttributeObject _vao;
    VertexBufferObject _vbo;
    ElementBufferObject _ebo;
    std::vector<Vertex>  _vertices;
    std::vector<GLuint>  _indices;
    
    std::vector<TextureMap> _textures;
    
    friend class Model;
};

#endif // !MESH_H
