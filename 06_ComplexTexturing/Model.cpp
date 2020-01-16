#include "Model.h"

Model::Model(const std::string& modelFile, const std::string& textureFile) 
{
    this->isLoaded = ModelLoader::LoadModel(modelFile, (int)FileType::OBJ, this);
}

Model::Model(const std::string& modelFile, Texture& texture) 
{
    this->isLoaded = ModelLoader::LoadModel(modelFile, (int)FileType::OBJ, this);
}

Model::Model(const std::string& modelFile) {
    this->isLoaded = ModelLoader::LoadModel(modelFile, (int)FileType::OBJ, this);
}

Model::~Model() {
    for (auto& m : meshes) {
        m.Delete();
    }
    delete vb;
}

void Model::Bind() const {
}

void Model::Unbind() const {
}

bool Model::Loaded() const {
    return this->isLoaded;
}

std::vector<Mesh>* Model::GetMeshes() {
    return &this->meshes;
}
