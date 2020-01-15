#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayoutData.h"
#include "ModelLoader.h"
#include "Texture.h"

class Model {
    public:
        Model(const std::string& modelFile, const std::string& textureFile);
        ~Model();
        void Bind() const;
        void Unbind() const;
        bool Loaded() const;
    private:
        friend class ModelLoader;
        friend class Renderer;
        VertexBuffer* vb;
        VertexArray* va;
        BufferLayoutData* bld;
        IndexBuffer* ib;
        Texture* texture;
        bool isLoaded;
};
