#version 460 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 coords;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    TexCoords = coords;
    gl_Position = projection * vec4(position, 0.0f, 1.0f);
}