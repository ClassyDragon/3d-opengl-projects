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

void ModelLoader::LoadModel(const std::string& modelFile, int filetype, Model* model) {
    std::ifstream file(modelFile);
    if (filetype == (int)FileType::OBJ) {

        std::vector<unsigned int> VertexIndices, TextureIndices, NormalIndices;
        std::vector<glm::vec3> VertexPositions, Normals;
        std::vector<glm::vec2> TextureCoordinates;

        std::string temp = "";
        while (file >> temp) {
            if (temp.find("vt") != std::string::npos) {
                // Read vertex texture coordinates
                float xyz[2];
                for (int i = 0; i < 2; i++) {
                    file >> xyz[i];
                }
                glm::vec2 texCoords(xyz[0], xyz[1]);
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
            else if (temp.find("f") != std::string::npos) {
                // Read triangle indices
                for (int i = 0; i < 3; i++) {
                    file >> temp;
                    for (auto& c : temp) {
                        if (c == '/') {
                            c = ' ';
                        }
                    }
                    unsigned int ptn[3];
                    std::stringstream ss(temp);
                    for (int j = 0; j < 3; j++) {
                        ss >> ptn[j];
                    }
                    VertexIndices.push_back(ptn[0]);
                    TextureIndices.push_back(ptn[1]);
                    NormalIndices.push_back(ptn[2]);
                }
            }
        }
        file.close();

        // At this point:
        // VertexPositions contains vec3's with each vertex position
        // TextureCoordinates contains vec2's with each texture coordinate
        // Normals contains vec3's with each normal vector
        // The indices vectors contain index data in the format:
        //      

        // Convert the indices:
        //std::map<Vector3f, int> indices;
        std::map<std::string, int> indices;
        unsigned int vertexNum = 0;
        for (int i = 0; i < VertexIndices.size(); i++) {
            std::string keyvalue = std::to_string(VertexIndices[i]) + " " + std::to_string(TextureIndices[i]) + " " + std::to_string(NormalIndices[i]);
            //Vector3f vec{ VertexIndices[i], TextureIndices[i], NormalIndices[i] };
            if (indices.find(keyvalue) == indices.end()) {
                //std::cout << "(" << vec.x << " " << vec.y << " " << vec.z << ")" << std::endl;
                indices[keyvalue] = vertexNum;
                vertexNum++;
                //std::cout << vertexNum << std::endl;
            }
        }
        std::vector<unsigned int> ib;
        for (int i = 0; i < VertexIndices.size(); i++) {
//            ib.push_back(indices[Vector3f(VertexIndices[i], TextureIndices[i], NormalIndices[i])]);
            ib.push_back(indices[std::to_string(VertexIndices[i]) + " " + std::to_string(TextureIndices[i]) + " " + std::to_string(NormalIndices[i])]);
        }
        std::cout << "Size: " << ib.size() << std::endl;

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
            /*
            vertices.push_back(VertexPositions[VertexIndices[i] - 1].x);
            std::cout << "(" << VertexPositions[VertexIndices[i] - 1].x << ", ";
            vertices.push_back(VertexPositions[VertexIndices[i] - 1].y);
            std::cout << VertexPositions[VertexIndices[i] - 1].y << ", ";
            vertices.push_back(VertexPositions[VertexIndices[i] - 1].z);
            std::cout << VertexPositions[VertexIndices[i] - 1].z << ", ";
            vertices.push_back(TextureCoordinates[TextureIndices[i] - 1].x);
            std::cout << TextureCoordinates[TextureIndices[i] - 1].x << ", ";
            vertices.push_back(TextureCoordinates[TextureIndices[i] - 1].y);
            std::cout << TextureCoordinates[TextureIndices[i] - 1].y << ", ";
            vertices.push_back(Normals[NormalIndices[i] - 1].x);
            std::cout << Normals[NormalIndices[i] - 1].x << ", ";
            vertices.push_back(Normals[NormalIndices[i] - 1].y);
            std::cout << Normals[NormalIndices[i] - 1].y << ", ";
            vertices.push_back(Normals[NormalIndices[i] - 1].z);
            std::cout << Normals[NormalIndices[i] - 1].z << ")" << std::endl;
            */
        }
        std::cout << "Vertices Size: " << vertices.size() << std::endl;

        /*
        for (auto& i : ib) {
            std::cout << i << std::endl;
        }
        */

        model->vb = new VertexBuffer(&vertices[0], vertices.size() * 8 * sizeof(float));
        model->ib = new IndexBuffer(&ib[0], ib.size());
        model->bld = new BufferLayoutData;
        model->bld->Insert<float>(3); // positions
        model->bld->Insert<float>(2); // tex coords
        model->bld->Insert<float>(3); // normals
        model->va = new VertexArray;
        model->va->Bind();
        model->va->AddBuffer(*model->vb, *model->bld);
    }
}
