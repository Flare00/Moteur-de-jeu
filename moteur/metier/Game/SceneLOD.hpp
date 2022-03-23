#ifndef __SCENE_SPACE_H__
#define __SCENE_SPACE_H__

#include "../Tools/PrimitiveMesh.hpp"
#include "../Shader/GlobalShader.hpp"
#include "Scene.hpp"

class SceneLOD : public Scene
{
private:
    GlobalShader *globalShader = NULL;

public:
    SceneLOD() {}

    virtual void Init()
    {
        Camera *c = new Camera(vec3(-20, 30, -20), 45, -45);
        this->cameras.push_back(c);
        this->input = new Input(c);

        globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
        this->activeCamera = 0;

        Modele *sphere = new Modele("Sphere", globalShader);
        PrimitiveMesh::generate_uv_sphere(sphere, 32, 32);
        scene->addChild(sphere);
    }

    virtual void Draw(float deltaTime)
    {
        input->processInput(window, deltaTime);

        if (!input->pause)
        {
            if (globalShader != NULL && this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
                globalShader->drawView(this->cameras[this->activeCamera]);
        }

        this->scene->draw(true);
    }
};

#endif
