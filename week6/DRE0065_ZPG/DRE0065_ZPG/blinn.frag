#version 330 core
in vec3 fragPos;
in vec3 normal;

out vec4 frag_colour;

uniform vec3 lightPosition;
uniform mat4 viewMatrix;

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
    vec3 V = normalize(vec3(inverse(viewMatrix)[3]) - fragPos);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = (dot(N, L) > 0.0) ? pow(max(dot(N, H), 0.0), 128.0) : 0.0;
    //float spec = pow(max(dot(N, H), 0.0), 128.0);

    vec3 color = vec3(0.70) * (0.10 + diff) + vec3(1.0) * (0.50 * spec);
    frag_colour = vec4(color, 1.0);

    if(numberOfLights >= 1)
    {
        vec3 sumD = vec3(0.0);
        vec3 sumS = vec3(0.0);
        for(int i = 0; i < numberOfLights; i++)
        {
            vec3 Li = normalize(uLights[i].position.xyz - fragPos);
            vec3 Hi = normalize(Li + V);
            float d = max(dot(N, Li), 0.0);
            float s = (d > 0.0) ? pow(max(dot(N, Hi), 0.0), 128.0) : 0.0;
            //float s = pow(max(dot(N, Hi), 0.0), 128.0);
            sumD += uLights[i].diffuse.rgb  * d;
            sumS += uLights[i].specular.rgb * s;
        }
        vec3 multi = vec3(0.70) * 0.10 + sumD + vec3(1.0) * (0.50) * sumS;
        frag_colour = vec4(multi, 1.0);
    }
    else if(numberOfLights == 0) frag_colour = vec4(vec3(0.70) * 0.10, 1.0);
}