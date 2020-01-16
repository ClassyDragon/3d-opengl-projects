#pragma once

#include "Texture.h"

class Material {
    public:
        Material() {}
        Material(const std::string& textureFile);
        void Bind() const {
            texture->Bind();
        }
        Texture* texture;
};
