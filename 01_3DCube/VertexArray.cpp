#include "VertexArray.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &this->ID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &this->ID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const BufferLayoutData& layout) {
    Bind();
    vb.Bind();
    const auto& Attributes = layout.getAttributes();
    int offset = 0;
    for (unsigned int i = 0; i < Attributes.size(); i++) {
        const auto& Attribute = Attributes[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, Attribute.count, Attribute.type, Attribute.normalized, layout.getStride(), (const void*)offset));
        offset += BufferAttribute::getSizeOfType(Attribute.type) * Attribute.count;
    }
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(this->ID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}
