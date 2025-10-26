#version 330 core
in vec3 fragPos;
in vec3 normal;

out vec4 frag_colour;

uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform mat4 viewMatrix;

uniform float emissive;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - fragPos);
    vec3 V = normalize(vec3(inverse(viewMatrix)[3]) - fragPos);
    vec3 R = reflect(-L, N);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    vec3 color = objectColor * (0.15 + diff) + vec3(1.0) * (0.35 * spec) + objectColor * emissive;
    frag_colour = vec4(color, 1.0);
}