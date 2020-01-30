#pragma once

#include <vector>

#include "Texture.h"
#include "Mesh.h"

class Mesh;

class Model {
    public:
        Model(const std::string& modelFile, const std::string& textureFile);
        Model(const std::string& modelFile, Texture& texture);
        Model(const std::string& modelFile);
        ~Model();
        void Bind() const;
        void Unbind() const;
        bool Loaded() const;
        std::vector<Mesh>* GetMeshes();
        std::vector<Mesh> meshes;
    private:
        friend class ModelLoader;
        friend class Renderer;
        bool isLoaded;
        VertexBuffer* vb; // Shared vertex buffer from all meshes. This must be destructed separately from the meshes.
};
