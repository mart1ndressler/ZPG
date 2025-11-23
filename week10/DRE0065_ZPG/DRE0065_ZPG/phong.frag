#version 330 core
in vec3 fragPos, normal;
out vec4 fragColor;

#define MAX_LIGHTS 5
struct Light {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
};

struct Material {
    float ra;
    float rd;
    float rs;
    float h;
};
uniform Material material;

uniform Light uLights[MAX_LIGHTS];
uniform int numberOfLights;
uniform vec3 cameraPos;

uniform float worldScale;

void main()
{
    vec3 N = normalize(normal);
    vec3 fragWorld = fragPos * worldScale;
    vec3 V = normalize(cameraPos - fragWorld);

    float Ra = (material.ra > 0.0) ? material.ra : 1.0;
    float Rd = (material.rd > 0.0) ? material.rd : 1.0;
    float Rs = (material.rs > 0.0) ? material.rs : 1.0;
    float H = (material.h > 0.0) ? material.h : 16.0;

    vec3 sumD = vec3(0.0), sumS = vec3(0.0);
    for(int i = 0; i < numberOfLights; i++)
    {
        vec3 Li = normalize(uLights[i].position.xyz - fragWorld);
        vec3 Ri = reflect(-Li, N);

        float d = max(dot(N, Li), 0.0);
        float s = (d > 0.0) ? pow(max(dot(V, Ri), 0.0), H) : 0.0;

        sumD += vec3(uLights[i].diffuse) * d;
        sumS += vec3(uLights[i].specular) * s;
    }

    vec3 ambient = vec3(0.70) * 0.10 * Ra;
    vec3 diffuse = sumD * Rd;
    vec3 specular = vec3(1.0) * 0.50 * sumS * Rs;

    vec3 color = ambient + diffuse + specular;
    fragColor = vec4(color, 1.0);
}