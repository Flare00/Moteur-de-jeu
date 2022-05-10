#ifndef __GLOBAL_SHADER_EXTENDED_H__
#define __GLOBAL_SHADER_EXTENDED_H__

#include <string>
#include <vector>

#include <Shader/Shader.hpp>
#include <Texture.hpp>
#include <Global.hpp>
#include <Light/ILight.hpp>
#include <Light/Material.hpp>

class GlobalShaderExtended : public GlobalShader
{
protected:
    GLuint u_heightmap;
    GLuint u_max_height;
    float maxHeight = 1.0f;
public:
    GlobalShaderExtended(std::string vertex, std::string fragment, std::string tessControl, std::string tessEval, std::string geometry = "") : GlobalShader(vertex, fragment, geometry, tessControl, tessEval)
    {
        this->u_heightmap = glGetUniformLocation(this->id, "u_heightmap");
        this->u_max_height = glGetUniformLocation(this->id, "u_max_height");
    }

    void setMaxHeight(float maxHeight) {
        this->maxHeight = maxHeight;
    }

    void drawHeightMap(Texture *texture, int id)
    {
        if (texture != NULL)
        {
            texture->draw(this->u_heightmap, id);
        }
    }

    virtual void drawMesh(CameraData* data, GLuint VAO, GLsizei size_indice, glm::mat4 transformMatrix)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(data->getType() == CameraData::CAMERA){
            glUniform1i(this->u_nb_texture, (GLint)this->u_textures.size());
        }
        glUniform1f(this->u_max_height, maxHeight);
        glUniformMatrix4fv(this->u_model, 1, GL_FALSE, &transformMatrix[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_PATCHES, 0, size_indice);

    }
};

#endif // !__GLOBAL_SHADER_H__
