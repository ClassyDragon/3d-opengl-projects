#shader type vertex
#version 130
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;
out vec2 v_TextureCoordinates;
uniform mat4 MVP;
//uniform mat4 trans;
void main() {
    //gl_Position = MVP * trans * position;
    gl_Position = MVP * position;
    v_TextureCoordinates = textureCoordinates;
}

#shader type fragment
#version 130
#extension GL_ARB_separate_shader_objects : require
out vec4 color;
in vec2 v_TextureCoordinates;
uniform sampler2D u_Texture;
void main() {
    vec4 texColor = texture(u_Texture, v_TextureCoordinates);
    color = texColor;
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}
