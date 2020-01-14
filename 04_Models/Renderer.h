#pragma once

#include <GL/glew.h>

#include "GLCall.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Cube.h"
#include "Model.h"

class Renderer {
    public:
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void Draw(const Cube& cube, const Shader& shader) const;
        void Draw(const BatchCubes& batchCubes, Shader& shader) const;
        void Draw(const Model& model, Shader& shader) const;
        void Clear() const;
};
