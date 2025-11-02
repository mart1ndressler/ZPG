#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;

out vec3 fragPos;
out vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 worldPos = modelMatrix * vec4(vp, 1.0);
    fragPos = worldPos.xyz;
    normal = normalize(transpose(mat3(inverse(modelMatrix))) * vn);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}