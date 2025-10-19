#version 330 core
in vec3 fragPos;
in vec3 normal;

out vec4 frag_colour;

uniform vec3 lightPosition;
uniform mat4 viewMatrix;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - fragPos);
    vec3 V = normalize(vec3(inverse(viewMatrix)[3]) - fragPos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 128.0);

    vec3 color = vec3(0.70) * (0.10 + diff) + vec3(1.0) * (0.50 * spec);
    frag_colour = vec4(color, 1.0);
}