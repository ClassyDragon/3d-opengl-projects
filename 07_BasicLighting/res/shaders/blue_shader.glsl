#shader type vertex
#version 130
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec4 position;
uniform mat4 MVP;
void main() {
    gl_Position = MVP * position;
}

#shader type fragment
#version 130
#extension GL_ARB_separate_shader_objects : require
out vec4 color;
void main() {
    color = vec4(0.2, 0.4, 0.9, 1.0);
}
