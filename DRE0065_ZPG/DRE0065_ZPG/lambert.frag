#version 330 core
in vec3 fragPos;
in vec3 normal;

out vec4 frag_colour;

uniform vec3 lightPosition;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - fragPos);
    float diff = max(dot(N, L), 0.0);

    vec3 color = vec3(0.70) * (0.10 + diff);
    frag_colour = vec4(color, 1.0);
}