#ifndef __SCENE_SPACE_H__
#define __SCENE_SPACE_H__

#include "../Tools/PrimitiveMesh.hpp"
#include "../Shader/GlobalShader.hpp"
#include "Scene.hpp"

class SceneSpace : public Scene {
private:
	GlobalShader* globalShader = NULL;
	float day = 100.0f;
	int meshPrecision = 16;
public:
	SceneSpace() {
	}

	virtual void Init() {
		Camera* c = new Camera(vec3(-20, 30, -20), 45, -45);
		this->cameras.push_back(c);
		//this->input = new Input(c);

		globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
		this->activeCamera = 0;

		Texture* back = new Texture("Textures/SystemeSolaire/stars_milky_way.jpg");
		Texture* texSoleil = new Texture("Textures/SystemeSolaire/sun.jpg");
		Texture* texTerre = new Texture("Textures/SystemeSolaire/earth_daymap.jpg");
		Texture* texLune = new Texture("Textures/SystemeSolaire/moon.jpg");
		Texture* texMercure = new Texture("Textures/SystemeSolaire/mercury.jpg");
		Texture* texVenus = new Texture("Textures/SystemeSolaire/venus.jpg");
		Texture* texMars = new Texture("Textures/SystemeSolaire/mars.jpg");
		Texture* texJupiter = new Texture("Textures/SystemeSolaire/jupiter.jpg");
		Texture* texSaturne = new Texture("Textures/SystemeSolaire/saturn.jpg");
		Texture* texUranus = new Texture("Textures/SystemeSolaire/uranus.jpg");
		Texture* texNeptune = new Texture("Textures/SystemeSolaire/neptune.jpg");

		//Creation du fond
		Modele* background = new Modele("Background", globalShader);
		PrimitiveMesh::generate_uv_sphere(background, 4, 4);
		background->setTexture(back, true);
		background->getTransform()->setScale(5000.0f);
		this->scene->addChild(background);

		//Creation du Soleil
		Modele* soleil = new Modele("Soleil", globalShader);
		PrimitiveMesh::generate_uv_sphere(soleil, meshPrecision, meshPrecision);
		soleil->setTexture(texSoleil, true);

		//Creation de la Terre
		Modele* terre = new Modele("Terre", globalShader);
		PrimitiveMesh::generate_uv_sphere(terre, meshPrecision, meshPrecision);
		terre->setTexture(texTerre, true);
		//Creation de la Lune
		Modele* lune = new Modele("Lune", globalShader);
		PrimitiveMesh::generate_uv_sphere(lune, meshPrecision, meshPrecision);
		lune->setTexture(texLune, true);

		//Creation de mercure
		Modele* mercure = new Modele("Mercure", globalShader);
		PrimitiveMesh::generate_uv_sphere(mercure, meshPrecision, meshPrecision);
		mercure->setTexture(texMercure, true);

		//Creation de Venus
		Modele* venus = new Modele("Venus", globalShader);
		PrimitiveMesh::generate_uv_sphere(venus, meshPrecision, meshPrecision);
		venus->setTexture(texVenus, true);

		//Creation de Mars
		Modele* mars = new Modele("Mars", globalShader);
		PrimitiveMesh::generate_uv_sphere(mars, meshPrecision, meshPrecision);
		mars->setTexture(texMars, true);

		//Creation de Jupiter
		Modele* jupiter = new Modele("Jupiter", globalShader);
		PrimitiveMesh::generate_uv_sphere(jupiter, meshPrecision, meshPrecision);
		jupiter->setTexture(texJupiter, true);

		//Creation de Saturne
		Modele* saturne = new Modele("Saturne", globalShader);
		PrimitiveMesh::generate_uv_sphere(saturne, meshPrecision, meshPrecision);
		saturne->setTexture(texSaturne, true);

		//Creation de Uranus
		Modele* uranus = new Modele("Uranus", globalShader);
		PrimitiveMesh::generate_uv_sphere(uranus, meshPrecision, meshPrecision);
		uranus->setTexture(texUranus, true);

		//Creation de Neptune
		Modele* neptune = new Modele("Neptune", globalShader);
		PrimitiveMesh::generate_uv_sphere(neptune, meshPrecision, meshPrecision);
		neptune->setTexture(texNeptune, true);

		scene->addChild(soleil);
		soleil->addChild(terre);
		terre->addChild(lune);

		soleil->addChild(mercure);
		soleil->addChild(venus);
		soleil->addChild(mars);
		soleil->addChild(jupiter);
		soleil->addChild(saturne);
		soleil->addChild(uranus);
		soleil->addChild(neptune);

		soleil->getTransform()->setSelfScale(3);
		terre->getTransform()->setSelfScale(1);
		lune->getTransform()->setSelfScale(0.37f);

		mercure->getTransform()->setSelfScale(0.5f);
		venus->getTransform()->setSelfScale(1);
		mars->getTransform()->setSelfScale(0.5f);
		jupiter->getTransform()->setSelfScale(2);
		saturne->getTransform()->setSelfScale(1.8f);
		uranus->getTransform()->setSelfScale(1.3f);
		neptune->getTransform()->setSelfScale(1.3f);



		terre->getTransform()->translate(vec3(0, 0, 15));
		lune->getTransform()->translate(vec3(0, 0, 2.5f));

		mercure->getTransform()->translate(vec3(0, 0, 5));
		venus->getTransform()->translate(vec3(0, 0, 10));
		mars->getTransform()->translate(vec3(0, 0, 20));
		jupiter->getTransform()->translate(vec3(0, 0, 25));
		saturne->getTransform()->translate(vec3(0, 0, 30));
		uranus->getTransform()->translate(vec3(0, 0, 35));
		neptune->getTransform()->translate(vec3(0, 0, 40));



		terre->getTransform()->rotate(vec3(1, 0, 0), radians(5.14f));
		terre->getTransform()->selfRotate(vec3(1, 0, 0), radians(23.44f) - radians(5.14f));
		lune->getTransform()->selfRotate(vec3(1, 0, 0), radians(6.68f));

		mercure->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));
		venus->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));
		mars->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));
		jupiter->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));
		saturne->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));
		uranus->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));
		neptune->getTransform()->selfRotate(vec3(1, 0, 0), radians(5.14f));

	}

	virtual void Draw(float deltaTime) {
		/*
		input->processInput(window, deltaTime);

		if (!input->pause) {
			if (globalShader != NULL && this->activeCamera >= 0 && this->activeCamera < this->cameras.size())
				globalShader->drawView(this->cameras[this->activeCamera]);
			// Clear the screen

			GameObject* soleil = this->scene->findDirectChild("Soleil");

			//Rotation par rapport au parent
			soleil->findDirectChild("Terre")->getTransform()->rotate(vec3(0, 1, 0), day / 365.0f * deltaTime);
			soleil->findDirectChild("Terre")->findDirectChild("Lune")->getTransform()->rotate(vec3(0, 1, 0), day / 27.0f * deltaTime);

			soleil->findDirectChild("Mercure")->getTransform()->rotate(vec3(0, 1, 0), day / 88.0f * deltaTime);
			soleil->findDirectChild("Venus")->getTransform()->rotate(vec3(0, 1, 0), day / 225.0f * deltaTime);
			soleil->findDirectChild("Mars")->getTransform()->rotate(vec3(0, 1, 0), day / 687.0f * deltaTime);
			soleil->findDirectChild("Jupiter")->getTransform()->rotate(vec3(0, 1, 0), day / 4329.0f * deltaTime);
			soleil->findDirectChild("Saturne")->getTransform()->rotate(vec3(0, 1, 0), day / 10751.0f * deltaTime);
			soleil->findDirectChild("Uranus")->getTransform()->rotate(vec3(0, 1, 0), day / 30664.0f * deltaTime);
			soleil->findDirectChild("Neptune")->getTransform()->rotate(vec3(0, 1, 0), day / 60148.0f * deltaTime);

			//annulation de la self rotation du a la rotation au parent
			soleil->findDirectChild("Terre")->getTransform()->selfRotate(vec3(0, 1, 0), day / 365.0f * -deltaTime);
			soleil->findDirectChild("Terre")->findDirectChild("Lune")->getTransform()->selfRotate(vec3(0, 1, 0), ((day / 27.0f) + (day / 365.0f)) * -deltaTime);

			soleil->findDirectChild("Mercure")->getTransform()->selfRotate(vec3(0, 1, 0), day / 88.0f * -deltaTime);
			soleil->findDirectChild("Venus")->getTransform()->selfRotate(vec3(0, 1, 0), day / 225.0f * -deltaTime);
			soleil->findDirectChild("Mars")->getTransform()->selfRotate(vec3(0, 1, 0), day / 687.0f * -deltaTime);
			soleil->findDirectChild("Jupiter")->getTransform()->selfRotate(vec3(0, 1, 0), day / 4329.0f * -deltaTime);
			soleil->findDirectChild("Saturne")->getTransform()->selfRotate(vec3(0, 1, 0), day / 10751.0f * -deltaTime);
			soleil->findDirectChild("Uranus")->getTransform()->selfRotate(vec3(0, 1, 0), day / 30664.0f * -deltaTime);
			soleil->findDirectChild("Neptune")->getTransform()->selfRotate(vec3(0, 1, 0), day / 60148.0f * -deltaTime);

			//Self Rotation
			soleil->getTransform()->selfRotate(vec3(0, 1, 0), day / 26.0f * deltaTime);
			soleil->findDirectChild("Terre")->getTransform()->selfRotate(vec3(0, 1, 0), day * deltaTime);
			soleil->findDirectChild("Terre")->findDirectChild("Lune")->getTransform()->selfRotate(vec3(0, 1, 0), day / 26.0f * deltaTime);

			soleil->findDirectChild("Mercure")->getTransform()->selfRotate(vec3(0, 1, 0), day / 59.0f * deltaTime);
			soleil->findDirectChild("Venus")->getTransform()->selfRotate(vec3(0, 1, 0), day / 243.0f * deltaTime);
			soleil->findDirectChild("Mars")->getTransform()->selfRotate(vec3(0, 1, 0), day * deltaTime);
			soleil->findDirectChild("Jupiter")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.42f * deltaTime);
			soleil->findDirectChild("Saturne")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.42f * deltaTime);
			soleil->findDirectChild("Uranus")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.71f * deltaTime);
			soleil->findDirectChild("Neptune")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.75f * deltaTime);
			
		}

		this->scene->compute(this->cameras[this->activeCamera], true);
		*/
	}
};

#endif