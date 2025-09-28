#version 330
in vec3 vColor;
out vec4 frag_colour;
void main() {frag_colour = vec4(vColor.bgr, 1.0);}