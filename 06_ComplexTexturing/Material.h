#pragma once

#include "Texture.h"

class Material {
    public:
        Material(const std::string& textureFile);
        Texture* texture;
}
