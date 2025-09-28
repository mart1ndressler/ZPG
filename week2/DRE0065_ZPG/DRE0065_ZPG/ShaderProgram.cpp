#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertexSrc, const char* fragmentSrc)
{
    Shader vertex(vertexSrc, GL_VERTEX_SHADER);
    Shader fragment(fragmentSrc, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertex.shaderID());
    glAttachShader(programID, fragment.shaderID());
    glLinkProgram(programID);
    checkLinking(programID);
}

ShaderProgram::~ShaderProgram() {if(programID) glDeleteProgram(programID);}
void ShaderProgram::use() const {glUseProgram(programID);}

void ShaderProgram::checkLinking(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, nullptr, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
        exit(EXIT_FAILURE);
    }
}