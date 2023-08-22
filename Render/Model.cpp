#include "Model.h"

#include "../utilities/utilities.h"
#include "../World Entities/Manager/ShaderManager.h"
#include "../World Entities/Manager/WorldManager.h"

Model::Model(std::string const  &path,
             std::string const &vertShaderpath,
             std::string const &fragShaderpath,
             Transform          transform,
             std::string const  &name,
             Entity             *parent)
{
    loadModel(path);
    this->transform = transform;
    _name           = name;
    p_parent        = parent;
    p_shader = new Shader(vertShaderpath.c_str(), fragShaderpath.c_str());
    TextureMap defaultTexture;
    defaultTexture._type = TextureMapType::DIFFUSE;
    _material._diffuseMaps.push_back(defaultTexture);
    
    defaultTexture._type = TextureMapType::SPECULAR;
    _material._specularMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::NORMAL;
    _material._normalMap = defaultTexture;

    defaultTexture._type = TextureMapType::METALLIC;
    _material._metallicMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::AO;
    _material._aoMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::ROUGHNESS;
    _material._roughnessMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::EMISSION;
    _material._emissionMaps.push_back(defaultTexture);
}

Model::Model(std::string const& path, Shader* shader, Transform transform, std::string const& name, Entity* parent)
{
    loadModel(path);
    this->transform = transform;
    _name           = name;
    p_parent        = parent;
    p_shader        = shader;
    TextureMap defaultTexture;
    defaultTexture._type = TextureMapType::DIFFUSE;
    _material._diffuseMaps.push_back(defaultTexture);
    
    defaultTexture._type = TextureMapType::SPECULAR;
    _material._specularMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::NORMAL;
    _material._normalMap = defaultTexture;

    defaultTexture._type = TextureMapType::METALLIC;
    _material._metallicMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::AO;
    _material._aoMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::ROUGHNESS;
    _material._roughnessMaps.push_back(defaultTexture);

    defaultTexture._type = TextureMapType::EMISSION;
    _material._emissionMaps.push_back(defaultTexture);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<TextureMap> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex    vertex;
        vertex.position.x        = mesh->mVertices[i].x;
        vertex.position.y        = mesh->mVertices[i].y;
        vertex.position.z        = mesh->mVertices[i].z;
        // normals
        if (mesh->HasNormals())
        {
            vertex.normal.x      = mesh->mNormals[i].x;
            vertex.normal.y      = mesh->mNormals[i].y;
            vertex.normal.z      = mesh->mNormals[i].z;
        }
        if (mesh->mColors[0])
        {
            vertex.color.r      = mesh->mColors[0][i].r;
            vertex.color.g      = mesh->mColors[0][i].g;
            vertex.color.b      = mesh->mColors[0][i].b;
            vertex.color.a      = mesh->mColors[0][i].a;
        }
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoord.x           = mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y           = mesh->mTextureCoords[0][i].y;
            // tangent
            vertex.tangent.x       = mesh->mTangents[i].x;
            vertex.tangent.y       = mesh->mTangents[i].y;
            vertex.tangent.z       = mesh->mTangents[i].z;
        } else
        {
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
            vertex.tangent = glm::vec3(0.0f,0.0f,0.0f);
        }
        vertices.push_back(vertex);
    }
    // process faces
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    // // process materials
    // aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    //
    // std::vector<TextureMap> diffuseMaps = loadMaterial(material,TextureMapType::DIFFUSE);
    // _material._diffuseMaps.insert(_material._diffuseMaps.end(), diffuseMaps.begin(), diffuseMaps.end());
    //
    // std::vector<TextureMap> specularMaps = loadMaterial(material,TextureMapType::SPECULAR);
    // _material._specularMaps.insert(_material._specularMaps.end(), specularMaps.begin(), specularMaps.end());
    //
    // std::vector<TextureMap> metallicMaps = loadMaterial(material,TextureMapType::METALLIC);
    // _material._metallicMaps.insert(_material._metallicMaps.end(), metallicMaps.begin(), metallicMaps.end());
    //
    // std::vector<TextureMap> normalMaps = loadMaterial(material,TextureMapType::NORMAL);
    // if (!normalMaps.empty())
    // {
    //     _material._normalMap = normalMaps[0];
    // }
    //
    // std::vector<TextureMap> aoMaps = loadMaterial(material, TextureMapType::AO);
    // _material._aoMaps.insert(_material._aoMaps.end(), aoMaps.begin(), aoMaps.end());
    //
    // std::vector<TextureMap> emissionMaps = loadMaterial(material, TextureMapType::EMISSION);
    // _material._emissionMaps.insert(_material._emissionMaps.end(), emissionMaps.begin(), emissionMaps.end());
    //
    // std::vector<TextureMap> roughnessMaps = loadMaterial(material, TextureMapType::ROUGHNESS);
    // _material._roughnessMaps.insert(_material._roughnessMaps.end(), roughnessMaps.begin(), roughnessMaps.end());
    //
    return Mesh(vertices, indices, textures);
}

void Model::loadModel(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(
        path, aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_FlipUVs);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
        }
    // retrieve the directory path of the filepath
    _directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

std::vector<TextureMap> Model::loadMaterial(aiMaterial* mat, TextureMapType tex)
{
}

