#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int sizeInBytes) {
    GLCall(glGenBuffers(1, &this->ID));
    this->Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW));
    this->Unbind();
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &this->ID));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->ID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
