#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0));
    shader.Unbind();
}

void Renderer::Draw(Model& model, Shader& shader) {
    std::vector<Mesh>* meshes = model.GetMeshes();
    shader.Bind();
    for (auto& mesh : *meshes) {
        mesh.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, mesh.ib->getCount(), GL_UNSIGNED_INT, 0));
    }
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}
