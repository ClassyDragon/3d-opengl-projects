#include "BufferLayoutData.h"

BufferLayoutData::BufferLayoutData()
    : LayoutStride(0)
{
}

template <typename T>
void BufferLayoutData::Insert(int count) {
}

template <>
void BufferLayoutData::Insert<float>(int count) {
    attributes.push_back({ GL_FLOAT, count, GL_FALSE });
    LayoutStride += sizeof(float) * count;
}

template <>
void BufferLayoutData::Insert<unsigned int>(int count) {
    attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    LayoutStride += sizeof(unsigned int) * count;
}

template <>
void BufferLayoutData::Insert<unsigned char>(int count) {
    attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    LayoutStride += sizeof(unsigned char) * count;
}
