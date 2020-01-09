#pragma once

#include <GL/glew.h>

#include "GLCall.h"

#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayoutData.h"
#include "Texture.h"

class Renderer;

/* 3D Cube Class */
// A cube of specified size and with a given texture.
// The cube can be copied to produce an identical cube with varying transformations.
class Cube {
    private:
        friend class Renderer;
        VertexArray* va;
        VertexBuffer* vb;
        IndexBuffer* ib;
        Texture* texture;
        BufferLayoutData* layout;
    public:
        Cube(const float size, const std::string& textureFilename);
        ~Cube();

        void Bind() const;
        void Unbind() const;
};

class BatchCubes {
    private:
        friend class Renderer;
        std::vector<glm::mat4> translations;
        Cube* cube;
    public:
        BatchCubes(Cube* cube)
            : cube(cube)
        {
        }

        void insertCube(glm::mat4 trans) {
            translations.push_back(trans);
        }

        inline const std::vector<glm::mat4>& getTranslations() const { return this->translations; }
};
