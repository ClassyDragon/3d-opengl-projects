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
    std::string indices[3];
};

bool ModelLoader::LoadModel(const std::string& modelFile, int filetype, Model* model) {
    std::ifstream file(modelFile);
    if (!file.good()) {
        std::cout << "Couldn't load obj file" << std::endl;
        return false;
    }
    if (filetype == (int)FileType::OBJ) {

        //std::vector<unsigned int> VertexIndices, TextureIndices, NormalIndices;
        std::vector<glm::vec3> VertexPositions, Normals;
        std::vector<glm::vec2> TextureCoordinates;
        std::unordered_map<std::string, Material*> materials;
        std::vector<Vertex> vertices;
        std::unordered_map<std::string, std::vector<TriangleIndex>> meshIndices;

        bool newMesh = false;
        std::string temp = "";
        while (file >> temp) {
            if (newMesh) {
                newMesh = false;
                // Create new mesh using specified material
                std::string matName = temp;
                std::vector<TriangleIndex> triIndices;
                while (file >> temp) {
                    if (temp.find("usemtl") != std::string::npos) {
                        newMesh = true;
                        break;
                    }
                    else if (temp == "f") {
                        TriangleIndex t;
                        file >> t.indices[0];
                        file >> t.indices[1];
                        file >> t.indices[2];
                        triIndices.push_back(t);
                    }
                }
                meshIndices.insert(std::pair<std::string, std::vector<TriangleIndex>>(matName, triIndices));
            }
            else if (
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
                file >> materialFile;
                std::ifstream mtfile("res/materials/" + materialFile);
                if (!mtfile.good()) {
                    std::cout << "Failed to open material file." << std::endl;
                    return false;
                }
                while (mtfile >> temp) {
                    if (temp.find("newmtl") == std::string::npos) {
                        mtfile.ignore(256, '\n');
                    }
                    else {
                        std::string materialName;
                        mtfile >> materialName;
                        while (mtfile >> temp) {
                            if (temp.find("newmtl") != std::string::npos) {
                            }
                            else if (temp.find("map_Kd") != std::string::npos) {
                                mtfile >> temp;
                                materials.emplace(std::pair<std::string, Material*>(materialName, new Material("res/textures/" + temp)));
                                break;
                            }
                            else {
                                mtfile.ignore(256, '\n');
                            }
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
                        newMesh = true;
                        break;
                    }
                    else if (temp == "f") {
                        TriangleIndex t;
                        file >> t.indices[0];
                        file >> t.indices[1];
                        file >> t.indices[2];
                        triIndices.push_back(t);
                    }
                }
                meshIndices.insert(std::pair<std::string, std::vector<TriangleIndex>>(matName, triIndices));
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
        //std::unordered_map<std::string, unsigned int> indices;
        std::unordered_map<std::string, unsigned int> indices;
        unsigned int vertexNum = 0;
        // Iterate through all indices and construct unique vertices for all of them.
        /*
            std::unordered_map<std::string, std::vector<TriangleIndex>> meshIndices;
        */
        for (auto& mesh : meshIndices) { // for each vector in map
            // New mesh to be added to the model:
            Mesh m;

            // Indices converted from v/vt/vn to i format
            std::vector<unsigned int> meshConvertedIndices;

            for (auto& triangle : mesh.second) { // for each TriangleIndex(string, string, string) in vector
                for (auto& index : triangle.indices) { // for each std::string in TriangleIndex
                    if (indices.find(index) == indices.end()) {
                        // Deconstruct string in v/vt/vn format
                        std::string vertexData = index;
                        for (auto& c : vertexData) {
                            if (c == '/')
                                c = ' ';
                        }

                        // Create vertex using converted string
                        std::stringstream ss(vertexData);
                        unsigned int vertexAttributes[3];
                        for (int i = 0; i < 3; i++) {
                            ss >> vertexAttributes[i];
                        }
                        vertices.push_back(Vertex {
                                VertexPositions[vertexAttributes[0] - 1].x,
                                VertexPositions[vertexAttributes[0] - 1].y,
                                VertexPositions[vertexAttributes[0] - 1].z,
                                TextureCoordinates[vertexAttributes[1] - 1].x,
                                TextureCoordinates[vertexAttributes[1] - 1].y,
                                Normals[vertexAttributes[2] - 1].x,
                                Normals[vertexAttributes[2] - 1].y,
                                Normals[vertexAttributes[2] - 1].z
                                });

                        // Create unique index in map
                        // Example: indices<"1/1/1", 5>
                        indices[index] = vertexNum;
                        vertexNum++;
                    }
                    meshConvertedIndices.push_back(indices[index]);
                }
            }
            // meshConvertedIndices example:
            // { 
            //      0, 1, 2,
            //      3, 4, 5
            // }
            m.SetMaterial(materials[mesh.first]);
            IndexBuffer* ib = new IndexBuffer(&meshConvertedIndices[0], meshConvertedIndices.size());
            m.SetIndexBuffer(ib);
            model->meshes.push_back(m);
        }

        VertexBuffer* vb = new VertexBuffer(&vertices[0], vertices.size() * 8 * sizeof(float));

        // Set completed VertexBuffer to all meshes:
        for (auto& mesh : model->meshes) {
            mesh.SetVertexBuffer(vb);
            mesh.ConstructVertexArray();
        }

        return true;
    }
    return false;
}
