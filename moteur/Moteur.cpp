// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <time.h>       /* time */
#include <fstream>
#include <string>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include <moteur/metier/World/Input.hpp>
#include <moteur/metier/GameObject/Modele.hpp>
#include <moteur/metier/GameObject/Terrain.hpp>
#include <moteur/metier/Transformation.hpp>
#include <moteur/metier/Texture.hpp>
#include <moteur/metier/World/Camera.hpp>
#include <moteur/metier/Compresseur.hpp>


//Shaders Programs
GlobalShader * globalShader = NULL;
TerrainShader * terrainShader = NULL;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(vec3(-20, 30, -20), 45, -45);

//Inputs
Input input(&camera);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float rotateSpeed = 0.5f;
bool textureChangeNotReleased = false;
bool rotateTerrain = true;

//Datas
GameObject* scene;
int meshPrecision = 16;
float meshSize = 5.0f;
bool pause = false;


Modele* soleil;

void initScene() {
	globalShader = new GlobalShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	terrainShader = new TerrainShader("Shaders/terrain_vertex_shader.glsl", "Shaders/terrain_fragment_shader.glsl");

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

	//Creation du noeud principal "Scene"
	scene = new GameObject("Scene");

	Modele* background = new Modele("Background", globalShader);
	generate_uv_sphere(background, 4,4);
	background->setTexture(back, true);
	background->getTransform()->setScale(5000.0f);
	scene->addChild(background);

	//Cr�ation du Soleil
	soleil = new Modele("Soleil", globalShader);
	generate_uv_sphere(soleil, meshPrecision, meshPrecision);
	soleil->setTexture(texSoleil, true);

	//Cr�ation de la Terre
	Modele* terre = new Modele("Terre", globalShader);
	generate_uv_sphere(terre, meshPrecision, meshPrecision);
	terre->setTexture(texTerre, true);
	//Création de la Lune
	Modele* lune = new Modele("Lune", globalShader);
	generate_uv_sphere(lune, meshPrecision, meshPrecision);
	lune->setTexture(texLune, true);

	//Création de mercure
	Modele* mercure = new Modele("Mercure", globalShader);
	generate_uv_sphere(mercure, meshPrecision, meshPrecision);
	mercure->setTexture(texMercure, true);

	//Création de Venus
	Modele* venus = new Modele("Venus", globalShader);
	generate_uv_sphere(venus, meshPrecision, meshPrecision);
	venus->setTexture(texVenus, true);

	//Création de Mars
	Modele* mars = new Modele("Mars", globalShader);
	generate_uv_sphere(mars, meshPrecision, meshPrecision);
	mars->setTexture(texMars, true);

	//Création de Jupiter
	Modele* jupiter = new Modele("Jupiter", globalShader);
	generate_uv_sphere(jupiter, meshPrecision, meshPrecision);
	jupiter->setTexture(texJupiter, true);

	//Création de Saturne
	Modele* saturne = new Modele("Saturne", globalShader);
	generate_uv_sphere(saturne, meshPrecision, meshPrecision);
	saturne->setTexture(texSaturne, true);

	//Création de Uranus
	Modele* uranus = new Modele("Uranus", globalShader);
	generate_uv_sphere(uranus, meshPrecision, meshPrecision);
	uranus->setTexture(texUranus, true);

	//Création de Neptune
	Modele* neptune = new Modele("Neptune", globalShader);
	generate_uv_sphere(neptune, meshPrecision, meshPrecision);
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

	mercure->getTransform()->translate(vec3(0,0, 5));
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

float day = 100.0f;

void mainLoop() {

	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		// Measure speed
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		input.processInput(window, deltaTime);
		if (!input.pause) {
			if (globalShader != NULL) 
				globalShader->drawView(&camera);
			if(terrainShader != NULL)
				terrainShader->drawView(&camera);
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			//Rotation par rapport au parent
			soleil->findDirectChild("Terre")->getTransform()->rotate(vec3(0, 1, 0), day/365.0f * deltaTime);
			soleil->findDirectChild("Terre")->findDirectChild("Lune")->getTransform()->rotate(vec3(0, 1, 0), day/27.0f * deltaTime);

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
			soleil->getTransform()->selfRotate(vec3(0, 1, 0), day/26.0f * deltaTime);
			soleil->findDirectChild("Terre")->getTransform()->selfRotate(vec3(0, 1, 0), day * deltaTime);
			soleil->findDirectChild("Terre")->findDirectChild("Lune")->getTransform()->selfRotate(vec3(0, 1, 0), day/26.0f * deltaTime);

			soleil->findDirectChild("Mercure")->getTransform()->selfRotate(vec3(0, 1, 0), day / 59.0f * deltaTime);
			soleil->findDirectChild("Venus")->getTransform()->selfRotate(vec3(0, 1, 0), day / 243.0f * deltaTime);
			soleil->findDirectChild("Mars")->getTransform()->selfRotate(vec3(0, 1, 0), day * deltaTime);
			soleil->findDirectChild("Jupiter")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.42f * deltaTime);
			soleil->findDirectChild("Saturne")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.42f * deltaTime);
			soleil->findDirectChild("Uranus")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.71f * deltaTime);
			soleil->findDirectChild("Neptune")->getTransform()->selfRotate(vec3(0, 1, 0), day / 0.75f * deltaTime);
			

			scene->draw(true);
		}
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Moteur - GLFW", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Hide the mouse and enable unlimited mouvement
	//  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//Launch main loop
	initScene();
	mainLoop();

	// Cleanup VBO and shader

	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
