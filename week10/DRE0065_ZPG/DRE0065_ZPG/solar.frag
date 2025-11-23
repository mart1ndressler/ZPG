#version 330 core
in vec3 fragPos, normal;
in vec2 uv;
out vec4 fragColor;

struct Material {
    float ra;
    float rd;
    float rs;
    float h;
};
uniform Material material;

uniform vec3 objectColor, lightPosition, cameraPos, lightAtten;
uniform float emissive;

uniform sampler2D textureUnitID;
uniform int useTexture;
uniform float uvTiling;

uniform float worldScale;

void main()
{
    vec3 N = normalize(normal);
    vec3 fragWorld = fragPos * worldScale;
    vec3 L = normalize(lightPosition - fragWorld);
    vec3 V = normalize(cameraPos - fragWorld);
    vec3 R = reflect(-L, N);

    float dist = length(lightPosition - fragWorld);
    float att = 1.0 / (lightAtten.x + lightAtten.y * dist + lightAtten.z * dist * dist);

    vec3 texColor = vec3(texture(textureUnitID, uv * uvTiling));
    vec3 base = (useTexture == 1) ? texColor : objectColor;

    float Ra = (material.ra > 0.0) ? material.ra : 1.0;
    float Rd = (material.rd > 0.0) ? material.rd : 1.0;
    float Rs = (material.rs > 0.0) ? material.rs : 1.0;
    float H = (material.h > 0.0) ? material.h : 32.0;

    float diff = max(dot(N, L), 0.0);
    float specPow = pow(max(dot(V, R), 0.0), H);

    vec3 ambient = base * 0.15 * Ra;
    vec3 diffuse = base * diff * Rd * att;
    vec3 specular = vec3(1.0) * 0.35 * specPow * Rs * att;
    vec3 emissiveTerm = base * emissive;

    vec3 color = ambient + diffuse + specular + emissiveTerm;
    fragColor = vec4(color, 1.0);
}