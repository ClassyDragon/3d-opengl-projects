#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayoutData.h"
#include "ModelLoader.h"
#include "Texture.h"
#include "Material.h"

class Mesh {
    public:
        Mesh();
        ~Mesh();
        void Bind() const;
        void Unbind() const;
        void SetMaterial(Material* material);
        void SetVertexBuffer(VertexBuffer* vb);
        void SetIndexBuffer(IndexBuffer* ib);
        void Delete();
        void ConstructVertexArray();
        IndexBuffer* ib;
    private:
        VertexBuffer* vb;
        VertexArray* va;
        BufferLayoutData* bld;
        Material* material;
};
