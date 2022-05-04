#ifndef __SCENE_COLLISION_H__
#define __SCENE_COLLISION_H__

#include <string>
#include <glm/glm.hpp>

#include <Tools/PrimitiveMesh.hpp>
#include <Shader/GlobalShader.hpp>
#include <GameObject/ModeleLOD.hpp>
#include <Collision/Physique.hpp>
#include <World/InputCollision.hpp>
#include <World/InputCollisionV2.hpp>
#include <Collision/RigidBody.hpp>
#include <Text2D.hpp>
#include "Scene.hpp"

#include <Global.hpp>

class SceneCollision : public Scene {
private:
	GlobalShader* globalShader = NULL;

	int precision = 32;
	int low_precision = 8;

	InputCollision* inputCol;
	Text2D* text2D;
	int fps = 0;
	float cooldownFPS = 0.1f;
public :
	SceneCollision() {
	}

	virtual void Init() {
		//Texte
		Text2DShader * textShader = new Text2DShader("Shaders/text2d_vertex.glsl", "Shaders/text2d_fragment.glsl", glm::ortho(0.0f, 1.0f * screen_width, 0.0f, 1.0f * screen_height));

		Texture * atlasText = new Texture("Textures/Font/Atlas_Monofonto.jpg");
		text2D = new Text2D(textShader, atlasText, 128,256);

		Camera* c = new Camera(vec3(0, -2, -16), 90, 0);
		this->cameras.push_back(c);

		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		Texture* texTerre = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");
		Texture* texSoleil = new Texture("Textures/SystemeSolaire/sun.jpg");
		// Terrain

		ModeleComponent* soleilHigh = new ModeleComponent(globalShader);
		PrimitiveMesh::generate_uv_sphere(soleilHigh, precision, precision);
		soleilHigh->addTexture(texSoleil, true);

		ModeleLOD* SoleilLOD = new ModeleLOD("Spleil", soleilHigh);
		SoleilLOD->getTransform()->translate(glm::vec3(-5, 0, 0));
		SoleilLOD->setRigidBody(new RigidBody(SoleilLOD, false, 2.0f, glm::vec3(2.0f,0,0)));

		ModeleComponent* TerreHigh = new ModeleComponent(globalShader);
		ModeleComponent* TerreLow = new ModeleComponent(globalShader);

		PrimitiveMesh::generate_uv_sphere(TerreHigh, precision, precision);
		PrimitiveMesh::generate_uv_sphere(TerreLow, low_precision, low_precision);

		TerreHigh->addTexture(texTerre, true);
		TerreLow->addTexture(texTerre, true);

		ModeleLOD* TerreLOD = new ModeleLOD("Terre", TerreHigh, TerreLow);
		TerreLOD->setRigidBody(new RigidBody(TerreLOD, false, 1.0f, glm::vec3(-2.0f,0,0)));
		TerreLOD->getTransform()->translate(glm::vec3(5, 0, 0));
		this->scene->addChild(SoleilLOD);
		this->scene->addChild(TerreLOD);

		this->inputCol = new InputCollision(c, TerreLOD);
	}

	virtual void Draw(float deltaTime) {
		this->cooldownFPS -= deltaTime;
		//Process input
		inputCol->processInput(deltaTime);
		if (!global_pause) {
			if (globalShader != NULL && this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
				globalShader->drawView(this->cameras[this->activeCamera]);
			// Clear the screen
			Physique::computePhysique(this->scene, deltaTime);
		}
		this->scene->compute(this->cameras[this->activeCamera], true);
		//compute FPS
		if (this->cooldownFPS <= 0) {
			fps = 1.0f / deltaTime;
			this->cooldownFPS = 1.0f;
		}

		this->text2D->DrawText(std::to_string(fps), -1, 1, 0.9f);
	}
};

#endif