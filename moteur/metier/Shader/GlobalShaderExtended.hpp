#ifndef __GLOBAL_SHADER_EXTENDED_H__
#define __GLOBAL_SHADER_EXTENDED_H__

#define MAX_LIGHT 128
#include <string>
#include <vector>

#include "Shader.hpp"
#include "Transformation.hpp"
#include "Texture.hpp"
#include <Global.hpp>
#include <Light/Light.hpp>
#include <Light/Material.hpp>

class GlobalShaderExtended : public GlobalShader
{
protected:
    GLuint u_heightmap;

public:
    GlobalShaderExtended(std::string vertex, std::string fragment, std::string tessControl, std::string tessEval, std::string geometry = "") : GlobalShader(vertex, fragment, geometry, tessControl, tessEval)
    {
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        this->u_heightmap = glGetUniformLocation(this->id, "u_heightmap");
    }

    void drawHeightMap(Texture *texture, int id)
    {
        if (texture != NULL)
        {
            texture->draw(this->u_heightmap, id);
        }
    }

    virtual void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix, Material material, bool tess = false)
    {
        GlobalShader::drawMesh(VAO, size_indice, transformMatrix, material, true);
    }
};

#endif // !__GLOBAL_SHADER_H__
