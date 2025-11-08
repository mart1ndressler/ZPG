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

void main()
{
    vec3 N = normalize(normal);

    float Ra = (material.ra > 0.0) ? material.ra : 1.0;
    float Rd = (material.rd > 0.0) ? material.rd : 1.0;

    vec3 sumD = vec3(0.0);
    for(int i = 0; i < numberOfLights; i++)
    {
        vec3 Li = normalize(uLights[i].position.xyz - fragPos);
        float d = max(dot(N, Li), 0.0);
        sumD += uLights[i].diffuse.rgb * d;
    }

    vec3 ambient = vec3(0.70) * 0.10 * Ra;
    vec3 diffuse = sumD * Rd;

    vec3 color = ambient + diffuse;
    fragColor = vec4(color, 1.0);
}