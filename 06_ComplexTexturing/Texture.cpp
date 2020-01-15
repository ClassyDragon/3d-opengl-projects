#include "Texture.h"

Texture::Texture(const std::string& filename)
    : ID(0), textureWidth(0), textureHeight(0), bytesPerPixel(0)
{
    stbi_set_flip_vertically_on_load(1);

    // Function signature:
    // stbi_load(char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
    data = stbi_load(filename.c_str(), &this->textureWidth, &this->textureHeight, &this->bytesPerPixel, 4);

    GLCall(glGenTextures(1, &this->ID));
    GLCall(glBindTexture(GL_TEXTURE_2D, this->ID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->textureWidth, this->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data));

    this->Unbind();
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &this->ID));
}

void Texture::Bind(int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, this->ID));
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
