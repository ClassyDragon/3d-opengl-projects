#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayoutData.h"
#include "ModelLoader.h"

class Model {
    public:
        Model(const std::string& modelFile);
        ~Model();
        void Bind() const;
        void Unbind() const;
    private:
        friend class ModelLoader;
        friend class Renderer;
        VertexBuffer* vb;
        VertexArray* va;
        BufferLayoutData* bld;
        IndexBuffer* ib;
};
