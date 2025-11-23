#include "Skybox.h"
#include "skycube.h"
#include "stb_image.h"

Skybox::Skybox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skycube), skycube, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (int)sizeof(float), (void*)0);
    glBindVertexArray(0);

    string faces[6] = {string("assets/") + posx, string("assets/") + negx, string("assets/") + posy, string("assets/") + negy, string("assets/") + posz, string("assets/") + negz};
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    stbi_set_flip_vertically_on_load(false);
    for(int i = 0; i < 6; i++)
    {
        int w, h, ch;
        void* data = stbi_load(faces[i].c_str(), &w, &h, &ch, 4);
        if(!data) cerr << "Failed to load texture: " << faces[i] << endl;
        else
        {
            //cerr << "Successfully loaded texture: " << faces[i] << endl;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::~Skybox()
{
    if(cubemap) glDeleteTextures(1, &cubemap);
    if(vbo) glDeleteBuffers(1, &vbo);
    if(vao) glDeleteVertexArrays(1, &vao);
}

void Skybox::applySB(ShaderProgram* shaderProgram, bool asBackground, const mat4* modelMatrix, int textureUnit)
{
    if(!shaderProgram || !vao || !cubemap) return;
    if(asBackground)
    {
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
    }

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    shaderProgram->use();
    shaderProgram->set("skyboxTex", textureUnit);
    if(asBackground)
    {
        shaderProgram->set("isSkybox", 1);
        shaderProgram->set("isSkyCube", 0);
    }
    else
    {
        shaderProgram->set("isSkybox", 0);
        shaderProgram->set("isSkyCube", 1);
        mat4 M = modelMatrix ? *modelMatrix : mat4(1.0f);
        shaderProgram->set("modelMatrix", M);
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    if(asBackground)
    {
        shaderProgram->set("isSkybox", 0);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }
    else shaderProgram->set("isSkyCube", 0);
}