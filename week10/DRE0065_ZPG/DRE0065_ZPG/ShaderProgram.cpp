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

ShaderProgram::~ShaderProgram()
{
    if(programID) glDeleteProgram(programID);
}

void ShaderProgram::use() const
{
    glUseProgram(programID);
}

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

GLint ShaderProgram::uniformLoc(const char* name) const 
{
    return glGetUniformLocation(programID, name);
}

void ShaderProgram::set(const char* name, int value) const 
{
    glUniform1i(uniformLoc(name), value);
}

void ShaderProgram::set(const char* name, float value) const
{
    glUniform1f(uniformLoc(name), value);
}

void ShaderProgram::set(const char* name, const vec3& vec) const
{
    glUniform3fv(uniformLoc(name), 1, &vec[0]);
}

void ShaderProgram::set(const char* name, const vec4& vec) const
{
    glUniform4fv(uniformLoc(name), 1, &vec[0]);
}

void ShaderProgram::set(const char* name, const mat4& mat) const
{
    glUniformMatrix4fv(uniformLoc(name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::updateViewProjection(const mat4& view, const mat4& projection)
{
    use();
    set("viewMatrix", view);
    set("projectionMatrix", projection);

	mat4 invView = inverse(view);
	vec3 camPos = vec3(invView[3]);
    set("cameraPos", camPos);
}

void ShaderProgram::updateLight(const vec3& lightPos)
{
    use();
    set("lightPosition", lightPos);
}

void ShaderProgram::updateTransform(const mat4& customMatrix, int transformMode)
{
    use();
    set("customMatrix", customMatrix);
    set("transformMode", transformMode);

    float worldScale = (transformMode == 2) ? 20.0f : 1.0f;
    set("worldScale", worldScale);
}