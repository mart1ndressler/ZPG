#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt;

out vec3 fragPos, normal, localPos;
out vec2 uv;

uniform mat4 modelMatrix, viewMatrix, projectionMatrix;
uniform int isSkybox, isSkyCube;

void main()
{
    localPos = vp;
    if(isSkybox == 1)
    {
        vec4 pos = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(vp, 1.0);
        fragPos = vec3(0.0);
        normal = vec3(0.0);
        uv = vec2(0.0);
        gl_Position = vec4(pos.xy, pos.w, pos.w);
        return;
    }

    vec4 worldPos = modelMatrix * vec4(vp, 1.0);
    fragPos = worldPos.xyz;
    normal = normalize(transpose(mat3(inverse(modelMatrix))) * vn);
    uv = vt;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}