#shader type vertex
#version 130
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 a_Color;
out vec4 o_Color;
uniform mat4 MVP;
// uniform mat4 trans;
void main() {
    gl_Position = MVP * position;
    o_Color = a_Color;
}

#shader type fragment
#version 130
#extension GL_ARB_separate_shader_objects : require
in vec4 o_Color;
out vec4 color;
void main() {
    color = o_Color;
}
