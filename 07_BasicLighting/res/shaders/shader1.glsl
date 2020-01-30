#shader type vertex
#version 130
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TextureCoordinates;
layout(location = 2) in vec3 a_Normal;

out vec2 v_TextureCoordinates;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 MVP;
uniform mat4 model;

void main() {
    gl_Position = MVP * a_Position;
    FragPos = vec3(model * a_Position);
    v_TextureCoordinates = a_TextureCoordinates;
    Normal = a_Normal;
}

#shader type fragment
#version 130
#extension GL_ARB_separate_shader_objects : require
out vec4 color;

in vec2 v_TextureCoordinates;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture;
uniform vec3 u_LightPosition;

void main() {
    float ambientStrength = 0.47;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = ambientStrength * lightColor;
    vec4 texColor = texture(u_Texture, v_TextureCoordinates);
    vec4 result = vec4(texColor.x * (ambient.x + diffuse.x), texColor.y * (ambient.y + diffuse.y), texColor.z * (ambient.z + diffuse.z), texColor.w);
    color = result;
}
