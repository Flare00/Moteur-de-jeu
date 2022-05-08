#ifndef __SCENE_PROJECTILE_H__
#define __SCENE_PROJECTILE_H__

#include <string>
#include <glm/glm.hpp>

#include <Tools/PrimitiveMesh.hpp>
#include <Shader/GlobalShader.hpp>
#include <GameObject/ModeleLOD.hpp>
#include <World/InputProjectile.hpp>
#include <Text2D.hpp>
#include "Scene.hpp"
#include <GameObject/Terrain.hpp>
#include <Tools/LoaderOBJ.hpp>

#include <Game/Canon.hpp>

#include <Global.hpp>

#include <Physique/PhysiqueBullet.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class SceneProjecile : public Scene, IProjectileCallback
{
private:
	// Init
	GlobalShader *globalShader = NULL;
	GlobalShaderExtended *shaderTerrain = NULL;
	DebugShader *debugShader = NULL;
	InputProjectile *inputCol;
	Text2D *text2D;
	PhysiqueBullet *bullet;
	Canon *canon;

	std::vector<ModeleLOD *> listBall;

	// Cooldowns
	int fps = 0;
	float cooldownFPS = 0.1f;
	float cooldownFireBall = 0.25f;
	float waitTimeFireBall = 0.0f;
	bool wait1Frame = true;

public:
	SceneProjecile()
	{
	}

	~SceneProjecile()
	{
		delete bullet;
	}

	virtual void Init()
	{
		// Texte
		Text2DShader *textShader = new Text2DShader("Shaders/text2d_vertex.glsl", "Shaders/text2d_fragment.glsl", glm::ortho(0.0f, 1.0f * screen_width, 0.0f, 1.0f * screen_height));
		Texture *atlasText = new Texture("Textures/Font/Atlas_Monofonto.jpg");
		text2D = new Text2D(textShader, atlasText, 128, 256);

		// Set Camera
		Camera *c = new Camera(vec3(0, -2, -16), 90, 0);
		this->cameras.push_back(c);

		// Set global Shader
		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		// Set Texture
		Texture *texTest = new Texture("Textures/texture_test.jpg");

		// Construct canon
		canon = new Canon(globalShader);

		// Terrain
		this->shaderTerrain = new GlobalShaderExtended("Shaders/Terrain/terrain_vertex.glsl", "Shaders/fragment_shader.glsl", "Shaders/Terrain/terrain_tessControl.glsl", "Shaders/Terrain/terrain_tessEval.glsl");
		Texture *textureTerrain = new Texture("Textures/HeightMap/test.png");
		Texture *heightMapTerrain = new Texture("Textures/HeightMap/test.png");
		Terrain *terrain = new Terrain("Terrain", shaderTerrain, textureTerrain, heightMapTerrain, 25, 1.0f, 1);

		// Add to scene
		this->scene->addChild(terrain);
		this->scene->addChild(canon->getGameObject());

		// Set inputCollision
		this->inputCol = new InputProjectile(c, this);

		// INIT Physique
		bullet = new PhysiqueBullet();
		bullet->init();
		bullet->addRigidbodyToPhysique(terrain->getRigidbody());

		// Place et tourne les objets
		terrain->getTransform()->translate(glm::vec3(0, -10, 0));
	}

	ModeleLOD *createBall()
	{
		ModeleComponent *ballComponent = new ModeleComponent(globalShader);
		PrimitiveMesh::generate_uv_sphere(ballComponent, 16, 16);
		BulletRigidbody *ballRigidBody = new BulletRigidbody();
		ballRigidBody->setToSphere(1.0f, 1.0f);
		ModeleLOD *ball = new ModeleLOD("Ball", ballComponent, NULL, NULL, ballRigidBody);
		return ball;
	}

	virtual void actionFireBall()
	{
		if (this->waitTimeFireBall <= 0)
		{
			this->waitTimeFireBall = this->cooldownFireBall;
			if (this->listBall.size() > 20)
			{
				ModeleLOD *b = this->listBall[0];
				this->scene->removeChild(b);
				this->bullet->removeRigidbodyFromPhysique(b->getRigidBody());
				this->listBall.erase(this->listBall.begin());
				delete b;
			}
			if (activeCamera == 0)
			{
				ModeleLOD *b = this->createBall();
				this->listBall.push_back(b);
				this->scene->addChild(b);
				this->bullet->addRigidbodyToPhysique(b->getRigidBody());
				b->getTransform()->setTranslate(this->cameras[0]->getPosition());
				b->getBulletTransform()->applyImpule(this->cameras[0]->getFront() * 50.0f);
			}
		}
	}

	virtual void Draw(float deltaTime)
	{
		this->waitTimeFireBall -= deltaTime;
		this->cooldownFPS -= deltaTime;
		// Process input
		inputCol->processInput(deltaTime);
		if (!global_pause)
		{
			if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
			{

				this->cameras[activeCamera]->frustumUpdate();

				if (globalShader != NULL)
				{
				}
				globalShader->drawView(this->cameras[this->activeCamera]);
				if (shaderTerrain != NULL)
					shaderTerrain->drawView(this->cameras[this->activeCamera]);
			}
			if (!wait1Frame)
			{
				bullet->loop(deltaTime);
			}
			else
			{
				wait1Frame = false;
			}
		}

		if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
		{
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