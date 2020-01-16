#include "Material.h"

Material::Material(const std::string& textureFile) 
    : texture(new Texture(textureFile))
{
}

Material::~Material() {
}

void Material::Bind() const {
    texture->Bind();
}
