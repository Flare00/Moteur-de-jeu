#ifndef __SCENE_COLLISION_H__
#define __SCENE_COLLISION_H__

#include "../Tools/PrimitiveMesh.hpp"
#include "../Shader/GlobalShader.hpp"
#include "../GameObject/Modele.hpp"
#include "../GameObject/ModeleLOD.hpp"
#include <Collision/Physique.hpp>
#include <World/InputCollision.hpp>
#include <World/InputCollisionV2.hpp>
#include <Collision/RigidBody.hpp>

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

		ModeleComponent* soleilHigh = new ModeleComponent(globalShader);
		PrimitiveMesh::generate_uv_sphere(soleilHigh, precision, precision);
		soleilHigh->setTexture(texSoleil, true);

		ModeleLOD* SoleilLOD = new ModeleLOD("Spleil", soleilHigh);
		SoleilLOD->getTransform()->translate(glm::vec3(-5, 0, 0));
		SoleilLOD->setRigidBody(new RigidBody(SoleilLOD, false, 2.0f, glm::vec3(2.0f,0,0)));

		ModeleComponent* TerreHigh = new ModeleComponent(globalShader);
		ModeleComponent* TerreLow = new ModeleComponent(globalShader);

		PrimitiveMesh::generate_uv_sphere(TerreHigh, precision, precision);
		PrimitiveMesh::generate_uv_sphere(TerreLow, low_precision, low_precision);

		TerreHigh->setTexture(texTerre, true);
		TerreLow->setTexture(texTerre, true);

		ModeleLOD* TerreLOD = new ModeleLOD("Terre", TerreHigh, TerreLow);
		TerreLOD->setRigidBody(new RigidBody(TerreLOD, false, 1.0f, glm::vec3(-2.0f,0,0)));
		TerreLOD->getTransform()->translate(glm::vec3(5, 0, 0));
		this->scene->addChild(SoleilLOD);
		this->scene->addChild(TerreLOD);

		this->inputCol = new InputCollision(c, TerreLOD);
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