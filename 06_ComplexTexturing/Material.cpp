#include "Material.h"

Material::Material(const std::string& textureFile) 
    : texture(new Texture(textureFile))
{
    std::cout << "Material constructed: " << textureFile << std::endl;
}
