#ifndef __SCENE_COLLISION_H__
#define __SCENE_COLLISION_H__

#include "../Tools/PrimitiveMesh.hpp"
#include "../Shader/GlobalShader.hpp"
#include "Scene.hpp"

class SceneCollision : public Scene {
private:
	GlobalShader* globalShader = NULL;
	int precision = 32;
public :
	SceneCollision() {
	}

	virtual void Init() {
		Camera* c = new Camera(vec3(-20, 30, -20), 45, -45);
		this->cameras.push_back(c);
		this->input = new Input(c);
		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		Texture* texTerre = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");
		Texture* texSoleil = new Texture("Textures/SystemeSolaire/sun.jpg");

		Modele* terrain = new Modele("Terrain", globalShader);
		PrimitiveMesh::generate_plane(terrain, precision, precision, 10.0f, 10.0f);
		terrain->setTexture(texTerre, true);

		Modele* objet = new Modele("Objet", globalShader);
		PrimitiveMesh::generate_uv_sphere(objet, precision, precision);
		objet->setTexture(texSoleil, true);
	}

	virtual void Draw(float deltaTime) {

		input->processInput(window, deltaTime);

		if (!input->pause) {
			if (globalShader != NULL && this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
				globalShader->drawView(this->cameras[this->activeCamera]);
			// Clear the screen

			
		}

		this->scene->compute(this->cameras[this->activeCamera], true);
	}
};

#endif