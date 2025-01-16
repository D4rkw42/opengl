#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 color;
out vec2 texCoord;

uniform float scale;

void main() {
    gl_Position = vec4(aPos.x * (1.0f + scale), aPos.y * (1.0f + scale), aPos.z * (1.0f + scale), 1.0f);

    color = aColor;
    texCoord = aTex;
}
