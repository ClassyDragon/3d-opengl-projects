#pragma once

#include <vector>

#include "Mesh.h"

class Model {
    public:
        Model(const std::string& modelFile, const std::string& textureFile);
        Model(const std::string& modelFile, Texture& texture);
        ~Model();
        void Bind() const;
        void Unbind() const;
        bool Loaded() const;
    private:
        friend class ModelLoader;
        friend class Renderer;
        std::vector<Mesh> meshes;
        bool isLoaded;
};