void Model::draw(Camera& camera)
{
    p_shader->Activate();
    p_shader->setMat4("_TransformMatrix", transform.getMatrixTransform());
    p_shader->setMat4("_ProjectionMatrix", camera.getPerspectiveProjectionMatrix());
    p_shader->setMat4("_ViewMatrix", camera.getViewMatrix());
    p_shader->setVec4("_Material.albedo", _material._albedo);
    p_shader->setFloat("_Material.shininess", _material._shininess);
    p_shader->setFloat("_Material.metallic", _material._metallic);
    p_shader->setFloat("_Material.normalMultiplier", _material._normalMultiplier);
    p_shader->setFloat("_Material.roughnessMultiplier", _material._roughnessMultiplier);
    p_shader->setBool("_Material.emissionOn", _material._emissionOn);
    p_shader->setVec3("F", _material._f0);
    p_shader->setBool("_Material.useMetallicMap", _material._useMetallicMap);
    int diffuseMapCount = 0;
    int specularCount = 0;
    int metallicMapCount = 0;
    int roughnessMapCount = 0;
    int aoMapCount = 0;
    int emissionMapCount = 0;
    int count = 0;
    // 1 DIFFUSE
    for (unsigned int i = 0; i < _material._diffuseMaps.size(); i++)
    {
        if (_material._diffuseMaps[i]._initialized)
        {
            // AppLog("add diffuse" + i);
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, _material._diffuseMaps[i]._texture._id);
            p_shader->setInt("_Material.diffuseMaps[" + std::to_string(i) + "]", diffuseMapCount++);
            count++;
        }
    }
    p_shader->setInt("_Material.diffuseMapsCount", diffuseMapCount);
    // 2 SPECULAR
    for (unsigned int i = 0; i < _material._specularMaps.size(); i++)
    {
        if (_material._specularMaps[i]._initialized)
        {
            // AppLog("add specu" + i);
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, _material._specularMaps[i]._texture._id);
            p_shader->setInt("_Material.specularMaps[" + std::to_string(i) + "]", specularCount++);
            count++;
        }
    }
    p_shader->setInt("_Material.specularMapsCount", specularCount);
    // 3 Roughness
    for (unsigned int i = 0; i < _material._roughnessMaps.size(); i++)
    {
        if (_material._roughnessMaps[i]._initialized)
        {
            // AppLog("add roughness" + i);
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, _material._roughnessMaps[i]._texture._id);
            p_shader->setInt("_Material.roughnessMaps[" + std::to_string(i) + "]", roughnessMapCount++);
            count++;
        }
    }
    p_shader->setInt("_Material.roughnessMapsCount", roughnessMapCount);
    // 4 AO
    for (unsigned int i = 0; i < _material._aoMaps.size(); i++)
    {
        if (_material._aoMaps[i]._initialized)
        {
            // AppLog("add ao" + i);
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, _material._aoMaps[i]._texture._id);
            p_shader->setInt("_Material.aoMaps[" + std::to_string(i) + "]", aoMapCount++);
            count++;
        }
    }
    p_shader->setInt("_Material.aoMapsCount", aoMapCount);
    // 5 Metallic
    for (unsigned int i = 0; i < _material._metallicMaps.size(); i++)
    {
        if (_material._metallicMaps[i]._initialized)
        {
            // AppLog("add metallic " + i);
            glActiveTexture(GL_TEXTURE0 + count);
            glBindTexture(GL_TEXTURE_2D, _material._metallicMaps[i]._texture._id);
            p_shader->setInt("_Material.metallicMaps[" + std::to_string(i) + "]", metallicMapCount++);
            count++;
        }
    }
    p_shader->setInt("_Material.metallicMapsCount", metallicMapCount);
    // 6 Emission
    if (_material._emissionOn)
    {
        for (unsigned int i = 0; i < _material._emissionMaps.size(); i++)
        {
            if (_material._aoMaps[i]._initialized)
            {
                // AppLog("add emission " + i);
                glActiveTexture(GL_TEXTURE0 + count);
                glBindTexture(GL_TEXTURE_2D, _material._emissionMaps[i]._texture._id);
                p_shader->setInt("_Material.emissionMaps[" + std::to_string(i) + "]", emissionMapCount++);
                count++;
            }
        }
        p_shader->setInt("_Material.emissionMapsCount", emissionMapCount);
    }
    // 7 Normal
    if(_material._normalMap._initialized)
    {
        // AppLog("add normal ");
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._normalMap._texture._id);
        p_shader->setInt("_Material.normalMap", count);
        p_shader->setBool("_Material.hasNormalMap", true);
        count++;
    }
    /*******************************************************************/
    // draw Meshes
    for (unsigned int i = 0; i < _meshes.size(); i++)
    {
        // _meshes[i].draw(*p_shader,camera,"", true);
        // or call direct opengl func for better performance about 5 - 10 %
        // have tested by DoNT
        // maybe latency when call function like static_cast<GLsizei>(_meshes[i]._indices.size()) below
        glBindVertexArray(_meshes[i]._vao._id);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_meshes[i]._indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}

void Model::init()
{
}

void Model::update()
{
}

void Model::clean()
{
    for (auto mesh : _meshes)
    {
        mesh.clean();
    }
    _material = Material();
    _directory = "";
    // don't clean Shader
    // ShaderManager 'll do it :D
    p_shader = nullptr;
}
