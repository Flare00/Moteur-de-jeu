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
        this->u_heightmap = glGetUniformLocation(this->id, "u_heightmap");
    }

    void drawHeightMap(Texture *texture, int id, float zHeight)
    {
        if (texture != NULL)
        {
            texture->draw(this->u_heightmap, id);
        }
    }

    virtual void drawMesh(GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform1i(this->u_nb_texture, (GLint)this->u_textures.size());
        glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_PATCHES, 0, size_indice);

    }
};

#endif // !__GLOBAL_SHADER_H__
