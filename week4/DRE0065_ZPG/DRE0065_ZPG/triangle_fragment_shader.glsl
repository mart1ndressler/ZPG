#version 330 core
in vec3 vColor;
out vec4 frag_colour;

void main() {frag_colour = vec4(vColor, 1.0);}