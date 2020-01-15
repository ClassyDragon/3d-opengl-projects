#pragma once

#include <GL/glew.h>

#include <iostream>
#include <vector>

#include "GLCall.h"

struct BufferAttribute {
    unsigned int type; // e.g. GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE 
    unsigned int count;
    unsigned char normalized;
    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return sizeof(float);
            case GL_UNSIGNED_INT: return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
        }
        return 0;
    }
};

class BufferLayoutData {
    private:
        std::vector<BufferAttribute> attributes;
        unsigned int LayoutStride;
    public:
        BufferLayoutData();

        template <typename T>
            void Insert(int count);

        inline unsigned int getStride() const { return this->LayoutStride; }
        inline const std::vector<BufferAttribute>& getAttributes() const { return this->attributes; }
};
