#ifndef __WALL_HPP__
#define __WALL_HPP__

#include <vector>
#include <GameObject/ModeleLOD.hpp>
#include <Physique/BulletRigidbody.hpp>
#include <GameObject/ModeleLOD.hpp>

class Wall
{
protected:
    GameObject *go;
    BulletRigidbody *rigid;
    int group, mask;

public:
    Wall(GlobalShader *globalShader, Texture *texture, int width = 2, int height = 2, int depth = 2)
    {
        go = new GameObject("GameObject");
        rigid = BulletRigidbody::generateAABB(glm::vec3(1.0f), 100.0f);

        ModeleComponent *cubeComponent = new ModeleComponent(globalShader);
        cubeComponent->addTexture(texture, false);
        PrimitiveMesh::generate_cube(cubeComponent);
        ModeleLOD *cube = new ModeleLOD("Cube", cubeComponent, NULL, NULL, rigid, go);
        glm::vec3 pos(10, -7.0f, -10.0f);
        cube->getTransform()->setTranslate(pos);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                for (int k = 0; k < depth; k++)
                {
                    if (!(i == 0 && k == 0 && j == 0))
                    {
                        ModeleLOD *cubeTmp = cube->duplicate();
                        cubeTmp->getTransform()->setTranslate(glm::vec3((float)k * 2, (float)j * 2, (float)i * 2) + pos);
                    }
                }
            }
        }
    }

    GameObject *getGameObject()
    {
        return this->go;
    }

    void addToPhysique(PhysiqueBullet *physique, int group = 1, int mask = 1)
    {
        this->group = group;
        this->mask = mask;

        std::vector<GameObject *> listWallGo = go->getChilds();

        for (size_t i = 0, maxSize = listWallGo.size(); i < maxSize; i++)
        {

            physique->addRigidbodyToPhysique(listWallGo[i]->getRigidBody(), group, mask);
        }
    }

    void removeFromPhysique(PhysiqueBullet *physique)
    {
        std::vector<GameObject *> listWallGo = go->getChilds();

        for (size_t i = 0, maxSize = listWallGo.size(); i < maxSize; i++)
        {

            physique->removeRigidbodyFromPhysique(listWallGo[i]->getRigidBody());
        }
    }
};

#endif