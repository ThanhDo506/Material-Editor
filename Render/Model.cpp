#include "Model.h"

Model::Model(std::string const  &path,
             Transform          transform,
             std::string const  &name,
             Entity             *parent)
{
    loadModel(path);
    this->transform = transform;
    _name           = name;
    p_parent        = parent;
}

Model::~Model()
{
    
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
    // std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Type::ALBEDO);
    // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //
    // std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, Texture::Type::NORMAL);
    // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //
    // std::vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, Texture::Type::AMBIENT_OCCLUSION);
    // textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
    //
    // std::vector<Texture> emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, Texture::Type::EMISSION);
    // textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
    //
    // std::vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, Texture::Type::ROUGHNESS);
    // textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

    return Mesh(vertices, indices, textures);
}

void Model::loadModel(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene*   scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) // if is Not Zero
            {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
            }
    // retrieve the directory path of the filepath
    _directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

void Model::loadMaterial(aiMaterial* mat, aiTexture textureType, TextureMapType tex)
{
}

void Model::draw(Shader& shader, Camera& camera)
{
    shader.Activate();
    shader.setMat4("_Projection", camera.getPerspectiveProjectionMatrix());
    shader.setMat4("_View", camera.getViewMatrix());
    unsigned int diffuseMapCount = 0;
    unsigned int specularCount = 0;
    unsigned int metallicMapCount = 0;
    unsigned int roughnessMapCount = 0;
    unsigned int aoMapCount = 0;
    unsigned int emissionMapCount = 0;
    unsigned int count = 0;
    // 1 DIFFUSE
    for (unsigned int i = 0; i < _material._diffuseMaps.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._diffuseMaps[i]._texture._id);
        shader.setInt("_Material.diffuseMaps[" + std::to_string(i) + "]", diffuseMapCount++);
        count++;
    }
    // 2 SPECULAR
    for (unsigned int i = 0; i < _material._specularMaps.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._specularMaps[i]._texture._id);
        shader.setInt("_Material.specularMaps[" + std::to_string(i) + "]", specularCount++);
        count++;
    }
    // 3 Roughness
    for (unsigned int i = 0; i < _material._roughnessMaps.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._roughnessMaps[i]._texture._id);
        shader.setInt("_Material.roughnessMaps[" + std::to_string(i) + "]", roughnessMapCount++);
        count++;
    }
    // 4 AO
    for (unsigned int i = 0; i < _material._aoMaps.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._aoMaps[i]._texture._id);
        shader.setInt("_Material.aoMaps[" + std::to_string(i) + "]", aoMapCount++);
        count++;
    }
    // 5 Metallic
    for (unsigned int i = 0; i < _material._metallicMaps.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._metallicMaps[i]._texture._id);
        shader.setInt("_Material.metallicMaps[" + std::to_string(i) + "]", metallicMapCount++);
        count++;
    }
    // 6 Emission
    for (unsigned int i = 0; i < _material._emissionMaps.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._emissionMaps[i]._texture._id);
        shader.setInt("_Material.emissionMaps[" + std::to_string(i) + "]", emissionMapCount++);
        count++;
    }
    // 7 Normal
    if(_material._normalMap._type != TextureMapType::NONE)
    {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, _material._normalMap._texture._id);
        shader.setInt("_Material.normalMap", count);
        shader.setBool("_Material.hasNormalMap", true);
        count++;
    }
    /*******************************************************************/
    // draw Meshes
    for (unsigned int i = 0; i < _meshes.size(); i++)
    {
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
