#include "Shader.h"

Shader::Shader(const char* filePath, GLenum shaderType)
{
    const string src = readTextFile(filePath);
    const char* csrc = src.c_str();

    id = glCreateShader(shaderType);
    glShaderSource(id, 1, &csrc, nullptr);
    glCompileShader(id);
    checkShaderCompilation(id);
}

Shader::~Shader() {if(id) glDeleteShader(id);}

string Shader::readTextFile(const char* path)
{
    ifstream in(path);
    if(!in.is_open())
    {
        fprintf(stderr, "ERROR: cannot open shader file: %s\n", path);
        exit(EXIT_FAILURE);
    }

    stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

void Shader::checkShaderCompilation(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "ERROR: Shader compilation failed: %s\n", infoLog);
        exit(EXIT_FAILURE);
    }
}