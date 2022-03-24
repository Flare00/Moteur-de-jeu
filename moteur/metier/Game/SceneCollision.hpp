#ifndef __SCENE_COLLISION_H__
#define __SCENE_COLLISION_H__

#include "../Tools/PrimitiveMesh.hpp"
#include "../Shader/GlobalShader.hpp"
#include "../GameObject/Modele.hpp"
#include "../GameObject/ModeleLOD.hpp"
#include <Collision/Gravity.hpp>
#include <Collision/Physique.hpp>
#include <World/InputCollision.hpp>
#include <World/InputCollisionV2.hpp>

#include "Scene.hpp"

class SceneCollision : public Scene {
private:
	GlobalShader* globalShader = NULL;

	int precision = 32;
	int low_precision = 8;

	InputCollision* inputCol;

public :
	SceneCollision() {
	}

	virtual void Init() {
		Camera* c = new Camera(vec3(0, -2, -16), 90, 0);
		this->cameras.push_back(c);

		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		Texture* texTerre = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");
		Texture* texSoleil = new Texture("Textures/SystemeSolaire/sun.jpg");
		// Terrain
		Modele* terrainHigh = new Modele("", globalShader);
		PrimitiveMesh::generate_plane(terrainHigh, precision, precision, 10.0f, 10.0f);
		terrainHigh->setTexture(texTerre, true);

		ModeleLOD* terrainLOD = new ModeleLOD("Terrain", terrainHigh);
		terrainLOD->getTransform()->translate(glm::vec3(0, -5, 0));

		Modele* objetHigh = new Modele("", globalShader);
		Modele* objetLow = new Modele("", globalShader);

		PrimitiveMesh::generate_uv_sphere(objetHigh, precision, precision);
		PrimitiveMesh::generate_uv_sphere(objetLow, low_precision, low_precision);
		objetHigh->setTexture(texSoleil, true);
		objetLow->setTexture(texSoleil, true);

		ModeleLOD* objetLOD = new ModeleLOD("Objet", objetHigh, objetLow);
		objetLOD->addComponent(new Gravity(objetLOD));

		this->scene->addChild(terrainLOD);
		this->scene->addChild(objetLOD);

		this->inputCol = new InputCollision(c, objetLOD);
	}

	virtual void Draw(float deltaTime) {

		inputCol->processInput(deltaTime);
		if (!global_pause) {
			if (globalShader != NULL && this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
				globalShader->drawView(this->cameras[this->activeCamera]);
			// Clear the screen
			Physique::computePhysique(this->scene, deltaTime);
		}
		this->scene->compute(this->cameras[this->activeCamera], true);
	}
};

#endif