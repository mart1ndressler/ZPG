#version 330 core
layout(location = 0) in vec3 vp;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
out vec3 vColor;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
    vColor = vp * 1.5 + 0.5;
}