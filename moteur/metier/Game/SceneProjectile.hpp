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
#include <Physique/GestionContraintes.hpp>

#include <Game/Canon.hpp>
#include <Game/Wall.hpp>

#include <Global.hpp>
#include <Light/Lightning.hpp>
#include <Light/PointLight.hpp>

#include <Light/DirectionnalLight.hpp>
#include <Physique/PhysiqueBullet.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


class SceneProjectile : public Scene, IProjectileCallback
{
private:
	// Init
	GlobalShader* globalShader = NULL;
	GlobalShaderExtended* shaderTerrain = NULL;
	Lightning* lightScene = NULL;
	DebugShader* debugShader = NULL;
	InputProjectile* inputProj;
	Text2D* text2D;
	PhysiqueBullet* bullet;
	Canon* canon;
	Wall* wall;
	Texture* texBall;

	std::vector<ModeleLOD*> listBall;

	// Cooldowns
	int fps = 0;
	float cooldownFPS = 0.1f;
	float cooldownFireBall = 0.25f;
	float waitTimeFireBall = 0.0f;
	bool wait1Frame = true;

public:
	SceneProjectile(IGlobalGameCallback* globalGameCallback) : Scene(globalGameCallback)
	{
		this->inputProj = new InputProjectile(globalGameCallback, this, this);
	}

	~SceneProjectile()
	{

	}

	virtual void UnLoad() {
		Scene::UnLoad();
		delete bullet;
		delete globalShader;
		delete shaderTerrain;
		delete lightScene;
		delete debugShader;
		delete text2D;
		delete texBall;
		delete canon;
		delete wall;
	}
	 
	virtual void Load()
	{
		Scene::Load();
		// Set Camera
		Camera* c = new Camera(vec3(0, 0, -15), 90, 0);
		this->cameras.push_back(c);

		//Create LightScene
		lightScene = new Lightning();
		this->lightScene->addLight(new PointLight(glm::vec3(-100, 100, -50), glm::vec3(1, 1, 1), 50.0f));
		//this->lightScene->addLight(new DirectionnalLight(glm::vec3(0, 0, -8), glm::vec3(0, 0, -1)));
		
		// Texte
		Text2DShader* textShader = new Text2DShader("Shaders/text2d_vertex.glsl", "Shaders/text2d_fragment.glsl", glm::ortho(0.0f, 1.0f * screen_width, 0.0f, 1.0f * screen_height));
		Texture* atlasText = new Texture("Textures/Font/Atlas_Monofonto.jpg");
		text2D = new Text2D(textShader, atlasText, 128, 256);



		// Set global Shader
		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		// Set Texture
		Texture* texTest = new Texture("Textures/texture_test.jpg");
		texBall = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");

		// Construct canon
		canon = new Canon(globalShader);

		// Constrcut wall
		wall = new Wall(globalShader, texTest, 10, 10, 1);

		// Terrain
		this->shaderTerrain = new GlobalShaderExtended("Shaders/Terrain/terrain_vertex.glsl", "Shaders/fragment_shader.glsl", "Shaders/Terrain/terrain_tessControl.glsl", "Shaders/Terrain/terrain_tessEval.glsl");
		Texture* textureTerrain = new Texture("Textures/HeightMap/test.png");
		Texture* heightMapTerrain = new Texture("Textures/HeightMap/test.png");
		Terrain* terrain = new Terrain("Terrain", shaderTerrain, textureTerrain, heightMapTerrain, 20, 1.0f, 1);

		// Plateforme;
		BulletRigidbody* plateformeRigid = BulletRigidbody::generateAABB(glm::vec3(30,1,30), 0.0f);
		ModeleLOD* plateforme = new ModeleLOD("Plane", globalShader, ModeleComponent::OBJ, "Model/plateforme.obj", plateformeRigid, this->scene);
		plateforme->getTransform()->setTranslate(glm::vec3(0, -9.0f, 0));

		// Add to scene
		this->scene->addChild(terrain);
		this->scene->addChild(canon->getGameObject());
		this->scene->addChild(wall->getGameObject());

		// INIT Physique
		bullet = new PhysiqueBullet();

		debugShader = new DebugShader("Shaders/Debug/debug_vertex.glsl", "Shaders/Debug/debug_fragment.glsl");
		DebugDrawer* debug = new DebugDrawer(debugShader);
		debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);

		bullet->init(debug);
		bullet->addRigidbodyToPhysique(terrain->getRigidbody(),5,0);
		bullet->addRigidbodyToPhysique(plateformeRigid, 5, 0);
		canon->addToPhysique(bullet);
		wall->addToPhysique(bullet);

