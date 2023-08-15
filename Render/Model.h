#ifndef MODEL_H
#define MODEL_H
#include <vector>

#include "Material.h"
#include "Mesh.h"

class Model : public Entity
{
public:
    Model(std::string const &path,
          std::string const &vertShaderpath,
          std::string const &fragShaderpath,
          Transform         transform = Transform::identity(),
          std::string const &name   = "Model",
          Entity*           parent    = nullptr);
private:
    std::vector<Mesh>   _meshes;
    Material            _material;
    std::string         _directory;
    
    // TODO: shouldn't have shader attribute, mush have a Shader holder if want to make optimize render pepline
    Shader              *p_shader;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    void loadModel(std::string const &path);
    void loadMaterial(aiMaterial *mat, aiTexture textureType, TextureMapType tex);

    void draw(Camera &camera);
    void init() override;
    void update() override;
    void clean();

    friend class GUI;
    friend class Application;
    friend class WorldManager;
};


#endif