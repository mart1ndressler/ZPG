#version 330 core
in vec3 fragPos;
in vec3 normal;
out vec4 fragColor;

#define MAX_LIGHTS 16
struct Light {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec3 atten;
};

struct Material {
    float ra;
    float rd;
    float rs;
    float h;
};
uniform Material material;
uniform vec3 baseColor;

uniform Light uLights[MAX_LIGHTS];
uniform int numberOfLights, forestLightMode;

uniform vec3 dirLightDir, dirLightDiffuse;
uniform float dirSplitOffset, dirSplitSoftness;

uniform int flashlightOn;
uniform vec3 flashlightPos, flashlightDir, flashlightDiffuse, flashlightAtten;
uniform float flashlightInnerCos, flashlightOuterCos;

void main()
{
    vec3 N = normalize(normal);

    float Ra = (material.ra > 0.0) ? material.ra : 1.0;
    float Rd = (material.rd > 0.0) ? material.rd : 1.0;

    vec3 ambient = baseColor * 0.04 * Ra;
    vec3 diffuse = vec3(0.0);
    if(forestLightMode == 1 || forestLightMode == 3)
    {
        for(int i = 0; i < numberOfLights; i++)
        {
            vec3 Ldir = uLights[i].position.xyz - fragPos;
            float dist = length(Ldir);
            vec3 L = (dist > 0.0) ? (Ldir / dist) : vec3(0.0);
            float d = max(dot(N, L), 0.0);

            float att = 1.0 / (uLights[i].atten.x + uLights[i].atten.y * dist + uLights[i].atten.z * dist * dist);
            diffuse += baseColor * uLights[i].diffuse.rgb * d * att;
        }
    }
    if(forestLightMode == 2 || forestLightMode == 3)
    {
        vec3 L = normalize(-dirLightDir);
        float ndlWrap = clamp((dot(N,L) + 0.30) / 1.30, 0.0, 1.0);
        float mask = smoothstep(0.0, dirSplitSoftness, dot(fragPos, L) - dirSplitOffset);
        diffuse += baseColor * dirLightDiffuse * ndlWrap * mask;
    }
    if(flashlightOn == 1)
    {
        vec3 Ldir = flashlightPos - fragPos;
        float dist = length(Ldir);
        vec3 L = (dist > 0.0) ? (Ldir / dist) : vec3(0.0);

        float theta = dot(normalize(fragPos - flashlightPos), normalize(flashlightDir));
        float epsilon = flashlightInnerCos - flashlightOuterCos;
        float spot = clamp((theta - flashlightOuterCos) / max(epsilon, 0.0001), 0.0, 1.0);

        float d = max(dot(N, L), 0.0);
        float att = 1.0 / (flashlightAtten.x + flashlightAtten.y * dist + flashlightAtten.z * dist * dist);
        diffuse += baseColor * flashlightDiffuse * d * att * spot;
    }
    vec3 color = ambient + diffuse * Rd;
    fragColor = vec4(color, 1.0);
}