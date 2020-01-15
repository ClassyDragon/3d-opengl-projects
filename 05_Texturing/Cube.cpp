#include "Cube.h"

Cube::Cube(const float size, const std::string& textureFilename, const int textureMode) {
    if (textureMode == TextureMode::BASIC) {
        float vertices[] = {
            -size, -size, -size, 0.0f, 0.0f, // 0
            -size, -size, -size, 1.0f, 0.0f, // 1

            -size, -size,  size, 0.0f, 1.0f, // 2
            -size, -size,  size, 1.0f, 1.0f, // 3

            -size,  size,  size, 1.0f, 1.0f, // 4
            -size,  size,  size, 0.0f, 0.0f, // 5
            -size,  size,  size, 0.0f, 1.0f, // 6

             size,  size, -size, 1.0f, 1.0f, // 7
             size,  size, -size, 0.0f, 0.0f, // 8
             size,  size, -size, 1.0f, 0.0f, // 9

            -size,  size, -size, 0.0f, 1.0f, // 10
            -size,  size, -size, 1.0f, 0.0f, // 11
            -size,  size, -size, 0.0f, 0.0f, // 12

             size, -size,  size, 0.0f, 1.0f, // 13
             size, -size,  size, 1.0f, 1.0f, // 14

             size, -size, -size, 1.0f, 0.0f, // 15
             size, -size, -size, 0.0f, 0.0f, // 16

             size,  size,  size, 0.0f, 1.0f, // 17
             size,  size,  size, 1.0f, 1.0f, // 18
             size,  size,  size, 1.0f, 0.0f, // 19
        };

        unsigned int indices[] = {
            0, 2, 4,
            7, 0, 10,
            13, 1, 16,
            7, 15, 0,
            0, 4, 11,
            13, 3, 1,
            5, 2, 14,
            17, 15, 8,
            15, 17, 14,
            18, 9, 12,
            18, 12, 6,
            19, 5, 14
        };
        this->vb       = new VertexBuffer(vertices, 5 * 20 * sizeof(float));
        this->ib       = new IndexBuffer (indices, 12 * 3);
    }
    else if (textureMode == TextureMode::SODACAN) {
        float vertices[] = {
            -size,  size,  size, 0.33f, 0.0f,
            -size,  size, -size, 0.33f, 1.0f,
             size,  size, -size, 0.66f, 1.0f,
             size,  size,  size, 0.66f, 0.0f,

            -size, -size,  size, 0.0f, 0.0f,
            -size,  size,  size, 0.0f, 1.0f,
             size,  size,  size, 0.33f, 1.0f,
             size, -size,  size, 0.33f, 0.0f,

             size, -size,  size, 0.0f, 0.0f,
             size,  size,  size, 0.0f, 1.0f,
             size,  size, -size, 0.33f, 1.0f,
             size, -size, -size, 0.33f, 0.0f,

             size, -size, -size, 0.0f, 0.0f,
             size,  size, -size, 0.0f, 1.0f,
            -size,  size, -size, 0.33f, 1.0f,
            -size, -size, -size, 0.33f, 0.0f,

            -size, -size, -size, 0.0f, 0.0f,
            -size,  size, -size, 0.0f, 1.0f,
            -size,  size,  size, 0.33f, 1.0f,
            -size, -size,  size, 0.33f, 0.0f,

            -size, -size, -size, 0.66f, 0.0f,
            -size, -size,  size, 0.66f, 1.0f,
             size, -size,  size, 1.0f, 1.0f,
             size, -size, -size, 1.0f, 0.0f
        };
        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 13, 14,
            12, 14, 15,
            16, 17, 18,
            16, 18, 19,
            20, 21, 22,
            20, 22, 23
        };
        this->vb       = new VertexBuffer(vertices, 5 * 24 * sizeof(float));
        this->ib       = new IndexBuffer (indices, 12 * 3);
    }

    this->layout   = new BufferLayoutData();
    this->texture  = new Texture     (textureFilename);

    layout->Insert<float>(3); // vertex position
    layout->Insert<float>(2); // texture coords

    this->va = new VertexArray();
    va->AddBuffer(*this->vb, *this->layout);
}

Cube::~Cube() {
}

void Cube::Bind() const {
    this->va->Bind();
    this->ib->Bind();
    this->texture->Bind();
}

void Cube::Unbind() const {
    this->va->Unbind();
    this->ib->Unbind();
    this->texture->Unbind();
}