		// Place et tourne les objets
		terrain->getTransform()->translate(glm::vec3(0, -15, 0));
		


	}

	ModeleLOD* createBall()
	{
		ModeleComponent* ballComponent = new ModeleComponent(globalShader);
		ballComponent->addTexture(texBall, false);
		PrimitiveMesh::generate_uv_sphere(ballComponent, 16, 16, 0.2f);
		BulletRigidbody* ballRigidBody = new BulletRigidbody();
		ballRigidBody->setToSphere(0.2f, 50.0f);
		ModeleLOD* ball = new ModeleLOD("Ball", ballComponent, NULL, NULL, ballRigidBody);
		return ball;
	}

	virtual void actionFireBall()
	{
		if (this->waitTimeFireBall <= 0)
		{
			this->waitTimeFireBall = this->cooldownFireBall;
			if (this->listBall.size() > 20)
			{
				ModeleLOD* b = this->listBall[0];
				this->scene->removeChild(b);
				this->bullet->removeRigidbodyFromPhysique(b->getRigidBody());
				this->listBall.erase(this->listBall.begin());
				delete b;
			}
			if (activeCamera == 0)
			{
				ModeleLOD* b = this->createBall();
				this->listBall.push_back(b);
				this->scene->addChild(b);
				this->bullet->addRigidbodyToPhysique(b->getRigidBody(), canon->getGroup(), 1);

				//b->getTransform()->setTranslate(this->canon->getCanonPos());
				//b->getBulletTransform()->applyImpule(this->canon->getFront() * 2000.0f);
				b->getTransform()->setTranslate(this->cameras[0]->getPosition());
				b->getBulletTransform()->applyImpule(this->cameras[0]->getFront() * 2000.0f);
			}
		}
	}

	virtual void actionGoUp()
	{
		canon->getTransform()->applyForce(glm::vec3(1.0f, 0.0f, 0.0f));
	}

	virtual void moveDirection(int id, Direction direction, float deltaTime) {
		
		if (id == 0) {
			switch (direction) {
			case Direction::DEVANT:
				this->cameras[this->activeCamera]->move(CameraAxe::Z, true, deltaTime * 10.0f);
				break;
			case Direction::DERRIERE:
				this->cameras[this->activeCamera]->move(CameraAxe::Z, false, deltaTime);
				break;
			case Direction::GAUCHE:
				this->cameras[this->activeCamera]->move(CameraAxe::X, false, deltaTime);
				break;
			case Direction::DROITE:
				this->cameras[this->activeCamera]->move(CameraAxe::X, true, deltaTime);
				break;
			case Direction::HAUT:
				this->cameras[this->activeCamera]->move(CameraAxe::Y, true, deltaTime);
				break;
			case Direction::BAS:
				this->cameras[this->activeCamera]->move(CameraAxe::Y, false, deltaTime);
				break;
			}
		}
		else if (id == 1) {
			switch (direction) {
			case Direction::DEVANT:
				canon->move(0);
				break;
			case Direction::DERRIERE:
				canon->move(1);
				break;
			case Direction::GAUCHE:
				canon->move(2);
				break;
			case Direction::DROITE:
				canon->move(3);
				break;
			}
		}
	}
	virtual void toggleTorchMode() {

	}
	virtual void togglePhysicDebug() {
		this->bullet->toogleDebug();
	}
	virtual void mouseMovement(float x, float y) {
		this->cameras[this->activeCamera]->rotate(CameraAxe::X, x);
		this->cameras[this->activeCamera]->rotate(CameraAxe::Y, y);
	}



	virtual void Draw(float deltaTime)
	{
		if (isActive()) {
			this->inputProj->processInput(deltaTime);
			Scene::Draw(deltaTime);
		}

		// Process input
		if (isActive()) {

			this->waitTimeFireBall -= deltaTime;
			this->cooldownFPS -= deltaTime;
			//Draw Shadows
			if (!global_pause)
			{
				if (!wait1Frame)
				{

					if (debugShader != NULL)
						debugShader->drawView(this->cameras[this->activeCamera]);
					bullet->loop(deltaTime);
				}
				else
				{
					wait1Frame = false;
				}
			}

			this->lightScene->compute(this->scene);

			if (!global_pause)
			{
				if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
				{
					this->cameras[activeCamera]->checkUpdate();
				}
			}

			if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
			{



				globalShader->drawView(this->cameras[this->activeCamera]);

				this->scene->compute(this->cameras[this->activeCamera]->getData(), this->lightScene, true);
				if (shaderTerrain != NULL)
					shaderTerrain->drawView(this->cameras[this->activeCamera]);
			}
			// compute FPS
			if (this->cooldownFPS <= 0)
			{
				fps = (int)(1.0f / deltaTime);
				this->cooldownFPS = 1.0f;
			}

			this->text2D->DrawText(std::to_string(fps), -1, 1, 0.9f);
		}
	}
};

#endif