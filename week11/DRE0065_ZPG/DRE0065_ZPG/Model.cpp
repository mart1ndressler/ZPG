#include "Model.h"
#include "tiny_obj_loader.h"

Model::Model() : hasMtl(false), mtlDiffuse(0.7f), mtlShininess(16.0f) {}

Model::Model(const char* name)
{
    string inputfile = string("assets/") + name;
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), "assets/");
    if(!warn.empty() && string(name) != "planet.obj") cout << "Warn: " << warn << endl;
    if(!err.empty()) cerr << "Err: " << err << endl;
    if(!ret) throw runtime_error("Failed to load OBJ file!");

    hasMtl = false;
    mtlDiffuse = vec3(0.7f);
    mtlShininess = 16.0f;

    if(!materials.empty())
    {
        const tinyobj::material_t& m = materials[0];

        mtlDiffuse = vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
        if(mtlDiffuse == vec3(0.0f)) mtlDiffuse = vec3(0.7f);

        mtlShininess = (m.shininess > 0.0f) ? m.shininess : 16.0f;
        hasMtl = true;
        //if(hasMtl) cerr << "[MTL] " << name << " diffuse = (" << m.diffuse[0] << ", " << m.diffuse[1] << ", " << m.diffuse[2] << ")" << ", shininess = " << m.shininess << endl;
    }

    vector<float> vertices;
    for(const auto& shape : shapes)
    {
        for(const auto& index : shape.mesh.indices)
        {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            if(index.normal_index >= 0)
            {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            if(index.texcoord_index >= 0)
            {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    vertexCount = (int)vertices.size() / 3;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

Model::~Model()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    VAO = 0;
    VBO = 0;
}

void Model::setupModel(const float* data, int sizeInBytes, bool hasNormals, bool hasUV)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW);

    int stride = 3;
    if(!hasNormals) stride = 3;
    else stride = hasUV ? 8 : 6;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * (int)sizeof(float), (void*)0);

    if(stride >= 6)
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * (int)sizeof(float), (void*)(3 * (int)sizeof(float)));
    }

    if(stride == 8)
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * (int)sizeof(float), (void*)(6 * (int)sizeof(float)));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    vertexCount = sizeInBytes / (stride * (int)sizeof(float));
}

void Model::applyMdlMtl(ShaderProgram* shaderProgram)
{
    if(!shaderProgram) return;
    shaderProgram->use();
    shaderProgram->set("useTexture", 0);

    if(hasMtl) Material(mtlDiffuse, 1.0f, 1.0f, 1.0f, mtlShininess).applyM(shaderProgram);
    else Material(vec3(0.70f, 0.70f, 0.70f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(shaderProgram);
}

GLuint Model::vao() const
{
    return VAO;
}

int Model::count() const
{
    return vertexCount;
}