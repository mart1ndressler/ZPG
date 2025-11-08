#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* fileName)
{
    string path = string("assets/") + fileName;

    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    void* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
    if(!data)
    {
        fprintf(stderr, "ERROR: cannot load texture: %s\n", fileName);
        return;
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
}

Texture::~Texture()
{
    if(tex) glDeleteTextures(1, &tex);
}

void Texture::bind(int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void Texture::applyT(ShaderProgram* shaderProgram, int unit, int useTexture, float uvTiling) const
{
    if(!shaderProgram) return;
    shaderProgram->use();
    shaderProgram->set("textureUnitID", unit);
    shaderProgram->set("useTexture", useTexture);
    shaderProgram->set("uvTiling", uvTiling);
    if(useTexture == 1)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, tex);
    }
}