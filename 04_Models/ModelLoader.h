#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Model.h"

class Model;

enum class FileType {
    OBJ
};

class ModelLoader {
    public:
        static void LoadModel(const std::string& modelFile, int filetype, Model* model);
};
