#version 330 core
in vec3 fragPos;
in vec3 normal;

out vec4 frag_colour;

uniform vec3 lightPosition;

#define MAX_LIGHTS 5
uniform int numberOfLights;
uniform vec3 lights[MAX_LIGHTS];

struct Light {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
};
uniform Light uLights[MAX_LIGHTS];

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - fragPos);

    float diff = max(dot(N, L), 0.0);
    vec3 color = vec3(0.70) * (0.10 + diff);
    frag_colour = vec4(color, 1.0);

    if(numberOfLights >= 1)
    {
        vec3 sum = vec3(0.0);
        for(int i = 0; i < numberOfLights; i++)
        {
            vec3 Li = normalize(uLights[i].position.xyz - fragPos);
            float d = max(dot(N, Li), 0.0);
            sum += uLights[i].diffuse.rgb * d;
        }
        vec3 multi = vec3(0.70) * 0.10 + sum;
        frag_colour = vec4(multi, 1.0);
    }
    else if(numberOfLights == 0) frag_colour = vec4(vec3(0.70) * 0.10, 1.0);
}