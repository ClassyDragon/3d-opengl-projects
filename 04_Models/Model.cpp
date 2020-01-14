#include "Model.h"

Model::Model(const std::string& modelFile) {
    ModelLoader::LoadModel(modelFile, (int)FileType::OBJ, this);
}

Model::~Model() {
    delete vb;
    delete va;
    delete bld;
    delete ib;
}

void Model::Bind() const {
    this->va->Bind();
    this->ib->Bind();
}

void Model::Unbind() const {
    this->va->Unbind();
    this->ib->Unbind();
}
