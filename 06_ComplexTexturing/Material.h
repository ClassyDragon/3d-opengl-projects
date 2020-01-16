#pragma once

#include "Texture.h"

class Material {
    public:
        Material() {}
        Material(const std::string& textureFile);
        ~Material();
        void Bind() const;
        Texture* texture;
};
