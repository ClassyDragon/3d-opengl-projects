#include "Mesh.h"

Mesh::Mesh() {
    va = new VertexArray;
    bld = new BufferLayoutData;
    bld->Insert<float>(3);
    bld->Insert<float>(2);
    bld->Insert<float>(3);
}

Mesh::~Mesh() {
}

void Mesh::Bind() const {
    std::cout << "Bind va" << std::endl;
    va->Bind();
    std::cout << "Bind ib" << std::endl;
    ib->Bind();
    std::cout << "Bind material" << std::endl;
    material->Bind();
}

void Mesh::Unbind() const {
}

void Mesh::SetMaterial(Material* material) {
    this->material = material;
}

void Mesh::SetVertexBuffer(VertexBuffer* vb) {
    this->vb = vb;
}

void Mesh::SetIndexBuffer(IndexBuffer* ib) {
    this->ib = ib;
}

void Mesh::ConstructVertexArray() {
    va->AddBuffer(*vb, *bld);
}

void Mesh::Delete() {
    delete va;
    delete bld;
    delete ib;
    delete material;
}
