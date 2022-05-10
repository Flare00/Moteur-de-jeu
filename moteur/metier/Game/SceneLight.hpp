#ifndef __SCENE_LIGHT_H__
#define __SCENE_LIGHT_H__

#include <string>
#include <glm/glm.hpp>

#include <Tools/PrimitiveMesh.hpp>
#include <Shader/GlobalShader.hpp>
#include <GameObject/ModeleLOD.hpp>
#include <World/InputLight.hpp>

#include <Text2D.hpp>
#include "Scene.hpp"
#include <GameObject/Terrain.hpp>
#include <Tools/LoaderOBJ.hpp>
#include <Physique/GestionContraintes.hpp>

#include <Game/Canon.hpp>
#include <Game/Wall.hpp>

#include <Global.hpp>
#include <Light/Lightning.hpp>
#include <Light/DirectionnalLight.hpp>
#include <Physique/PhysiqueBullet.hpp>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <Callback/ILightCallback.hpp>


class SceneLight : public Scene, ILightCallback
{
private:
	// Init
	GlobalShader* globalShader = NULL;
	GlobalShaderExtended* shaderTerrain = NULL;
	Lightning* lightScene = NULL;
	DebugShader* debugShader = NULL;
	InputLight* inputLight;
	Text2D* text2D;
	PhysiqueBullet* bullet;

	std::vector<ModeleLOD*> listBall;

	Texture* texBall;
	// Cooldowns
	int fps = 0;
	float cooldownFPS = 0.1f;
	float cooldownFireBall = 0.25f;
	float waitTimeFireBall = 0.0f;
	bool wait1Frame = true;

public:
	SceneLight(IGlobalGameCallback* globalGameCallback) : Scene(globalGameCallback)
	{
		this->inputLight = new InputLight(globalGameCallback, this, this);
	}

	~SceneLight()
	{
		delete bullet;
		delete globalShader;
		delete shaderTerrain;
		delete lightScene;
		delete debugShader;
		delete inputLight;
		delete text2D;
		delete texBall;
	}

	virtual void Init()
	{
		//Create LightScene
		lightScene = new Lightning();

		// Texte
		Text2DShader* textShader = new Text2DShader("Shaders/text2d_vertex.glsl", "Shaders/text2d_fragment.glsl", glm::ortho(0.0f, 1.0f * screen_width, 0.0f, 1.0f * screen_height));
		Texture* atlasText = new Texture("Textures/Font/Atlas_Monofonto.jpg");
		text2D = new Text2D(textShader, atlasText, 128, 256);

		// Set Camera
		Camera* c = new Camera(vec3(0, 0, -15), 90, 0);
		this->cameras.push_back(c);

		// Set global Shader
		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		// Set inputCollision

		// INIT Physique
		bullet = new PhysiqueBullet();

		debugShader = new DebugShader("Shaders/Debug/debug_vertex.glsl", "Shaders/Debug/debug_fragment.glsl");
		DebugDrawer* debug = new DebugDrawer(debugShader);
		debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawContactPoints);
		bullet->init(debug);


		// Place et tourne les objets

		this->lightScene->addLight(new DirectionnalLight(glm::vec3(-8, 0, 0), glm::vec3(-1, 0, 0)));
		//this->lightScene->addLight(new DirectionnalLight(glm::vec3(0, 0, -8), glm::vec3(0, 0, -1)));

		texBall = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");
		ModeleComponent* ballComponent = new ModeleComponent(globalShader);
		ballComponent->addTexture(texBall, false);
		PrimitiveMesh::generate_uv_sphere(ballComponent, 16, 16, 1.0f);

		ModeleLOD* ball = new ModeleLOD("Ball", ballComponent, NULL, NULL, NULL, this->scene);
		ball->getTransform()->setTranslate(glm::vec3(0, -1, 0));
		ModeleLOD* tmpB = ball->duplicate();
		tmpB->getTransform()->setTranslate(glm::vec3(0, 1, 0));
		tmpB = ball->duplicate();
		tmpB->getTransform()->setTranslate(glm::vec3(-1, 0, 0));
		tmpB = ball->duplicate();
		tmpB->getTransform()->setTranslate(glm::vec3(1, 0, 0));
		tmpB = ball->duplicate();
		tmpB->getTransform()->setTranslate(glm::vec3(0, 0, -1));
		tmpB = ball->duplicate();
		tmpB->getTransform()->setTranslate(glm::vec3(0, 0, 1));

		ModeleLOD* plane = new ModeleLOD("Plane", globalShader, ModeleComponent::OBJ, "Model/wallplane.obj", NULL, this->scene);
		plane->getTransform()->setTranslate(glm::vec3(0, -10, 0));

		plane = plane->duplicate();
		plane->getOriginalTransform()->setTranslate(glm::vec3(0, 0, 20));
		plane->getOriginalTransform()->setSelfRotation(glm::vec3(M_PI / 2.0f, 0, 0));

		plane = plane->duplicate();
		plane->getOriginalTransform()->setTranslate(glm::vec3(0, 0, -20));
		plane->getOriginalTransform()->setSelfRotation(glm::vec3(M_PI / 2.0f, 0, 0));

		plane = plane->duplicate();
		plane->getOriginalTransform()->setTranslate(glm::vec3(20, 0, 0));
		plane->getOriginalTransform()->setSelfRotation(glm::vec3(M_PI / 2.0f, M_PI / 2.0f, 0));

		plane = plane->duplicate();
		plane->getOriginalTransform()->setTranslate(glm::vec3(-20, 0, 0));
		plane->getOriginalTransform()->setSelfRotation(glm::vec3(M_PI / 2.0f, -M_PI / 2.0f, 0));


		plane = plane->duplicate();
		plane->getOriginalTransform()->setTranslate(glm::vec3(0, 20, 0));
	}

	virtual void Draw(float deltaTime)
	{
		this->waitTimeFireBall -= deltaTime;
		this->cooldownFPS -= deltaTime;
		// Process input
		inputLight->processInput(deltaTime);
		//Draw Shadows
		if (!global_pause)
		{
			if (!wait1Frame)
			{
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

				if (globalShader != NULL)
				{
				}
				globalShader->drawView(this->cameras[this->activeCamera]);

				if (debugShader != NULL)
					debugShader->drawView(this->cameras[this->activeCamera]);
			}
		}

		if (this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
		{
			this->scene->compute(this->cameras[this->activeCamera]->getData(), this->lightScene, true);
		}
		// compute FPS
		if (this->cooldownFPS <= 0)
		{
			fps = (int)(1.0f / deltaTime);
			this->cooldownFPS = 1.0f;
		}

		this->text2D->DrawText(std::to_string(fps), -1, 1, 0.9f);
	}


	// CALLBACKS

	virtual void moveDirection(int id, Direction direction, float deltaTime) {

		if (id == 0) {

			switch (direction) {
			case Direction::DEVANT:
				this->cameras[this->activeCamera]->move(CameraAxe::Z, true, deltaTime);
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

};

#endif