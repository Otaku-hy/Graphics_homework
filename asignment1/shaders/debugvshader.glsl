#version 430 core
in vec3 vPosition;
in vec2 vTexCoords;

out vec2 TexCoords;

layout(binding = 0) uniform sampler2D depthMap;

void main()
{
    gl_Position = vec4(vPosition,1.0);
    TexCoords = vTexCoords;
}