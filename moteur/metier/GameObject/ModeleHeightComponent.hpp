#ifndef __MODELE_HEIGHT_COMPONENT_HPP__
#define __MODELE_HEIGHT_COMPONENT_HPP__

#include <objloader.hpp>

#include <vector>
#include <glm/glm.hpp>

#include <Shader/GlobalShaderExtended.hpp>
#include "ModeleComponent.hpp"
#include <Texture.hpp>
#include <Collision/RigidBody.hpp>

class ModeleHeightComponent : public ModeleComponent
{
private:
    GlobalShaderExtended *shader;

protected:
    TextureContainer heightMap;

public:
    // --- CONSTRUCTEURS ET DESTRUCTEURS ---

    ModeleHeightComponent(GlobalShaderExtended *shader, std::string fileOff) : ModeleComponent(shader, fileOff)
    {
        this->shader = shader;
    }

    ModeleHeightComponent(GlobalShaderExtended *shader) : ModeleComponent(shader)
    {
        this->shader = shader;
    }

    ModeleHeightComponent(GlobalShaderExtended *shader, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices, std::vector<glm::vec2> texCoords) : ModeleComponent(shader, indexed_vertices, normals, indices, texCoords)
    {
        this->shader = shader;
    }

    virtual void draw(Camera *camera, glm::mat4 transform, RigidBody *rigidbody = NULL)
    {
        bool isInFOV = true;
        if (rigidbody != NULL) {
            isInFOV = camera->isInFrustum(rigidbody->getCollision()->getCollision());
        }
        if (isInFOV) {

            this->shader->use();

            glEnable(GL_TEXTURE_2D);
            this->shader->drawHeightMap(heightMap.texture, (int)textures.size());
            ModeleComponent::draw(camera, transform, rigidbody);
        }
    }

    void setHeightMap(Texture *t, bool destroyEnd)
    {
        heightMap.texture = t;
        heightMap.destroyAtEnd = destroyEnd;
    }
};

#endif