#version 330
layout(location=0) in vec3 vp;
uniform mat4 modelMatrix;
out vec3 vColor;

void main() 
{
    gl_Position = modelMatrix * vec4(vp, 1.0);
    vColor = vp * 1.5 + 0.5;
}