#ifndef MODEL_H
#define MODEL_H
#include <vector>

#include "Material.h"
#include "Mesh.h"

class Model : public Entity
{
public:
    Model(std::string const &path,
          Transform         transform = Transform::identity(),
          std::string const &name   = "Model",
          Entity*           parent    = nullptr);
    ~Model();
private:
    std::vector<Mesh> _meshes;
    Material          _material;
    std::string       _directory;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    void loadModel(std::string const &path);
    void loadMaterial(aiMaterial *mat, aiTexture textureType, TextureMapType tex);

    void draw(Shader &shader, Camera &camera);
    void init() override;
    void update() override;

    friend class GUI;
};


#endif