#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt;

out vec3 fragPos, normal, localPos;
out vec2 uv;

uniform mat4 modelMatrix, viewMatrix, projectionMatrix, customMatrix;
uniform int isSkybox, transformMode = 0;
uniform float w = 500.0;

void main()
{
    localPos = vp;
    if(isSkybox == 1)
    {
        gl_Position = projectionMatrix * vec4(mat3(viewMatrix) * vp, 1.0);
        return;
    }

    vec4 worldPos;
    if(transformMode == 0) worldPos = modelMatrix * vec4(vp, 1.0);
    else
    {
        worldPos = modelMatrix * vec4(vp * w, w);
        if(transformMode == 2) worldPos = customMatrix * worldPos;
    }

    fragPos = worldPos.xyz / worldPos.w;
    normal = normalize(transpose(mat3(inverse(modelMatrix))) * vn);
    uv = vt;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}