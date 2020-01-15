#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0));
    shader.Unbind();
}

void Renderer::Draw(const Cube& cube, const Shader& shader) const {
    cube.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, cube.ib->getCount(), GL_UNSIGNED_INT, 0));
    shader.Unbind();
}

void Renderer::Draw(const BatchCubes& batchCubes, Shader& shader) const {
    batchCubes.cube->Bind();
    shader.Bind();
    auto& translations = batchCubes.getTranslations();
    for (auto& i : translations) {
        shader.SetUniformMat4f("trans", i);
        GLCall(glDrawElements(GL_TRIANGLES, batchCubes.cube->ib->getCount(), GL_UNSIGNED_INT, 0));
    }
    shader.Unbind();
}

void Renderer::Draw(const Model& model, Shader& shader) const {
    model.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, model.ib->getCount(), GL_UNSIGNED_INT, 0));
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}
