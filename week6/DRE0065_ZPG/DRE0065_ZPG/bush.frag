#version 330 core
in vec3 fragPos;
in vec3 normal;
out vec4 fragColor;

#define MAX_LIGHTS 5
uniform int numberOfLights;

struct Light {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec3 atten;
};
uniform Light uLights[MAX_LIGHTS];

void main()
{
    vec3 N = normalize(normal);
    vec3 base = vec3(0.20, 0.65, 0.28);
    vec3 amb  = base * 0.05;
    vec3 sum = vec3(0.0);

    for(int i = 0; i < numberOfLights; i++)
    {
        vec3 Ldir = uLights[i].position.xyz - fragPos;
        float dist = length(Ldir);
        vec3 L = normalize(Ldir);
        float d = max(dot(N, L), 0.0);
        float att = 1.0 / (uLights[i].atten.x + uLights[i].atten.y * dist + uLights[i].atten.z * dist * dist);
        sum += base * uLights[i].diffuse.rgb * d * att;
    }
    fragColor = vec4(amb + sum, 1.0);
}