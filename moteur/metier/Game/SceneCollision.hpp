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
#include <GameObject/Terrain.hpp>

#include <Global.hpp>

class SceneCollision : public Scene
{
private:
	GlobalShader *globalShader = NULL;
	GlobalShaderExtended *shaderTerrain = NULL;

	int precision = 32;
	int low_precision = 8;

	InputCollision *inputCol;
	Text2D *text2D;
	int fps = 0;
	float cooldownFPS = 0.1f;

	bool wait1Frame = true;
public:
	SceneCollision()
	{
	}

	virtual void Init()
	{
		// Texte
		Text2DShader *textShader = new Text2DShader("Shaders/text2d_vertex.glsl", "Shaders/text2d_fragment.glsl", glm::ortho(0.0f, 1.0f * screen_width, 0.0f, 1.0f * screen_height));
		Texture *atlasText = new Texture("Textures/Font/Atlas_Monofonto.jpg");
		text2D = new Text2D(textShader, atlasText, 128, 256);

		//Set Camera
		Camera *c = new Camera(vec3(0, -2, -16), 90, 0);
		this->cameras.push_back(c);
		//Set global Shader
		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;
		//Set Texture
		Texture *texTerre = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");
		Texture *texSoleil = new Texture("Textures/SystemeSolaire/sun.jpg");

		// Terrain
		this->shaderTerrain = new GlobalShaderExtended("Shaders/Terrain/terrain_vertex.glsl", "Shaders/fragment_shader.glsl", "Shaders/Terrain/terrain_tessControl.glsl", "Shaders/Terrain/terrain_tessEval.glsl");
		Texture *textureTerrain = new Texture("Textures/HeightMap/test.png");
		Texture *heightMapTerrain = new Texture("Textures/HeightMap/test.png");
		Terrain *terrain = new Terrain("Terrain", shaderTerrain, textureTerrain, heightMapTerrain);
		terrain->getTransform()->translate(glm::vec3(0, -10, 0));

		//soleil
		ModeleComponent *soleilHigh = new ModeleComponent(globalShader);
		PrimitiveMesh::generate_uv_sphere(soleilHigh, precision, precision);
		soleilHigh->addTexture(texSoleil, true);

		ModeleLOD *SoleilLOD = new ModeleLOD("Soleil", soleilHigh);
		SoleilLOD->getTransform()->translate(glm::vec3(3,5, 1));
		SoleilLOD->setRigidBody(new RigidBody(SoleilLOD, false, 2.0f, glm::vec3(-2.0f, 0, 0)));
		//Terre
		ModeleComponent *TerreHigh = new ModeleComponent(globalShader);
		ModeleComponent *TerreLow = new ModeleComponent(globalShader);

		PrimitiveMesh::generate_uv_sphere(TerreHigh, precision, precision);
		PrimitiveMesh::generate_uv_sphere(TerreLow, low_precision, low_precision);

		TerreHigh->addTexture(texTerre, true);
		TerreLow->addTexture(texTerre, true);

		ModeleLOD *TerreLOD = new ModeleLOD("Terre", TerreHigh, TerreLow);
		TerreLOD->getTransform()->translate(glm::vec3(1, 0, 0));
		TerreLOD->setRigidBody(new RigidBody(TerreLOD, false, 1.0f, glm::vec3(0, -2, 0)));

		//Add to scene
		this->scene->addChild(SoleilLOD);
		this->scene->addChild(TerreLOD);
		this->scene->addChild(terrain);

		//Set inputCollision
		this->inputCol = new InputCollision(c, TerreLOD);
	}

	virtual void Draw(float deltaTime)
	{
		this->cooldownFPS -= deltaTime;
		// Process input
		inputCol->processInput(deltaTime);
		if (!global_pause)
		{
			if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size()) {

				this->cameras[activeCamera]->frustumUpdate();

				if (globalShader != NULL) {}
					globalShader->drawView(this->cameras[this->activeCamera]);
				if (shaderTerrain != NULL)
					shaderTerrain->drawView(this->cameras[this->activeCamera]);
			}

			// Clear the screen
			if (wait1Frame) {
				Physique::computePhysique(this->scene, deltaTime);
			}
			else {
				wait1Frame = false;
			}
		}
		if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size()) {
			this->scene->compute(this->cameras[this->activeCamera], true);
		}
		// compute FPS
		if (this->cooldownFPS <= 0)
		{
			fps = (int)(1.0f / deltaTime);
			this->cooldownFPS = 1.0f;
		}

		this->text2D->DrawText(std::to_string(fps), -1, 1, 0.9f);
	}
};

#endif