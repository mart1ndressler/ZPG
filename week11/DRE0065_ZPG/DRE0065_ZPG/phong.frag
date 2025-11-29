#version 330 core
in vec3 fragPos, normal, localPos;
in vec2 uv;
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
uniform vec3 baseColor, cameraPos;
uniform float emissive, worldScale;
uniform int sceneId;

uniform samplerCube skyboxTex;
uniform int isSkybox, isSkyCube;

uniform sampler2D textureUnitID;
uniform int useTexture;
uniform float uvTiling;

uniform Light uLights[MAX_LIGHTS];
uniform int numberOfLights, forestLightMode;

uniform vec3 dirLightDir, dirLightDiffuse;
uniform float dirSplitOffset, dirSplitSoftness;

uniform int flashlightOn;
uniform vec3 flashlightPos, flashlightDir, flashlightDiffuse, flashlightAtten;
uniform float flashlightInnerCos, flashlightOuterCos;

void main()
{
    if(isSkybox != 0 || isSkyCube != 0)
    {
        fragColor = texture(skyboxTex, normalize(localPos));
        return;
    }

    vec3 N = normalize(normal);
    vec3 fragWorld = fragPos * worldScale;
    vec3 V = normalize(cameraPos - fragWorld);

    float Ra = material.ra; /*(material.ra > 0.0) ? material.ra : 1.0;*/
    float Rd = material.rd; /*(material.rd > 0.0) ? material.rd : 1.0;*/
    float Rs = material.rs; /*(material.rs > 0.0) ? material.rs : 1.0;*/
    float H = material.h; /*(material.h > 0.0) ? material.h : 16.0;*/
    if(sceneId == 2)
    {
        vec3 sumD = vec3(0.0), sumS = vec3(0.0);
        for(int i = 0; i < numberOfLights; i++)
        {
            vec3 Li = normalize(uLights[i].position.xyz - fragWorld);
            vec3 Ri = reflect(-Li, N);

            float d = max(dot(N, Li), 0.0);
            float s = (d > 0.0) ? pow(max(dot(V, Ri), 0.0), H) : 0.0;

            sumD += vec3(uLights[i].diffuse)  * d;
            sumS += vec3(uLights[i].specular) * s;
        }

        vec3 color = vec3(0.07) * Ra + sumD * Rd + vec3(0.5) * sumS * Rs;
        fragColor = vec4(color, 1.0);
        return;
    }

    vec3 texColor = vec3(texture(textureUnitID, uv * uvTiling));
    vec3 base = (useTexture == 1) ? texColor : baseColor;
    
    vec3 ambient = base * 0.35 * Ra;
    vec3 diffuse = vec3(0.0), specular = vec3(0.0);
    if(forestLightMode == 1 || forestLightMode == 3)
    {
        for(int i = 0; i < numberOfLights; i++)
        {
            vec3 Ldir = uLights[i].position.xyz - fragWorld;
            float dist = length(Ldir);
            vec3 L = (dist > 0.0) ? (Ldir / dist) : vec3(0.0);
            float d = max(dot(N, L), 0.0);

            vec3 Rv = reflect(-L, N);
            float s = (d > 0.0) ? pow(max(dot(V, Rv), 0.0), H) : 0.0;

            float att = 1.0 / (uLights[i].atten.x + uLights[i].atten.y * dist + uLights[i].atten.z * dist * dist);
            diffuse += base * vec3(uLights[i].diffuse) * d * att;
            specular += vec3(uLights[i].specular) * s * Rs * att * 0.35;
        }
    }
    if(forestLightMode == 2 || forestLightMode == 3)
    {
        vec3 L = normalize(-dirLightDir);
        float ndlWrap = clamp((dot(N,L) + 0.30) / 1.30, 0.0, 1.0);
        float mask = smoothstep(0.0, dirSplitSoftness, dot(fragWorld, L) - dirSplitOffset);
        diffuse += base * dirLightDiffuse * ndlWrap * mask;
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
        vec3 Rv = reflect(-L, N);
        float s = (d > 0.0) ? pow(max(dot(V, Rv), 0.0), H) : 0.0;

        float att = 1.0 / (flashlightAtten.x + flashlightAtten.y * dist + flashlightAtten.z * dist * dist);
        diffuse += base * flashlightDiffuse * d * att * spot;
        specular += flashlightDiffuse * s * Rs * att * spot * 0.35;
    }

    vec3 emissiveTerm = base * emissive;
    vec3 color = ambient + diffuse * Rd + specular + emissiveTerm;
    fragColor = vec4(color, 1.0);
}