#include "Shader.h"

Shader::Shader(const std::string& filename) 
    : ID(0) 
{
    ShaderData source = ReadShader(filename); // Read vertex and fragment shaders
    ID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(this->ID));
}

int Shader::GetUniformLocation(const std::string& name) {
    if (uniforms.find(name) != uniforms.end()) {
        return uniforms[name];
    }
    GLCall(int location = glGetUniformLocation(this->ID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    uniforms[name] = location;
    return location;
}

void Shader::SetUniform1i(const std::string& name, int i) {
    GLCall(glUniform1i(GetUniformLocation(name), i));
}

void Shader::SetUniform1f(const std::string& name, float f) {
    GLCall(glUniform1f(GetUniformLocation(name), f));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3) {
    GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::Bind() const {
    GLCall(glUseProgram(this->ID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}


// Private Functions:
ShaderData Shader::ReadShader(const std::string& filename) {
    ShaderData data;
    ShaderType type;
    std::ifstream stream(filename);
    std::string line;
    std::stringstream ss[2];
    while (getline(stream, line)) {
        if (line.find("#shader type") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::Vertex;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::Fragment;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    stream.close();
    data = 
        ShaderData({ ss[(int)ShaderType::Vertex].str(), ss[(int)ShaderType::Fragment].str() });
    return data;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);

    glCompileShader(id);

    // Error handling
    int result;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
