#include "ModelLoader.h"

struct Vector3f {
    float x;
    float y;
    float z;
    bool operator< (const Vector3f& vectorObj) const {
        if (vectorObj.x < this->x)
            return true;
    }
    Vector3f(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }
};

struct Vertex {
    float x;
    float y;
    float z;
    float texW;
    float texH;
    float nX;
    float nY;
    float nZ;
    bool operator== (const Vertex& vertexObj) const {
        if (vertexObj.x == this->x &&
                vertexObj.y == this->y &&
                vertexObj.z == this->z &&
                vertexObj.texW == this->texW &&
                vertexObj.texH == this->texH &&
                vertexObj.nX == this->nX &&
                vertexObj.nY == this->nY &&
                vertexObj.nZ == this->nZ) {
            return true;
        }
        return false;
    }
};

struct TriangleIndex {
    std::string i1, i2, i3;
}

bool ModelLoader::LoadModel(const std::string& modelFile, int filetype, Model* model) {
    std::ifstream file(modelFile);
    if (!file.good()) {
        return false;
    }
    if (filetype == (int)FileType::OBJ) {

        //std::vector<unsigned int> VertexIndices, TextureIndices, NormalIndices;
        std::vector<glm::vec3> VertexPositions, Normals;
        std::vector<glm::vec2> TextureCoordinates;
        std::map<std::string, Material> materials;
        std::map<std::string, Vertex> vertices;
        std::unordered_map<std::string, std::vector<TriangleIndex>> meshIndices;

        std::string temp = "";
        while (file >> temp) {
            if (
                       temp == "#" 
                    || temp == "o"
                    || temp == "s" 
                    || temp == "g"
                    ) 
            {
                file.ignore(256, '\n');
            }
            else if (temp.find("mtllib") != std::string::npos) {
                std::string materialFile;
                temp >> materialFile;
                ifstream mtfile("res/materials/" + materialFile);
                while (mtfile >> temp) {
                    if (temp.find("newmtl") == std::string::npos) {
                        mtfile.ignore(256, '\n');
                    }
                    else {
                        std::string materialName;
                        mtfile >> materialName;
                        if (temp.find("map_Kd") == std::string::npos) {
                            mtfile.ignore(256, '\n');
                        }
                        else {
                            mtfile >> temp;
                            materials.insert(materialName, Material("res/textures/" + temp));
                        }
                    }
                }
                mtfile.close();
            }
            else if (temp.find("usemtl") != std::string::npos) {
                // Create new mesh using specified material
                std::string matName;
                file >> matName;
                std::vector<TriangleIndex> triIndices;
                while (file >> temp) {
                    if (temp.find("usemtl") != std::string::npos) {
                        for (int i = 5; i >= 0; i++) {
                            ifstream.putback(temp[i]);
                        }
                        break;
                    }
                    else if (temp == "f") {
                        TriangleIndex t;
                        file >> t.i1;
                        file >> t.i2;
                        file >> t.i3;
                        triIndices.push_back(t);
                    }
                }
                meshIndices.insert(matName, triIndices);
            }
            else if (temp.find("vt") != std::string::npos) {
                // Read vertex texture coordinates
                float xy[2];
                for (int i = 0; i < 2; i++) {
                    file >> xy[i];
                }
                glm::vec2 texCoords(xy[0], xy[1]);
                TextureCoordinates.push_back(texCoords);
            }
            else if (temp.find("vn") != std::string::npos) {
                // Read vertex normal
                float xyz[3];
                for (int i = 0; i < 3; i++) {
                    file >> xyz[i];
                }
                glm::vec3 normalValues(xyz[0], xyz[1], xyz[2]);
                Normals.push_back(normalValues);
            }
            else if (temp.find("v") != std::string::npos) {
                // Read vertex positions
                float xyz[3];
                for (int i = 0; i < 3; i++) {
                    file >> xyz[i];
                }
                glm::vec3 vertex(xyz[0], xyz[1], xyz[2]);
                VertexPositions.push_back(vertex);
            }
        }
        file.close();

        // At this point:
        // meshIndices contains mesh data in <material name, indices> format.
        // VertexPositions contains vec3's of all vertex positions.
        // TextureCoordinates contains vec2's of all texture coordinates.
        // Normals contains vec3's of all normal vectors.

        // Convert the indices:
        std::map<std::string, int> indices;
        unsigned int vertexNum = 0;
        // Iterate through all indices and construct unique vertices for all of them.
        for (auto& mesh : meshIndices) {
            for (auto& index : mesh) {
                if (indices.find(*index) == indices.end()) {
                    // Deconstruct string in v/vt/vn format
                    std::string vertexData = *index;
                    for (auto& c : vertexData) {
                        if (c == '/')
                            c = ' ';
                    }
                    std::stringstream ss(vertexData);
                    unsigned int vertexAttributes[3];
                    for (int i = 0; i < 3; i++) {
                        ss >> vertexAttributes[i];
                    }
                    vertices.push_back(Vertex {
                            VertexPositions[index - 1].x,
                            VertexPositions[index - 1].y,
                            VertexPositions[index - 1].z,
                            TextureCoordinates[index - 1].x,
                            TextureCoordinates[index - 1].y,
                            Normals[index - 1].x,
                            Normals[index - 1].y,
                            Normals[index - 1].z
                            });
                    indices[index] = vertexNum;
                    vertexNum++;
                }
            }
        }
        for (int i = 0; i < VertexIndices.size(); i++) {
            std::string keyvalue = std::to_string(VertexIndices[i]) + " " + std::to_string(TextureIndices[i]) + " " + std::to_string(NormalIndices[i]);
            if (indices.find(keyvalue) == indices.end()) {
                indices[keyvalue] = vertexNum;
                vertexNum++;
            }
        }
        std::vector<unsigned int> ib;
        for (int i = 0; i < VertexIndices.size(); i++) {
            ib.push_back(indices[std::to_string(VertexIndices[i]) + " " + std::to_string(TextureIndices[i]) + " " + std::to_string(NormalIndices[i])]);
        }

        // Create float array with all combinations of vertex positions and tex coords:
        std::vector<Vertex> vertices;
        /* {
         *      v.x, v.y, v.z, t.w, t.h
         * }
         */

        for (int i = 0; i < VertexIndices.size(); i++) {
            Vertex v{
                VertexPositions[VertexIndices[i] - 1].x,
                VertexPositions[VertexIndices[i] - 1].y,
                VertexPositions[VertexIndices[i] - 1].z,
                TextureCoordinates[TextureIndices[i] - 1].x,
                TextureCoordinates[TextureIndices[i] - 1].y,
                Normals[NormalIndices[i] - 1].x,
                Normals[NormalIndices[i] - 1].y,
                Normals[NormalIndices[i] - 1].z
            };
            bool good = true;
            for (auto& q : vertices) {
                if (q == v)
                    good = false;
            }
            if (good) {
                vertices.push_back(v);
            }
        }

        model->vb = new VertexBuffer(&vertices[0], vertices.size() * 8 * sizeof(float));
        model->ib = new IndexBuffer(&ib[0], ib.size());
        model->bld = new BufferLayoutData;
        model->bld->Insert<float>(3); // positions
        model->bld->Insert<float>(2); // tex coords
        model->bld->Insert<float>(3); // normals
        model->va = new VertexArray;
        model->va->Bind();
        model->va->AddBuffer(*model->vb, *model->bld);
        return true;
    }
}
