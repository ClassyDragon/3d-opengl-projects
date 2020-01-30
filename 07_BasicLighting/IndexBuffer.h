#pragma once

#include <GL/glew.h>

#include "GLCall.h"

class IndexBuffer {
    private:
        unsigned int ID;
        unsigned int indexCount;
    public:
        IndexBuffer(const void* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int getCount() const { return this->indexCount; }
};
