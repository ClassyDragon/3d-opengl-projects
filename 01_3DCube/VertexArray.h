#pragma once

#include <GL/glew.h>

#include "VertexBuffer.h"
#include "BufferLayoutData.h"

#include "GLCall.h"

class VertexArray {
    private:
        unsigned int ID;
    public:
        VertexArray();
        ~VertexArray();
        void AddBuffer(const VertexBuffer& vb, const BufferLayoutData& layout);

        void Bind() const;
        void Unbind() const;
};
