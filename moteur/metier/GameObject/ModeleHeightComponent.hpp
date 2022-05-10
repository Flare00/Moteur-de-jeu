#ifndef __MODELE_HEIGHT_COMPONENT_HPP__
#define __MODELE_HEIGHT_COMPONENT_HPP__

#include <objloader.hpp>

#include <vector>
#include <glm/glm.hpp>

#include <Shader/GlobalShaderExtended.hpp>
#include "ModeleComponent.hpp"
#include <Texture.hpp>

class ModeleHeightComponent : public ModeleComponent
{
private:
    GlobalShaderExtended *shader;

protected:
    TextureContainer heightMap;
    float maxHeight;
public:
    // --- CONSTRUCTEURS ET DESTRUCTEURS ---

    ModeleHeightComponent(GlobalShaderExtended *shader, FileType type, std::string file, float maxHeight) : ModeleComponent(shader, type, file)
    {
        this->shader = shader;
        this->maxHeight = maxHeight;
        this->shader->setMaxHeight(this->maxHeight);
    }

    ModeleHeightComponent(GlobalShaderExtended *shader, float maxHeight) : ModeleComponent(shader)
    {
        this->shader = shader;
        this->maxHeight = maxHeight;
        this->shader->setMaxHeight(this->maxHeight);
    }

    ModeleHeightComponent(GlobalShaderExtended *shader, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords, float maxHeight) : ModeleComponent(shader, indexed_vertices, normals, indices, texCoords)
    {
        this->shader = shader;
        this->maxHeight = maxHeight;
        this->shader->setMaxHeight(this->maxHeight);
    }

    virtual void draw(CameraData *data,Lightning* lights, glm::mat4 transform, glm::vec3 position)
    {
        bool isInFOV = true;
        if(data->getType() == CameraData::CAMERA){
            this->shader->use();
        
            glEnable(GL_TEXTURE_2D);
            this->shader->drawHeightMap(heightMap.texture, (int)textures.size());
            ModeleComponent::draw(data, lights, transform, position);
        }
        else {
            data->getShadowShader()->drawHeightMap(heightMap.texture, 0, this->maxHeight);
            ModeleComponent::draw(data, lights, transform, position);
        }
    }

    void setHeightMap(Texture *t, bool destroyEnd)
    {
        heightMap.texture = t;
        heightMap.destroyAtEnd = destroyEnd;
    }
};

#endif