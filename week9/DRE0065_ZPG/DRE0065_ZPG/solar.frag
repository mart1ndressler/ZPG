#version 330 core
in vec3 fragPos, normal;
out vec4 fragColor;

struct Material {
    float ra;
    float rd;
    float rs;
    float h;
};
uniform Material material;

uniform vec3 objectColor, lightPosition;
uniform mat4 viewMatrix;
uniform float emissive;

uniform sampler2D textureUnitID;
uniform int useTexture;
uniform float uvTiling;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - fragPos);
    vec3 V = normalize(vec3(inverse(viewMatrix)[3]) - fragPos);
    vec3 R = reflect(-L, N);

    vec2 sphereUV = vec2(atan(N.z, N.x) / (2.0 * 3.14) + 0.5, 0.5 - asin(N.y) / 3.14);
    vec3 base = (useTexture == 1) ? texture(textureUnitID, sphereUV * uvTiling).rgb : objectColor;

    float Ra = (material.ra > 0.0) ? material.ra : 1.0;
    float Rd = (material.rd > 0.0) ? material.rd : 1.0;
    float Rs = (material.rs > 0.0) ? material.rs : 1.0;
    float H = (material.h > 0.0) ? material.h : 32.0;

    float diff = max(dot(N, L), 0.0);
    float specPow = pow(max(dot(V, R), 0.0), H);

    vec3 ambient = base * 0.15 * Ra;
    vec3 diffuse = base * diff * Rd;
    vec3 specular = vec3(1.0) * 0.35 * specPow * Rs;
    vec3 emissiveTerm = base * emissive;

    vec3 color = ambient + diffuse + specular + emissiveTerm;
    fragColor = vec4(color, 1.0);
}